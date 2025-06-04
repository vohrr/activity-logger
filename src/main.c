#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "log.h"
//GtkWidget *widget, gpointer data
static void print_click_me() {
  g_print("I have been clicked!\n");
  log_t *first_log = log_create("Test Log");
  if (first_log == NULL) {
    g_print("Failed to create log\n");
  }
  g_print("First Log name: %s\n", first_log->name);
  g_print("First log size: %ld\n", first_log->size);
  log_free(first_log);


}

static void activate(GtkApplication *app) {

  //initialize window
  GtkBuilder *builder = gtk_builder_new(); 
  gtk_builder_add_from_file(builder, "src/ui/main.ui", NULL);
  

  GObject *window = gtk_builder_get_object(builder, "window");
  if(window == NULL) {
    printf("Window object not found");
    exit(1);
  }

  gtk_window_set_application(GTK_WINDOW(window), app);

  GObject *button = gtk_builder_get_object(builder, "newlog");
  g_signal_connect(button, "clicked", G_CALLBACK(print_click_me), NULL);
  
  gtk_widget_set_visible (GTK_WIDGET(window), TRUE);

  g_object_unref(builder);
/*  GTK_WINDOW(gtk_application_window_new(app));
  gtk_window_set_title(window, "Activity Logger");
  gtk_window_set_default_size(window, 1920, 1080);
 
  //initialize grid 
  GtkWidget *grid = gtk_grid_new();
  gtk_window_set_child(window, grid);
 
  //initialize button and attach to grid
  GtkWidget *button = gtk_button_new_with_label("New Log\n");
  */
 // gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);
  
  //display 
  //gtk_window_present(window);
}

int main(int argc, char **argv) {
  GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_HANDLES_OPEN); //4 
  int status;
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  printf("Activity Logger Running...");
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}

