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

/**
 * @file    cry_ctr.h
 * @brief   CTR block cipher mode of operation.
 *
 * Turns a block cipher into a stream cipher. It generates the next
 * keystream block by encrypting successive values of a "counter". The
 * counter can be any function which produces a sequence which is
 * guaranteed not to repeat for a long time, although an actual
 * increment-by-one counter is the simplest and most popular.
 */

#ifndef _CRY_CTR_H_
#define _CRY_CTR_H_

#include "cry_ciph.h"

/** Block size. */
#define CRY_CTR_BLOCK_SIZE    16

/* Initialization helper macro */
#define CRY_CTR_INIT(ctx, _crypto_ctx, _crypto_itf) do { \
    memset((ctx), 0, sizeof(struct cry_ctr_ctx)); \
    (ctx)->ciph_ctx = (_crypto_ctx); \
    (ctx)->ciph_itf = (_crypto_itf); \
    } while(0)

/**
 * CTR context.
 */
struct cry_ctr_ctx
{
    /** Block cipher context. */
    void                      *ciph_ctx;
    /** Block cipher interface. */
    const struct cry_ciph_itf *ciph_itf;
    /** Counter. */
    unsigned char             v[CRY_CTR_BLOCK_SIZE];
};

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Set the cipher key in the cipher context.
 *
 * @param ctx   CTR context.
 * @param key   Cipher key.
 * @param size  Size of cipher key.
 */
void cry_ctr_key_set(struct cry_ctr_ctx *ctx, const unsigned char *key,
                     unsigned int size);

/**
 * Set the initialization vector in the CTR context.
 *
 * @param ctx   CTR context.
 * @param iv    Initialization vector.
 * @param size  Size of initialization vector.
 *              Must be less then CRY_CTR_BLOCK_SIZE (16),
 *              If less then the iv is zero padded to 16.
 */
void cry_ctr_iv_set(struct cry_ctr_ctx *ctx, const unsigned char *iv,
                    unsigned int size);

/**
 * Encryption function.
 *
 * @param ctx   CTR context.
 * @param dst   Destination pointer (cleartext).
 * @param src   Source pointer (ciphertext).
 * @param size  Size of ciphertext.
 */
void cry_ctr_encrypt(struct cry_ctr_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

#ifdef __cplusplus
}
#endif

/**
 * The decrypt is equal to the encrypt function.
 */
#define cry_ctr_decrypt cry_ctr_encrypt

#endif /* _CRY_CTR_H_ */

