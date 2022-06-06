#include "accman_app.h"

static void
activate(GApplication const *const self)
{
    AccmanApp *const accman_app = accman_app_new(GTK_APPLICATION(self));
    g_signal_connect(accman_app, "close-request", G_CALLBACK(g_object_unref), NULL);
    gtk_window_present(GTK_WINDOW(accman_app));
}

int
main(int argc, char **argv)
{
    GtkApplication *const application = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(application), argc, argv);

    g_object_unref(application);
    return status;
}
