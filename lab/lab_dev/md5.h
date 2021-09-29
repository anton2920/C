#ifndef MD5_H
#define MD5_H

#include <glib.h>

typedef struct _md5_hash_ctx {
    guint32 a;
    guint32 b;
    guint32 c;
    guint32 d;
} md5_hash_ctx_t;


void md5_hash_ctx_init(md5_hash_ctx_t *ctx);
void md5_hash_data(md5_hash_ctx_t *ctx, const guint8 *data, gssize len);
GString *md5_hash_get_hexdigest(md5_hash_ctx_t *ctx);

#endif // MD5_H
