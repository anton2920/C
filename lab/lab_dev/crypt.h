#ifndef CRYPT_H
#define CRYPT_H

#include <glib.h>
#include <jmorecfg.h>

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


/* Functions to operate on strings */
GString *encrypt(GString *text, gssize offset, GString *keyword, GError **error);
GString *decrypt(GString *text, gssize offset, GString *keyword, GError **error);


/* Functions to operate on files */
boolean encrypt_file(GIOChannel *input_file, GIOChannel *key_file, GIOChannel *output_file, GError **error);
boolean decrypt_file(GIOChannel *input_file, GIOChannel *key_file, GIOChannel *output_file, GError **error);

#endif // CRYPT_H
