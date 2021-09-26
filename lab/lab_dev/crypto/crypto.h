#ifndef CRYPTO_H
#define CRYPTO_H

#include <glib.h>

#define CRYPTO_MAIN_ERROR crypto_main_error_quark()

typedef enum {
    CRYPTO_MAIN_ERROR_FAILED,
    CRYPTO_MAIN_ERROR_MODULE_OPEN,
    CRYPTO_MAIN_ERROR_MODULE_SYMBOL
} crypto_main_error;

typedef GString *(*crypto_encrypt_cb_t)(GString *, gssize, GString *, GError **);
typedef GString *(*crypto_decrypt_cb_t)(GString *, gssize, GString *, GError **);
typedef gboolean (*crypto_key_parser_cb_t)(GString *, gssize *, GString **, GError **);

typedef struct _crypto_cipher {
    crypto_encrypt_cb_t encrypt_cb;
    crypto_decrypt_cb_t decrypt_cb;
    crypto_key_parser_cb_t key_parser_cb;
} crypto_cipher_t;

/* Adding support for 'g_autoptr' */
G_DEFINE_AUTOPTR_CLEANUP_FUNC(crypto_cipher_t, g_free)

#endif // CRYPTO_H
