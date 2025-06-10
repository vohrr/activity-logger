#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "log.h"
#include "actions.h"

void new_log_click(GtkWidget *widget, gpointer user_data) {
  g_print("New log clicked!\n");
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "viewlogpage");
  log_t *first_log = log_create("Test Log");
  if (first_log == NULL) {
    g_print("Failed to create log\n");
  }
  log_free(first_log);
}

void log_list_click(GtkWidget *widget, gpointer user_data) {
  g_print("log list clicked!\n");

  GtkWidget *page = gtk_stack_get_child_by_name(GTK_STACK(user_data), "loglistpage");
  if(page == NULL) {
    g_print("Could not find log list page");
  }
  gtk_stack_set_visible_child(GTK_STACK(user_data), page);
  GtkBox *box = GTK_BOX(page);

  if(gtk_widget_get_first_child(page) != gtk_widget_get_last_child(page)) { return; }

  log_list_t *log_list = log_list_get();
  if(log_list != NULL) {
    g_print("Loading logs..\n");
    size_t i = 0;
    while(i < log_list->filecount) {
      char *log = log_list->log_names[i];
      if(log != NULL) { 
        g_print("Log: %s\n", log);
        GtkWidget *button = gtk_button_new_with_label(log);
        gtk_box_append(box, button); 
        g_signal_connect(button, "clicked", G_CALLBACK(view_log_click), user_data);
      }
      i++;
    }
  }

  log_list_free(log_list);
}

void view_log_click(GtkWidget *widget, gpointer user_data) {
  char* label = gtk_button_get_label(GTK_BUTTON(widget));
  g_print("Viewing log: %s\n", label);
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "viewlogpage"); 

  log_t *log = log_load(label);

  if (log == NULL) {
    g_print("Failed to load data for %s\n", label);
  }
  //etc do stuff here to load existing log and log entries
  //also need to bind to UI after, programatically add elements like above
}

void save_log_click() {
  return;
}
