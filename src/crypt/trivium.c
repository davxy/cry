#include <cry/trivium.h>
#include <string.h>
#include "misc.h"


#define S(a, n) (s##a##n)
#define T(a)    (t##a)

#define LOAD(s) do { \
    CRY_READ32_LE(S(1, 1), (s) +  0); \
    CRY_READ32_LE(S(1, 2), (s) +  4); \
    CRY_READ32_LE(S(1, 3), (s) +  8); \
    CRY_READ32_LE(S(2, 1), (s) + 12); \
    CRY_READ32_LE(S(2, 2), (s) + 16); \
    CRY_READ32_LE(S(2, 3), (s) + 20); \
    CRY_READ32_LE(S(3, 1), (s) + 24); \
    CRY_READ32_LE(S(3, 2), (s) + 28); \
    CRY_READ32_LE(S(3, 3), (s) + 32); \
    CRY_READ32_LE(S(3, 4), (s) + 36); \
} while (0)

#define STORE(s) do { \
    CRY_WRITE32_LE(S(1, 1), (s) +  0); \
    CRY_WRITE32_LE(S(1, 2), (s) +  4); \
    CRY_WRITE32_LE(S(1, 3), (s) +  8); \
    CRY_WRITE32_LE(S(2, 1), (s) + 12); \
    CRY_WRITE32_LE(S(2, 2), (s) + 16); \
    CRY_WRITE32_LE(S(2, 3), (s) + 20); \
    CRY_WRITE32_LE(S(3, 1), (s) + 24); \
    CRY_WRITE32_LE(S(3, 2), (s) + 28); \
    CRY_WRITE32_LE(S(3, 3), (s) + 32); \
    CRY_WRITE32_LE(S(3, 4), (s) + 36); \
} while (0)


#define S64(a, b) ((S(a, 3) << ( 96 - (b))) | (S(a, 2) >> ((b) - 64)))
#define S96(a, b) ((S(a, 4) << (128 - (b))) | (S(a, 3) >> ((b) - 96)))

#define UPDATE() do { \
    T(1) = S64(1,  66) ^ S64(1,  93); \
    T(2) = S64(2,  69) ^ S64(2,  84); \
    T(3) = S64(3,  66) ^ S96(3, 111); \
    Z(T(1) ^ T(2) ^ T(3));            \
    T(1) ^= (S64(1,  91) & S64(1,  92)) ^ S64(2,  78); \
    T(2) ^= (S64(2,  82) & S64(2,  83)) ^ S64(3,  87); \
    T(3) ^= (S96(3, 109) & S96(3, 110)) ^ S64(1,  69); \
} while (0)

#define ROTATE() do {   \
    S(1, 3) = S(1, 2);  \
    S(1, 2) = S(1, 1);  \
    S(1, 1) = T(3);     \
    S(2, 3) = S(2, 2);  \
    S(2, 2) = S(2, 1);  \
    S(2, 1) = T(1);     \
    S(3, 4) = S(3, 3);  \
    S(3, 3) = S(3, 2);  \
    S(3, 2) = S(3, 1);  \
    S(3, 1) = T(2);     \
} while (0)



void cry_trivium_init(cry_trivium_ctx *ctx)
{
    memset(ctx, 0, sizeof(*ctx));
}

void cry_trivium_key_set(cry_trivium_ctx *ctx, const unsigned char *key,
                         size_t size)
{
    size_t i;

    ctx->keylen = (size <= CRY_TRIVIUM_KEYLEN) ? size : CRY_TRIVIUM_KEYLEN;

    for (i = 0; i < ctx->keylen; i++)
        ctx->key[i] = key[i];
    for ( ; i < CRY_TRIVIUM_KEYLEN; i++)
        ctx->key[i] = 0;
}

void cry_trivium_iv_set(cry_trivium_ctx *ctx, const unsigned char *iv,
                        size_t size)
{
    size_t i;
    uint32_t s11, s12, s13;
    uint32_t s21, s22, s23;
    uint32_t s31, s32, s33, s34;
    uint32_t t1, t2, t3;

    ctx->ivlen = (size <= CRY_TRIVIUM_IVLEN) ? size : CRY_TRIVIUM_IVLEN;

    /*
     * S initialized as:
     *  [ key || 0 ] ||      (96 bits)
     *  [  iv || 0 ] ||      (96 bits)
     *  [ 0 ... 0 || 0x70 || 0 ... 0 ]  (128 bits)
     *
     * The last byte of the second FSR and the last two bytes of the
     * third FSR are not used.
     */
    for (i = 0; i < ctx->keylen; i++)
        ctx->s[i] = ctx->key[i];
    for ( ; i < 12; i++)
        ctx->s[i] = 0;
    for (i = 0; i < ctx->ivlen; i++)
        ctx->s[12 + i] = iv[i];
    for ( ; i < 12; i++)
        ctx->s[12 + i] = 0;
    for (i = 0; i < 13; i++)
        ctx->s[24 + i] = 0;
    ctx->s[24 + 13] = 0x70;

    LOAD(ctx->s);
#define Z(w)
    for (i = 0; i < 4 * 9; i++) {
        UPDATE();
        ROTATE();
    }
    STORE(ctx->s);
}


static void operate(cry_trivium_ctx *ctx, unsigned char *dst,
                    const unsigned char *src, size_t size)

{
    size_t i;
    uint32_t s11, s12, s13;
    uint32_t s21, s22, s23;
    uint32_t s31, s32, s33, s34;
    uint32_t z;
    uint32_t t1, t2, t3;

    LOAD(ctx->s);

#undef Z
#define Z(w) do { \
    CRY_READ32_LE(z, src + 4 * i);  \
    z ^= w;                         \
    CRY_WRITE32_LE(z, dst + 4 * i); \
} while (0)

    for (i = 0; i < size / 4; i++) {
        UPDATE();
        ROTATE();
    }

#undef Z
#define Z(w) (z = w)

    i *= 4;
    if (i < size) {
        UPDATE();
        ROTATE();
        for ( ; i < size; i++, z >>= 8)
            dst[i] = src[i] ^ (uint8_t)(z);
    }

    STORE(ctx->s);
}

void cry_trivium_encrypt(cry_trivium_ctx *ctx, unsigned char *dst,
                         const unsigned char *src, size_t size)
{
    operate(ctx, dst, src, size);
}

void cry_trivium_decrypt(cry_trivium_ctx *ctx, unsigned char *dst,
                         const unsigned char *src, size_t size)
{
    operate(ctx, dst, src, size);
}
