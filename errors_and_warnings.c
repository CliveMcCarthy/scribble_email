/*------------------------------------------------------------------------------


------------------------------------------------------------------------------*/
#include "common_header.h"	// all the standard stuff
#include <gtk/gtk.h> // sudo apt-get install libgtk-3-dev
#define TEXT_BUFFER_SIZE 4096

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
extern GtkWidget *window;
extern GtkWidget *console;
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void gui_program_error_dialog_box
(
	const char *path_file, int line, const char *function_name, 
	const char *control, ...
)
{
	va_list parms;
	char text[TEXT_BUFFER_SIZE];
	GtkWidget *dialog;

	va_start(parms, control);
	vsnprintf(text, TEXT_BUFFER_SIZE, control, parms);
	printf(ANSI_COLOR_RED"** error ** %-40s\n", text);
	printf("(in file: %s at line: %d in function: %s)\n"ANSI_COLOR_RESET,
		(path_file), line, function_name);

	dialog = gtk_message_dialog_new
	(
		GTK_WINDOW(window),
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_CLOSE,
		"%s\nin function '%s'\nin file '%s' line %d",
			text, function_name, (path_file), line
	);

	//gtk_window_set_title(GTK_WINDOW(dialog),
		//"Okay, Houston, we've had a problem here.");
	gtk_window_set_title(GTK_WINDOW(dialog),
		"Oh, shit ! ");

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	exit(EXIT_FAILURE);
	va_end(parms);
}
/*------------------------------------------------------------------------------
	gui_warning_dialog_box(__FILE__, __LINE__, __FUNC__, "a test warning");

------------------------------------------------------------------------------*/
void gui_warning_dialog_box
(
	const char *path_file, int line, const char *function_name, 
	const char *control, ...
)
{
	va_list parms;
	char text[TEXT_BUFFER_SIZE];
	GtkWidget *dialog;

	va_start(parms, control);
	vsnprintf(text, TEXT_BUFFER_SIZE, control, parms);
	printf("** warning ** %-40s\n", text);
	printf("(in file: %s at line: %d\nin function: %s)\n", 
		(path_file), line, function_name);

	dialog = gtk_message_dialog_new
	(
		GTK_WINDOW(window),
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_WARNING,
		GTK_BUTTONS_CLOSE,
		"%s\nin function '%s'\nin file '%s' line %d",
			text, function_name, (path_file), line
	);

	gtk_window_set_title(GTK_WINDOW(dialog), "Oh dear ! ");

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	va_end(parms);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void gui_information_dialog_box(const char *control, ...)
{
	va_list parms;
	char text[TEXT_BUFFER_SIZE];
	GtkWidget *dialog;

	va_start(parms, control);
	vsnprintf(text, TEXT_BUFFER_SIZE, control, parms);
	printf("gtk_message_dialog_new='%s'", text);

	dialog = gtk_message_dialog_new
	(
		GTK_WINDOW(window),
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_WARNING,
		GTK_BUTTONS_CLOSE,
		"%s", text
	);

	gtk_window_set_title(GTK_WINDOW(dialog), "information");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	va_end(parms);
}
/*------------------------------------------------------------------------------
	this is a gui version of printf that prints to the Gtk console window
------------------------------------------------------------------------------*/
void gui_printf(const char *control, ...)
{
	va_list parms;
	GtkTextBuffer *console_buffer;
	GtkTextMark *console_mark;
	char text[TEXT_BUFFER_SIZE];

	va_start(parms, control);
	vsnprintf(text, TEXT_BUFFER_SIZE, control, parms);
	//vsprintf(text, control, parms);
	console_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(console));
	//printf("OK '%s' %d\n", text, __LINE__);
	gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(console_buffer), text, -1);
	//printf("OK '%s' %d\n", text, __LINE__);
	console_mark = gtk_text_buffer_get_insert(GTK_TEXT_BUFFER(console_buffer));
	gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(console), console_mark);
	va_end(parms);
	while(gtk_events_pending()) gtk_main_iteration();
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void program_information
(
	const char *path_file, int line, const char *func, 
	const char *control, ...
)
{
	va_list parms;
	char text[4096]; // text expansion buffer

	printf(ANSI_COLOR_GREEN);
	va_start(parms, control);
	vsprintf(text, control, parms);
	printf("\n* information * %s\n", text);
	printf("(in file: %s at line: %d in function: %s)\n", 
		path_file, line, func);
	printf(ANSI_COLOR_RESET);
	va_end(parms);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
