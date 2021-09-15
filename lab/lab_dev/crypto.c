#include "crypto_cmdline.h"
#include "crypto_io.h"

#include "crypto_caesar.h"

#include "crypto.h"


static crypto_cipher_t *crypto_get_caesar(GError **error);


static GQuark crypto_main_error_quark(void)
{
    return g_quark_from_static_string("crypto-main-error-quark");
}


int main(gint argc, gchar *argv[])
{
    crypto_cmdline_results_t cmd_results = {};
    crypto_cipher_t *cipher = NULL;
    GError *error = NULL;
    gboolean rv;

    /* Checking, if dynamic module loading is supported */
    if (!g_module_supported()) {
        g_printerr("Unfortunately, dynamic loading is not supported; "
                   "honestly, I don't know what the best to do now :(\n");
        exit(EXIT_FAILURE);
    }

    /* Parsing command-line arguments */
    if (!crypto_cmdline_parse(&cmd_results, &argc, &argv, &error)) {
        g_printerr("Failed to parse command-line arguments: %s\n", error->message);
        crypto_cmdline_cleanup(&cmd_results);
        exit(EXIT_FAILURE);
    }

    /* Getting cipher */
    /* TODO: add option to choose ciphers */
    if ((cipher = crypto_get_caesar(&error)) == NULL) {
        g_printerr("%s\n", error->message);
        exit(EXIT_FAILURE);
    }

    /* Doing the job */
    if (cmd_results.toggled_encrypt) {
        rv = crypto_encrypt_file(cmd_results.input_file_path,
                                 cmd_results.key_file_path,
                                 cmd_results.output_file_path,
                                 cipher, &error);
        if (!rv) {
            g_printerr("Failed to encrypt '%s' with keyfile '%s': %s\n",
                       cmd_results.input_file_path, cmd_results.key_file_path,
                       error->message);
            crypto_cmdline_cleanup(&cmd_results);
            exit(EXIT_FAILURE);
        }
    } else {
        rv = crypto_decrypt_file(cmd_results.input_file_path,
                                 cmd_results.key_file_path,
                                 cmd_results.output_file_path,
                                 cipher, &error);
        if (!rv) {
            g_printerr("Failed to decrypt '%s' with keyfile '%s': %s\n",
                       cmd_results.input_file_path, cmd_results.key_file_path,
                       error->message);
            crypto_cmdline_cleanup(&cmd_results);
            exit(EXIT_FAILURE);
        }
    }

    /* Final cleanup */
    crypto_cmdline_cleanup(&cmd_results);

    exit(EXIT_SUCCESS);
}


static crypto_cipher_t *crypto_get_caesar(GError **error)
{
    GModule *caesar_module;
    crypto_cipher_t *ret;
    gchar *module_path;

    module_path = g_module_build_path(".", CRYPTO_CAESAR_MODULE_FILENAME);
    g_assert_nonnull(module_path);

    if ((caesar_module = g_module_open(module_path, G_MODULE_BIND_LAZY)) == NULL) {
        g_free(module_path);
        g_set_error(error,
                    CRYPTO_MAIN_ERROR,
                    CRYPTO_MAIN_ERROR_MODULE_OPEN,
                    "Failed to open module %s", g_module_error());
        return NULL;
    }
    g_free(module_path);
    g_module_make_resident(caesar_module);

    if (!g_module_symbol(caesar_module, "crypto_cypher_caesar_decl", (gpointer *) &ret)) {
        g_set_error(error,
                    CRYPTO_MAIN_ERROR,
                    CRYPTO_MAIN_ERROR_MODULE_OPEN,
                    "Failed to load symbol 'crypto_cypher_caesar_decl' %s",
                    g_module_error());
        return NULL;
    }

    return ret;
}


static crypto_cipher_t *crypto_get_simple_table(GError **error)
{

}