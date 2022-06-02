#pragma once

#include <gtk/gtk.h>

#define ACCMAN_TYPE_APP accman_app_get_type()
G_DECLARE_FINAL_TYPE(AccmanApp, accman_app, ACCMAN, APP, GtkApplicationWindow)

AccmanApp *accman_app_new(GtkApplication const *const application);
