#ifndef CMDLINE_H
#define CMDLINE_H

#include <glib.h>

#define CMDLINE_ERROR cmdline_error_quark()

struct cmdline_results {
    gboolean toggled_encrypt;
    gboolean toggled_decrypt;
    gchar *input;
    gchar *key;
    gchar *output;
};

typedef enum {
    CMDLINE_ERROR_FAILED,
    CMDLINE_ERROR_NONE,
    CMDLINE_ERROR_BOTH,
    CMDLINE_ERROR_MISSING
} cmdline_error;

gboolean cmdline_parse(struct cmdline_results *results, gint *argc, gchar ***argv, GError **error);
void cmdline_cleanup(struct cmdline_results *results);

#endif // CMDLINE_H
