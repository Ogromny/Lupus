#pragma once

#include <gtk/gtk.h>

void gtk_widget_set_margin(GtkWidget *const widget, int margin);
void gtk_window_unfocus(GtkWindow *const window);
void gtk_entry_clear(GtkEntry *const entry);
char const *gtk_entry_get_text(GtkEntry *const entry);
void gtk_message_dialog_error(GtkWindow *const parent, char const *const text);
void gtk_message_dialog_info(GtkWindow *const parent, char const *const text);
