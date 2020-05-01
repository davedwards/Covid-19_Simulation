#include <gtk/gtk.h>
// #include <gdk-3/gdk/gdk.h>
// #include <bsd/stdlib.h>
#include "ui.h"
#include "person.h"
#include "location.h"
#include "math.h"

#define WINDOW_X 500
#define WINDOW_Y 500

/* Surface to store current scribbles  */
static cairo_surface_t *surface = NULL;
static GtkWidget *drawing_area;

static void clear_surface ()
{
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);

    cairo_destroy (cr);
}

/* Create a new surface to store our drawing */
static gboolean configure_event_callback (GtkWidget *widget,
                                          GdkEventConfigure *event,
                                          gpointer data)
{
    if (surface)
        cairo_surface_destroy (surface);

    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                                 CAIRO_CONTENT_COLOR,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));
    clear_surface();

    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t
 * that is already clipped to only draw the exposed areas of the widget
 */
static gboolean draw_callback (GtkWidget *widget,
                                cairo_t *cr,
                                gpointer data)
{
    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint (cr);

    return FALSE;
}

static void draw_history_bar(cairo_t *cr, GtkWidget *widget, int x, int total, int y) {
    double gx = WINDOW_X + (x * WINDOW_X * 1.0 / total);
    double gy = WINDOW_Y - (y * WINDOW_X * 1.0 / NUM_PEOPLE);

    if (y > SATURATION_THRESHOLD) {
        cairo_set_source_rgb (cr, 50, 0, 0);
    } else {
        cairo_set_source_rgb (cr, 0, 0, 0);
    }

    cairo_rectangle (cr, gx - 1, gy, 2, 6);
    cairo_fill (cr);
}

/* Draw a rectangle on the surface at the given position */
static void draw_person (cairo_t *cr, GtkWidget *widget, Person *p)
{
    double x = p->location.getX() * WINDOW_X * 1.0 / X_LIMIT;
    double y = p->location.getY() * WINDOW_Y * 1.0 / Y_LIMIT;

    switch (p->status) {
        case DEAD:
            cairo_set_source_rgba (cr, 1.0, 0, 0, 1.0);
            break;
        case VULNERABLE:
            cairo_set_source_rgba (cr, 0, 0, 0, 0.4);
            break;
        case IMMUNE:
            cairo_set_source_rgba (cr, 0, 0, 1.0, 0.9);
            break;
        case INFECTED:
            cairo_set_source_rgba (cr, 0, 0.7, 0, 0.9);
            break;
    }
    // cairo_arc(cr, x, y, 4, 0, 2 * M_PI);
    cairo_rectangle (cr, x - 3, y - 3, 6, 6);
    cairo_fill (cr);
}

static void close_window(void)
{
    exit(1);
};

static void activate (GtkApplication *app,
                       gpointer      user_main_func)
{
    GtkWidget *window;
    GtkWidget *frame;

    sim_func_t simmain = (sim_func_t)user_main_func;

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "COVID-19 Animation");

    // enable window closing
    g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

    //create a new frame
    frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(window), frame);

    // create a new drawing area in the frame
    drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (drawing_area, WINDOW_X*2, WINDOW_Y);
    gtk_container_add (GTK_CONTAINER(frame), drawing_area);

    /* Signals used to handle the backing surface */
    g_signal_connect (drawing_area, "draw",
                      G_CALLBACK (draw_callback), NULL);
    g_signal_connect (drawing_area, "configure-event",
                      G_CALLBACK (configure_event_callback),
                      NULL);

    gtk_widget_show_all (window);
    clear_surface();
    simmain();
}

void ui_redraw(Person *people, int numpeople, int *history, int historylength) {

    clear_surface();

    cairo_t *cr;

    /* Paint to the surface, where we store our state */
    cr = cairo_create (surface);

    for (int i=0; i < numpeople; i++){
        draw_person(cr, drawing_area, &(people[i]));
    }

    for (int i=0; i < historylength; i++) {
        draw_history_bar (cr, drawing_area, i, historylength,
                          history[i]);
    }

    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area  */
    gtk_widget_queue_draw_area (drawing_area, 0, 0, WINDOW_X*2, WINDOW_Y);

    // respond to any events to keep the window working
    while (gtk_events_pending()) {
            gtk_main_iteration ();
        }

}

int start_ui (sim_func_t simfunc)
{
    GtkApplication *app;
    int result;

    app = gtk_application_new ("com.jacobsorber", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(activate), (gpointer) simfunc);
    result = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);

    return result;
}
