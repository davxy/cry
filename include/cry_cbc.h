/*
 * Copyright (c) 2013, Davide Galassi. All rights reserved.
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

/**
 * @file    cry_cbc.h
 * @brief   CBC block cipher mode of operation.
 */

#ifndef _CRY_CBC_H_
#define _CRY_CBC_H_

#include "cry_ciph.h"

/** CTR block size. */
#define CRY_CBC_BLOCK_SIZE      16

/** Initialization helper macro */
#define CRY_CBC_INIT(ctx, _crypto_ctx, _crypto_itf) do { \
    memset((ctx), 0U, sizeof(struct cry_cbc_ctx)); \
    (ctx)->ciph_ctx = (_crypto_ctx); \
    (ctx)->ciph_itf = (_crypto_itf); \
    } while(0)

/** CBC context structure. */
struct cry_cbc_ctx {
    /** Block cipher interface. */
    const struct cry_ciph_itf *ciph_itf;
    /** Block cipher context. */
    void                      *ciph_ctx;
    /** Counter */
    unsigned char             v[CRY_CBC_BLOCK_SIZE];
};

typedef struct cry_cbc_ctx cry_cbc_ctx;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Set the cipher key.
 *
 * @param ctx   CBC context.
 * @param key   Cipher key.
 * @param size  Cipher key size.
 */
void cry_cbc_key_set(struct cry_cbc_ctx *ctx, const unsigned char *key,
                     unsigned int size);

/**
 * Set the initialization vector.
 *
 * @param ctx   CBC context.
 * @param iv    Initialization vector.
 * @param size  Initialization vector size.
 */
void cry_cbc_iv_set(struct cry_cbc_ctx *ctx, const unsigned char *iv,
                    unsigned int size);

/**
 * Encryption function.
 *
 * @param ctx   CBC context.
 * @param dst   Destination buffer (ciphertext).
 * @param src   Source buffer (cleartext).
 * @param size  Size should be a multiple of CBC_BLOCK_SIZE
 */
void cry_cbc_encrypt(struct cry_cbc_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

/**
 * Decryption function.
 *
 * @param ctx   CBC context.
 * @param dst   Destination buffer (cleartext).
 * @param src   Source buffer (ciphertext).
 * @param size  Size should be a multiple of CBC_BLOCK_SIZE.
 */
void cry_cbc_decrypt(struct cry_cbc_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_CBC_H_ */

