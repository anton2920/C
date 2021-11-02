#include "../crypto/crypto.h"

#include "crypto_gamma.h"


/* Let's assume, lowercase letters are needed; 'ё' is excluded */
static const gchar *alphabet = "абвгдежзийклмнопрстуфхцчшщъыьэюя., ";


/* ======== ERROR DOMAIN SETUP ======== */
static GQuark crypto_gamma_error_quark(void)
{
    return g_quark_from_static_string("crypto-gamma-error-quark");
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
                    CRYPTO_GAMMA_ERROR,
                    CRYPTO_GAMMA_ERROR_TEXT_NOT_UTF8,
                    "given input file is not in UTF-8: invalid character '%c'", *endptr);
        return FALSE;
    }

    for (i = 0; i < g_utf8_strlen(text->str, -1); ++i) {
        ptr = g_utf8_offset_to_pointer(text->str, (glong) i);
        g_assert_nonnull(ptr);

        if ((g_utf8_strchr(alphabet, -1, g_utf8_get_char_validated(ptr, -1)) == NULL)) {
            g_set_error(error,
                        CRYPTO_GAMMA_ERROR,
                        CRYPTO_GAMMA_ERROR_TEXT_NOT_IN_ALPHABET,
                        "character 'U+%04"G_GINT32_FORMAT"X' is not from alphabet",
                        g_utf8_get_char_validated(ptr, -1));
            return FALSE;
        }
    }

    return TRUE;
}


/* TODO: random renerator */


/* ======== BLOCK OF GAMMA ======== */
static GString *_gamma_process_ex(GString *key_file, gssize key, GString *_dummy G_GNUC_UNUSED, GError **error)
{
    GString *result;

    g_assert_nonnull(key_file);

    result = g_string_new(NULL);
    g_assert_nonnull(result);

    /* TODO: set initial seed */
    /*_gamma_rand_init(key);*/

    /* TODO: process with XOR */


    return result;
}


static gboolean _gamma_key_parser(GString *key_file, gssize *key, GString **_dummy G_GNUC_UNUSED, GError **error)
{
    gchar *endptr;

    g_assert_nonnull(key_file);
    g_assert_nonnull(key);

    if (key_file->len == 0) {
        g_set_error(error,
                    CRYPTO_GAMMA_ERROR,
                    CRYPTO_GAMMA_ERROR_INVALID_KEY,
                    "supplied key file is empty");
        return FALSE;
    }

    *key = g_ascii_strtoll(g_strstrip(key_file->str), &endptr, 2);
    if ((endptr != NULL) && (*endptr != '\0')) {
        g_set_error(error,
                    CRYPTO_GAMMA_ERROR,
                    CRYPTO_GAMMA_ERROR_INVALID_KEY,
                    "supplied key contains invalid characters (should be base-2 string)");
        return FALSE;
    }

    return TRUE;
}


crypto_cipher_t crypto_gamma_decl = {
    .encrypt_cb = _gamma_process_ex,
    .decrypt_cb = _gamma_process_ex,
    .key_parser_cb = _gamma_key_parser
};
