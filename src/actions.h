#include <gtk/gtk.h>
#include <stdlib.h>

typedef enum ELEMENT_TYPE {
  BUTTON,
  SCROLLED_WINDOW,
  LABEL
} element_type ;

void new_log_click(GtkWidget *widget, gpointer user_data);
void view_log_click(GtkWidget *widget, gpointer user_data);
void save_log_click(GtkButton *button, gpointer user_data);
void delete_log_click(GtkButton *button, gpointer user_data);
void log_list_click(GtkWidget *widget, gpointer user_data);
void log_entry_click(GtkButton *button, gpointer user_data);
void clear_elements(GtkWidget *boxwidget, element_type element_type);
void render_action_buttons(gpointer *stackpage);

