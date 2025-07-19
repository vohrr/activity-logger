#include <gtk/gtk.h>
#include <stdlib.h>
#include "log.h"

typedef enum ELEMENT_TYPE {
  BUTTON,
  SCROLLED_WINDOW,
  LABEL
} element_type;

void new_log_click(GtkWidget *widget, gpointer user_data);
void view_log_click(GtkWidget *widget, gpointer user_data);
void save_log_click(GtkButton *button, gpointer user_data);
void delete_log_click(GtkButton *button, gpointer user_data);
void log_list_click(GtkWidget *widget, gpointer user_data);

void log_entry_click(GtkButton *button, gpointer user_data);
void new_log_entry_click(GtkButton *button, gpointer user_data);
void save_log_entry_click(GtkWidget *widget, log_entry_handler_t *log_entry_handler);
void clear_elements(GtkWidget *boxwidget, element_type element_type);
void render_action_buttons(GtkWidget *boxwidget, log_entry_handler_t *log_handler, gpointer user_data);
void render_log_entries(GtkWidget *boxwidget, char *label, gpointer user_data);
void render_new_button(GtkWidget *boxwidget, char *log_name, gpointer user_data);
void render_message_box(GtkBox *box, char *entry_message);
