#ifndef CRYPTO_CAESAR_H
#define CRYPTO_CAESAR_H

#define CRYPTO_CAESAR_ERROR crypto_caesar_error_quark()

typedef enum {
    CRYPTO_CAESAR_ERROR_FAILED,
    CRYPTO_CAESAR_ERROR_TEXT_NOT_UTF8,
    CRYPTO_CAESAR_ERROR_TEXT_NOT_IN_ALPHABET,
    CRYPTO_CAESAR_ERROR_KEYWORD_TOO_LONG,
    CRYPTO_CAESAR_ERROR_KEYWORD_NOT_UNIQUE,
    CRYPTO_CAESAR_ERROR_INVALID_OFFSET,
    CRYPTO_CAESAR_ERROR_INVALID_KEY
} crypto_caesar_error;

#endif // CRYPTO_CAESAR_H
