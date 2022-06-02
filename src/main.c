#include "accman_app.h"

static void
activate(GtkApplication *app)
{
    AccmanApp *accman_app = accman_app_new(app);
    gtk_window_present(GTK_WINDOW(accman_app));
}

int
main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
