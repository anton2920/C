#ifndef CRYPTO_IO_H
#define CRYPTO_IO_H

#include <glib.h>
#include <gio/gio.h>

#include "crypto.h"


gboolean crypto_encrypt_file(gchar *input_file_name, gchar *key_file_name, gchar *output_file_name,
                             crypto_cipher_t *cipher, GError **error);

gboolean crypto_decrypt_file(gchar *input_file_name, gchar *key_file_name, gchar *output_file_name,
                             crypto_cipher_t *cipher, GError **error);

#endif // CRYPTO_IO_H
