#include "../crypto/crypto.h"

#include "crypto_vigenere.h"


/* Let's assume, lowercase letters are needed; 'ё' is excluded */
static const gchar *alphabet = "абвгдежзийклмнопрстуфхцчшщъыьэюя., ";


/* ======== ERROR DOMAIN SETUP ======== */
static GQuark crypto_vigenere_error_quark(void)
{
    return g_quark_from_static_string("crypto-vigenere-error-quark");
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
                    CRYPTO_VIGENERE_ERROR,
                    CRYPTO_VIGENERE_ERROR_TEXT_NOT_UTF8,
                    "given input file is not in UTF-8: invalid character '%c'", *endptr);
        return FALSE;
    }

    for (i = 0; i < g_utf8_strlen(text->str, -1); ++i) {
        ptr = g_utf8_offset_to_pointer(text->str, (glong) i);
        g_assert_nonnull(ptr);

        if ((g_utf8_strchr(alphabet, -1, g_utf8_get_char_validated(ptr, -1)) == NULL)) {
            g_set_error(error,
                        CRYPTO_VIGENERE_ERROR,
                        CRYPTO_VIGENERE_ERROR_TEXT_NOT_IN_ALPHABET,
                        "character 'U+%04"G_GINT32_FORMAT"X' is not from alphabet",
                        g_utf8_get_char_validated(ptr, -1));
            return FALSE;
        }
    }

    return TRUE;
}


static gboolean _is_keyword_valid(GString *keyword, GError **error)
{
    gchar *ptr1, *ptr2;
    gunichar ch1, ch2;
    gssize i, j;

    g_assert_nonnull(keyword);

    /* Checking for the rules of input_file_path first */
    if (!_is_text_valid(keyword, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    if (keyword->len > g_utf8_strlen(alphabet, -1)) {
        g_set_error(error,
                    CRYPTO_VIGENERE_ERROR,
                    CRYPTO_VIGENERE_ERROR_KEYWORD_TOO_LONG,
                    "keyword string must be less than length of an alphabet (<%lu)",
                    g_utf8_strlen(alphabet, -1));
        return FALSE;
    }

    for (i = 0; i < g_utf8_strlen(keyword->str, -1) - 1; ++i) {
        for (j = i + 1; j < g_utf8_strlen(keyword->str, -1); ++j) {
            ptr1 = g_utf8_offset_to_pointer(keyword->str, i);
            ptr2 = g_utf8_offset_to_pointer(keyword->str, j);

            ch1 = g_utf8_get_char_validated(ptr1, -1);
            ch2 = g_utf8_get_char_validated(ptr2, -1);

            if (ch1 == ch2) {
                g_set_error(error,
                            CRYPTO_VIGENERE_ERROR,
                            CRYPTO_VIGENERE_ERROR_KEYWORD_NOT_UNIQUE,
                            "characters in keyword should be unique, but 'U+%04"G_GINT32_FORMAT"X' is not", ch1);
                return FALSE;
            }
        }
    }

    return TRUE;
}


/* ======== BLOCK OF VIGENERE ======== */
static GString *_vigenere_get_alphabet(GString *keyword)
{
    GString *working_alphabet;
    glong alphabet_len;
    gchar *ptr;
    gssize i;

    g_assert_nonnull(keyword);

    /* Finding length of our basic alphabet */
    alphabet_len = g_utf8_strlen(alphabet, -1);

    /* Constructing new alphabet */
    working_alphabet = g_string_new(NULL);
    g_assert_nonnull(working_alphabet);

    /* Constructing new alphabet using given keyword */
    g_string_append_len(working_alphabet, keyword->str, (gssize) keyword->len);

    for (i = 0; i < alphabet_len; ++i) {
        ptr = g_utf8_offset_to_pointer(alphabet, i);
        g_assert_nonnull(ptr);

        if (g_utf8_strchr(keyword->str, -1, g_utf8_get_char_validated(ptr, -1)) == NULL) {
            ptr = g_utf8_offset_to_pointer(alphabet, i);
            g_assert_nonnull(ptr);

            g_string_append_unichar(working_alphabet, g_utf8_get_char_validated(ptr, -1));
        }
    }

    return working_alphabet;
}


static gboolean _vigenere_parse_key(GString *key, GString **alphabet_keyword, GString **data_keyword, GError **error)
{
    gchar **tokens, *token;

    g_assert_nonnull(key);
    g_assert_nonnull(alphabet_keyword);
    g_assert_null(*alphabet_keyword);
    g_assert_nonnull(data_keyword);
    g_assert_null(*data_keyword);

    /* key->len is >0 at this point */
    tokens = g_strsplit_set(key->str, " \t\v\r\n", 2);
    g_assert_nonnull(tokens);

    token = g_strchug(g_strchomp(tokens[0]));
    g_assert_nonnull(token);

    *alphabet_keyword = g_string_new(token);
    g_assert_nonnull(*alphabet_keyword);
    if (!_is_keyword_valid(*alphabet_keyword, error)) {
        g_strfreev(tokens);
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    token = g_strchug(g_strchomp(tokens[1]));
    if (token == NULL) {
        g_set_error(error,
                    CRYPTO_VIGENERE_ERROR,
                    CRYPTO_VIGENERE_ERROR_INVALID_KEY,
                    "supplied key contains only keyword for building alphabet");
        g_strfreev(tokens);
        return FALSE;
    }

    *data_keyword = g_string_new(token);
    g_assert_nonnull(*data_keyword);
    if (!_is_keyword_valid(*data_keyword, error)) {
        g_strfreev(tokens);
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    g_strfreev(tokens);

    return TRUE;
}


static GString *_vigenere_process_ex(GString *text, GString *key,
                                     void (*process_cb)(GString *, GString *, GString *, GString *),
                                     GError **error)
{
    GString *result, *working_alphabet, *alphabet_keyword = NULL, *data_keyword = NULL;

    g_assert_nonnull(text);
    g_assert_nonnull(key);

    if (!_is_text_valid(text, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    if (!_vigenere_parse_key(key, &alphabet_keyword, &data_keyword, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    result = g_string_new(NULL);
    g_assert_nonnull(result);

    working_alphabet = _vigenere_get_alphabet(alphabet_keyword);
    g_assert_nonnull(working_alphabet);

    process_cb(text, working_alphabet, data_keyword, result);

    g_string_free(alphabet_keyword, TRUE);
    g_string_free(data_keyword, TRUE);
    g_string_free(working_alphabet, TRUE);

    return result;
}


static void _vigenere_encrypt_cb(GString *text, GString *working_alphabet, GString *data_keyword, GString *result)
{
    gchar *text_ptr, *kword_ptr, *text_alphabet_ptr, *kword_alphabet_ptr, *res_ptr;
    gunichar text_ch, kword_ch;
    gssize i, offset;

    g_assert_nonnull(text);
    g_assert_nonnull(working_alphabet);
    g_assert_nonnull(data_keyword);
    g_assert_nonnull(result);

    for (i = 0; i < g_utf8_strlen(text->str, -1); ++i) {
        /* Finding current char to process */
        text_ptr = g_utf8_offset_to_pointer(text->str, i);
        g_assert_nonnull(text_ptr);
        text_ch = g_utf8_get_char_validated(text_ptr, -1);

        /* Finding current keyword char */
        kword_ptr = g_utf8_offset_to_pointer(data_keyword->str, i % g_utf8_strlen(data_keyword->str, -1));
        g_assert_nonnull(kword_ptr);
        kword_ch = g_utf8_get_char_validated(kword_ptr, -1);

        /* Finding positions of both chars in working alphabet */
        text_alphabet_ptr = g_utf8_strchr(working_alphabet->str, -1, text_ch);
        kword_alphabet_ptr = g_utf8_strchr(working_alphabet->str, -1, kword_ch);

        /* Sum of both offsets to get the position of resulting letter */
        offset = g_utf8_pointer_to_offset(working_alphabet->str, text_alphabet_ptr) +
                 g_utf8_pointer_to_offset(working_alphabet->str, kword_alphabet_ptr);
        offset %= g_utf8_strlen(working_alphabet->str, -1);

        res_ptr = g_utf8_offset_to_pointer(working_alphabet->str, offset);
        g_assert_nonnull(res_ptr);

        g_string_append_unichar(result, g_utf8_get_char_validated(res_ptr, -1));
    }
}


static GString *_vigenere_encrypt(GString *text, gssize _dummy G_GNUC_UNUSED, GString *key, GError **error)
{
    return _vigenere_process_ex(text, key, _vigenere_encrypt_cb, error);
}


static void _vigenere_decrypt_cb(GString *text, GString *working_alphabet, GString *data_keyword, GString *result)
{
    gchar *text_ptr, *kword_ptr, *text_alphabet_ptr, *kword_alphabet_ptr, *res_ptr;
    gunichar text_ch, kword_ch;
    gssize i, offset;

    g_assert_nonnull(text);
    g_assert_nonnull(working_alphabet);
    g_assert_nonnull(data_keyword);
    g_assert_nonnull(result);

    for (i = 0; i < g_utf8_strlen(text->str, -1); ++i) {
        /* Finding current char to process */
        text_ptr = g_utf8_offset_to_pointer(text->str, i);
        g_assert_nonnull(text_ptr);
        text_ch = g_utf8_get_char_validated(text_ptr, -1);

        /* Finding current keyword char */
        kword_ptr = g_utf8_offset_to_pointer(data_keyword->str, i % g_utf8_strlen(data_keyword->str, -1));
        g_assert_nonnull(kword_ptr);
        kword_ch = g_utf8_get_char_validated(kword_ptr, -1);

        /* Finding positions of both chars in working alphabet */
        text_alphabet_ptr = g_utf8_strchr(working_alphabet->str, -1, text_ch);
        kword_alphabet_ptr = g_utf8_strchr(working_alphabet->str, -1, kword_ch);

        /* Sum of both offsets to get the position of resulting letter */
        offset = g_utf8_pointer_to_offset(working_alphabet->str, text_alphabet_ptr) -
                 g_utf8_pointer_to_offset(working_alphabet->str, kword_alphabet_ptr);
        if (offset < 0) {
            offset = g_utf8_strlen(working_alphabet->str, -1) + offset;
        }

        res_ptr = g_utf8_offset_to_pointer(working_alphabet->str, offset);
        g_assert_nonnull(res_ptr);

        g_string_append_unichar(result, g_utf8_get_char_validated(res_ptr, -1));
    }
}


static GString *_vigenere_decrypt(GString *text, gssize _dummy G_GNUC_UNUSED, GString *key, GError **error)
{
    return _vigenere_process_ex(text, key, _vigenere_decrypt_cb, error);
}


static gboolean _vigenere_key_parser(GString *key_file, gssize *_dummy G_GNUC_UNUSED, GString **key, GError **error)
{
    g_assert_nonnull(key_file);
    g_assert_nonnull(key);
    g_assert_null(*key);

    if (key_file->len == 0) {
        g_set_error(error,
                    CRYPTO_VIGENERE_ERROR,
                    CRYPTO_VIGENERE_ERROR_INVALID_KEY,
                    "supplied key file is empty");
        return FALSE;
    }

    /* TODO: make sure, that task is understood correctly */
    /* Just pass the key AS-IS; it will be parsed later */
    *key = g_string_new(key_file->str);
    g_assert_nonnull(*key);

    return TRUE;
}


crypto_cipher_t crypto_vigenere_decl = {
    .encrypt_cb = _vigenere_encrypt,
    .decrypt_cb = _vigenere_decrypt,
    .key_parser_cb = _vigenere_key_parser
};
