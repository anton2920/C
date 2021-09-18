#include "crypto_cmdline.h"


static const gchar *crypto_cmdline_default_output = "./output";


static GQuark crypto_cmdline_error_quark(void)
{
    return g_quark_from_static_string("crypto-cmdline-error-quark");
}


static inline void crypto_cmdline_missing(GOptionEntry *entry, GError **error)
{
    g_set_error(error,
                CRYPTO_CMDLINE_ERROR,
                CRYPTO_CMDLINE_ERROR_MISSING,
                "Missing mandatory parameter '--%s|-%c'",
                entry->long_name, entry->short_name);
}


gboolean crypto_cmdline_parse(crypto_cmdline_results_t *results, gint *argc, gchar ***argv, GError **error)
{
    GOptionEntry cmdline_entries[] = {
        { "encrypt", 'e', 0, G_OPTION_ARG_NONE,     &results->toggled_encrypt,  "Encrypt provided 'input' with provided 'key'",             NULL },
        { "decrypt", 'd', 0, G_OPTION_ARG_NONE,     &results->toggled_decrypt,  "Decrypt previously encrypted 'input' with provided 'key'", NULL },
        { "input",   'i', 0, G_OPTION_ARG_FILENAME, &results->input_file_path,  "File to perform operation on",                          "file" },
        { "key",     'k', 0, G_OPTION_ARG_FILENAME, &results->key_file_path,    "Key file to use for operation",                         "file" },
        { "output",  'o', 0, G_OPTION_ARG_FILENAME, &results->output_file_path, "Optional location to store program's output_file_path", "file" },
        { "cipher",  'c', 0, G_OPTION_ARG_FILENAME, &results->cipher_lib,       "DLL to load cipher capabilities from",                  "file" },
        { NULL }
    };
    g_autoptr(GOptionContext) cmdline_ctx;

    cmdline_ctx = g_option_context_new("- Professional encryptor/decrypter");
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
        crypto_cmdline_missing(cmdline_entries + 2, error);
        return FALSE;
    }

    if (results->key_file_path == NULL) {
        crypto_cmdline_missing(cmdline_entries + 3, error);
        return FALSE;
    }

    if (results->output_file_path == NULL) {
        results->output_file_path = g_strdup(crypto_cmdline_default_output);
        g_assert_nonnull(results->output_file_path);
    }

    if (results->cipher_lib == NULL) {
        crypto_cmdline_missing(cmdline_entries + 5, error);
        return FALSE;
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

    if (results->cipher_lib != NULL) {
        g_free(results->cipher_lib);
        results->output_file_path = NULL;
    }
}
