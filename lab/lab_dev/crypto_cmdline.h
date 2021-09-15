#ifndef CMDLINE_H
#define CMDLINE_H

#include <glib.h>

#define CRYPTO_CMDLINE_ERROR crypto_cmdline_error_quark()

typedef struct _crypto_cmdline_results {
    gboolean toggled_encrypt;
    gboolean toggled_decrypt;
    gchar *input_file_path;
    gchar *key_file_path;
    gchar *output_file_path;
    gboolean toggled_verbose;
} crypto_cmdline_results_t;

typedef enum {
    CRYPTO_CMDLINE_ERROR_FAILED,
    CRYPTO_CMDLINE_ERROR_NONE,
    CRYPTO_CMDLINE_ERROR_BOTH,
    CRYPTO_CMDLINE_ERROR_MISSING
} crypto_cmdline_error;

gboolean crypto_cmdline_parse(crypto_cmdline_results_t *results, gint *argc, gchar ***argv, GError **error);
void crypto_cmdline_cleanup(crypto_cmdline_results_t *results);

#endif // CMDLINE_H
