#include "md5.h"


#define MD5_HASH_CHUNK_SIZE         (512 / 8)
#define MD5_HASH_CHUNK_DATA_SIZE    (448 / 8)

/* MD5 functions */
#define MD5_F(B, C, D) (((B) & (C)) | ((~(B)) & (D)))
#define MD5_G(B, C, D) (((B) & (D)) | ((C) & (~(D))))
#define MD5_H(B, C, D) ((B) ^ (C) ^ (D))
#define MD5_I(B, C, D) ((C) ^ ((B) | (~(D))))


static inline guint32 md5_left_rotate(guint32 number, guint32 count)
{
    return (number << count) | (number >> ((sizeof(number) * 8) - count));
}


static GByteArray *md5_hash_get_chunk(const guint8 *data, gsize max_len, gsize *pos)
{
    GByteArray *chunk;
    guint64 chunk_len;
    guint8 byte;

    g_assert_nonnull(data);
    g_assert_nonnull(pos);

    chunk = g_byte_array_sized_new(MD5_HASH_CHUNK_SIZE);
    g_assert_nonnull(chunk);

    chunk_len = MIN(MD5_HASH_CHUNK_SIZE, max_len - (*pos));
    g_byte_array_append(chunk, data + (*pos), chunk_len);

    /* Updating current position in original data */
    *pos += chunk_len;

    /* Padding if necessary */
    if (chunk_len % MD5_HASH_CHUNK_SIZE) {
        /* Appending a single bit */
        byte = 0x80; /* 0b10000000 */
        g_byte_array_append(chunk, &byte, sizeof(byte));

        /* Padding the remaining length with zeros */
        byte = 0x0;
        while ((chunk->len % MD5_HASH_CHUNK_SIZE) < MD5_HASH_CHUNK_DATA_SIZE) {
            g_byte_array_append(chunk, &byte, sizeof(byte));
        }
        chunk_len = max_len * 8;
        g_byte_array_append(chunk, (const guint8 *) &chunk_len, sizeof(chunk_len));
    }

    return chunk;
}


void md5_hash_ctx_init(md5_hash_ctx_t *ctx)
{
    g_assert_nonnull(ctx);

    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
}


void md5_hash_data(md5_hash_ctx_t *ctx, const guint8 *data, gssize len)
{
    /* MD5 hash constants */
    static const guint32 shift_amounts[] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
                                             5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                                             4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                                             6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };
    static const guint32 K[] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                                 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                                 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                                 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                                 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                                 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                                 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                                 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                                 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                                 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                                 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                                 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                                 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                                 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                                 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                                 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
    guint32 M[16];
    guint32 A, B, C, D;
    guint32 i, F, g;

    /* Other variables */
    GByteArray *chunk = NULL;
    gsize pos;

    g_assert_nonnull(data);

    pos = 0;
    do {
        chunk = md5_hash_get_chunk(data, len, &pos);
        g_assert(chunk->len == sizeof(M));
        memcpy(M, chunk->data, sizeof(M));

        A = ctx->a;
        B = ctx->b;
        C = ctx->c;
        D = ctx->d;

        for (i = 0; i < 64; ++i) {
            switch ( i >> 4 ) {  /* Switch on current round */
                case 0:
                    F = MD5_F(B, C, D);
                    g = i;
                    break;
                case 1:
                    F = MD5_G(B, C, D);
                    g = (5 * i + 1) % 16;
                    break;
                case 2:
                    F = MD5_H(B, C, D);
                    g = (3 * i + 5) % 16;
                    break;
                case 3:
                    F = MD5_I(B, C, D);
                    g = (7 * i) % 16;
                    break;
                default:
                    break;
            }

            F += A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + md5_left_rotate(F, shift_amounts[i]);
        }

        ctx->a += A;
        ctx->b += B;
        ctx->c += C;
        ctx->d += D;

        g_byte_array_free(chunk, TRUE);
    } while (pos < len);

    /* Updating hexdigest */
    g_snprintf(ctx->hexdigest, sizeof(ctx->hexdigest),
               "%.8x%.8x%.8x%.8x",
               GUINT32_SWAP_LE_BE(ctx->a),
               GUINT32_SWAP_LE_BE(ctx->b),
               GUINT32_SWAP_LE_BE(ctx->c),
               GUINT32_SWAP_LE_BE(ctx->d));
}


const gchar *md5_hash_get_hexdigest(md5_hash_ctx_t *ctx)
{
    g_assert_nonnull(ctx);

    return ctx->hexdigest;
}
