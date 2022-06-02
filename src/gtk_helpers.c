#include "gtk_helpers.h"

void
gtk_widget_set_margin(GtkWidget *const widget, int margin)
{
    gtk_widget_set_margin_top(widget, margin);
    gtk_widget_set_margin_end(widget, margin);
    gtk_widget_set_margin_bottom(widget, margin);
    gtk_widget_set_margin_start(widget, margin);
}

void
gtk_window_unfocus(GtkWindow *const window)
{
    gtk_window_set_focus(window, NULL);
}

void
gtk_entry_clear(GtkEntry *const entry)
{
    GtkEntryBuffer *const buffer = gtk_entry_get_buffer(entry);
    gtk_entry_buffer_delete_text(buffer, 0, -1);
}

char const *
gtk_entry_get_text(GtkEntry *const entry)
{
    GtkEntryBuffer *const buffer = gtk_entry_get_buffer(entry);
    return gtk_entry_buffer_get_text(buffer);
}

static void
gtk_message_dialog_default(GtkWindow *const parent, GtkMessageType type, char const *const text)
{
    GtkMessageDialog *message_dialog = GTK_MESSAGE_DIALOG(gtk_message_dialog_new(
        parent, GTK_DIALOG_USE_HEADER_BAR, type, GTK_BUTTONS_OK, "%s", text));
    g_signal_connect(message_dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
    gtk_window_present(GTK_WINDOW(message_dialog));
}

void
gtk_message_dialog_error(GtkWindow *const parent, char const *const text)
{
    gtk_message_dialog_default(parent, GTK_MESSAGE_ERROR, text);
}

void
gtk_message_dialog_info(GtkWindow *const parent, char const *const text)
{
    gtk_message_dialog_default(parent, GTK_MESSAGE_INFO, text);
}
