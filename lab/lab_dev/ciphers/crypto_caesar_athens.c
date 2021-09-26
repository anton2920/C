#include "../crypto/crypto.h"

#include "crypto_caesar_athens.h"


static const gchar *alphabet = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";


/* ======== ERROR DOMAIN SETUP ======== */
static GQuark crypto_caesar_athens_error_quark(void)
{
    return g_quark_from_static_string("crypto-caesar-athens-error-quark");
}


/* ======== BLOCK OF MISC HELPERS ======== */
static gboolean _is_text_valid(GString *text, GError **error)
{
    const gchar *endptr, *ptr;
    gsize i;

    g_assert_nonnull(text);
    g_assert_nonnull(text->str);

    if (!g_utf8_validate(text->str, (gssize) text->len, &endptr)) {
        g_assert_nonnull(endptr);
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_TEXT_NOT_UTF8,
                    "given input file is not in UTF-8: invalid character '%c'", *endptr);
        return FALSE;
    }

    for (i = 0; i < g_utf8_strlen(text->str, -1); ++i) {
        ptr = g_utf8_offset_to_pointer(text->str, (glong) i);
        g_assert_nonnull(ptr);

        if ((g_utf8_strchr(alphabet, -1, g_utf8_get_char_validated(ptr, -1)) == NULL)) {
            g_set_error(error,
                        CRYPTO_CAESAR_ATHENS_ERROR,
                        CRYPTO_CAESAR_ATHENS_ERROR_TEXT_NOT_IN_ALPHABET,
                        "character 'U+%04"G_GINT32_FORMAT"X' is not from alphabet",
                        g_utf8_get_char_validated(ptr, -1));
            return FALSE;
        }
    }

    return TRUE;
}


/* Euclidean algorithm */
static gint32 _greatest_common_divisor(gint32 a, gint32 b)
{
    gint32 tmp;

    if (!a || !b) {
        return 0;
    }

    while (b) {
        tmp = b;
        b = a % b;
        a = tmp;
    }

    return MAX(a, -a);
}


/* ======== BLOCK OF CAESAR ATHENS ======== */
static GString *_caesar_athens_get_alphabet(gint32 multiplier, gint32 offset)
{
    GString *working_alphabet;
    gssize i, alphabet_pos;
    gunichar alphabet_ch;
    gchar *alphabet_ptr;

    /* Constructing new alphabet */
    working_alphabet = g_string_new(NULL);
    g_assert_nonnull(working_alphabet);

    for (i = 0; i < g_utf8_strlen(alphabet, -1); ++i) {
        alphabet_pos = ((i * multiplier) + offset) % g_utf8_strlen(alphabet, -1);

        /* Get resulted alphabet char */
        alphabet_ptr = g_utf8_offset_to_pointer(alphabet, alphabet_pos);
        g_assert_nonnull(alphabet_ptr);
        alphabet_ch = g_utf8_get_char_validated(alphabet_ptr, -1);

        g_string_append_unichar(working_alphabet, alphabet_ch);
    }

    return working_alphabet;
}


static GString *_caesar_athens_process_ex(GString *text, gssize key,
                                          void (*process_cb)(GString *, GString *, GString *),
                                          GError **error)
{
    GString *working_alphabet;
    gint32 offset, multiplier;
    GString *result = NULL;

    g_assert_nonnull(text);
    g_assert_nonnull(process_cb);

    if (!_is_text_valid(text, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    multiplier = (gint32) ((key >> 32) & G_MAXUINT32);
    offset = (gint32) (key & G_MAXUINT32);

    if ((offset < 0) || (offset >= g_utf8_strlen(alphabet, -1))) {
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_INVALID_OFFSET,
                    "offset should be in following range: 0 <= offset < %ld, but offset is %d",
                    g_utf8_strlen(alphabet, -1), offset);
        return NULL;
    }

    if ((multiplier < 0) || (multiplier >= g_utf8_strlen(alphabet, -1))) {
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_INVALID_MULTIPLIER,
                    "multiplier should be in following range: 0 <= multiplier < %ld, but multiplier is %d",
                    g_utf8_strlen(alphabet, -1), offset);
        return NULL;
    }

    if (_greatest_common_divisor(multiplier, (gint32) g_utf8_strlen(alphabet, -1)) != 1) {
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_INVALID_GCD,
                    "greatest common divisor of 'multiplier' and 'offset' should be '1', but it's %d",
                    _greatest_common_divisor(multiplier, (gint32) g_utf8_strlen(alphabet, -1)));
        return NULL;
    }

    working_alphabet = _caesar_athens_get_alphabet(multiplier, offset);
    g_assert_nonnull(working_alphabet);

    result = g_string_new(NULL);
    g_assert_nonnull(result);

    process_cb(text, working_alphabet, result);

    g_string_free(working_alphabet, TRUE);

    return result;
}


static void _caesar_athens_encrypt_cb(GString *text, GString *working_alphabet, GString *result)
{
    gchar *text_ptr, *alphabet_ptr, *result_ptr;
    gssize i, alphabet_offset;
    gunichar text_ch;

    g_assert_nonnull(text);
    g_assert_nonnull(working_alphabet);
    g_assert_nonnull(result);

    for (i = 0; i < g_utf8_strlen(text->str, -1); ++i) {
        /* Current char from text */
        text_ptr = g_utf8_offset_to_pointer(text->str, i);
        g_assert_nonnull(text_ptr);
        text_ch = g_utf8_get_char_validated(text_ptr, -1);

        /* Offset in original alphabet (to get char numeric code) */
        alphabet_ptr = g_utf8_strchr(alphabet, -1, text_ch);
        g_assert_nonnull(alphabet_ptr);
        alphabet_offset = g_utf8_pointer_to_offset(alphabet, alphabet_ptr);

        /* Resulting char */
        result_ptr = g_utf8_offset_to_pointer(working_alphabet->str, alphabet_offset);
        g_assert_nonnull(result_ptr);
        g_string_append_unichar(result, g_utf8_get_char_validated(result_ptr, -1));
    }
}


static GString *_caesar_athens_encrypt(GString *text, gssize key, GString *_dummy G_GNUC_UNUSED, GError **error)
{
    return _caesar_athens_process_ex(text, key, _caesar_athens_encrypt_cb, error);
}


static void _caesar_athens_decrypt_cb(GString *text, GString *working_alphabet, GString *result)
{
    gchar *text_ptr, *alphabet_ptr, *result_ptr;
    gssize i, alphabet_offset;
    gunichar text_ch;

    g_assert_nonnull(text);
    g_assert_nonnull(working_alphabet);
    g_assert_nonnull(result);

    for (i = 0; i < g_utf8_strlen(text->str, -1); ++i) {
        /* Current char from text */
        text_ptr = g_utf8_offset_to_pointer(text->str, i);
        g_assert_nonnull(text_ptr);
        text_ch = g_utf8_get_char_validated(text_ptr, -1);

        /* Offset in original alphabet (to get char numeric code) */
        alphabet_ptr = g_utf8_strchr(working_alphabet->str, -1, text_ch);
        g_assert_nonnull(alphabet_ptr);
        alphabet_offset = g_utf8_pointer_to_offset(working_alphabet->str, alphabet_ptr);

        /* Resulting char */
        result_ptr = g_utf8_offset_to_pointer(alphabet, alphabet_offset);
        g_assert_nonnull(result_ptr);
        g_string_append_unichar(result, g_utf8_get_char_validated(result_ptr, -1));
    }
}


static GString *_caesar_athens_decrypt(GString *text, gssize key, GString *_dummy G_GNUC_UNUSED, GError **error)
{
    return _caesar_athens_process_ex(text, key, _caesar_athens_decrypt_cb, error);
}


static gboolean _caesar_cipher_key_parser(GString *key_text, gssize *key,
                                          GString **_dummy G_GNUC_UNUSED,
                                          GError **error)
{
    glong big_multiplier, big_offset;
    gchar **tokens, *token, *endptr;
    guint32 multiplier, offset;

    g_assert_nonnull(key_text);
    g_assert_nonnull(key);

    if (key_text->len == 0) {
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_INVALID_KEY,
                    "supplied key file is empty");
        return FALSE;
    }

    /* Splitting given key string */
    tokens = g_strsplit_set(key_text->str, " \t\v\r\n", 2);
    token = g_strchug(g_strchomp(tokens[0]));
    g_assert_nonnull(token);

    big_multiplier = g_ascii_strtoll(token, &endptr, 10);
    if ((big_multiplier < 0) || (big_multiplier > G_MAXINT32) || ((endptr != NULL) && (*endptr != '\0'))) {
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_INVALID_KEY,
                    "supplied key contains invalid value for Caesar's multiplier");
        g_strfreev(tokens);
        return FALSE;
    }

    token = g_strchug(g_strchomp(tokens[1]));
    if (token == NULL) {
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_INVALID_KEY,
                    "supplied key contains only value for Caesar's multiplier");
        g_strfreev(tokens);
        return FALSE;
    }

    big_offset = g_ascii_strtoll(token, &endptr, 10);
    if ((big_offset < 0) || (big_offset > G_MAXINT32) || ((endptr != NULL) && (*endptr != '\0'))) {
        g_set_error(error,
                    CRYPTO_CAESAR_ATHENS_ERROR,
                    CRYPTO_CAESAR_ATHENS_ERROR_INVALID_KEY,
                    "supplied key contains invalid value for Caesar's multiplier");
        g_strfreev(tokens);
        return FALSE;
    }

    multiplier = (guint32) big_multiplier;
    offset = (guint32) big_offset;

    *key = (gssize) (((gsize) multiplier << 32) | offset);

    g_strfreev(tokens);

    return TRUE;
}


crypto_cipher_t crypto_caesar_athens_decl = {
    .encrypt_cb = _caesar_athens_encrypt,
    .decrypt_cb = _caesar_athens_decrypt,
    .key_parser_cb = _caesar_cipher_key_parser
};
