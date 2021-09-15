#include "crypto_io.h"

static gboolean _process_file_ex(gchar *input_file_name, gchar *key_file_name, gchar *output_file_name,
                                 crypto_encrypt_cb_t process_func,
                                 crypto_key_parser_cb_t key_parser,
                                 GError **error)
{
    /* 'g_io_channel' representations of each file */
    g_autoptr(GIOChannel) input_file_channel = NULL;
    g_autoptr(GIOChannel) key_file_channel = NULL;
    g_autoptr(GIOChannel) output_file_channel = NULL;

    /* One 'GString' pointer for each file */
    GString *input_str = NULL, *output_str = NULL;
    g_autoptr(GString) key_str = NULL;

    /* Supplement for reading keyfile */
    gchar *key_file_str;
    gsize key_file_len;

    /* Key data after parsing keyfile */
    gssize keyval;
    g_autoptr(GString) keyword;

    /* Misc. context variables */
    gsize bytes_written, term_pos;
    GIOStatus status;

    g_assert_nonnull(input_file_name);
    g_assert_nonnull(key_file_name);
    g_assert_nonnull(output_file_name);
    g_assert_nonnull(process_func);
    g_assert_nonnull(key_parser);

    /* Let's go with g_io_channel's */
    if ((input_file_channel = g_io_channel_new_file(input_file_name, "r", error)) == NULL) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    if ((key_file_channel = g_io_channel_new_file(key_file_name, "r", error)) == NULL) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    if ((output_file_channel = g_io_channel_new_file(output_file_name, "w", error)) == NULL) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    /* Reading key_value file first */
    status = g_io_channel_read_to_end(key_file_channel, &key_file_str, &key_file_len, error);
    if (status != G_IO_STATUS_NORMAL) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    key_str = g_string_new_len(key_file_str, (gssize) key_file_len);
    g_assert_nonnull(key_str);

    g_free(key_file_str);

    if (!key_parser(key_str, &keyval, &keyword, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    input_str = g_string_new(NULL);
    g_assert_nonnull(input_str);

    /* For each line in input_file_path, produce encrypted line in output_file_path */
    while (TRUE) {
        status = g_io_channel_read_line_string(input_file_channel, input_str, &term_pos, error);
        if (status == G_IO_STATUS_EOF) {
            break;
        } else if (status != G_IO_STATUS_NORMAL) {
            g_assert((error == NULL) || (*error != NULL));
            return FALSE;
        }
        g_assert_nonnull(input_str);

        /* Removing trailing '\n' */
        input_str->str[term_pos] = '\0';
        --input_str->len;

        if ((output_str = process_func(input_str, keyval, keyword, error)) == NULL) {
            g_assert((error == NULL) || (*error != NULL));
            g_string_free(input_str, TRUE);
            return FALSE;
        }

        status = g_io_channel_write_chars(output_file_channel, output_str->str, (gssize) output_str->len,
                                          &bytes_written, error);
        if ((bytes_written != output_str->len) || (status == G_IO_STATUS_ERROR)) {
            g_string_free(input_str, TRUE);
            g_string_free(output_str, TRUE);
            return FALSE;
        }
        g_string_free(output_str, TRUE);

        status = g_io_channel_write_unichar(output_file_channel, '\n', error);
        if (status == G_IO_STATUS_ERROR) {
            return FALSE;
        }
    }

    g_string_free(input_str, TRUE);

    return TRUE;
}


/* ======== BLOCK OF EXPORTED API ======== */
gboolean crypto_encrypt_file(gchar *input_file_name, gchar *key_file_name, gchar *output_file_name,
                             crypto_cipher_t *cipher, GError **error)
{
    return _process_file_ex(input_file_name, key_file_name, output_file_name,
                            cipher->encrypt_cb, cipher->key_parser_cb, error);
}


gboolean crypto_decrypt_file(gchar *input_file_name, gchar *key_file_name, gchar *output_file_name,
                             crypto_cipher_t *cipher, GError **error)
{
    return _process_file_ex(input_file_name, key_file_name, output_file_name,
                            cipher->decrypt_cb, cipher->key_parser_cb, error);
}
