#include <glib.h>
#include <gmodule.h>

#include "../crypto/crypto.h"
#include "crack_cmdline.h"
#include "crack_process.h"

#define CRACK_MAIN_ERROR crack_main_error_quark()


typedef enum {
    CRACK_MAIN_ERROR_FAILED,
    CRACK_MAIN_ERROR_MODULE_OPEN,
    CRACK_MAIN_ERROR_MODULE_SYMBOL
} crack_main_error;


static crypto_cipher_t *crack_get_cipher(gchar *name, GError **error);


static GQuark crack_main_error_quark(void)
{
    return g_quark_from_static_string("crack-main-error-quark");
}


int main(gint argc, gchar **argv)
{
    g_autoptr(crypto_cipher_t) cipher = NULL;
    crack_cmdline_results_t cmd_results = {};
    GHashTable *letter_stats;
    GError *error = NULL;

    /* Checking, if dynamic module loading is supported */
    if (!g_module_supported()) {
        g_printerr("Unfortunately, dynamic loading is not supported; "
                   "honestly, I don't know what the best to do now :(\n");
        exit(EXIT_FAILURE);
    }

    /* Parsing command-line arguments */
    if (!crack_cmdline_parse(&cmd_results, &argc, &argv, &error)) {
        g_printerr("Failed to parse command-line arguments: %s\n", error->message);
        crack_cmdline_cleanup(&cmd_results);
        exit(EXIT_FAILURE);
    }

    /* Getting statistics */
    letter_stats = crack_process_statistics_file(cmd_results.reference_path, &error);
    if (letter_stats == NULL) {
        g_printerr("Failed to get letter statistics: %s\n", error->message);
        exit(EXIT_FAILURE);
    }

    /* Getting cipher facilities */
    cipher = crack_get_cipher(cmd_results.cipher_lib, &error);
    if (cipher == NULL) {
        g_printerr("Failed to get cipher structure: %s\n", error->message);
        exit(EXIT_FAILURE);
    }

    /* Trying to crack the thing... */
    /* TODO: add more crack methods */
    if (!crack_bruteforce_numeric_key(cipher->decrypt_cb, letter_stats,
                                      cmd_results.input_file_path,
                                      cmd_results.output_file_path,
                                      &error)) {
        g_printerr("Failed to crack the cipher: %s\n", error->message);
    }

    crack_cmdline_cleanup(&cmd_results);

    exit(EXIT_SUCCESS);
}


static crypto_cipher_t *crack_get_cipher(gchar *name, GError **error)
{
    g_autoptr(GString) symbol_name = NULL;
    GModule *caesar_module;
    crypto_cipher_t *ret;
    gchar *module_path;

    g_assert_nonnull(name);

    module_path = g_module_build_path(".", name);
    g_assert_nonnull(module_path);

    if ((caesar_module = g_module_open(module_path, G_MODULE_BIND_LAZY)) == NULL) {
        g_free(module_path);
        g_set_error(error,
                    CRACK_MAIN_ERROR,
                    CRACK_MAIN_ERROR_MODULE_OPEN,
                    "failed to open module %s", g_module_error());
        return NULL;
    }
    g_free(module_path);
    g_module_make_resident(caesar_module);

    symbol_name = g_string_new(NULL);
    g_assert_nonnull(symbol_name);

    g_string_printf(symbol_name, "%s_decl", name);

    if (!g_module_symbol(caesar_module, symbol_name->str, (gpointer *) &ret)) {
        g_set_error(error,
                    CRACK_MAIN_ERROR,
                    CRACK_MAIN_ERROR_MODULE_SYMBOL,
                    "failed to load symbol '%s' %s",
                    symbol_name->str,
                    g_module_error());
        return NULL;
    }

    return ret;
}
