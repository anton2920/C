#include <glib.h>

#include "md5.h"


#define FILE_NAME ("test.txt")


main()
{
    GString *input_string, *hashed_string;
    GIOChannel *input_file_channel;
    GError *error = NULL;
    gsize input_length;
    gchar *input_data;
    GIOStatus status;

    input_file_channel = g_io_channel_new_file(FILE_NAME, "r", &error);
    if (input_file_channel == NULL) {
        g_assert_nonnull(error);
        g_printerr("Failed to open file: %s\n", error->message);
        exit(EXIT_FAILURE);
    }

    status = g_io_channel_read_to_end(input_file_channel, &input_data, &input_length, &error);
    if (status != G_IO_STATUS_NORMAL) {
        g_assert_nonnull(error);
        g_printerr("Failed to read input file: %s\n", error->message);
    }
    g_io_channel_unref(input_file_channel);

    input_string = g_string_new(input_data);
    g_assert_nonnull(input_string);

    g_free(input_data);

    hashed_string = md5_hash_get_digest_from_string(input_string);
    if (hashed_string == NULL) {
        g_assert_nonnull(error);
        g_printerr("Failed to calculate MD5 hash from input string: %s\n", error->message);
    }

    g_print("MD5 digest: %s\n", hashed_string->str);

    g_string_free(input_string, TRUE);
    g_string_free(hashed_string, TRUE);
}
