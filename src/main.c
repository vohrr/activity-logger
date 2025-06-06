#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "log.h"

static void new_log_click(GtkWidget *widget, gpointer user_data) {
  g_print("New log clicked!\n");
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "viewlogpage");
  log_t *first_log = log_create("Test Log");
  if (first_log == NULL) {
    g_print("Failed to create log\n");
  }
  log_free(first_log);
}

static void log_list_click(GtkWidget *widget, gpointer user_data) {
  g_print("log list clicked!\n"); 
  
  GtkWidget *page = gtk_stack_get_child_by_name(GTK_STACK(user_data), "loglistpage");
  if(page == NULL) {
    g_print("Could not find log list page");
  }
  gtk_stack_set_visible_child(GTK_STACK(user_data), page);
  GtkBox *box = GTK_BOX(page);

  if(gtk_widget_get_first_child(page) != gtk_widget_get_last_child(page)) { return; }

  char **log_list = log_list_get();
  if(log_list != NULL) {
    g_print("Loading logs..\n");
    size_t i = 0; 
    while(i < sizeof(log_list)) {
      char *log = log_list[i];
      if(log != NULL) { 
        g_print("Log: %s\n", log);
      GtkWidget *button = gtk_button_new_with_label(log);
      gtk_box_append(box, button); 
        //TODO: append the view_log_click() callback event to these buttons
      }
      i++;
    }
  }

  log_list_free(log_list);
}

static void save_log_click() {
  return;
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

  GObject *stack = gtk_builder_get_object(builder, "mainstack");
  //bind button events
  GObject *button = gtk_builder_get_object(builder, "newlog");
  g_signal_connect(button, "clicked", G_CALLBACK(new_log_click), stack);

  button = gtk_builder_get_object(builder, "openlog");
  g_signal_connect(button, "clicked", G_CALLBACK(log_list_click), stack);

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

