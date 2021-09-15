#include "crypto_cmdline.h"


static const gchar *crypto_cmdline_default_output = "./output";


static GQuark crypto_cmdline_error_quark(void)
{
    return g_quark_from_static_string("crypto-cmdline-error-quark");
}


gboolean crypto_cmdline_parse(crypto_cmdline_results_t *results, gint *argc, gchar ***argv, GError **error)
{
    GOptionEntry cmdline_entries[] = {
        { "encrypt", 'e', 0, G_OPTION_ARG_NONE,     &results->toggled_encrypt,  "Encrypt provided input_file_path with provided key_file_path",             NULL },
        { "decrypt", 'd', 0, G_OPTION_ARG_NONE,     &results->toggled_decrypt,  "Decrypt previously encrypted input_file_path with provided key_file_path", NULL },
        { "input",   'i', 0, G_OPTION_ARG_FILENAME, &results->input_file_path,  "File to perform operation on",                          "file" },
        { "key",     'k', 0, G_OPTION_ARG_FILENAME, &results->key_file_path,    "Key file to use for operation",                         "file" },
        { "output",  'o', 0, G_OPTION_ARG_FILENAME, &results->output_file_path, "Optional location to store program's output_file_path", "file" },
        { "verbose", 'v', 0, G_OPTION_ARG_NONE,     &results->toggled_verbose,  "Be verbose (TODO)",                                                        NULL },
        { NULL }
    };
    g_autoptr(GOptionContext) cmdline_ctx;

    cmdline_ctx = g_option_context_new(" - Caesar++ encryptor/decrypter");
    g_assert_nonnull(cmdline_ctx);

    g_option_context_add_main_entries(cmdline_ctx, cmdline_entries, NULL);

    if (!g_option_context_parse(cmdline_ctx, argc, argv, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    if (results->toggled_encrypt == results->toggled_decrypt) {
        if (!results->toggled_encrypt) {
            g_set_error(error,
                        CRYPTO_CMDLINE_ERROR,
                        CRYPTO_CMDLINE_ERROR_NONE,
                        "One of '--%s|-%c' or '--%s|-%c' must be used, but none provided",
                        cmdline_entries[0].long_name, cmdline_entries[0].short_name,
                        cmdline_entries[1].long_name, cmdline_entries[1].short_name);
            return FALSE;
        } else {
            g_set_error(error,
                        CRYPTO_CMDLINE_ERROR,
                        CRYPTO_CMDLINE_ERROR_BOTH,
                        "Either '--%s|-%c' or '--%s|-%c' must be used, but not both",
                        cmdline_entries[0].long_name, cmdline_entries[0].short_name,
                        cmdline_entries[1].long_name, cmdline_entries[1].short_name);
            return FALSE;
        }
    }

    if (results->input_file_path == NULL) {
        g_set_error(error,
                    CRYPTO_CMDLINE_ERROR,
                    CRYPTO_CMDLINE_ERROR_MISSING,
                    "Missing mandatory parameter '--%s|-%c'",
                    cmdline_entries[2].long_name, cmdline_entries[2].short_name);
        return FALSE;
    }

    if (results->key_file_path == NULL) {
        g_set_error(error,
                    CRYPTO_CMDLINE_ERROR,
                    CRYPTO_CMDLINE_ERROR_MISSING,
                    "Missing mandatory parameter '--%s|-%c'",
                    cmdline_entries[3].long_name, cmdline_entries[3].short_name);
        return FALSE;
    }

    if (results->output_file_path == NULL) {
        results->output_file_path = g_strdup(crypto_cmdline_default_output);
        g_assert_nonnull(results->output_file_path);
    }

    return TRUE;
}


void crypto_cmdline_cleanup(crypto_cmdline_results_t *results)
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
}
