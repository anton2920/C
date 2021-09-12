#include "cmdline.h"
#include "crypt.h"


int main(gint argc, gchar *argv[])
{
    GIOChannel *input_file, *key_file, *output_file;
    struct cmdline_results cmd_results = {};
    GError *error = NULL;

    /* Parsing command-line arguments */
    if (!cmdline_parse(&cmd_results, &argc, &argv, &error)) {
        g_printerr("Failed to parse command-line arguments: %s\n", error->message);
        cmdline_cleanup(&cmd_results);
        exit(EXIT_FAILURE);
    }

    /* Getting all files ready */
    if ((input_file = g_io_channel_new_file(cmd_results.input, "r", &error)) == NULL) {
        g_printerr("Failed to open input file: %s\n", error->message);
        cmdline_cleanup(&cmd_results);
        exit(EXIT_FAILURE);
    }

    if ((key_file = g_io_channel_new_file(cmd_results.key, "r", &error)) == NULL) {
        g_printerr("Failed to open key file: %s\n", error->message);
        cmdline_cleanup(&cmd_results);
        exit(EXIT_FAILURE);
    }

    if ((output_file = g_io_channel_new_file(cmd_results.output, "w", &error)) == NULL) {
        g_printerr("Failed to open output file: %s\n", error->message);
        cmdline_cleanup(&cmd_results);
        exit(EXIT_FAILURE);
    }

    /* Doing the job */
    if (cmd_results.toggled_encrypt) {
        if (!encrypt_file(input_file, key_file, output_file, &error)) {
            g_printerr("Failed to encrypt '%s' with keyfile '%s': %s\n",
                       cmd_results.input, cmd_results.key,
                       error->message);
            exit(EXIT_FAILURE);
        }
    } else {
        if (!decrypt_file(input_file, key_file, output_file, &error)) {
            g_printerr("Failed to decrypt '%s' with keyfile '%s': %s\n",
                       cmd_results.input, cmd_results.key,
                       error->message);
            exit(EXIT_FAILURE);
        }
    }

    /* Final cleanup */
    cmdline_cleanup(&cmd_results);
    g_io_channel_shutdown(input_file, FALSE, &error);
    g_io_channel_shutdown(key_file, FALSE, &error);
    g_io_channel_shutdown(output_file, TRUE, &error);

    exit(EXIT_SUCCESS);
}