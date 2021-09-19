#include "crack_process.h"


static void crack_ht_key_destroy_cb(gpointer key)
{
    g_assert_nonnull(key);

    g_free(key);
}


static void crack_ht_value_desroy_cb(gpointer value)
{
    g_assert_nonnull(value);

    g_free(value);
}


static gpointer crack_ht_make_key(gunichar ch)
{
    gunichar *ret;

    ret = g_new(gunichar, 1);
    g_assert_nonnull(ret);

    *ret = ch;

    return ret;
}


GHashTable *crack_get_statistics(const gchar *file_path, GError **error)
{
    g_autoptr(GIOChannel) file_io_channel = NULL;
    gsize total_chars = 0;
    GHashTable *result;
    gunichar ch, *key;
    GIOStatus status;
    gsize *val_ptr;

    g_assert_nonnull(file_path);

    file_io_channel = g_io_channel_new_file(file_path, "r", error);
    if (file_io_channel == NULL) {
        g_assert(error == NULL || *error != NULL);
        return NULL;
    }

    result = g_hash_table_new_full(NULL, NULL, crack_ht_key_destroy_cb, crack_ht_value_desroy_cb);
    g_assert_nonnull(result);

    while (TRUE) {
        status = g_io_channel_read_unichar(file_io_channel, &ch, error);
        if (status == G_IO_STATUS_EOF) {
            break;
        }
        else if (status != G_IO_STATUS_NORMAL) {
            g_assert(error == NULL || *error != NULL);
            g_hash_table_destroy(result);
            return FALSE;
        }

        ++total_chars;
        key = crack_ht_make_key(ch);
        g_assert_nonnull(key);

        if ((val_ptr = g_hash_table_lookup(result, key)) != NULL) {
            *val_ptr += 1;
        } else {
            g_hash_table_insert(result, key, g_new0(gsize, 1));
        }
    }

    return result;
}
