/*------------------------------------------------------------------------------


	sudo apt-get install libgtk-3-dev
	sudo apt-get install libcurl4-gnutls-dev
	sudo apt-get install libcurl4-nss-dev
	sudo apt-get install libcurl4-openssl-dev
	sudo apt-get install libjpeg-dev

------------------------------------------------------------------------------*/
#include "common_header.h"	// all the standard stuff
#include <gtk/gtk.h>	// $(shell pkg-config --libs gtk+-3.0)
#include <curl/curl.h>	// libcurl.a
#include "jpeglib.h"	// libjpeg.a

void gui_printf(const char *control, ...);
void gui_warning_dialog_box(const char *path_file, int line, const char *function_name, const char *control, ...);
void gui_program_error_dialog_box(const char *path_file, int line, const char *function_name, const char *control, ...);
void program_information(const char *path_file, int line, const char *func, const char *control, ...);
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
#define EMAIL_LINE_LENGTH 256

/*------------------------------------------------------------------------------
	Use a multiple of 4 for base64 data
	64 * 4 = 256 lines
------------------------------------------------------------------------------*/
#define ENCODED_LINE_LENGTH (64 * 5)
#define RAW_LINE_BYTE_COUNT (ENCODED_LINE_LENGTH * 3 / 4)
#define ENCODED_AND_TERMINATED_LINE_LENGTH (ENCODED_LINE_LENGTH + 3)
#define MAX_TEXT_LINE_COUNT 2000
// define a silly MIME boundary string
#define MIME_BOUNDARY "mime-boundary-yradnuob-emim"
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
typedef struct email_table_entry
{
	int col;
	int row;
	
	char *name;
	char *email_address;
	
	GtkWidget *button;
}
EMAIL_TABLE_ENTRY;

extern EMAIL_TABLE_ENTRY email_table[]; 

typedef struct upload_status
{
    int lines_read;
}
UPLOAD_STATUS;

char (*email_buffer)[ENCODED_AND_TERMINATED_LINE_LENGTH + 1] = NULL;
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
size_t email_buffer_source(void* ptr, size_t size, size_t nmemb, void* userp)
{
    struct upload_status* upload_ctx = (struct upload_status*)userp;
    const char* data;

    if((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
	{
		return 0;
    }

    data = email_buffer[upload_ctx->lines_read];

    if(data)
	{
		size_t len = strlen(data);
		__builtin_memcpy(ptr, data, len);
		upload_ctx->lines_read++;
		return len;
    }
    return 0;
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void add_email_line(int *line, size_t *buffer_size, const char *control, ...)
{
	va_list parms;
	char text_line[1024];
	int line_length;

	va_start(parms, control);
	line_length = vsprintf(text_line, control, parms);

	strcpy(email_buffer[*line], text_line);
	*buffer_size += line_length;

	// terminate the line and update the buffer size
	strcat(email_buffer[*line], "\r\n");
	*buffer_size += 2;
	(*line)++;
	va_end(parms);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void add_email_html(int *line, size_t *buffer_size, char *filename)
{
    FILE *fptr;
	char html_line[EMAIL_LINE_LENGTH + 1];

    if((fptr = fopen(filename, "rt")) == NULL)
	{
		gui_program_error_dialog_box(__FILE__, __LINE__, __FUNC__,
			"can't open %s", filename);
	}

	while(!feof(fptr))
	{
		*html_line = '\0';
		// scan until a newline
		fscanf(fptr, " %255[^\n\r]", html_line);
		add_email_line(line, buffer_size, "%s", html_line);
	}
	fclose(fptr);

	add_email_line(line, buffer_size,"");
}
/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void convert_pixbuf_to_jpeg_in_memory
(
	GdkPixbuf *pixbuf, int quality, 
	uint8_t **buffer, unsigned long *buffer_size
)
{
	uint8_t *buffer_ptr;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW *row_pointer;		// pointer to JSAMPLE row
	int row_stride;				// physical row width in output buffer

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	// The destination manager automatically allocates memory for the buffer
	// and the program needs to free the memory after use.
	jpeg_mem_dest(&cinfo, buffer, buffer_size);

	// First we supply a description of the input image.
	// Four fields of the cinfo struct must be filled in:
	// image width and height, in pixels */
	cinfo.image_width		= gdk_pixbuf_get_width(pixbuf);	
	cinfo.image_height		= gdk_pixbuf_get_height(pixbuf);
	// # of color components per pixel */
	cinfo.input_components	= gdk_pixbuf_get_n_channels(pixbuf);	
	cinfo.in_color_space	= JCS_RGB; 	// colorspace of image = sRGB

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE); // limit to baseline-JPEG values

	cinfo.write_JFIF_header  = FALSE;
	cinfo.write_Adobe_marker = FALSE;

	jpeg_start_compress(&cinfo, TRUE);

	// JSAMPLEs per row in image_buffer
	row_stride = gdk_pixbuf_get_rowstride(pixbuf);	

	// the scan line is written directly from the pixbuf image buffer
	buffer_ptr = (uint8_t *)gdk_pixbuf_get_pixels(pixbuf);
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer = &buffer_ptr;
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
		buffer_ptr += row_stride; // step buffer ptr to next row
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
}
/*------------------------------------------------------------------------------
	https://en.wikipedia.org/wiki/Base64
------------------------------------------------------------------------------*/
static inline void encode_block_base64(uint8_t *in_block, uint8_t *out_block, int block_length)
{
	static const uint8_t b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    out_block[0] = (uint8_t)b64_table[(int)(in_block[0] >> 2)];
    out_block[1] = (uint8_t)b64_table[(int)(((in_block[0] & 0x03) << 4) | ((in_block[1] & 0xF0) >> 4))];
    out_block[2] = (uint8_t)(block_length > 1 ? b64_table[(int)(((in_block[1] & 0x0F) << 2) | ((in_block[2] & 0xC0) >> 6))] : '=');
    out_block[3] = (uint8_t)(block_length > 2 ? b64_table[(int)(in_block[2] & 0x3F)] : '=');
}
/*------------------------------------------------------------------------------

The input buffer may contain raw binary data but the output buffer will contains
just printable chars from the base64 character set.

------------------------------------------------------------------------------*/
void base64_encode(uint8_t *input_buffer, int length, char *output_buffer)
{
    uint8_t input_block[3], output_block[4];
    int block_length, i, j;

    *output_buffer = '\0';
	i = 0;

    while(i < length)
	{
		for(j = 0, block_length = 0; j < 3; ++j)
		{
			input_block[j] = input_buffer[i++];
			if(i > length)
			{
				input_block[j] = '\0';
			}
			else
			{
				block_length++;
			}
		}

		if(length > 0)
		{
			encode_block_base64(input_block, output_block, block_length);
			strncat(output_buffer, (char *)output_block, 4);
		}
    }

	strcat(output_buffer, "");
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void encode_binary_to_base64
(
	int *line,
	size_t *buffer_size,
	uint8_t *binary_buffer,
	unsigned long binary_buffer_size
)
{
	uint8_t *chunk_start_ptr, *chunk_end_ptr;
	int chunk_size;
	char encoded_buffer[ENCODED_AND_TERMINATED_LINE_LENGTH + 1];
		// ASCII data base64 encoded from the binary buffer

	// send chunks of the image data to be base64 encoded into email ASCII text lines
	chunk_start_ptr = binary_buffer;
	while(chunk_start_ptr < binary_buffer + binary_buffer_size)
	{
		chunk_end_ptr = chunk_start_ptr + RAW_LINE_BYTE_COUNT;
		if(chunk_end_ptr > binary_buffer + binary_buffer_size) chunk_end_ptr = binary_buffer + binary_buffer_size;

		chunk_size = chunk_end_ptr - chunk_start_ptr;
		base64_encode(chunk_start_ptr, chunk_size, encoded_buffer);
		add_email_line(line, buffer_size, encoded_buffer);
		chunk_start_ptr = chunk_end_ptr;
	}

	// The jpeg destination manager automatically allocates memory for the
	// buffer and the program needs to free the memory after use.
	free_null(binary_buffer);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void setup_curl_email(CURL **curl)
{
    CURLcode res = CURLE_OK;

    *curl = curl_easy_init();

	if(!*curl)
	{
		gui_program_error_dialog_box(__FILE__, __LINE__, __FUNC__,
			"curl_easy_init() failed: %s",
			curl_easy_strerror(res));
	}

	// *** this is where you put your own email information ***

	curl_easy_setopt(*curl, CURLOPT_USERNAME, 	"your-name@gmail.com");
	curl_easy_setopt(*curl, CURLOPT_PASSWORD,	"your-password");
	curl_easy_setopt(*curl, CURLOPT_URL,		"smtp.gmail.com:465");
	curl_easy_setopt(*curl, CURLOPT_MAIL_FROM, 	"your-name@gmail.com");


	curl_easy_setopt(*curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
	curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(*curl, CURLOPT_FORBID_REUSE, 1L);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void create_email_header
(
	int *line, size_t *buffer_size, char *email_address
)
{
	static char date_string[40];
	struct tm *broken_time;
	time_t tea_time;


	add_email_line(line, buffer_size,
		"X-Mozilla-Status: 0001");
	add_email_line(line, buffer_size,
		"X-Mozilla-Status2: 00800000");
	add_email_line(line, buffer_size,
		"X-Mozilla-Keys:");
	add_email_line(line, buffer_size,
		"Message-ID: <28647fee-297b-0468-91d5-67d0f3ba84cf@gmail.com>");
	add_email_line(line, buffer_size,
		"User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
		"AppleWebKit/537.36 (KHTML, like Gecko) "
		"Chrome/101.0.4951.41 Safari/537.36");

	add_email_line
	(
		line, buffer_size,
		"Subject: a scribbled message"
	);

	tea_time	= time(NULL);
	broken_time	= localtime(&tea_time);
	strftime(date_string, 40, "%a, %d %b %Y %T %z", broken_time);

	add_email_line(line, buffer_size,
		"To: %s", email_address);
		
	// the first (0th) table entry is the sender
	add_email_line(line, buffer_size,
		"From: %s", email_table->email_address);
	add_email_line(line, buffer_size,
		"Reply-To: %s", email_table->email_address);

	add_email_line(line, buffer_size,
		"Organization: FourteenthStreetStudio");
	add_email_line(line, buffer_size,
		"MIME-Version: 1.0");

	add_email_line(line, buffer_size,
		"Content-Type: multipart/related; boundary=\"%s\"", MIME_BOUNDARY);
	add_email_line(line, buffer_size, "");
	add_email_line(line, buffer_size,
		"This is a multi-part message in MIME format.");
	add_email_line(line, buffer_size, "--"MIME_BOUNDARY"");

	// begin HTML
	add_email_line(line, buffer_size,
		"Content-Type: text/html; charset=utf-8");
	add_email_line(line, buffer_size,
		"Content-Transfer-Encoding: 7bit");
	add_email_line(line, buffer_size, "");
	
	// external HTML
	//add_email_html(line, buffer_size, "../comment.html");	
	
	// embeded HTML
	add_email_line(line, buffer_size,
		"<html xmlns=\"http://www.w3. org/1999/xhtml\" class=\"tabid-1 \">");
	add_email_line(line, buffer_size,
		"<body bgcolor=#808080>");
	add_email_line(line, buffer_size,
		"<br><br>");
	add_email_line(line, buffer_size,
		"<div align=\"center\">");
	add_email_line(line, buffer_size,	//	Content-ID
		"<img alt =\"email_scribbled\" src =\"cid:email_scribble_image\"");
	add_email_line(line, buffer_size,
		"style =\"width:auto; height:auto; border:none;\" >");
	add_email_line(line, buffer_size,
		"</div>");
	add_email_line(line, buffer_size,
		"</body>");
	add_email_line(line, buffer_size,
		"</html>");
	// end HTML

	add_email_line(line, buffer_size, "--"MIME_BOUNDARY"");

	if(*line > MAX_TEXT_LINE_COUNT)
	{
		gui_program_error_dialog_box(__FILE__, __LINE__, __FUNC__,
			"line %d > %d MAX_TEXT_LINE_COUNT\n",
			*line, MAX_TEXT_LINE_COUNT);
	}
#if 0
	else
	{
		int i;
		printf("\n   --- Email Header ---\n");
		for(i = 0; i < *line; i++)
		{
			printf("line %5d %s", i, email_buffer[i]);
		}
	}
#endif
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static size_t create_message_with_pixbuf
(
	GdkPixbuf *pixbuf_image, char *email_address
)
{
    size_t buffer_size = 0;
	int line = 0, message_lines = 0;
	uint8_t *image_buffer = NULL;
	unsigned long image_buffer_size;

	convert_pixbuf_to_jpeg_in_memory
	(
		pixbuf_image, 90, 
		&image_buffer, &image_buffer_size
	);
	
	message_lines = MAX_TEXT_LINE_COUNT + image_buffer_size / RAW_LINE_BYTE_COUNT;

	// an array of pointers to char
	if((email_buffer = (char (*)[])calloc(message_lines, ENCODED_AND_TERMINATED_LINE_LENGTH)) == NULL)
	{
		gui_program_error_dialog_box(__FILE__, __LINE__, __FUNC__,
			"out of memory for email_buffer\n");
	}

	create_email_header(&line, &buffer_size, email_address);
	add_email_line(&line, &buffer_size, "");
	add_email_line(&line, &buffer_size, "--"MIME_BOUNDARY"");
	add_email_line(&line, &buffer_size,
		"Content-Type: image/jpeg; name=\"scribble\"");
	add_email_line(&line, &buffer_size,
		"Content-Transfer-Encoding: base64");
	add_email_line(&line, &buffer_size, 	// this ID is picked up by the HTML
		"Content-ID: <email_scribble_image>");

	// inline disposition is not always obeyed by the recipients client
	// an inline content-disposition, means that it should be
	// automatically displayed when the message is displayed
	add_email_line(&line, &buffer_size,
		"Content-Disposition: inline; filename=\"scribble\"");

	add_email_line(&line, &buffer_size, "");
	encode_binary_to_base64(&line, &buffer_size, image_buffer, image_buffer_size);
	add_email_line(&line, &buffer_size, "");
	add_email_line(&line, &buffer_size, "--"MIME_BOUNDARY"--");

	return buffer_size;
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void email_pixbuf_scribble(GdkPixbuf *scribble, char *email_address)
{
    CURL* curl;
    CURLcode result = CURLE_OK;
    struct curl_slist* recipients = NULL;
    struct upload_status file_upload_ctx;
    size_t file_size;

	gui_printf("sending email to:\n  %s\n", email_address);
    file_size = create_message_with_pixbuf(scribble, email_address);
	
	file_upload_ctx.lines_read = 0;
    curl = curl_easy_init();

	setup_curl_email(&curl);

	recipients = curl_slist_append(recipients, email_address);
	// copy the email to the first (0th) name in the list -- to sender
	recipients = curl_slist_append(recipients, email_table->email_address);

	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
	curl_easy_setopt(curl, CURLOPT_INFILESIZE, file_size);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, email_buffer_source);
	curl_easy_setopt(curl, CURLOPT_READDATA, &file_upload_ctx);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	result = curl_easy_perform(curl);

	if(result != CURLE_OK)
	{
		gui_warning_dialog_box(__FILE__, __LINE__, __FUNC__, 
			"curl_easy_perform() failed:\n%s",
			curl_easy_strerror(result));
	}
	else
	{
		gui_printf("sent\n");
	}
	
	curl_slist_free_all(recipients);
	curl_easy_cleanup(curl);
	free(email_buffer);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
