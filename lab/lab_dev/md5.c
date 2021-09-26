#include "md5.h"


#define MD5_HASH_CHUNK_SIZE         (512 / 8)
#define MD5_HASH_CHUNK_DATA_SIZE    (448 / 8)


static inline guint32 md5_left_rotate(guint32 number, guint32 count)
{
    return (number << count) | (number >> ((sizeof(number) * 8) - count));
}


GByteArray *md5_hash_get_chunk(const guint8 *data, gsize max_len, gsize *pos)
{
    GByteArray *chunk;
    guint64 chunk_len;
    guint8 byte;

    g_assert_nonnull(data);
    g_assert_nonnull(pos);

    chunk = g_byte_array_sized_new(MD5_HASH_CHUNK_SIZE);
    g_assert_nonnull(chunk);

    chunk_len = MIN(MD5_HASH_CHUNK_DATA_SIZE, max_len - (*pos));
    g_byte_array_append(chunk, data, chunk_len);

    byte = 0x80; /* 0b10000000 */
    g_byte_array_append(chunk, &byte, sizeof(byte));

    byte = 0x0;
    while ((chunk->len % MD5_HASH_CHUNK_SIZE) < MD5_HASH_CHUNK_DATA_SIZE) {
        g_byte_array_append(chunk, &byte, sizeof(byte));
    }
    *pos += chunk_len;

    chunk_len *= 8;
    /*chunk_len = GUINT64_TO_BE(chunk_len);*/ /* Convert length to big-endian */
    g_byte_array_append(chunk, (const guint8 *) &chunk_len, sizeof(chunk_len));

    g_print("Chunk bytes: ");
    guint32 i;
    for (i = 0; i < chunk->len; ++i) {
        g_print("%2x ", *(chunk->data + i));
    }
    g_print("\n");

    return chunk;
}


GString *md5_hash_get_digest_from_string(const GString *input)
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
    guint32 a0 = 0x67452301, b0 = 0xefcdab89, c0 = 0x98badcfe, d0 = 0x10325476;
    guint32 M[16];
    guint32 A, B, C, D;
    guint32 i, F, g;

    /* Other variables */
    GByteArray *chunk = NULL;
    GString *digest;
    gsize pos;

    g_assert_nonnull(input);

    digest = g_string_new(NULL);
    g_assert_nonnull(digest);

    pos = 0;
    do {
        chunk = md5_hash_get_chunk((guint8 *) input->str, input->len, &pos);
        g_assert(chunk->len == sizeof(M));
        memcpy(M, chunk->data, sizeof(M));

        A = a0;
        B = b0;
        C = c0;
        D = d0;

        for (i = 0; i < 64; ++i) {
            if (i < 16) {
                F = (B & C) | ((~B) & D);
                g = i;
            } else if (i < 32) {
                F = (B & D) | (C & (~D));
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else {
                F = C ^ (B | (~D));
                g = (7 * i) % 16;
            }

            F += A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + md5_left_rotate(F, shift_amounts[i]);
        }

        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;

        g_byte_array_free(chunk, TRUE);
    } while (pos < input->len);

    g_print("\n");

    g_string_sprintf(digest, "%x%.8x%.8x%.8x",
                     GUINT32_SWAP_LE_BE(a0),
                     GUINT32_SWAP_LE_BE(b0),
                     GUINT32_SWAP_LE_BE(c0),
                     GUINT32_SWAP_LE_BE(d0));

    return digest;
}