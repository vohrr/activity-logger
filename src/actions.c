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
        g_signal_connect(button, "clicked", G_CALLBACK(view_log_click), button);
      }
      i++;
    }
  }

  log_list_free(log_list);
}

void view_log_click(GtkWidget *widget, gpointer user_data) {
  const char* label = gtk_button_get_label(GTK_BUTTON(user_data));
  g_print("Viewing log: %s\n", label);
  return;
}

void save_log_click() {
  return;
}
