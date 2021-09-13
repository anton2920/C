#include "caesar.h"

static const gchar *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .,0123456789";


/* ======== ERROR DOMAIN SETUP ======== */
static GQuark caesar_error_quark(void)
{
    return g_quark_from_static_string("caesar-error-quark");
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
                    CAESAR_ERROR,
                    CAESAR_ERROR_TEXT_NOT_UTF8,
                    "Given input is not in UTF-8: invalid character '%c'", *endptr);
        return FALSE;
    }

    for (i = 0; i < text->len; ++i) {
        if (g_utf8_strchr(alphabet, -1, text->str[i]) == NULL) {
            g_set_error(error,
                        CAESAR_ERROR,
                        CAESAR_ERROR_TEXT_NOT_IN_ALPHABET,
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

    if (keyword->len > g_utf8_strlen(alphabet, -1)) {
        g_set_error(error,
                    CAESAR_ERROR,
                    CAESAR_ERROR_KEYWORD_TOO_LONG,
                    "Keyword string must be less than length of an alphabet (<%lu)",
                    g_utf8_strlen(alphabet, -1));
        return FALSE;
    }

    for (i = 0; i < keyword->len - 1; ++i) {
        for (j = i + 1; j < keyword->len; ++j) {
            if (keyword->str[i] == keyword->str[j]) {
                g_set_error(error,
                            CAESAR_ERROR,
                            CAESAR_ERROR_KEYWORD_NOT_UNIQUE,
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
                    CAESAR_ERROR,
                    CAESAR_ERROR_CAESAR_INVALID_OFFSET,
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


static gboolean _process_file_ex(GIOChannel *input_file, GIOChannel *key_file, GIOChannel *output_file, GError **error,
                         GString *(*process_func)(GString *, gssize, GString *, GError **))
{
    GString *input_str = NULL, *output_str = NULL, *keyword_str = NULL;
    gsize key_file_len, bytes_written, term_pos;
    gchar *key_file_str, *endptr;
    GIOStatus status;
    gint key_value;

    g_assert_nonnull(input_file);
    g_assert_nonnull(key_file);
    g_assert_nonnull(output_file);

    /* Reading key_value file first */
    status = g_io_channel_read_to_end(key_file, &key_file_str, &key_file_len, error);
    if ((status != G_IO_STATUS_NORMAL) || (key_file_str == NULL) || (key_file_len == 0)) {
        g_assert((error == NULL) || (*error != NULL));
        return FALSE;
    }

    key_value = (int) g_ascii_strtoll(key_file_str, &endptr, 10);
    if ((endptr + 1) < (key_file_str + key_file_len)) {
        keyword_str = g_string_new_len(endptr + 1, ((gssize) key_file_len) - ((endptr + 1) - key_file_str));
        g_assert_nonnull(keyword_str);

        /* Strip anything at the end */
        while (keyword_str->str[keyword_str->len - 1] == '\n') {
            keyword_str->str[--keyword_str->len] = '\0';
        }
    } else {
        keyword_str = NULL; /* No keyword in the file */
    }
    g_free(key_file_str);

    input_str = g_string_new(NULL);
    g_assert_nonnull(input_str);

    /* For each line in input, produce encrypted line in output */
    while (TRUE) {
        status = g_io_channel_read_line_string(input_file, input_str, &term_pos, error);
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

        if ((output_str = process_func(input_str, key_value, keyword_str, error)) == NULL) {
            g_assert((error == NULL) || (*error != NULL));
            g_string_free(input_str, TRUE);
            g_string_free(keyword_str, TRUE);
            return FALSE;
        }

        status = g_io_channel_write_chars(output_file, output_str->str, (gssize) output_str->len,
                                          &bytes_written, error);
        if ((bytes_written != output_str->len) || (status == G_IO_STATUS_ERROR)) {
            g_string_free(input_str, TRUE);
            g_string_free(keyword_str, TRUE);
            g_string_free(output_str, TRUE);
            return FALSE;
        }
        g_string_free(output_str, TRUE);

        status = g_io_channel_write_unichar(output_file, '\n', error);
        if (status == G_IO_STATUS_ERROR) {
            g_string_free(input_str, TRUE);
            g_string_free(keyword_str, TRUE);
            return FALSE;
        }
    }

    g_string_free(input_str, TRUE);
    g_string_free(keyword_str, TRUE);

    return TRUE;
}


/* ======== BLOCK OF EXPORTED API ======== */
gboolean encrypt_file(GIOChannel *input_file, GIOChannel *key_file, GIOChannel *output_file, GError **error)
{
    return _process_file_ex(input_file, key_file, output_file, error, _caesar_cypher_encrypt);
}


gboolean decrypt_file(GIOChannel *input_file, GIOChannel *key_file, GIOChannel *output_file, GError **error)
{
    return _process_file_ex(input_file, key_file, output_file, error, _caesar_cypher_decrypt);
}
