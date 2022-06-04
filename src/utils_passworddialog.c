#include "utils_passworddialog.h"
#include "gtk_helpers.h"

struct _UtilsPasswordDialog {
    GtkDialog parent_instance;

    GtkEntry *entry;
    void const *userdata;
};

G_DEFINE_TYPE(UtilsPasswordDialog, utils_password_dialog, GTK_TYPE_DIALOG)

typedef enum {
    PROP_USERDATA = 1,
    N_PROPERTIES,
} UtilsPasswordDialogProperty;
static GParamSpec *obj_properties[N_PROPERTIES];

typedef enum {
    // void (*)(int response, char const *const password)
    SIGNAL_INPUT = 1,
    N_SIGNALS,
} UtilsPasswordDialogSignals;
static guint obj_signals[N_SIGNALS];

static void
utils_password_dialog_response(GtkDialog *const dialog, int response)
{
    UtilsPasswordDialog *self = UTILS_PASSWORD_DIALOG(dialog);

    char const *password = NULL;
    if (gtk_entry_get_text_length(self->entry)) {
        password = gtk_entry_get_text(self->entry);
    }

    g_signal_emit(self, obj_signals[SIGNAL_INPUT], 0, response, password);

    gtk_widget_hide(GTK_WIDGET(self));
}

static void
utils_password_dialog_get_property(GObject *const object, guint property_id, GValue *const value,
                                   GParamSpec *const pspec)
{
    UtilsPasswordDialog *self = UTILS_PASSWORD_DIALOG(object);

    switch ((UtilsPasswordDialogProperty)property_id) {
    case PROP_USERDATA:
        g_value_set_pointer(value, (gpointer)self->userdata);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
utils_password_dialog_set_property(GObject *const object, guint property_id,
                                   GValue const *const value, GParamSpec *const pspec)
{
    UtilsPasswordDialog *self = UTILS_PASSWORD_DIALOG(object);

    switch ((UtilsPasswordDialogProperty)property_id) {
    case PROP_USERDATA:
        self->userdata = g_value_get_pointer(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
utils_password_dialog_constructed(GObject *const object)
{
    G_OBJECT_CLASS(utils_password_dialog_parent_class)->constructed(object);

    // must put gtk_window_present in constructed instead of init, otherwise
    // gtk_window_present will presents a unfinished GtkDialog (GtkDialog
    // will not have header-bar, ...
    gtk_window_present(GTK_WINDOW(object));
}

static void
utils_password_dialog_class_init(UtilsPasswordDialogClass *const klass)
{
    GObjectClass *const object_class = G_OBJECT_CLASS(klass);
    GtkDialogClass *const dialog_class = GTK_DIALOG_CLASS(klass);

    // vfunction override
    object_class->constructed = utils_password_dialog_constructed;
    object_class->get_property = utils_password_dialog_get_property;
    object_class->set_property = utils_password_dialog_set_property;
    dialog_class->response = utils_password_dialog_response;

    // properties
    obj_properties[PROP_USERDATA] =
        g_param_spec_pointer("userdata", NULL, NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

    // signals
    obj_signals[SIGNAL_INPUT] =
        g_signal_newv("input", G_TYPE_FROM_CLASS(object_class),
                      G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS, NULL, NULL, NULL,
                      NULL, G_TYPE_NONE, 2, (GType[2]){G_TYPE_INT, G_TYPE_POINTER});

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void
utils_password_dialog_init(UtilsPasswordDialog *const self)
{
    // add headerbar button
    gtk_dialog_add_button(GTK_DIALOG(self), "Ok", GTK_RESPONSE_ACCEPT);

    // create password's entry
    GtkEntry *const entry = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entry, "Password");
    gtk_entry_set_visibility(entry, false);
    gtk_widget_set_margin(GTK_WIDGET(entry), 20);

    // append to content_area
    GtkBox *const content_area = GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(self)));
    gtk_box_append(content_area, GTK_WIDGET(entry));

    self->entry = entry;
}

UtilsPasswordDialog *
utils_password_dialog_new(GtkWindow const *const parent, void const *const userdata)
{
    // clang-format off
    return g_object_new(UTILS_TYPE_PASSWORD_DIALOG,
                        "use-header-bar", true,
                        "modal", true,
                        "title", "Password",
                        "transient-for", parent,
                        "width-request", 250,
                        "resizable", false,
                        "userdata", userdata,
                        NULL);
    // clang-format on
}
