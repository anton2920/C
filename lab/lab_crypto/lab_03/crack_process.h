#ifndef CRACK_PROCESS_H
#define CRACK_PROCESS_H

#include <glib.h>


GHashTable *crack_process_statistics_file(const gchar *file_path, GError **error);
gboolean
crack_bruteforce_numeric_key(crypto_decrypt_cb_t process_func, GHashTable *letter_stats,
                             const gchar *input_file_path,
                             const gchar *output_file_path,
                             GError **error);

#endif // CRACK_PROCESS_H
