#include "accman_app.h"
#include "gtk_helpers.h"
#include "helpers.h"
#include "tox_errors.h"
#include "toxcore/tox.h"
#include "toxencryptsave/toxencryptsave.h"

struct _AccmanApp {
    GtkApplicationWindow parent_instance;

    GtkHeaderBar *header_bar;
    GtkStackSwitcher *stack_switcher;
    GtkStack *stack;
    GtkBox *login_box;
    GtkBox *create_box;
    GtkEntry *username;
    GtkEntry *password;
};

G_DEFINE_TYPE(AccmanApp, accman_app, GTK_TYPE_APPLICATION_WINDOW)

// TODO: move multiple GTK_XXX(yyy) into distinct variable ?

static void
refresh(AccmanApp const *const self)
{
    // remove all child
    GtkWidget *child;
    GtkWidget *box = GTK_WIDGET(self->login_box);
    while ((child = gtk_widget_get_first_child(box)) != NULL) {
        gtk_box_remove(self->login_box, child);
    }

    // list all files
    GError *error = NULL;
    GDir *const dir = g_dir_open(get_tox_config_dir(), 0, &error);
    if (error != NULL) {
        gtk_message_dialog_error(GTK_WINDOW(self), error->message);
        g_error_free(error);
    }

    char const *filename = NULL;
    while ((filename = g_dir_read_name(dir)) != NULL) {
        // filename = (stem + ext)
        if (!g_str_has_suffix(filename, ".tox")) {
            continue;
        }

        // check if file is not a directory
        char const *const filepath = get_tox_profile_path(filename);
        if (!g_file_test(filepath, G_FILE_TEST_IS_REGULAR)) {
            g_free((gpointer)filepath);
        }

        // create stem
        GString *stem = g_string_new(filename);
        g_string_replace(stem, ".tox", "", 0);
        GtkWidget *button = gtk_button_new_with_label(stem->str);
        gtk_widget_set_size_request(button, -1, 50);
        // TODO: connect signal clicked
        gtk_box_append(self->login_box, button);
        g_string_free(stem, true);
    }
    g_dir_close(dir);

    // no child
    if (gtk_widget_get_first_child(box) == NULL) {
        GtkLabel *label = GTK_LABEL(gtk_label_new("No profile yet !"));
        gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_CENTER);
        gtk_widget_set_valign(GTK_WIDGET(label), GTK_ALIGN_CENTER);
        gtk_widget_set_size_request(GTK_WIDGET(label), -1, 50);
        gtk_box_append(self->login_box, GTK_WIDGET(label));
    }
}

// TODO: rw
static void
create_profile(AccmanApp const *const self)
{
    // check username
    if (gtk_entry_get_text_length(self->username) == 0) {
        gtk_message_dialog_error(GTK_WINDOW(self), "Profile username is required.");
        return;
    }

    // check if profile already exists
    char const *const profile_path = get_tox_profile_path(gtk_entry_get_text(self->username));
    if (g_file_test(profile_path, G_FILE_TEST_IS_REGULAR | G_FILE_TEST_EXISTS)) {
        gtk_message_dialog_error(GTK_WINDOW(self), "Profile already exists.");
        goto cleanup_1;
    }

    // construct tox instance
    Tox_Err_New err = TOX_ERR_NEW_OK;
    Tox *const tox = tox_new(NULL, &err);
    if (err != TOX_ERR_NEW_OK) {
        gtk_message_dialog_error(GTK_WINDOW(self), tox_error_message(err));
        goto cleanup_1;
    }

    // get tox instance data
    size_t savedata_size = tox_get_savedata_size(tox);
    uint8_t *savedata = g_malloc(savedata_size);
    tox_get_savedata(tox, savedata);

    // password provided ? So let's encrypt the data before saving
    size_t password_length = gtk_entry_get_text_length(self->password);
    if (password_length != 0) {
        uint8_t const *const password = (uint8_t const *const)gtk_entry_get_text(self->password);
        size_t cipher_size = savedata_size + tox_pass_encryption_extra_length();
        uint8_t *const cipher = g_malloc(cipher_size);
        Tox_Err_Encryption err = TOX_ERR_ENCRYPTION_OK;

        tox_pass_encrypt(savedata, savedata_size, password, password_length, cipher, &err);

        if (err != TOX_ERR_ENCRYPTION_OK) {
            g_free(cipher);
            gtk_message_dialog_error(GTK_WINDOW(self), tox_error_message(err));
            goto cleanup_2;
        }

        g_free(savedata);
        savedata = cipher;
        savedata_size = cipher_size;
    }

    // write profile to file
    GError *error = NULL;
    g_file_set_contents(profile_path, (char const *)savedata, savedata_size, &error);
    if (error != NULL) {
        gtk_message_dialog_error(GTK_WINDOW(self), error->message);
        g_error_free(error);
        goto cleanup_2;
    }

    // success !
    gtk_message_dialog_info(GTK_WINDOW(self), "Profile created successfully !");

    // clean ui afterall
    gtk_stack_set_visible_child(self->stack, GTK_WIDGET(self->login_box));
    gtk_entry_clear(self->username);
    gtk_entry_clear(self->password);

cleanup_2:
    g_free(savedata);
    tox_kill(tox);
cleanup_1:
    g_free((gpointer)profile_path);
}

static void
init_login_box(AccmanApp *const self)
{
    self->login_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    refresh(self);
}

static void
init_create_box(AccmanApp *const self)
{
    // create username's entry
    GtkEntry *username = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(username, "Profile name");

    // create password's entry
    GtkEntry *password = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(password, "Password (optional)");
    gtk_entry_set_visibility(password, false);

    // create submit button
    GtkButton *submit = GTK_BUTTON(gtk_button_new_with_label("Submit"));
    GtkCenterBox *center_box = GTK_CENTER_BOX(gtk_center_box_new());
    gtk_center_box_set_center_widget(center_box, GTK_WIDGET(submit));
    g_signal_connect_swapped(submit, "clicked", G_CALLBACK(create_profile), self);

    // create the box itself
    GtkBox *create_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 20));
    gtk_widget_set_margin(GTK_WIDGET(create_box), 20);
    gtk_box_append(create_box, GTK_WIDGET(username));
    gtk_box_append(create_box, GTK_WIDGET(password));
    gtk_box_append(create_box, GTK_WIDGET(center_box));

    self->username = username;
    self->password = password;
    self->create_box = create_box;
}

static void
accman_app_class_init(AccmanAppClass *const klass)
{
    (void)klass;
}

static void
accman_app_init(AccmanApp *const self)
{
    // init panels
    init_login_box(self);
    init_create_box(self);

    // create stack
    GtkStack *stack = GTK_STACK(gtk_stack_new());
    gtk_stack_set_transition_duration(stack, 250);
    gtk_stack_set_transition_type(stack, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_vhomogeneous(stack, false);
    gtk_stack_set_interpolate_size(stack, true);
    gtk_stack_add_titled(stack, GTK_WIDGET(self->login_box), "Login", "Login");
    gtk_stack_add_titled(stack, GTK_WIDGET(self->create_box), "Register", "Register");
    // by default will autofocus stack_switcher, so remove this behavior
    g_signal_connect_swapped(stack, "notify::visible-child", G_CALLBACK(gtk_window_unfocus), self);

    // create stack_switcher
    GtkStackSwitcher *stack_switcher = GTK_STACK_SWITCHER(gtk_stack_switcher_new());
    gtk_stack_switcher_set_stack(stack_switcher, stack);

    // create header_bar
    GtkHeaderBar *header_bar = GTK_HEADER_BAR(gtk_header_bar_new());
    gtk_header_bar_pack_start(header_bar, GTK_WIDGET(stack_switcher));

    // window's settings
    gtk_window_set_default_size(GTK_WINDOW(self), 500, 0);
    gtk_window_set_resizable(GTK_WINDOW(self), false);
    gtk_window_set_titlebar(GTK_WINDOW(self), GTK_WIDGET(header_bar));
    gtk_window_set_child(GTK_WINDOW(self), GTK_WIDGET(stack));
    // by default will autofocus first child, so remove this behavior
    g_signal_connect(self, "notify::visible", G_CALLBACK(gtk_window_unfocus), NULL);

    self->stack = stack;
    self->stack_switcher = stack_switcher;
    self->header_bar = header_bar;
}

AccmanApp *
accman_app_new(GtkApplication const *const application)
{
    return g_object_new(ACCMAN_TYPE_APP, "application", application, NULL);
}
