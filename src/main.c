#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

//GtkWidget *widget, gpointer data
static void print_click_me() {
	g_print("I have been clicked!\n");
}

static void activate(GtkApplication *app) {
	GtkWidget *window = gtk_application_window_new (app);
	gtk_window_set_title((GtkWindow *)window, "Activity Logger");
	gtk_window_set_default_size((GtkWindow *)window, 1920, 1080);

	GtkWidget *button = gtk_button_new_with_label("Try Me!\n");
	g_signal_connect(button, "clicked", G_CALLBACK (print_click_me), NULL);
	gtk_window_set_child((GtkWindow *)window, button);

	gtk_window_present((GtkWindow *)window);
}

int main(int argc, char **argv) {
	GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_HANDLES_OPEN); //4 
	int status;
	g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
	printf("Activity Logger Running...");
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref(app);
	return status;
}

