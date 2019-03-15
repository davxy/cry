#include <cry/cmac.h>
#include <string.h>
#include "misc.h"


static void xor_128(unsigned char *o, const unsigned char *a,
                    const unsigned char *b)
{
    size_t i;

    for (i = 0; i < CRY_CMAC_BLOCK_SIZE; i++)
        o[i] = a[i] ^ b[i];
}

static void cbc_compute_subkeys(cry_cmac_ctx *ctx,
                                unsigned char *k1, unsigned char *k2)
{
    unsigned char blank[CRY_CMAC_BLOCK_SIZE] = {0};
    unsigned char msb;
    size_t i;

    ctx->ciph_itf->encrypt(ctx->ciph_ctx, k1, blank, CRY_CMAC_BLOCK_SIZE);

    msb = (k1[0] & 0x80);
    for (i = 0; i < CRY_CMAC_BLOCK_SIZE; i++) {
        k1[i] <<= 1;
        if (i < CRY_CMAC_BLOCK_SIZE-1 && ((k1[i + 1] & 0x80) != 0))
            k1[i] |= 1;
    }
    if (msb != 0)
        k1[CRY_CMAC_BLOCK_SIZE-1] ^= 0x87;

    msb = (k1[0] & 0x80);
    for (i = 0; i < CRY_CMAC_BLOCK_SIZE; i++) {
        k2[i] = k1[i] << 1;
        if (i < CRY_CMAC_BLOCK_SIZE-1 && ((k1[i + 1] & 0x80) != 0))
            k2[i] |= 1;
    }
    if (msb != 0)
        k2[CRY_CMAC_BLOCK_SIZE-1] ^= 0x87;
}




void cry_cmac_init(cry_cmac_ctx *ctx, void *ciph_ctx,
                   const cry_ciph_itf *ciph_itf,
                   unsigned char *key, size_t keylen)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->ciph_ctx = ciph_ctx;
    ctx->ciph_itf = ciph_itf;
    ciph_itf->key_set(ciph_ctx, key, keylen);
}

void cry_cmac_update(cry_cmac_ctx *ctx, const unsigned char *data, size_t len)
{
    unsigned char block[CRY_CMAC_BLOCK_SIZE];
    size_t n, i;

    /* There was an incomplete block */
    if (ctx->blklen > 0) {
        n = CRY_MIN(len, CRY_CMAC_BLOCK_SIZE - ctx->blklen);
        memcpy(ctx->blk + ctx->blklen, data, n);
        len -= n;
        data += n;
        ctx->blklen += n;
        if (ctx->blklen < CRY_CMAC_BLOCK_SIZE || len == 0)
            return; /* may be the last block */
        xor_128(block, ctx->mac, ctx->blk);
        ctx->ciph_itf->encrypt(ctx->ciph_ctx, ctx->mac, block, CRY_CMAC_BLOCK_SIZE);
    }

    /* Compute number of rounds */
    n = (len + CRY_CMAC_BLOCK_SIZE-1) / CRY_CMAC_BLOCK_SIZE;
    if (n == 0)
        return;
    n--;

    /* last block shall be processed in the digest */
    for (i = 0; i < n; i++) {
        xor_128(block, ctx->mac, &data[CRY_CMAC_BLOCK_SIZE*i]);
        ctx->ciph_itf->encrypt(ctx->ciph_ctx, ctx->mac, block, CRY_CMAC_BLOCK_SIZE);
    }

    /* last block is not complete block */
    ctx->blklen = len % CRY_CMAC_BLOCK_SIZE;
    if (ctx->blklen == 0)
        ctx->blklen = CRY_CMAC_BLOCK_SIZE;
    memcpy(ctx->blk, &data[CRY_CMAC_BLOCK_SIZE*n], ctx->blklen);
}

static void pad(unsigned char *blk, size_t size)
{
    size_t i;

    blk[size] = 0x80;
    for (i = size + 1; i < CRY_CMAC_BLOCK_SIZE; i++)
        blk[i] = 0x00;
}

void cry_cmac_digest(cry_cmac_ctx *ctx, unsigned char *mac)
{
    unsigned char k1[CRY_CMAC_BLOCK_SIZE], k2[CRY_CMAC_BLOCK_SIZE], last[CRY_CMAC_BLOCK_SIZE];

    cbc_compute_subkeys(ctx, k1, k2);

    if (ctx->blklen == CRY_CMAC_BLOCK_SIZE) {
        xor_128(last, ctx->blk, k1);
    } else {
        pad(ctx->blk, ctx->blklen);
        xor_128(last, ctx->blk, k2);
    }

    xor_128(ctx->blk, ctx->mac, last);
    ctx->ciph_itf->encrypt(ctx->ciph_ctx, ctx->mac, ctx->blk, CRY_CMAC_BLOCK_SIZE);
    memcpy(mac, ctx->mac, sizeof(ctx->mac));
}
