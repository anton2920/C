#include "../crypto/crypto.h"

#include "crypto_caesar.h"


static const gchar *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .,0123456789";


/* ======== ERROR DOMAIN SETUP ======== */
static GQuark crypto_caesar_error_quark(void)
{
    return g_quark_from_static_string("crypto-caesar-error-quark");
}


/* ======== BLOCK OF MISC HELPERS ======== */
static gboolean _is_text_valid(GString *text, GError **error)
{
    const gchar *endptr;
    gsize i;

    g_assert_nonnull(text);

    if (!g_utf8_validate(text->str, (gssize) text->len, &endptr)) {
        g_assert_nonnull(endptr);
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_TEXT_NOT_UTF8,
                    "given input is not in UTF-8: invalid character '%c'", *endptr);
        return FALSE;
    }

    for (i = 0; i < text->len; ++i) {
        if (g_utf8_strchr(alphabet, -1, text->str[i]) == NULL) {
            g_set_error(error,
                        CRYPTO_CAESAR_ERROR,
                        CRYPTO_CAESAR_ERROR_TEXT_NOT_IN_ALPHABET,
                        "character '%c' is not from alphabet", text->str[i]);
            return FALSE;
        }
    }

    return TRUE;
}


static gboolean _is_keyword_valid(GString *keyword, GError **error)
{
    gssize i, j;

    g_assert_nonnull(keyword);

    /* Checking for the rules of input file first */
    if (!_is_text_valid(keyword, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    if (keyword->len > g_utf8_strlen(alphabet, -1)) {
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_KEYWORD_TOO_LONG,
                    "keyword string must be less than length of an alphabet (<%lu)",
                    g_utf8_strlen(alphabet, -1));
        return FALSE;
    }

    for (i = 0; i < keyword->len - 1; ++i) {
        for (j = i + 1; j < keyword->len; ++j) {
            if (keyword->str[i] == keyword->str[j]) {
                g_set_error(error,
                            CRYPTO_CAESAR_ERROR,
                            CRYPTO_CAESAR_ERROR_KEYWORD_NOT_UNIQUE,
                            "characters in keyword should be unique, but '%c' is not", keyword->str[i]);
                return FALSE;
            }
        }
    }

    return TRUE;
}


/* ======== BLOCK OF CAESAR ======== */
static GString *_caesar_get_alphabet(gssize offset, GString *keyword)
{
    GString *working_alphabet;
    glong alphabet_len;
    gsize i;

    /* Finding length of our basic alphabet */
    alphabet_len = g_utf8_strlen(alphabet, -1);

    /* Constructing new alphabet */
    working_alphabet = g_string_new(NULL);
    g_assert_nonnull(working_alphabet);

    if (keyword != NULL) {
        /* Constructing new alphabet using given keyword */
        g_string_append_len(working_alphabet, keyword->str, (gssize) keyword->len);

        for (i = 0; i < alphabet_len; ++i) {
            if (g_utf8_strchr(keyword->str, -1, *(alphabet + i)) == NULL) {
                g_string_insert_c(working_alphabet,
                                  (gssize) (working_alphabet->len % (alphabet_len - keyword->len + 1)),
                                  *(alphabet + i));
            }
        }
    } else {
        /* Copying alphabet with offset */
        for (i = 0; i < g_utf8_strlen(alphabet, -1); ++i) {
            g_string_append_c_inline(working_alphabet, *(alphabet + (offset + i) % alphabet_len));
        }
    }

    return working_alphabet;
}


static GString *_caesar_cipher_process_ex(GString *text, gssize offset, GString *keyword,
                                          void (*process_cb)(GString *, GString *, GString *),
                                          GError **error)
{
    GString *working_alphabet;
    GString *result = NULL;

    g_assert_nonnull(text);
    g_assert_nonnull(process_cb);

    if (!_is_text_valid(text, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    if ((offset < 0) || (offset > g_utf8_strlen(alphabet, -1))) {
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_INVALID_OFFSET,
                    "offset should be in following range: 0 <= offset <= %ld, but offset is %ld",
                    g_utf8_strlen(alphabet, -1), offset);
        return NULL;
    }

    working_alphabet = _caesar_get_alphabet(offset, keyword);
    g_assert_nonnull(working_alphabet);

    result = g_string_new(NULL);
    g_assert_nonnull(result);

    process_cb(text, working_alphabet, result);

    g_string_free(working_alphabet, TRUE);

    return result;
}


static void _caesar_encrypt_cb(GString *text, GString *working_alphabet, GString *result)
{
    gchar ch;
    gsize i;

    g_assert_nonnull(text);
    g_assert_nonnull(working_alphabet);
    g_assert_nonnull(result);

    for (i = 0; i < text->len; ++i) {
        ch = working_alphabet->str[g_utf8_strchr(alphabet, -1, *(text->str + i)) - alphabet];
        g_string_append_c_inline(result, ch);
    }
}


static GString *_caesar_cipher_encrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    return _caesar_cipher_process_ex(text, offset, keyword, _caesar_encrypt_cb, error);
}


static void _caesar_decrypt_cb(GString *text, GString *working_alphabet, GString *result)
{
    gchar ch;
    gsize i;

    g_assert_nonnull(text);
    g_assert_nonnull(working_alphabet);
    g_assert_nonnull(result);

    for (i = 0; i < text->len; ++i) {
        ch = alphabet[g_utf8_strchr(working_alphabet->str, -1, *(text->str + i)) - working_alphabet->str];
        g_string_append_c_inline(result, ch);
    }
}


static GString *_caesar_cipher_decrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    return _caesar_cipher_process_ex(text, offset, keyword, _caesar_decrypt_cb, error);
}


static gboolean _caesar_cipher_key_parser(GString *key_text, gssize *offset, GString **keyword, GError **error)
{
    gchar **tokens, *token, *endptr;

    g_assert_nonnull(key_text);
    g_assert_nonnull(offset);
    g_assert_nonnull(keyword);
    g_assert_null(*keyword);

    if (key_text->len == 0) {
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_INVALID_KEY,
                    "supplied key file is empty");
        return FALSE;
    }

    /* Splitting given key string */
    tokens = g_strsplit_set(key_text->str, " \t\v\r\n", 2);
    token = g_strchug(g_strchomp(tokens[0]));
    g_assert_nonnull(token);

    *offset = g_ascii_strtoll(token, &endptr, 10);
    if ((endptr != NULL) && (*endptr != '\0')) {
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_INVALID_KEY,
                    "supplied key contains invalid value for Caesar's offset");
        g_strfreev(tokens);
        return FALSE;
    }

    token = g_strchug(g_strchomp(tokens[1]));
    if ((token != NULL) && (g_utf8_strlen(token, -1) > 0)) {
        *keyword = g_string_new(token);
        g_assert_nonnull(*keyword);

        /* Checking the keyword */
        if (!_is_keyword_valid(*keyword, error)) {
            g_assert((error == NULL) || (*error != NULL));
            return FALSE;
        }
    } else {
        *keyword = NULL; /* No keyword in the file */
    }

    g_strfreev(tokens);

    return TRUE;
}


crypto_cipher_t crypto_caesar_decl = {
    .encrypt_cb = _caesar_cipher_encrypt,
    .decrypt_cb = _caesar_cipher_decrypt,
    .key_parser_cb = _caesar_cipher_key_parser
};
