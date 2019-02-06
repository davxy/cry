#include <cry/cmac.h>
#include <string.h>

static void xor_128(unsigned char *o, const unsigned char *a,
                    const unsigned char *b)
{
    size_t i;

    for (i = 0; i < 16; i++)
        o[i] = a[i] ^ b[i];
}

static void cbc_compute_subkeys(cry_cmac_ctx *ctx)
{
    unsigned char blank[16] = {0};
    unsigned char msb;
    size_t i;
    unsigned char *k1 = ctx->k1;
    unsigned char *k2 = ctx->k2;

    ctx->ciph_itf->encrypt(ctx->ciph_ctx, k1, blank, 16);

    msb = (k1[0] & 0x80);
    for (i = 0; i < 16; i++) {
        k1[i] <<= 1;
        if (i < 15 && ((k1[i + 1] & 0x80) != 0))
            k1[i] |= 1;
    }
    if (msb != 0)
        k1[15] ^= 0x87;

    msb = (k1[0] & 0x80);
    for (i = 0; i < 16; i++) {
        k2[i] = k1[i] << 1;
        if (i < 15 && ((k1[i + 1] & 0x80) != 0))
            k2[i] |= 1;
    }
    if (msb != 0)
        k2[15] ^= 0x87;
}

static void pad(unsigned char *pad, const unsigned char *last, size_t size)
{
    size_t i;

    for (i = 0; i < 16; i++) {
        if (i < size)
            pad[i] = last[i];
        else if (i == size)
            pad[i] = 0x80;
        else
            pad[i] = 0x00;
    }
}


void cry_cmac_init(cry_cmac_ctx *ctx, void *ciph_ctx,
                   const cry_ciph_itf *ciph_itf,
                   unsigned char *key, size_t keylen)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->ciph_ctx = ciph_ctx;
    ctx->ciph_itf = ciph_itf;
    ciph_itf->key_set(ciph_ctx, key, keylen);
    cbc_compute_subkeys(ctx);
}

void cry_cmac_update(cry_cmac_ctx *ctx, const unsigned char *data, size_t len)
{
    unsigned char block[16], last[16], padded[16];
    size_t n, i;
    int flag;

    n = (len + 15) / 16;  /* n is number of rounds */
    if (n == 0) {
        n = 1;
        flag = 0;
    } else {
        if ((len % 16) == 0)
            flag = 1; /* last block is a complete block */
        else
            flag = 0; /* last block is not complete block */
    }

    if (flag) {
        /* last block is complete block */
        xor_128(last, &data[16*(n-1)], ctx->k1);
    } else {
        pad(padded, &data[16*(n-1)], len % 16);
        xor_128(last, padded, ctx->k2);
    }

    memset(ctx->mac, 0, 16);
    for (i = 0; i < (n - 1); i++) {
        xor_128(block, ctx->mac, &data[16*i]);
        ctx->ciph_itf->encrypt(ctx->ciph_ctx, ctx->mac, block, 16);
    }

    xor_128(block, ctx->mac, last);
    ctx->ciph_itf->encrypt(ctx->ciph_ctx, ctx->mac, block, 16);
}

void cry_cmac_digest(cry_cmac_ctx *ctx, unsigned char *mac)
{
    memcpy(mac, ctx->mac, sizeof(ctx->mac));
}
