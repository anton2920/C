#ifndef MD5_HEADER
#define MD5_HEADER

#include <glib.h>


#define MD5_HEXDIGEST_SIZE (32 + sizeof('\0'))


enum {
    MD5_A,
    MD5_B,
    MD5_C,
    MD5_D,
    MD5_HASH_SIZE
};


typedef struct _md5_hash_ctx {
    guint32 hash[MD5_HASH_SIZE];
    guint64 total_bytes;
    gchar   hexdigest[MD5_HEXDIGEST_SIZE];
} md5_hash_ctx_t;


void md5_hash_ctx_init(md5_hash_ctx_t *ctx);
void md5_hash_update(md5_hash_ctx_t *ctx, const guint8 *data, gssize len);
const gchar *md5_hash_get_hexdigest(md5_hash_ctx_t *ctx);

#endif // MD5_HEADER
