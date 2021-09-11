#include "crypt.h"

static const gchar *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .,0123456789";


GQuark crypt_error_quark(void)
{
    return g_quark_from_static_string("crypt-error-quark");
}


static gboolean _is_text_valid(GString *text, GError **error)
{
    const gchar *endptr;
    gsize i;

    g_assert_nonnull(text);
    g_assert_nonnull(text->str);

    if (!g_utf8_validate(text->str, (gssize) text->len, &endptr)) {
        g_set_error_literal(error,
                            CRYPT_ERROR,
                            CRYPT_ERROR_TEXT_NOT_UTF8,
                            "Given input is not in UTF-8");
        return FALSE;
    }

    for (i = 0; i < text->len; ++i) {
        if (g_utf8_strchr(alphabet, -1, text->str[i])) {
            g_set_error(error,
                        CRYPT_ERROR,
                        CRYPT_ERROR_TEXT_NOT_IN_ALPHABET,
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

    /* Checking for the rules of input first */
    if (!_is_text_valid(keyword, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    /* Checking length */
    if (keyword->len > g_utf8_strlen(alphabet, -1)) {
        g_set_error(error,
                    CRYPT_ERROR,
                    CRYPT_ERROR_KEYWORD_TOO_LONG,
                    "Keyword string must be less than length of an alphabet (<%lu)",
                    g_utf8_strlen(alphabet, -1));
        return FALSE;
    }

    /* Checking character uniqueness */
    for (i = 0; i < keyword->len - 1; ++i) {
        for (j = i + 1; j < keyword->len; ++j) {
            if (keyword->str[i] == keyword->str[j]) {
                g_set_error_literal(error,
                                    CRYPT_ERROR,
                                    CRYPT_ERROR_KEYWORD_NOT_UNIQUE,
                                    "Characters in keyword should be unique");
                return FALSE;
            }
        }
    }

    return TRUE;
}


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
        if (!_is_keyword_valid(keyword, error)) {
            g_assert((error == NULL) || (*error != NULL));
            return NULL;
        }

        /* Starting new alphabet from given keyword */
        g_string_append_len(working_alphabet, keyword->str, (gssize) keyword->len);

        for (i = 0; i < alphabet_len; ++i) {
            if (g_utf8_strchr(keyword->str, -1, *(alphabet + i)) == NULL) {
                g_string_insert_c(working_alphabet, (gssize) (i + keyword->len) % alphabet_len, *(alphabet + i));
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


static GString *_caesar_cypher_encrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    GString *working_alphabet;
    GString *result = NULL;
    gchar ch;
    gsize i;

    g_assert_nonnull(text);

    if (!_is_text_valid(text, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    if ((offset >= 0) && (offset <= g_utf8_strlen(alphabet, -1))) {
        g_set_error(error,
                    CRYPT_ERROR,
                    CRYPT_ERROR_CAESAR_INVALID_OFFSET,
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

    for (i = 0; i < text->len; ++i) {
        ch = working_alphabet->str[g_utf8_strchr(alphabet, -1, *(text->str + i)) - alphabet];
        g_string_append_c_inline(result, ch);
    }

    return result;
}


static GString *_caesar_cypher_decrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    GString *working_alphabet;
    GString *result = NULL;
    gchar ch;
    gsize i;

    g_assert_nonnull(text);

    if (!_is_text_valid(text, error)) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    if ((offset >= 0) && (offset <= g_utf8_strlen(alphabet, -1))) {
        g_set_error(error,
                    CRYPT_ERROR,
                    CRYPT_ERROR_CAESAR_INVALID_OFFSET,
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

    for (i = 0; i < text->len; ++i) {
        ch = alphabet[g_utf8_strchr(working_alphabet->str, -1, *(text->str + i)) - working_alphabet->str];
        g_string_append_c_inline(result, ch);
    }

    return result;
}


static GString *_simple_table_encrypt(GString *text)
{
    GString *result = NULL;

    g_assert_nonnull(text);

    return result;
}


static GString *_simple_table_decrypt(GString *text)
{
    GString *result = NULL;

    g_assert_nonnull(text);

    return result;
}


GString *encrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    GString *result;

    g_assert_nonnull(text);

    if ((result = _caesar_cypher_encrypt(text, offset, keyword, error)) == NULL) {
        g_assert((error == NULL) || (*error != NULL));
        return NULL;
    }

    return _simple_table_encrypt(result);
}


GString *decrypt(GString *text, gssize offset, GString *keyword, GError **error)
{
    GString *result;

    g_assert_nonnull(text);

    result = _simple_table_decrypt(text);
    g_assert_nonnull(result);

    return _caesar_cypher_decrypt(result, offset, keyword, error);
}
