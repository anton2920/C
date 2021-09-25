#include <omp.h>

#include "crypto.h"
#include "crypto_caesar_athens.h"
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

    ret = g_new0(gunichar, 2);
    g_assert_nonnull(ret);

    *ret = ch;

    return ret;
}


GHashTable *crack_process_statistics_file(const gchar *file_path, GError **error)
{
    g_autoptr(GIOChannel) file_io_channel = NULL;
    gsize total_chars = 0;
    double *new_val_ptr;
    GHashTableIter iter;
    GHashTable *result;
    gunichar ch, *key;
    GIOStatus status;
    gsize *val_ptr;

    g_assert_nonnull(file_path);

    file_io_channel = g_io_channel_new_file(file_path, "r", error);
    if (file_io_channel == NULL) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    result = g_hash_table_new_full(g_str_hash, g_str_equal, crack_ht_key_destroy_cb, crack_ht_value_desroy_cb);
    g_assert_nonnull(result);

    while (TRUE) {
        status = g_io_channel_read_unichar(file_io_channel, &ch, error);
        if (status == G_IO_STATUS_EOF) {
            break;
        } else if (status != G_IO_STATUS_NORMAL) {
            g_assert((error == NULL) || (*error != NULL));
            g_hash_table_destroy(result);
            return FALSE;
        }

        if (ch == '\n') {
            continue;
        }

        ++total_chars;
        key = crack_ht_make_key(ch);
        g_assert_nonnull(key);

        if ((val_ptr = g_hash_table_lookup(result, key)) != NULL) {
            *val_ptr += 1;
            crack_ht_key_destroy_cb(key);
        } else {
            g_hash_table_insert(result, key, g_new0(gsize, 1));
        }
    }

    /* Normalizing values */
    g_hash_table_iter_init(&iter, result);
    while (g_hash_table_iter_next(&iter, (gpointer *) &key, (gpointer *) &val_ptr)) {
        new_val_ptr = g_new0(double, 1);
        g_assert_nonnull(new_val_ptr);

        *new_val_ptr = (double) *val_ptr / (double) total_chars;
        g_hash_table_replace(result, crack_ht_make_key(*key), new_val_ptr);
    }

    return result;
}


gboolean crack_bruteforce_numeric_key(crypto_decrypt_cb_t process_func, GHashTable *letter_stats,
                                      const gchar *input_file_path,
                                      const gchar *output_file_path,
                                      GError **error)
{
    g_autoptr(GIOChannel) output_file_channel = NULL;
    g_autoptr(GIOChannel) input_file_channel = NULL;
    double divergence_min = G_MAXDOUBLE;
    gssize key_value, key_value_to_use;
    GString *input_str, *decrypted_str;
    gsize term_pos, bytes_written;
    GIOStatus status;

    g_assert_nonnull(process_func);
    g_assert_nonnull(letter_stats);

    input_file_channel = g_io_channel_new_file(input_file_path, "r", error);
    if (input_file_channel == NULL) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    /* Read entire input file */
    gchar *input_data, *newline_ptr;
    gsize input_length;
    status = g_io_channel_read_to_end(input_file_channel, &input_data, &input_length, error);
    if (status == G_IO_STATUS_EOF) {
        g_set_error(error,
                    G_FILE_ERROR,
                    0,
                    "input file is empty");
        return FALSE;
    } else if (status != G_IO_STATUS_NORMAL) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    input_str = g_string_new(input_data);
    g_assert_nonnull(input_str);

    g_free(input_data);

    while ((newline_ptr = g_utf8_strchr(input_str->str, -1, '\n')) != NULL) {
        *newline_ptr = ' ';
    }

    /* Calculating table based on input file */
#pragma omp parallel for                                                                \
    ordered,                                                                            \
    default(none),                                                                      \
    private(decrypted_str),                                                             \
    shared(input_str, letter_stats, process_func,                                       \
           key_value_to_use, divergence_min)
    for (key_value = 0; key_value < G_MAXSSIZE; ++key_value) {
        GError *err = NULL;
        decrypted_str = process_func(input_str, key_value, NULL, &err);
        if (decrypted_str == NULL) {
            g_assert_nonnull(err);
            if (err->code == CRYPTO_CAESAR_ATHENS_ERROR_INVALID_MULTIPLIER) {
                key_value = G_MAXSSIZE - 1;
            } else if (err->code == CRYPTO_CAESAR_ATHENS_ERROR_INVALID_OFFSET) {
                key_value &= (gssize) G_MAXUINT32 << 32;
                key_value += ((gssize) (omp_get_thread_num() + 1) << 32);
            }

            g_error_free(err);
            err = NULL;
            continue;
        }

        /* Filling in hashtable itself */
        GHashTable *decrypted_stats;
        gsize total_chars = 0;
        gunichar ch, *key;
        gsize *val_ptr;
        gssize pos;
        gchar *ptr;

        decrypted_stats = g_hash_table_new_full(g_str_hash, g_str_equal, crack_ht_key_destroy_cb, crack_ht_value_desroy_cb);
        g_assert_nonnull(decrypted_stats);

        for (pos = 0; pos < g_utf8_strlen(decrypted_str->str, -1); ++pos) {
            ptr = g_utf8_offset_to_pointer(decrypted_str->str, pos);
            g_assert_nonnull(ptr);

            ch = g_utf8_get_char_validated(ptr, -1);

            ++total_chars;
            key = crack_ht_make_key(ch);
            g_assert_nonnull(key);

            if ((val_ptr = g_hash_table_lookup(decrypted_stats, key)) != NULL) {
                *val_ptr += 1;
                crack_ht_key_destroy_cb(key);
            } else {
                g_hash_table_insert(decrypted_stats, key, g_new0(gsize, 1));
            }
        }

        /* Normalizing values */
        double divergence = 0, new_val, *val;
        GHashTableIter iter;
        g_hash_table_iter_init(&iter, decrypted_stats);

        while (g_hash_table_iter_next(&iter, (gpointer *) &key, (gpointer *) &val_ptr)) {
            /* Normalizing new value */
            new_val = (double) *val_ptr / (double) total_chars;

            /* Finding old value */
            val = g_hash_table_lookup(letter_stats, key);
            if (val == NULL) {
                continue;
            }

            divergence += (new_val - *val) * (new_val - *val);
        }

        /* Saving minimum divergence value with the proper key */
        #pragma omp critical
        {
            if (divergence < divergence_min) {
                divergence_min = divergence;
                key_value_to_use = key_value;

                g_print("New minimum divergence value found: %lf; a = %ld; b = %ld\n",
                        divergence_min,
                        (key_value_to_use >> 32) & G_MAXUINT32,
                        key_value_to_use & G_MAXUINT32);
            }
        }

        g_string_free(decrypted_str, TRUE);
        g_hash_table_destroy(decrypted_stats);
    }
    /* FIXME: are they needed? */
    #pragma omp barrier
    #pragma omp single

    output_file_channel = g_io_channel_new_file(output_file_path, "w", error);
    if (output_file_channel == NULL)
    {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    status = g_io_channel_seek_position(input_file_channel, 0, G_SEEK_SET, error);
    if (status != G_IO_STATUS_NORMAL) {
        g_assert(error == NULL || *error != NULL);
        return FALSE;
    }

    /* Proceeding with the rest of the file */
    while (TRUE) {
        status = g_io_channel_read_line_string(input_file_channel, input_str, &term_pos, error);
        if (status == G_IO_STATUS_EOF) {
            break;
        } else if (status != G_IO_STATUS_NORMAL) {
            g_assert((error == NULL) || (*error != NULL));
            return FALSE;
        }

        /* Removing trailing '\n' */
        input_str->str[term_pos] = '\0';
        --input_str->len;

        if ((decrypted_str = process_func(input_str, key_value_to_use, NULL, error)) == NULL) {
            g_assert((error == NULL) || (*error != NULL));
            g_string_free(input_str, TRUE);
            return FALSE;
        }

        status = g_io_channel_write_chars(output_file_channel, decrypted_str->str, (gssize) decrypted_str->len,
                                          &bytes_written, error);
        if ((bytes_written != decrypted_str->len) || (status == G_IO_STATUS_ERROR)) {
            g_string_free(input_str, TRUE);
            g_string_free(decrypted_str, TRUE);
            return FALSE;
        }
        g_string_free(decrypted_str, TRUE);

        status = g_io_channel_write_unichar(output_file_channel, '\n', error);
        if (status == G_IO_STATUS_ERROR) {
            return FALSE;
        }
    }

    g_string_free(input_str, TRUE);

    return TRUE;
}
