#include "helpers.h"
#include "glib.h"

char const *
get_tox_config_dir(void)
{
    static char dir[PATH_MAX];

    if (dir[0] == '\0') {
        char *const path = g_build_filename(g_get_user_config_dir(), "tox", NULL);
        strcat(dir, path);
        g_free(path);
    }

    return dir;
}

char const *
get_tox_profile_path(char const *const name)
{
    if (g_str_has_suffix(name, ".tox")) {
        return g_build_filename(get_tox_config_dir(), name, NULL);
    }

    char *const filename = g_strconcat(name, ".tox", NULL);
    char const *const path = g_build_filename(get_tox_config_dir(), filename, NULL);

    g_free(filename);

    return path;
}
