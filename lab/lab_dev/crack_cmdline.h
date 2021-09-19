#ifndef CRACK_CMDLINE_H
#define CRACK_CMDLINE_H

#include <glib.h>

#define CRYPTO_CMDLINE_ERROR crack_cmdline_error_quark()

typedef struct _crack_cmdline_results {
    gchar *input_file_path;
    gchar *key_file_path;
    gchar *output_file_path;
    gchar *cipher_lib;
    gchar *reference_path;
} crack_cmdline_results_t;

typedef enum {
    CRYPTO_CMDLINE_ERROR_FAILED,
    CRYPTO_CMDLINE_ERROR_NONE,
    CRYPTO_CMDLINE_ERROR_MISSING
} crack_cmdline_error;

gboolean crack_cmdline_parse(crack_cmdline_results_t *results, gint *argc, gchar ***argv, GError **error);
void crack_cmdline_cleanup(crack_cmdline_results_t *results);

#endif // CRACK_CMDLINE_H
