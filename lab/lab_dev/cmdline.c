#include "cmdline.h"


static const gchar *cmdline_default_output = "./output";


static GQuark cmdline_error_quark(void)
{
    return g_quark_from_static_string("cmdline-error-quark");
}


gboolean cmdline_parse(struct cmdline_results *results, gint *argc, gchar ***argv, GError **error)
{
    GOptionEntry cmdline_entries[] = {
            { "encrypt", 'e', 0, G_OPTION_ARG_NONE,     &results->toggled_encrypt, "Encrypt provided input with provided key",             NULL },
            { "decrypt", 'd', 0, G_OPTION_ARG_NONE,     &results->toggled_decrypt, "Decrypt previously encrypted input with provided key", NULL },
            { "input",   'i', 0, G_OPTION_ARG_FILENAME, &results->input,           "File to perform operation on",                "file" },
            { "key",     'k', 0, G_OPTION_ARG_FILENAME, &results->key,             "Key file to use for operation",               "file" },
            { "output",  'o', 0, G_OPTION_ARG_FILENAME, &results->output,          "Optional location to store program's output", "file" },
            { NULL }
    };
    GOptionContext *cmdline_ctx;

    cmdline_ctx = g_option_context_new(" - Caesar++ encryptor/decrypter");
    g_assert_nonnull(cmdline_ctx);

    g_option_context_add_main_entries(cmdline_ctx, cmdline_entries, NULL);

    if (!g_option_context_parse(cmdline_ctx, argc, argv, error)) {
        g_option_context_free(cmdline_ctx);

        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    g_option_context_free(cmdline_ctx);

    if (results->toggled_encrypt == results->toggled_decrypt) {
        if (!results->toggled_encrypt) {
            g_set_error(error,
                        CMDLINE_ERROR,
                        CMDLINE_ERROR_NONE,
                        "One of '--%s|-%c' or '--%s|-%c' must be used, but none provided",
                        cmdline_entries[0].long_name, cmdline_entries[0].short_name,
                        cmdline_entries[1].long_name, cmdline_entries[1].short_name);
            return FALSE;
        } else {
            g_set_error(error,
                        CMDLINE_ERROR,
                        CMDLINE_ERROR_BOTH,
                        "Either '--%s|-%c' or '--%s|-%c' must be used, but not both",
                        cmdline_entries[0].long_name, cmdline_entries[0].short_name,
                        cmdline_entries[1].long_name, cmdline_entries[1].short_name);
            return FALSE;
        }
    }

    if (results->input == NULL) {
        g_set_error(error,
                    CMDLINE_ERROR,
                    CMDLINE_ERROR_MISSING,
                    "Missing mandatory parameter '--%s|-%c'",
                    cmdline_entries[2].long_name, cmdline_entries[2].short_name);
        return FALSE;
    }

    if (results->key == NULL) {
        g_set_error(error,
                    CMDLINE_ERROR,
                    CMDLINE_ERROR_MISSING,
                    "Missing mandatory parameter '--%s|-%c'",
                    cmdline_entries[3].long_name, cmdline_entries[3].short_name);
        return FALSE;
    }

    if (results->output == NULL) {
        results->output = g_strdup(cmdline_default_output);
        g_assert_nonnull(results->output);
    }

    return TRUE;
}


void cmdline_cleanup(struct cmdline_results *results)
{
    g_assert_nonnull(results);

    if (results->input != NULL) {
        g_free(results->input);
        results->input = NULL;
    }

    if (results->key != NULL) {
        g_free(results->key);
        results->key = NULL;
    }

    if (results->output != NULL) {
        g_free(results->output);
        results->output = NULL;
    }
}
