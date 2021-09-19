#include "crack_cmdline.h"


static const gchar *crack_cmdline_default_output = "./output";


static GQuark crack_cmdline_error_quark(void)
{
    return g_quark_from_static_string("crack-cmdline-error-quark");
}


static inline void crack_cmdline_missing(GOptionEntry *entry, GError **error)
{
    g_set_error(error,
                CRYPTO_CMDLINE_ERROR,
                CRYPTO_CMDLINE_ERROR_MISSING,
                "missing mandatory parameter '--%s|-%c'",
                entry->long_name, entry->short_name);
}


gboolean crack_cmdline_parse(crack_cmdline_results_t *results, gint *argc, gchar ***argv, GError **error)
{
    GOptionEntry cmdline_entries[] = {
        { "input",     'i', 0, G_OPTION_ARG_FILENAME, &results->input_file_path,  "File to perform operation on",                          "file" },
        { "key",       'k', 0, G_OPTION_ARG_FILENAME, &results->key_file_path,    "Key file to use for operation",                         "file" },
        { "output",    'o', 0, G_OPTION_ARG_FILENAME, &results->output_file_path, "Optional location to store program's output_file_path", "file" },
        { "reference", 'r', 0, G_OPTION_ARG_FILENAME, &results->reference_path,   "Location of alphabet reference file",                   "file" },
        { "cipher",    'c', 0, G_OPTION_ARG_FILENAME, &results->cipher_lib,       "DLL to load cipher capabilities from",                  "file" },
        { NULL }
    };
    g_autoptr(GOptionContext) cmdline_ctx;

    cmdline_ctx = g_option_context_new("- Professional cracker");
    g_assert_nonnull(cmdline_ctx);

    g_option_context_add_main_entries(cmdline_ctx, cmdline_entries, NULL);

    if (!g_option_context_parse(cmdline_ctx, argc, argv, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    if (results->input_file_path == NULL) {
        crack_cmdline_missing(cmdline_entries, error);
        return FALSE;
    }

    if (results->key_file_path == NULL) {
        crack_cmdline_missing(cmdline_entries + 1, error);
        return FALSE;
    }

    if (results->reference_path == NULL) {
        crack_cmdline_missing(cmdline_entries + 2, error);
        return FALSE;
    }

    if (results->output_file_path == NULL) {
        results->output_file_path = g_strdup(crack_cmdline_default_output);
        g_assert_nonnull(results->output_file_path);
    }

    if (results->cipher_lib == NULL) {
        crack_cmdline_missing(cmdline_entries + 4, error);
        return FALSE;
    }

    return TRUE;
}


void crack_cmdline_cleanup(crack_cmdline_results_t *results)
{
    g_assert_nonnull(results);

    if (results->input_file_path != NULL) {
        g_free(results->input_file_path);
        results->input_file_path = NULL;
    }

    if (results->key_file_path != NULL) {
        g_free(results->key_file_path);
        results->key_file_path = NULL;
    }

    if (results->output_file_path != NULL) {
        g_free(results->output_file_path);
        results->output_file_path = NULL;
    }

    if (results->cipher_lib != NULL) {
        g_free(results->cipher_lib);
        results->output_file_path = NULL;
    }
}
