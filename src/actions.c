#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "actions.h"

static void void_widget(GtkWidget *widget) {
  (void)widget;
}

void render_log_entries(GtkWidget *stack_box_widget, char *label, gpointer main_stack) {
  gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "logentrylist"); 
  GtkBox *stack_box = GTK_BOX(stack_box_widget);
  GtkWidget *box_label = gtk_widget_get_first_child(stack_box_widget);
  size_t buffer = strlen(label) + 10;
  char label_prefix[buffer];
  strcpy(label_prefix,"Viewing ");
  gtk_label_set_label(GTK_LABEL(box_label), strcat(label_prefix, label));
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
        GtkWidget *log_entry_button = gtk_button_new_with_label(entry->datetime);
        g_object_set_data(G_OBJECT(log_entry_button), "entry", entry);
        g_object_set_data(G_OBJECT(log_entry_button), "log_name", log->name);
        gtk_box_append(stack_box,log_entry_button);
        g_signal_connect(log_entry_button, "clicked", G_CALLBACK(log_entry_click), main_stack);
    } 
    i++;
  }
}

void return_to_log_click(GtkWidget *return_button_widget, gpointer main_stack) {
  log_entry_handler_t *log_entry_handler = g_object_get_data(G_OBJECT(return_button_widget), "entry_handler");
  gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "logentrylist"); 
  log_entry_handler_free(log_entry_handler);
}

void render_new_log_entry_button(GtkWidget *stack_box_widget, char *log_name, gpointer main_stack) {
  GtkBox *stack_box = GTK_BOX(stack_box_widget);
  GtkWidget *new_log_entry_button = gtk_button_new_with_label("Add New Entry");
  g_object_set_data(G_OBJECT(new_log_entry_button), "log_name", log_name);
  gtk_box_append(stack_box, new_log_entry_button);
  g_signal_connect(new_log_entry_button, "clicked", G_CALLBACK(new_log_entry_click), main_stack);
}

void render_message_box(GtkBox *stack_box, char *log_entry_message) {
  GtkWidget *message_scrolled_container = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(message_scrolled_container, TRUE);
  gtk_widget_set_hexpand(message_scrolled_container, TRUE);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(message_scrolled_container), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  
  GtkWidget *message_text_view = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(message_text_view), GTK_WRAP_WORD_CHAR);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(message_scrolled_container), message_text_view);
  gtk_box_append(GTK_BOX(stack_box), message_scrolled_container);
  
  if(log_entry_message != NULL) {
    GtkTextBuffer *message = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_text_view));
    gtk_text_buffer_set_text(message,  log_entry_message, -1);
  }
}

void render_action_buttons(GtkWidget *stack_box_widget, log_entry_handler_t *log_entry_handler, gpointer main_stack) {
  clear_child_elements(stack_box_widget, BUTTON);
  GtkBox *stack_box = GTK_BOX(stack_box_widget);
  GtkWidget *button = gtk_button_new_with_label("Save");
  gtk_box_append(stack_box, button);
  g_object_set_data(G_OBJECT(button), "entry_handler", log_entry_handler);
  g_signal_connect(button, "clicked", G_CALLBACK(save_log_entry_click), main_stack);
  button = gtk_button_new_with_label("Return");
  g_object_set_data(G_OBJECT(button), "entry_handler", log_entry_handler);
  g_signal_connect(button, "clicked", G_CALLBACK(return_to_log_click), main_stack);
  gtk_box_append(stack_box, button);
}

void new_log_click(GtkWidget *create_log_button_widget, gpointer main_stack) {
  void_widget(create_log_button_widget);
  gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "viewlogpage");
  log_t *first_log = log_create("Test Log");
  if (first_log == NULL) {
    g_print("Failed to create log\n");
  }
  log_free(first_log);
}

void log_list_click(GtkWidget *open_log_button_widget, gpointer main_stack) {
  void_widget(open_log_button_widget);

  GtkWidget *log_list_page_box_widget = gtk_stack_get_child_by_name(GTK_STACK(main_stack), "loglistpage");
  if(log_list_page_box_widget == NULL) {
    g_print("Could not find log list log_list_page_box");
  }
  gtk_stack_set_visible_child(GTK_STACK(main_stack), log_list_page_box_widget);
  GtkBox *log_list_page_box = GTK_BOX(log_list_page_box_widget);

  if(gtk_widget_get_first_child(log_list_page_box_widget) != gtk_widget_get_last_child(log_list_page_box_widget)) { return; }

  log_list_t *log_list = log_list_get();
  if(log_list != NULL) {
    size_t i = 0;
    while(i < log_list->filecount) {
      char *log_name = log_list->log_names[i];
      if(log_name != NULL) { 
        g_print("Log: %s\n", log_name);
        GtkWidget *log_button = gtk_button_new_with_label(log_name);
        gtk_box_append(log_list_page_box, log_button); 
        g_signal_connect(log_button, "clicked", G_CALLBACK(view_log_click), main_stack);
      }
      i++;
    }
  }

  log_list_free(log_list);
}

void view_log_click(GtkWidget *log_entry_widget, gpointer main_stack) {
  const char* label = gtk_button_get_label(GTK_BUTTON(log_entry_widget));
  GtkWidget *log_entry_list_box_widget = gtk_stack_get_child_by_name(GTK_STACK(main_stack), "logentrylist");

  clear_child_elements(log_entry_list_box_widget, BUTTON);
  render_log_entries(log_entry_list_box_widget, label, main_stack);
  render_new_log_entry_button(log_entry_list_box_widget, label, main_stack);
}

void save_log_click(GtkButton *save_button, gpointer user_data) {
  (void)save_button;
  (void)user_data;
}

void delete_log_click(GtkButton *delete_button, gpointer user_data) {
  (void)delete_button;
  (void)user_data;
}

void log_entry_click(GtkButton *log_entry_list_button, gpointer main_stack) {
  gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "logentryview"); 
  log_entry_t *log_entry = g_object_get_data(G_OBJECT(log_entry_list_button), "entry");
  char *log_name = g_object_get_data(G_OBJECT(log_entry_list_button), "log_name");
  
  GtkWidget *stack_box_widget = gtk_stack_get_child_by_name(GTK_STACK(main_stack), "logentryview");
  GtkBox *stack_box = GTK_BOX(stack_box_widget);

  clear_child_elements(stack_box_widget, SCROLLED_WINDOW);
  render_message_box(stack_box, log_entry->message);
  log_entry_handler_t *log_entry_handler = log_entry_handler_new(log_name, log_entry); 

  render_action_buttons(stack_box_widget, log_entry_handler, main_stack);
}

void save_log_entry_click(GtkWidget *save_log_entry_button_widget, gpointer main_stack) {
  log_entry_handler_t *log_entry_handler = g_object_get_data(G_OBJECT(save_log_entry_button_widget), "entry_handler");
  GtkWidget *message_scolled_container = gtk_widget_get_prev_sibling(save_log_entry_button_widget);
  GtkWidget *message_text_view = gtk_widget_get_first_child(message_scolled_container);
  GtkTextBuffer *message_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_text_view));
  
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(message_buffer, &start);
  gtk_text_buffer_get_end_iter(message_buffer, &end);
  gchar *message = gtk_text_buffer_get_text(message_buffer, &start, &end, FALSE);
  
  if(log_entry_handler->log_entry == NULL) {
    log_t *log = log_load(log_entry_handler->log_name);
    log_entry_t *new_log_entry = log_entry_new(log->size + 1, message);
    if(new_log_entry == NULL) {
      g_print("Failed to save entry.");
      return;
    }
    log_entry_create(new_log_entry, log_entry_handler->log_name);
    log_entry_free(new_log_entry);
    log_free(log);
  }
  else {

    g_print("updating entry...");
    log_entry_update(log_entry_handler->log_entry, log_entry_handler->log_name, message); 
  }
  g_free(message);
  
  GtkWidget *log_entry_list_box_widget = gtk_stack_get_child_by_name(GTK_STACK(main_stack), "logentrylist");
  clear_child_elements(log_entry_list_box_widget, BUTTON);
  render_log_entries(log_entry_list_box_widget, log_entry_handler->log_name, main_stack);
  render_new_log_entry_button(log_entry_list_box_widget, log_entry_handler->log_name, main_stack);
}

void new_log_entry_click(GtkButton *new_log_entry_button, gpointer main_stack) {
  gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "logentryview"); 
  char *log_name = g_object_get_data(G_OBJECT(new_log_entry_button), "log_name");
  GtkWidget *stack_box_widget = gtk_stack_get_child_by_name(GTK_STACK(main_stack), "logentryview");
  GtkBox *stack_box = GTK_BOX(stack_box_widget);

  clear_child_elements(stack_box_widget, SCROLLED_WINDOW);
  render_message_box(stack_box, NULL);
  log_entry_handler_t *log_entry_handler = log_entry_handler_new(log_name, NULL); 
  render_action_buttons(stack_box_widget, log_entry_handler, main_stack);
}


void clear_child_elements(GtkWidget *stack_box_widget, element_type element_type) {
  GtkWidget *child = gtk_widget_get_first_child(stack_box_widget);

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

