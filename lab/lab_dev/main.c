#include "cmdline.h"
#include "crypt.h"


int main(gint argc, gchar *argv[])
{
    struct cmdline_results cmd_results = {};
    GError *error = NULL;

    /* Parsing command-line arguments */
    if (!cmdline_parse(&cmd_results, &argc, &argv, &error)) {
        g_printerr("Failed to parse command-line arguments: %s\n", error->message);
        exit(EXIT_FAILURE);
    }

    /* TODO: Getting all files ready */


    /* TODO: Doing the job */
    error = NULL;
    if (cmd_results.toggled_encrypt) {
        encrypt_file(input_file, key_file, output_file, error);
    } else {
        decrypt_file(input_file, key_file, output_file, error);
    }

    cmdline_cleanup(&cmd_results);

    exit(EXIT_SUCCESS);
}