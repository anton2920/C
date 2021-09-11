#ifndef CRYPT_H
#define CRYPT_H

#include <glib.h>

#define CRYPT_ERROR crypt_error_quark()

typedef enum {
    CRYPT_ERROR_FAILED,
    CRYPT_ERROR_TEXT_NOT_UTF8,
    CRYPT_ERROR_TEXT_NOT_IN_ALPHABET,
    CRYPT_ERROR_KEYWORD_TOO_LONG,
    CRYPT_ERROR_KEYWORD_NOT_UNIQUE,
    CRYPT_ERROR_CAESAR_INVALID_OFFSET
} crypt_error;

typedef enum {
    CRYPT
} crypt_keyword_validation_erro;

GString *encrypt(GString *text, gssize offset, GString *keyword, GError **error);
GString *decrypt(GString *text, gssize offset, GString *keyword, GError **error);

#endif // CRYPT_H
