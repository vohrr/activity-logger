#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "actions.h"

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

  GObject *stack = gtk_builder_get_object(builder, "mainstack");
  //bind button events
  GObject *button = gtk_builder_get_object(builder, "newlog");
  g_signal_connect(button, "clicked", G_CALLBACK(new_log_click), stack);

  button = gtk_builder_get_object(builder, "openlog");
  g_signal_connect(button, "clicked", G_CALLBACK(log_list_click), stack);

  gtk_widget_set_visible (GTK_WIDGET(window), TRUE);

  g_object_unref(builder);
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



