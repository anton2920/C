#include <glib.h>
#include <gio/gio.h>

#include "md5.h"


#define FILE_NAME    ("test.txt")
#define SIZE_TO_READ (4194304) /* 4MiB */


main()
{
    md5_hash_ctx_t md5_ctx;

    g_autoptr(GFileInputStream) input_stream = NULL;
    g_autoptr(GFile) input_file = NULL;

    GError *error = NULL;
    GString *hexdigest;

    guint8 *input_data;
    gssize nbytes_read;

    md5_hash_ctx_init(&md5_ctx);

    input_file = g_file_new_for_path(FILE_NAME);
    g_assert_nonnull(input_file);

    input_stream = g_file_read(input_file, NULL, &error);
    if (input_stream == NULL) {
        g_assert_nonnull(error);
        g_printerr("Cannot open file '%s' for input: %s\n", FILE_NAME, error->message);
        exit(EXIT_FAILURE);
    }

    input_data = g_new(guint8, SIZE_TO_READ);
    g_assert_nonnull(input_data);

    while ((nbytes_read = g_input_stream_read((GInputStream *) input_stream, input_data, SIZE_TO_READ, NULL, &error))) {
        if (nbytes_read == -1) {
            g_free(input_data);

            g_assert_nonnull(error);
            g_printerr("Cannot read %d bytes from '%s': %s\n", SIZE_TO_READ, FILE_NAME, error->message);

            exit(EXIT_FAILURE);
        }
        md5_hash_data(&md5_ctx, input_data, nbytes_read);
    }
    g_free(input_data);

    hexdigest = md5_hash_get_hexdigest(&md5_ctx);
    g_assert_nonnull(hexdigest);

    g_print("MD5 hexdigest of '%s': %s\n", FILE_NAME, hexdigest->str);
    g_string_free(hexdigest, TRUE);
}
