#include "crypto.h"

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
    g_assert_nonnull(text->str);

    if (!g_utf8_validate(text->str, (gssize) text->len, &endptr)) {
        g_assert_nonnull(endptr);
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_TEXT_NOT_UTF8,
                    "Given input_file_path is not in UTF-8: invalid character '%c'", *endptr);
        return FALSE;
    }

    for (i = 0; i < text->len; ++i) {
        if (g_utf8_strchr(alphabet, -1, text->str[i]) == NULL) {
            g_set_error(error,
                        CRYPTO_CAESAR_ERROR,
                        CRYPTO_CAESAR_ERROR_TEXT_NOT_IN_ALPHABET,
                        "Character '%c' is not from alphabet", text->str[i]);
            return FALSE;
        }
    }

    return TRUE;
}


static gboolean _is_keyword_valid(GString *keyword, GError **error)
{
    gssize i, j;

    g_assert_nonnull(keyword);

    /* Checking for the rules of input_file_path first */
    if (!_is_text_valid(keyword, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    if (keyword->len > g_utf8_strlen(alphabet, -1)) {
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_KEYWORD_TOO_LONG,
                    "Keyword string must be less than length of an alphabet (<%lu)",
                    g_utf8_strlen(alphabet, -1));
        return FALSE;
    }

    for (i = 0; i < keyword->len - 1; ++i) {
        for (j = i + 1; j < keyword->len; ++j) {
            if (keyword->str[i] == keyword->str[j]) {
                g_set_error(error,
                            CRYPTO_CAESAR_ERROR,
                            CRYPTO_CAESAR_ERROR_KEYWORD_NOT_UNIQUE,
                            "Characters in keyword should be unique, but '%c' is not", keyword->str[i]);
                return FALSE;
            }
        }
    }

    return TRUE;
}


/* ======== BLOCK OF CAESAR ======== */
static GString *_caesar_get_alphabet(gssize offset, GString *keyword, GError **error)
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
        if (!_is_keyword_valid(keyword, error)) {
            g_assert((error == NULL) || (*error != NULL));
            g_string_free(working_alphabet, TRUE);
            return NULL;
        }

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


static GString *_caesar_cypher_process_ex(GString *text, gssize offset, GString *keyword, GError **error,
                                          void (*process_cb)(GString *, GString *, GString *))
{
    GString *working_alphabet;
    GString *result = NULL;

    g_assert_nonnull(text);

    if (!_is_text_valid(text, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    if ((offset < 0) || (offset > g_utf8_strlen(alphabet, -1))) {
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_CAESAR_INVALID_OFFSET,
                    "Offset should be in following range: 0 <= offset <= %ld, but offset is %ld",
                    g_utf8_strlen(alphabet, -1), offset);
        return NULL;
    }

    if ((working_alphabet = _caesar_get_alphabet(offset, keyword, error)) == NULL) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

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


static GString *_caesar_cypher_encrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    return _caesar_cypher_process_ex(text, offset, keyword, error, _caesar_encrypt_cb);
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


static GString *_caesar_cypher_decrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    return _caesar_cypher_process_ex(text, offset, keyword, error, _caesar_decrypt_cb);
}


static gboolean _caesar_cypher_key_parser(GString *key_text, gssize *offset, GString **keyword, GError **error)
{
    gchar *endptr;

    *offset = (int) g_ascii_strtoll(key_text->str, &endptr, 10);
    if (endptr == key_text->str) {
        g_set_error(error,
                    CRYPTO_CAESAR_ERROR,
                    CRYPTO_CAESAR_ERROR_INVALID_KEY,
                    "Supplied key contains invalid value for Caesar's offset");
        return FALSE;
    }

    if ((endptr + 1) < (key_text->str + key_text->len)) {
        (*keyword) = g_string_new_len(endptr + 1, ((gssize) key_text->len) - ((endptr + 1) - key_text->str));
        g_assert_nonnull(*keyword);

        /* Strip anything at the end */
        while ((*keyword)->str[(*keyword)->len - 1] == '\n') {
            (*keyword)->str[--(*keyword)->len] = '\0';
        }
    } else {
        (*keyword) = NULL; /* No keyword in the file */
    }

    return TRUE;
}


crypto_cipher_t crypto_caesar_decl =
{
    .encrypt_cb = _caesar_cypher_encrypt,
    .decrypt_cb = _caesar_cypher_decrypt,
    .key_parser_cb = _caesar_cypher_key_parser
};
