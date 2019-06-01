#include <cry/gcm.h>
#include "memxor.h"
#include "misc.h"
#include <string.h>
#include <stdint.h>


#define POLY 0xE1UL

static void gcm_gf_mul(unsigned char *x, const unsigned char *y)
{
    uint32_t V[CRY_GCM_BLOCK_SIZE >> 2];
    uint32_t Z[CRY_GCM_BLOCK_SIZE >> 2] = {0};
    uint32_t mask;
    unsigned char b;
    size_t i, j;

    CRY_READ32_BE(V[0], x);
    CRY_READ32_BE(V[1], x+4);
    CRY_READ32_BE(V[2], x+8);
    CRY_READ32_BE(V[3], x+12);
    for (i = 0; i < CRY_GCM_BLOCK_SIZE; i++) {
        b = y[i];
        for (j = 0; j < 8; j++, b <<= 1) {
            if (b & 0x80) {
                /* GF add (xor) */
                Z[0] ^= V[0];
                Z[1] ^= V[1];
                Z[2] ^= V[2];
                Z[3] ^= V[3];
            }
            /* GF double (shift left) */
            mask = (~(V[3] & 1U)) + 1;
            V[3] = (V[3] >> 1U) | ((V[2] & 1) << 31U);
            V[2] = (V[2] >> 1U) | ((V[1] & 1) << 31U);
            V[1] = (V[1] >> 1U) | ((V[0] & 1) << 31U);
            V[0] = (V[0] >> 1U) ^ (mask & (POLY << 24U));
        }
    }
    CRY_WRITE32_BE(Z[0], x);
    CRY_WRITE32_BE(Z[1], x + 4);
    CRY_WRITE32_BE(Z[2], x + 8);
    CRY_WRITE32_BE(Z[3], x + 12);
}

static void gcm_hash(unsigned char *hash, const unsigned char *key,
                     const unsigned char *data, size_t len)
{
    while (len >= CRY_GCM_BLOCK_SIZE) {
        cry_memxor(hash, data, CRY_GCM_BLOCK_SIZE);
        gcm_gf_mul(hash, key);
        len -= CRY_GCM_BLOCK_SIZE;
        data += CRY_GCM_BLOCK_SIZE;
    }
    if (len > 0) {
        cry_memxor(hash, data, len);
        gcm_gf_mul(hash, key);
    }
}

static void gcm_hash_sizes(unsigned char *hash, const unsigned char *key,
                           size_t auth_size, size_t ciph_size)
{
    unsigned char buf[CRY_GCM_BLOCK_SIZE] = {0};
    uint32_t bits;

    /* Sizes in bits */
    bits = (uint32_t)(auth_size << 3);
    CRY_WRITE32_BE(bits, buf + 4);
    bits = (uint32_t)(ciph_size << 3);
    CRY_WRITE32_BE(bits, buf + 12);
    gcm_hash(hash, key, buf, CRY_GCM_BLOCK_SIZE);
}


void cry_gcm_init(struct cry_gcm_ctx *ctx, void *ciph_ctx,
                  const struct cry_ciph_itf *ciph_itf)
{
    memset(ctx, 0, sizeof(struct cry_gcm_ctx));
    ctx->ciph_ctx = ciph_ctx;
    ctx->ciph_itf = ciph_itf;
}

void cry_gcm_key_set(struct cry_gcm_ctx *gcm, const unsigned char *key,
                     size_t size)
{
    void *ciph = gcm->ciph_ctx;
    cry_ciph_encrypt_f encrypt = gcm->ciph_itf->encrypt;
    cry_ciph_key_set_f key_set = gcm->ciph_itf->key_set;

    /* Reset all */
    memset(gcm->hs, 0, CRY_GCM_BLOCK_SIZE);
    memset(gcm->ctr, 0, CRY_GCM_BLOCK_SIZE);
    memset(gcm->iv, 0, CRY_GCM_BLOCK_SIZE);
    memset(gcm->key, 0, CRY_GCM_BLOCK_SIZE);
    gcm->auth_len = 0;
    gcm->ciph_len = 0;

    key_set(ciph, key, size);

    memset(gcm->key, 0, CRY_GCM_BLOCK_SIZE);
    encrypt(ciph, gcm->key, gcm->key, CRY_GCM_BLOCK_SIZE);
}


void cry_gcm_iv_set(struct cry_gcm_ctx *ctx, const unsigned char *iv,
                    size_t size)
{
    if (size == (CRY_GCM_BLOCK_SIZE-4)) {
        memcpy(ctx->iv, iv, CRY_GCM_BLOCK_SIZE-4);
        ctx->iv[CRY_GCM_BLOCK_SIZE-4] = 0;
        ctx->iv[CRY_GCM_BLOCK_SIZE-3] = 0;
        ctx->iv[CRY_GCM_BLOCK_SIZE-2] = 0;
        ctx->iv[CRY_GCM_BLOCK_SIZE-1] = 1;
    } else {
        memset(ctx->iv, 0, CRY_GCM_BLOCK_SIZE);
        gcm_hash(ctx->iv, ctx->key, iv, size);
        gcm_hash_sizes(ctx->iv, ctx->key, 0, size);
    }

    memcpy(ctx->ctr, ctx->iv, CRY_GCM_BLOCK_SIZE);
    CRY_INCREMENT_BE(ctx->ctr + CRY_GCM_BLOCK_SIZE - 4, 4);

    memset(ctx->hs, 0, CRY_GCM_BLOCK_SIZE);
    ctx->auth_len = 0;
    ctx->ciph_len = 0;
}

static void gcm_operate(struct cry_gcm_ctx *ctx, unsigned char *dst,
                        const unsigned char *src, size_t size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;
    unsigned char buffer[CRY_GCM_BLOCK_SIZE];

    if (src != dst) {
        for (; size >= CRY_GCM_BLOCK_SIZE; (size -= CRY_GCM_BLOCK_SIZE,
               src += CRY_GCM_BLOCK_SIZE, dst += CRY_GCM_BLOCK_SIZE)) {
            encrypt(ciph, dst, ctx->ctr, CRY_GCM_BLOCK_SIZE);
            cry_memxor(dst, src, CRY_GCM_BLOCK_SIZE);
            CRY_INCREMENT_BE(&ctx->ctr[CRY_GCM_BLOCK_SIZE-4], 4);
        }
    } else {
        for (; size >= CRY_GCM_BLOCK_SIZE; (size -= CRY_GCM_BLOCK_SIZE,
               src += CRY_GCM_BLOCK_SIZE, dst += CRY_GCM_BLOCK_SIZE)) {
            encrypt(ciph, buffer, ctx->ctr, CRY_GCM_BLOCK_SIZE);
            cry_memxor2 (dst, src, buffer, CRY_GCM_BLOCK_SIZE);
            CRY_INCREMENT_BE(&ctx->ctr[CRY_GCM_BLOCK_SIZE-4], 4);
        }
    }

    if (size > 0) {
        encrypt(ciph, buffer, ctx->ctr, CRY_GCM_BLOCK_SIZE);
        cry_memxor2(dst, src, buffer, size);
        CRY_INCREMENT_BE(&ctx->ctr[CRY_GCM_BLOCK_SIZE-4], 4);
    }
}

void cry_gcm_encrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size)
{
    gcm_operate(ctx, dst, src, size);
    gcm_hash(ctx->hs, ctx->key, dst, size);
    ctx->ciph_len += size;
}

void cry_gcm_decrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size)
{
    gcm_hash(ctx->hs, ctx->key, src, size);
    gcm_operate(ctx, dst, src, size);
    ctx->ciph_len += size;
}

void cry_gcm_update(struct cry_gcm_ctx *ctx, const unsigned char *aad,
                    size_t size)
{
    gcm_hash(ctx->hs, ctx->key, aad, size);
    ctx->auth_len += size;
}

void cry_gcm_digest(struct cry_gcm_ctx *ctx, unsigned char *mac,
                    size_t size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;
    unsigned char buffer[CRY_GCM_BLOCK_SIZE];

    gcm_hash_sizes(ctx->hs, ctx->key, ctx->auth_len, ctx->ciph_len);
    encrypt(ciph, buffer, ctx->iv, CRY_GCM_BLOCK_SIZE);
    cry_memxor2(mac, ctx->hs, buffer, size);
}
