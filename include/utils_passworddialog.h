#pragma once

#include <gtk/gtk.h>

#define UTILS_TYPE_PASSWORD_DIALOG (utils_password_dialog_get_type())
G_DECLARE_FINAL_TYPE(UtilsPasswordDialog, utils_password_dialog, UTILS, PASSWORD_DIALOG, GtkDialog)

UtilsPasswordDialog *utils_password_dialog_new(GtkWindow const *const parent,
                                               void const *const userdata);
