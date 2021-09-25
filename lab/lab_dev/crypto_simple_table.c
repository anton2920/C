#include "crypto.h"

#include "crypto_simple_table.h"

#define CRYPTO_SIMPLE_TABLE_DELIM '_'


static const gchar *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .,0123456789";


/* ======== ERROR DOMAIN SETUP ======== */
static GQuark crypto_simple_table_error_quark(void)
{
    return g_quark_from_static_string("crypto-simple-table-error-quark");
}


/* ======== BLOCK OF MISC HELPERS ======== */
static gboolean _is_text_valid(GString *text, GError **error)
{
    const gchar *endptr;
    gsize i;

    g_assert_nonnull(text);
    g_assert_nonnull(text->str);

    if (!g_utf8_validate(text->str, (gssize) text->len, &endptr)) {
        g_assert_nonnull(endptr);
        g_set_error(error,
                    CRYPTO_SIMPLE_TABLE_ERROR,
                    CRYPTO_SIMPLE_TABLE_ERROR_TEXT_NOT_UTF8,
                    "given input file is not in UTF-8: invalid character '%c'", *endptr);
        return FALSE;
    }

    for (i = 0; i < text->len; ++i) {
        if ((g_utf8_strchr(alphabet, -1, text->str[i]) == NULL) && (text->str[i] != CRYPTO_SIMPLE_TABLE_DELIM)) {
            g_set_error(error,
                        CRYPTO_SIMPLE_TABLE_ERROR,
                        CRYPTO_SIMPLE_TABLE_ERROR_TEXT_NOT_IN_ALPHABET,
                        "character '%c' is not from alphabet", text->str[i]);
            return FALSE;
        }
    }

    return TRUE;
}


/* ======== BLOCK OF SIMPLE TABLES ======== */
static GString *_simple_table_process_ex(GString *text, gssize key,
                                         void (*process_cb)(GString *, gint32, gint32, GString *),
                                         GError **error)
{
    GString *result = NULL;
    gint32 nrows, ncols;

    g_assert_nonnull(text);
    g_assert_nonnull(process_cb);

    /* Validating the initial text */
    if (!_is_text_valid(text, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    nrows = (gint32) ((key >> 32) & G_MAXUINT32);
    ncols = (gint32) (key & G_MAXUINT32);

    result = g_string_new(NULL);
    g_assert_nonnull(result);

    process_cb(text, nrows, ncols, result);

    return result;
}


static void _simple_table_encrypt_cb(GString *text, gint32 nrows, gint32 ncols, GString *result)
{
    gsize i, j, processed_len, ind;
    gchar ch;

    g_assert_nonnull(text);
    g_assert_nonnull(result);

    for (processed_len = 0;
         processed_len < text->len;
         processed_len += nrows * ncols) {
        for (i = 0; i < nrows; ++i) {
            for (j = 0; j < ncols; ++j) {
                ind = processed_len + (j * nrows) + i;
                if (ind >= text->len) {
                    ch = CRYPTO_SIMPLE_TABLE_DELIM;
                } else {
                    ch = text->str[ind];
                }
                g_string_append_c_inline(result, ch);
            }
        }
    }
}


static GString *_simple_table_encrypt(GString *text, gssize key, GString *_dummy G_GNUC_UNUSED, GError **error)
{
    return _simple_table_process_ex(text, key, _simple_table_encrypt_cb, error);
}


static void _simple_table_decrypt_cb(GString *text, gint32 nrows, gint32 ncols, GString *result)
{
    gsize i, j, processed_len, ind;
    gchar ch;

    g_assert_nonnull(text);
    g_assert_nonnull(result);

    for (processed_len = 0;
         processed_len < text->len;
         processed_len += nrows * ncols) {
        for (i = 0; i < ncols; ++i) {
            for (j = 0; j < nrows; ++j) {
                ind = processed_len + (j * ncols) + i;
                ch = text->str[ind];
                if (ch != CRYPTO_SIMPLE_TABLE_DELIM) {
                    g_string_append_c_inline(result, ch);
                }
            }
        }
    }
}


static GString *_simple_table_decrypt(GString *text, gssize key, GString *_dummy G_GNUC_UNUSED, GError **error)
{
    return _simple_table_process_ex(text, key, _simple_table_decrypt_cb, error);
}


static gboolean _simple_table_key_parser(GString *key_text, gssize *key,
                                         GString **_dummy G_GNUC_UNUSED,
                                         GError **error)
{
    gchar **tokens, *token, *endptr;
    gint64 big_nrows, big_ncols;
    guint32 nrows, ncols;

    g_assert_nonnull(key_text);
    g_assert_nonnull(key);

    if (key_text->len == 0) {
        g_set_error(error,
                    CRYPTO_SIMPLE_TABLE_ERROR,
                    CRYPTO_SIMPLE_TABLE_ERROR_INVALID_KEY,
                    "supplied key file is empty");
        return FALSE;
    }

    /* Splitting given key string */
    tokens = g_strsplit_set(key_text->str, " \t\v\r\n", 2);
    token = g_strchug(g_strchomp(tokens[0]));
    g_assert_nonnull(token);

    big_nrows = g_ascii_strtoll(token, &endptr, 10);
    if ((big_nrows <= 0) || (big_nrows > G_MAXINT32) || ((endptr != NULL) && (*endptr != '\0'))) {
        g_set_error(error,
                    CRYPTO_SIMPLE_TABLE_ERROR,
                    CRYPTO_SIMPLE_TABLE_ERROR_INVALID_KEY,
                    "supplied key contains invalid number of rows");
        g_strfreev(tokens);
        return FALSE;
    }

    token = g_strchug(g_strchomp(tokens[1]));
    if (token == NULL) {
        g_set_error(error,
                    CRYPTO_SIMPLE_TABLE_ERROR,
                    CRYPTO_SIMPLE_TABLE_ERROR_INVALID_KEY,
                    "supplied key contains only the number of rows");
        g_strfreev(tokens);
        return FALSE;
    }

    big_ncols = g_ascii_strtoll(token, &endptr, 10);
    if ((big_ncols <= 0) || (big_ncols > G_MAXINT32) || ((endptr != NULL) && (*endptr != '\0'))) {
        g_set_error(error,
                    CRYPTO_SIMPLE_TABLE_ERROR,
                    CRYPTO_SIMPLE_TABLE_ERROR_INVALID_KEY,
                    "supplied key contains invalid number of columns");
        g_strfreev(tokens);
        return FALSE;
    }

    nrows = (guint32) big_nrows;
    ncols = (guint32) big_ncols;

    *key = (gssize) (((gsize) nrows << 32) | ncols);

    g_strfreev(tokens);

    return TRUE;
}


crypto_cipher_t crypto_simple_table_decl = {
    .encrypt_cb = _simple_table_encrypt,
    .decrypt_cb = _simple_table_decrypt,
    .key_parser_cb = _simple_table_key_parser
};
