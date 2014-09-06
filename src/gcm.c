/*
 * Copyright (c) 2013-2014, Davide Galassi. All rights reserved.
 *
 * This file is part of CRY software.
 *
 * CRY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CRY; if not, see <http://www.gnu/licenses/>.
 */

#include "cry/gcm.h"
#include "memxor.h"
#include "misc.h"
#include <string.h>
#include <stdint.h>

static void gcm_gf_add(unsigned char *r, const unsigned char *x,
                       const unsigned char *y)
{
    uint32_t *rw = (uint32_t *) r;
    const uint32_t *xw = (const uint32_t *) x;
    const uint32_t *yw = (const uint32_t *) y;

    rw[0] = xw[0] ^ yw[0];
    rw[1] = xw[1] ^ yw[1];
    rw[2] = xw[2] ^ yw[2];
    rw[3] = xw[3] ^ yw[3];
}

#define RSHIFT_WORD(x) \
        ((((x) & 0xfefefefeUL) >> 1U) | (((x) & 0x00010101UL) << 15U))

#define GHASH_POLYNOMIAL 0xE1UL

/*
 * Multiplication by 010...0; a big-endian shift right. If the bit
 * shifted out is one, the defining polynomial is added to cancel it
 * out. r == x is allowed.
 */
static void gcm_gf_shift(unsigned char *r, const unsigned char *x)
{
    uint32_t mask;
    uint32_t *rw = (uint32_t *) r;
    const uint32_t *xw = (const uint32_t *) x;

    /* Shift uses big-endian representation. */
#ifndef CRY_BIG_ENDIAN
    mask = (~((xw[3] >> 24U) & 1U)) + 1;
    rw[3] = RSHIFT_WORD(xw[3]) | ((xw[2] >> 17) & 0x80U);
    rw[2] = RSHIFT_WORD(xw[2]) | ((xw[1] >> 17) & 0x80U);
    rw[1] = RSHIFT_WORD(xw[1]) | ((xw[0] >> 17) & 0x80U);
    rw[0] = RSHIFT_WORD(xw[0]) ^ (mask & GHASH_POLYNOMIAL);
#else
    mask = (~(xw[3] & 1U)) + 1;
    rw[3] = (xw[3] >> 1U) | ((xw[2] & 1) << 31U);
    rw[2] = (xw[2] >> 1U) | ((xw[1] & 1) << 31U);
    rw[1] = (xw[1] >> 1U) | ((xw[0] & 1) << 31U);
    rw[0] = (xw[0] >> 1U) ^ (mask & (GHASH_POLYNOMIAL << 24U));
#endif
}

/*
 * Sets x <- x * y mod r, using the plain bitwise algorithm from the
 * specification. y may be shorter than a full block, missing bytes
 * are assumed zero.
 */
static void gcm_gf_mul(unsigned char *x, const unsigned char *y)
{
    unsigned char V[CRY_GCM_BLOCK_SIZE];
    unsigned char Z[CRY_GCM_BLOCK_SIZE];
    unsigned char b;
    unsigned int i, j;

    memcpy(V, x, CRY_GCM_BLOCK_SIZE);
    memset(Z, 0, CRY_GCM_BLOCK_SIZE);

    for (i = 0; i < CRY_GCM_BLOCK_SIZE; i++) {
        b = y[i];
        for (j = 0; j < 8; j++, b <<= 1) {
            if (b & 0x80)
                gcm_gf_add(Z, Z, V);
            gcm_gf_shift(V, V);
        }
    }
    memcpy(x, Z, CRY_GCM_BLOCK_SIZE);
}

static void cry_gcm_hash(const unsigned char *key, unsigned char *x,
                         unsigned int length, const unsigned char *data)
{
    for (; length >= CRY_GCM_BLOCK_SIZE;
           length -= CRY_GCM_BLOCK_SIZE, data += CRY_GCM_BLOCK_SIZE) {
        cry_memxor(x, data, CRY_GCM_BLOCK_SIZE);
        gcm_gf_mul(x, key);
    }

    if (length > 0) {
        cry_memxor(x, data, length);
        gcm_gf_mul(x, key);
    }
}

#define WRITE32_BE(val, dst) do { \
    ((unsigned char *)(dst))[0] = (unsigned char)(((val) >> 24U) & 0xffU); \
    ((unsigned char *)(dst))[1] = (unsigned char)(((val) >> 16U) & 0xffU); \
    ((unsigned char *)(dst))[2] = (unsigned char)(((val) >> 8U) & 0xffU); \
    ((unsigned char *)(dst))[3] = (unsigned char) ((val) & 0xff); \
    } while(0)

static void cry_gcm_hash_sizes(const unsigned char *key,
                               unsigned char *x,
                               unsigned long auth_size,
                               unsigned long ciph_size)
{
    unsigned char buffer[CRY_GCM_BLOCK_SIZE] = {0};

    /* Get the sizes in bits */
    ciph_size <<= 3;
    auth_size <<= 3;

    WRITE32_BE(auth_size, buffer + 4);
    WRITE32_BE(ciph_size, buffer + 12);

    cry_gcm_hash(key, x, CRY_GCM_BLOCK_SIZE, buffer);
}

void cry_gcm_key_set(struct cry_gcm_ctx *gcm, const unsigned char *key,
                     unsigned int size)
{
    void *ciph = gcm->ciph_ctx;
    cry_ciph_encrypt_f encrypt = gcm->ciph_itf->encrypt;
    cry_ciph_key_set_f key_set = gcm->ciph_itf->key_set;

    memset(gcm->x, 0, CRY_GCM_BLOCK_SIZE);
    memset(gcm->ctr, 0, CRY_GCM_BLOCK_SIZE);
    memset(gcm->iv, 0, CRY_GCM_BLOCK_SIZE);
    memset(gcm->key, 0, CRY_GCM_BLOCK_SIZE);
    gcm->auth_size = 0;
    gcm->ciph_size = 0;

    key_set(ciph, key, size);

    memset(gcm->key, 0, CRY_GCM_BLOCK_SIZE);
    encrypt(ciph, gcm->key, gcm->key, CRY_GCM_BLOCK_SIZE);
}


void cry_gcm_iv_set(struct cry_gcm_ctx *ctx, const unsigned char *iv,
                    unsigned int size)
{
    if (size == (CRY_GCM_BLOCK_SIZE-4)) {
        memcpy(ctx->iv, iv, CRY_GCM_BLOCK_SIZE-4);
        ctx->iv[CRY_GCM_BLOCK_SIZE-4] = 0;
        ctx->iv[CRY_GCM_BLOCK_SIZE-3] = 0;
        ctx->iv[CRY_GCM_BLOCK_SIZE-2] = 0;
        ctx->iv[CRY_GCM_BLOCK_SIZE-1] = 1;
    } else {
        memset(ctx->iv, 0, CRY_GCM_BLOCK_SIZE);
        cry_gcm_hash(ctx->key, ctx->iv, size, iv);
        cry_gcm_hash_sizes(ctx->key, ctx->iv, 0, size);
    }

    memcpy(ctx->ctr, ctx->iv, CRY_GCM_BLOCK_SIZE);
    CRY_INCREMENT_BE(ctx->ctr + CRY_GCM_BLOCK_SIZE - 4, 4);

    /* Reset the rest of the message-dependent state */
    memset(ctx->x, 0, CRY_GCM_BLOCK_SIZE);
    ctx->auth_size = ctx->ciph_size = 0;
}

static void cry_gcm_crypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                          const unsigned char *src, unsigned int size)
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
            cry_memxor3 (dst, src, buffer, CRY_GCM_BLOCK_SIZE);
            CRY_INCREMENT_BE(&ctx->ctr[CRY_GCM_BLOCK_SIZE-4], 4);
        }
    }

    if (size > 0) {
        /* A final partial block */
        encrypt(ciph, buffer, ctx->ctr, CRY_GCM_BLOCK_SIZE);
        cry_memxor3(dst, src, buffer, size);
        CRY_INCREMENT_BE(&ctx->ctr[CRY_GCM_BLOCK_SIZE-4], 4);
    }
}

void cry_gcm_encrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size)
{
    cry_gcm_crypt(ctx, dst, src, size);
    cry_gcm_hash(ctx->key, ctx->x, size, dst);
    ctx->ciph_size += size;
}

void cry_gcm_decrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size)
{
    cry_gcm_hash(ctx->key, ctx->x, size, src);
    cry_gcm_crypt(ctx, dst, src, size);
    ctx->ciph_size += size;
}

void cry_gcm_update(struct cry_gcm_ctx *ctx, const unsigned char *aad,
                    unsigned int size)
{
    cry_gcm_hash(ctx->key, ctx->x, size, aad);
    ctx->auth_size += size;
}

void cry_gcm_digest(struct cry_gcm_ctx *ctx, unsigned char *mac,
                    unsigned int size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;
    unsigned char buffer[CRY_GCM_BLOCK_SIZE];

    cry_gcm_hash_sizes(ctx->key, ctx->x, ctx->auth_size, ctx->ciph_size);
    encrypt(ciph, buffer, ctx->iv, CRY_GCM_BLOCK_SIZE);
    cry_memxor3(mac, ctx->x, buffer, size);
}

