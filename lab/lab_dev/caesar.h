#ifndef CAESAR_H
#define CAESAR_H

#include <glib.h>

#define CAESAR_ERROR caesar_error_quark()

typedef enum {
    CAESAR_ERROR_FAILED,
    CAESAR_ERROR_TEXT_NOT_UTF8,
    CAESAR_ERROR_TEXT_NOT_IN_ALPHABET,
    CAESAR_ERROR_KEYWORD_TOO_LONG,
    CAESAR_ERROR_KEYWORD_NOT_UNIQUE,
    CAESAR_ERROR_CAESAR_INVALID_OFFSET
} caesar_error;


/* Functions to operate on files */
gboolean encrypt_file(GIOChannel *input_file, GIOChannel *key_file, GIOChannel *output_file, GError **error);
gboolean decrypt_file(GIOChannel *input_file, GIOChannel *key_file, GIOChannel *output_file, GError **error);

#endif // CAESAR_H
