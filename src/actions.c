#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "actions.h"

static void void_widget(GtkWidget *widget) {
  (void)widget;
}

void render_log_entries(GtkWidget *boxwidget, char *label, gpointer user_data) {
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "logentrylist"); 
  GtkBox *box = GTK_BOX(boxwidget);
  GtkWidget *boxlabel = gtk_widget_get_first_child(boxwidget);
  size_t buffer = strlen(label) + 10;
  char label_prefix[buffer];
  strcpy(label_prefix,"Viewing ");
  gtk_label_set_label(GTK_LABEL(boxlabel), strcat(label_prefix, label));
  log_t *log = log_load(label);
  
  if (log == NULL) {
    g_print("Failed to load data for %s\n", label);
    return;
  }
  if (log->entries == NULL) {
    g_print("No log entries for this log\n");
    return;
  }
  size_t i = 0;
  while(i < log->size) {
    log_entry_t *entry = log->entries[i];
    if(entry != NULL) {
        GtkWidget *button = gtk_button_new_with_label(entry->datetime);
        g_object_set_data(G_OBJECT(button), "entry", entry);
        g_object_set_data(G_OBJECT(button), "log_name", log->name);
        gtk_box_append(box,button);
        g_signal_connect(button, "clicked", G_CALLBACK(log_entry_click), user_data);
    } 
    i++;
  }
}

void return_to_log_click(GtkWidget *widget, gpointer user_data) {
  GtkWidget *boxwidget = gtk_stack_get_child_by_name(GTK_STACK(user_data), "logentrylist");
  log_entry_handler_t *log_entry_handler = g_object_get_data(G_OBJECT(widget), "entry_handler");
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "logentrylist"); 
  log_entry_handler_free(log_entry_handler);
}

void render_new_button(GtkWidget *boxwidget, char *log_name, gpointer user_data) {
  GtkBox *box = GTK_BOX(boxwidget);
  GtkWidget *button = gtk_button_new_with_label("Add New Entry");
  g_object_set_data(G_OBJECT(button), "log_name", log_name);
  gtk_box_append(box, button);
  g_signal_connect(button, "clicked", G_CALLBACK(new_log_entry_click), user_data);
}

void render_message_box(GtkBox *box, char *entry_message) {
  GtkWidget *scrolled = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(scrolled, TRUE);
  gtk_widget_set_hexpand(scrolled, TRUE);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  
  GtkWidget *message_box = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(message_box), GTK_WRAP_WORD_CHAR);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), message_box);
  gtk_box_append(GTK_BOX(box), scrolled);
  
  if(entry_message != NULL) {
    GtkTextBuffer *message = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_box));
    gtk_text_buffer_set_text(message,  entry_message, -1);
  }
}

void render_action_buttons(GtkWidget *boxwidget, log_entry_handler_t *log_entry_handler, gpointer user_data) {
  clear_elements(boxwidget, BUTTON);
  GtkBox *box = GTK_BOX(boxwidget);
  GtkWidget *button = gtk_button_new_with_label("Save");
  gtk_box_append(box, button);
  g_object_set_data(G_OBJECT(button), "entry_handler", log_entry_handler);
  g_signal_connect(button, "clicked", G_CALLBACK(save_log_entry_click), user_data);
  button = gtk_button_new_with_label("Return");
  g_object_set_data(G_OBJECT(button), "entry_handler", log_entry_handler);
  g_signal_connect(button, "clicked", G_CALLBACK(return_to_log_click), user_data);
  gtk_box_append(box, button);
}

void new_log_click(GtkWidget *widget, gpointer user_data) {
  void_widget(widget);
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "viewlogpage");
  log_t *first_log = log_create("Test Log");
  if (first_log == NULL) {
    g_print("Failed to create log\n");
  }
  log_free(first_log);
}

void log_list_click(GtkWidget *widget, gpointer user_data) {
  void_widget(widget);

  GtkWidget *boxwidget = gtk_stack_get_child_by_name(GTK_STACK(user_data), "loglistpage");
  if(boxwidget == NULL) {
    g_print("Could not find log list box");
  }
  gtk_stack_set_visible_child(GTK_STACK(user_data), boxwidget);
  GtkBox *box = GTK_BOX(boxwidget);

  if(gtk_widget_get_first_child(boxwidget) != gtk_widget_get_last_child(boxwidget)) { return; }

  log_list_t *log_list = log_list_get();
  if(log_list != NULL) {
    size_t i = 0;
    while(i < log_list->filecount) {
      char *log_name = log_list->log_names[i];
      if(log_name != NULL) { 
        g_print("Log: %s\n", log_name);
        GtkWidget *button = gtk_button_new_with_label(log_name);
        gtk_box_append(box, button); 
        g_signal_connect(button, "clicked", G_CALLBACK(view_log_click), user_data);
      }
      i++;
    }
  }

  log_list_free(log_list);
}

void view_log_click(GtkWidget *widget, gpointer user_data) {
  const char* label = gtk_button_get_label(GTK_BUTTON(widget));
  GtkWidget *boxwidget = gtk_stack_get_child_by_name(GTK_STACK(user_data), "logentrylist");

  clear_elements(boxwidget, BUTTON);
  render_log_entries(boxwidget, label, user_data);
  render_new_button(boxwidget, label, user_data);
}

void save_log_click(GtkButton *button, gpointer user_data) {
  (void)button;
  (void)user_data;
}

void delete_log_click(GtkButton *button, gpointer user_data){
  (void)button;
  (void)user_data;
}

void log_entry_click(GtkButton *button, gpointer user_data) {
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "logentryview"); 
  log_entry_t *log_entry = g_object_get_data(G_OBJECT(button), "entry");
  char *log_name = g_object_get_data(G_OBJECT(button), "log_name");
  
  GtkWidget *boxwidget = gtk_stack_get_child_by_name(GTK_STACK(user_data), "logentryview");
  GtkBox *box = GTK_BOX(boxwidget);

  clear_elements(boxwidget, SCROLLED_WINDOW);
  render_message_box(box, log_entry->message);
  log_entry_handler_t *log_entry_handler = log_entry_handler_new(log_name, log_entry); 

  render_action_buttons(boxwidget, log_entry_handler, user_data);
}

void save_log_entry_click(GtkWidget *widget, gpointer user_data) {
  log_entry_handler_t *log_entry_handler = g_object_get_data(G_OBJECT(widget), "entry_handler");
  GtkWidget *scrolled_view = gtk_widget_get_prev_sibling(widget);
  GtkWidget *text_view = gtk_widget_get_first_child(scrolled_view);
  GtkTextBuffer *message_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(message_buffer, &start);
  gtk_text_buffer_get_end_iter(message_buffer, &end);
  gchar *message = gtk_text_buffer_get_text(message_buffer, &start, &end, FALSE);
  
  if(log_entry_handler->log_entry == NULL) {
    log_t *log = log_load(log_entry_handler->log_name);
    log_entry_t *new_entry = log_entry_new(log->size + 1, message);
    if(new_entry == NULL) {
      g_print("Failed to save entry.");
      return;
    }
    log_entry_create(new_entry, log_entry_handler->log_name);
    log_entry_free(new_entry);
    log_free(log);
  }
  else {

    g_print("updating entry...");
    log_entry_update(log_entry_handler->log_entry, log_entry_handler->log_name, message); 
  }
  g_free(message);
  
  GtkWidget *boxwidget = gtk_stack_get_child_by_name(GTK_STACK(user_data), "logentrylist");
  clear_elements(boxwidget, BUTTON);
  render_log_entries(boxwidget, log_entry_handler->log_name, user_data);
  render_new_button(boxwidget, log_entry_handler->log_name, user_data);

}

void new_log_entry_click(GtkButton *button, gpointer user_data) {
  gtk_stack_set_visible_child_name(GTK_STACK(user_data), "logentryview"); 
  char *log_name = g_object_get_data(G_OBJECT(button), "log_name");
  GtkWidget *boxwidget = gtk_stack_get_child_by_name(GTK_STACK(user_data), "logentryview");
  GtkBox *box = GTK_BOX(boxwidget);

  clear_elements(boxwidget, SCROLLED_WINDOW);
  render_message_box(box, NULL);
  log_entry_handler_t *log_entry_handler = log_entry_handler_new(log_name, NULL); 
  render_action_buttons(boxwidget, log_entry_handler, user_data);
}


void clear_elements(GtkWidget *boxwidget, element_type element_type) {
  GtkWidget *child = gtk_widget_get_first_child(boxwidget);

  while(child) {
    GtkWidget *next_child = gtk_widget_get_next_sibling(child);
    switch(element_type) {
      case BUTTON:
        if(GTK_IS_BUTTON(child)) { gtk_widget_unparent(child); }
        break;  

      case SCROLLED_WINDOW:
        if(GTK_IS_SCROLLED_WINDOW(child)) { gtk_widget_unparent(child); }
        break;
        
      default:
        break;
    }
    child = next_child;
  }
}

