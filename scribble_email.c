/*------------------------------------------------------------------------------

	-g;-O3;-Wall;$(shell pkg-config --cflags gtk+-3.0);-Wno-unused-result
	$(shell pkg-config --libs gtk+-3.0);-g -lstdc++;-no-pie
	libcurl.a 	libjpeg.a 	libpthread.a 	libm.a

------------------------------------------------------------------------------*/
#include "common_header.h"	// all the standard stuff
#include <gtk/gtk.h>		// sudo apt-get install libgtk-3-dev

const char *version = "Scribble Email";
void email_pixbuf_scribble(GdkPixbuf *scribble, char *email_address);
void program_information
(
	const char *path_file, int line, const char *func, 
	const char *control, ...
);
void gui_printf(const char *control, ...);

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
GtkWidget *window;
GtkWidget *grid;
GtkWidget *drawing_area;
GtkWidget *scroll_window;
GtkWidget *console;
static int page_width		= 1500; 
static int page_height		=  750;
static int console_width	=  300; 
static gdouble last_x, last_y;
static cairo_surface_t *cairo_surface = NULL; // the surface to store scribbles
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

EMAIL_TABLE_ENTRY email_table[] = 
{
	// the first (0th) table entry is the sender
	{ 2, 2, "You",		"<You@gmail.com>", 				NULL },
	
	// subsequent entries are possible recipients
	{ 2, 6, "Tricia",	"<xxx@gmail.com>",				NULL },
	{ 2, 7, "Stephen",	"<xxx@gmail.com>",				NULL },
	{ 2, 8, "Beth",		"<xxx@gmail.com>",				NULL },
	{ 2, 9, "Brendan",	"<xxx@gmail.com>", 				NULL },
	{ 2,10, "Hamdi",	"<xxx@gmail.com>",				NULL },
	{ 2,11, "Hartmann",	"<xxx@gmail.com>",				NULL },
	{ 2,12, "Lewis",	"<xxx@gmail.com>",				NULL },
	{ 2,13, "Alex",		"<xxx@gmail.com>",				NULL },
	{ 2,14, "Friend",	"<Friend@gmail.com>",			NULL },
	{ 2,15, "Friend",	"<Friend@gmail.com>",			NULL },
	{ 2,16, "Friend",	"<Friend@gmail.com>",			NULL },
	{ 2,17, "Friend",	"<Friend@gmail.com>",			NULL },
	{ 2,18, "Friend",	"<Friend@gmail.com>",			NULL },
	{ 2,19, "Cairo",	"<cairo@cairographics.org>",	NULL },
	{ 0, 0, NULL,		NULL,							NULL },
};
/*------------------------------------------------------------------------------
	install a console window of specified dimensions
	the scroll window widget must be attached to the application
------------------------------------------------------------------------------*/
void install_console(int width, int height, GtkWidget **console_text_view)
{
	GtkTextBuffer *console_buffer;
	PangoFontDescription *font;
	GdkRGBA rgba;

	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_set_border_width(GTK_CONTAINER(scroll_window), 5);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(scroll_window, width, height);

	*console_text_view = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scroll_window), *console_text_view);

	font = pango_font_description_from_string("Monospace 10");
	gtk_widget_override_font(*console_text_view, font);

	console_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(*console_text_view));

	// Change default color throughout the widget
	gdk_rgba_parse (&rgba, "blue");
	gtk_widget_override_color(*console_text_view, GTK_STATE_FLAG_NORMAL, &rgba);

	gtk_text_buffer_insert_at_cursor(console_buffer, "", -1);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static void initialize_cairo_surface(void)
{
	cairo_t *cairo_context;
	int y;

	program_information(__FILE__, __LINE__, __FUNC__,
		"initialize_cairo_surface");
		
	cairo_context = cairo_create(cairo_surface);
	
	// clear the surface to a tan paper color
	cairo_set_source_rgb(cairo_context, 0.8, 0.8, 0.7);
	cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
	cairo_paint(cairo_context);
	cairo_set_operator(cairo_context, CAIRO_OPERATOR_OVER);
	
	// draw the rulings
	cairo_set_source_rgb(cairo_context, 0.6, 0.6, 0.6);
	cairo_set_line_width(cairo_context, 1);
	for(y = 0; y < page_height; y+= 40) 
	{
		cairo_move_to(cairo_context, 0, y);
		cairo_line_to(cairo_context, page_width, y);
	}

	cairo_move_to(cairo_context, 100, 0);
	cairo_line_to(cairo_context, 100, page_height);
	cairo_stroke_preserve(cairo_context);

	gtk_widget_queue_draw(drawing_area);

	cairo_destroy(cairo_context);
}
/*------------------------------------------------------------------------------
	https://developer-old.gnome.org/gtkmm-tutorial/stable/
	sec-cairo-drawing-arcs.html.en
------------------------------------------------------------------------------*/
static inline void draw_brush(GtkWidget *widget, gdouble x, gdouble y)
{
	cairo_t *cairo_context;
	
	if(last_x == 0 && last_y == 0)
	{
		last_x = x; last_y = y;
		return;
	}

	cairo_context = cairo_create(cairo_surface);

	// default is black, this is blue
	cairo_set_source_rgb(cairo_context, 0.0, 0.2, 0.7);
 
	cairo_arc
	(
		cairo_context, 
		x, y, 
		1.5,			// radius -- the pen weight
		0.0, 2 * M_PI	// a full circle
	);	
	
	cairo_fill(cairo_context);
	
	//cairo_set_source_rgb(cairo_context, 1.0, 0.0, 0.0);
	cairo_set_line_width(cairo_context, 2.0);
	cairo_move_to(cairo_context, last_x, last_y);
	cairo_line_to(cairo_context, x, y);	// connect the dots
	cairo_stroke_preserve(cairo_context);
	
	cairo_destroy(cairo_context);

	// invalidate the affected region of the drawing area
	//gtk_widget_queue_draw_area(widget, x - 2, y - 2, 4, 4);
	gtk_widget_queue_draw(drawing_area);

	last_x = x; last_y = y;
}
/*------------------------------------------------------------------------------
	https://stackoverflow.com/questions/48557583/
	freehand-drawing-gtk-set-event-compressionfalse-does-not-work	
---------------------------------------------------------------------------*/
static gboolean motion_notify_event_callback
(
	GtkWidget      *widget,
	GdkEventMotion *event,
	gpointer        data
)
{
	if(event->state)
	//if(event->state & GDK_BUTTON2_MASK)
	{
		draw_brush(widget, event->x, event->y);
	}

	return TRUE;
}
/*------------------------------------------------------------------------------
	Handle button press events by either drawing a rectangle
	or clearing the surface, depending on which button was pressed.
	The button-press signal handler receives a GdkEventButton
	struct which contains this information.
---------------------------------------------------------------------------*/
static gboolean button_press_event_callback 
(
	GtkWidget      *widget,
	GdkEventButton *event,
	gpointer        data
)
{
	if(event->button == GDK_BUTTON_SECONDARY)
	{
		initialize_cairo_surface();
		gtk_widget_queue_draw(widget);
	}

	if(event->button == GDK_BUTTON_PRIMARY)
	{
		last_x = 0; last_y = 0;	// reset the line drawing
	}

	return TRUE;
}
/*------------------------------------------------------------------------------
	Create a new surface of the appropriate size to store our scribbles
	
---------------------------------------------------------------------------*/
static gboolean configure_event_callback
(
	GtkWidget         *widget,
	GdkEventConfigure *event,
	gpointer           data
)
{
	if(cairo_surface) cairo_surface_destroy(cairo_surface);

	cairo_surface = gdk_window_create_similar_surface
	(
		gtk_widget_get_window(widget),
		CAIRO_CONTENT_COLOR,
		page_width, page_height
	);

	initialize_cairo_surface();

	// this be the trick
	gdk_window_set_event_compression(((GdkEventButton *)event)->window, false);

	return TRUE;
}
/*------------------------------------------------------------------------------
	Redraw the screen from the surface. Note that the draw
	signal receives a ready-to-be-used cairo_t that is already
	clipped to only draw the exposed areas of the widget
------------------------------------------------------------------------------*/
static gboolean draw_callback
(
	GtkWidget *widget,
	cairo_t   *cairo_context,
	gpointer   data
)
{
	cairo_set_source_surface(cairo_context, cairo_surface, 0, 0);
	cairo_paint(cairo_context);
	cairo_set_source_rgb(cairo_context, 0.0, 0.2, 1.0);
 
	return FALSE;
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static void email_person(GtkWidget *widget, gpointer email_address)
{
	GdkPixbuf *pixbuf;
	
	pixbuf = gdk_pixbuf_get_from_surface
	(
		cairo_surface, 0, 0, page_width, page_height
	);
	
	email_pixbuf_scribble(pixbuf, (char *)email_address);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static void install_email_buttons(void)
{
	EMAIL_TABLE_ENTRY *entry;
	GtkWidget *email_button;
	
	entry = email_table;
	while(entry->name != NULL)
	{
		email_button = gtk_button_new_with_label(entry->name);
		gtk_grid_attach
		(
			GTK_GRID(grid), email_button, 
			entry->col, entry->row, 1, 1
		);
		g_signal_connect(email_button, "clicked", 
		G_CALLBACK(email_person), entry->email_address);
		entry->button = email_button;	// for future use ?
		
		entry++;
	}		
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	GtkWidget *clear_button;
	GdkCursor *pen_cursor;
	GdkWindow *gdk_window;
	
	gtk_init(&argc, &argv);

	window			= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	grid			= gtk_grid_new();
	drawing_area	= gtk_drawing_area_new();
	
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_NONE);
	gtk_window_set_title(GTK_WINDOW(window), version);
	gtk_container_set_border_width(GTK_CONTAINER(window), 1);
	
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

	gtk_widget_set_size_request(drawing_area, page_width, page_height);
	gtk_grid_attach(GTK_GRID(grid), drawing_area, 1, 1, 1, 25);
	
	install_email_buttons();
	
	clear_button = gtk_button_new_with_label("Clear");
	gtk_grid_attach(GTK_GRID(grid), clear_button, 2, 22, 1, 1);
	g_signal_connect(clear_button, "clicked", 
		G_CALLBACK(initialize_cairo_surface), NULL);
		
	install_console(console_width, page_height, &console);
	gtk_grid_attach(GTK_GRID(grid), scroll_window, 3, 1, 1, 25);
	
	// signals used to handle the backing surface
	g_signal_connect(drawing_area, "draw",
		G_CALLBACK(draw_callback), NULL);
					
	g_signal_connect(drawing_area,"configure-event",
		G_CALLBACK(configure_event_callback), NULL);

	g_signal_connect(drawing_area, "motion-notify-event",
		G_CALLBACK(motion_notify_event_callback), NULL);
					
	g_signal_connect(drawing_area, "button-press-event",
		G_CALLBACK(button_press_event_callback), NULL);

	gtk_widget_set_events
	(
		drawing_area, 
		gtk_widget_get_events(drawing_area) 
		| GDK_BUTTON_PRESS_MASK
		| GDK_POINTER_MOTION_MASK
	);

	g_signal_connect(window, "destroy", 
		G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);

	// ?? pen_cursor = gdk_cursor_new_for_display(GDK_DISPLAY(drawing_area), GDK_DOT);
	pen_cursor	= gdk_cursor_new(GDK_DOT);
	gdk_window	= gtk_widget_get_window(GTK_WIDGET(drawing_area));
	
	gdk_window_set_cursor(gdk_window, pen_cursor);
	gui_printf("Gtk+ version %d.%d\n", gtk_major_version, gtk_minor_version);

	gtk_main();
	return EXIT_SUCCESS;
}
/*------------------------------------------------------------------------------

	The following are deprecated but I can't figure out how to replace them:
		gdk_cursor_new
		gtk_widget_override_font
		gtk_widget_override_color
------------------------------------------------------------------------------*/
