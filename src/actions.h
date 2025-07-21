#include <gtk/gtk.h>
#include <stdlib.h>
#include "log.h"

typedef enum ELEMENT_TYPE {
  BUTTON,
  SCROLLED_WINDOW,
  LABEL
} element_type;

void new_log_click(GtkWidget *create_log_button_widget, gpointer main_stack); 
void view_log_click(GtkWidget *log_entry_widget, gpointer main_stack); 
void save_log_click(GtkWidget *save_button, gpointer user_data); 
void delete_log_click(GtkButton *delete_button, gpointer user_data);
void log_list_click(GtkWidget *open_log_button_widget, gpointer main_stack); 

void log_entry_click(GtkButton *log_entry_list_button, gpointer main_stack); 
void new_log_entry_click(GtkButton *new_log_entry_button, gpointer main_stack); 
void save_log_entry_click(GtkWidget *save_log_entry_button_widget, gpointer main_stack); 
void clear_child_elements(GtkWidget *stack_box_widget, element_type element_type); 
void return_to_log_click(GtkWidget *return_button_widget, gpointer main_stack); 

void render_action_buttons(GtkWidget *stack_box_widget, log_entry_handler_t *log_entry_handler, gpointer main_stack); 
void render_log_entries(GtkWidget *stack_box_widget, char *label, gpointer main_stack); 
void render_new_log_entry_button(GtkWidget *stack_box_widget, char *log_name, gpointer main_stack); 
void render_message_box(GtkBox *stack_box, char *log_entry_message); 


