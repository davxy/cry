/*
 * Copyright (c) 2013-2017, Davide Galassi. All rights reserved.
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
 * @file    cry_gcm.h
 * @brief   GCM block cipher mode of operation.
 */

#ifndef _CRY_GCM_H_
#define _CRY_GCM_H_

#include "cry/ciph.h"

/** Size of a GCM block. */
#define CRY_GCM_BLOCK_SIZE      16

/* Initialization helper macro */
#define CRY_GCM_INIT(ctx, _ciph_ctx, _ciph_itf) do { \
    memset((ctx), 0, sizeof(struct cry_gcm_ctx)); \
    (ctx)->ciph_ctx = (_ciph_ctx); \
    (ctx)->ciph_itf = (_ciph_itf); \
    } while(0)

/** GCM context. */
struct cry_gcm_ctx {
    /** Block cipher context. */
    void                      *ciph_ctx;
    /** Block cipher interface. */
    const struct cry_ciph_itf *ciph_itf;
    /** Original counter block */
    unsigned char              iv[CRY_GCM_BLOCK_SIZE];
    /** Updated for each block. */
    unsigned char              ctr[CRY_GCM_BLOCK_SIZE];
    /** Hashing state */
    unsigned char              x[CRY_GCM_BLOCK_SIZE];
    /** Hashing subkey. */
    unsigned char              key[CRY_GCM_BLOCK_SIZE];
    /** Auth data size. */
    unsigned int               auth_size;
    /** Cipher data size. */
    unsigned int               ciph_size;
};

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Set the key in the gcm and in the cipher context.
 *
 * @param ctx   GCM context.
 * @param key   Cipher key.
 * @param size  Size of cipher key.
 */
void cry_gcm_key_set(struct cry_gcm_ctx *ctx, const unsigned char *key,
                     unsigned int size);

/**
 * Set of the initialization vector.
 *
 * @param ctx   GCM context.
 * @param iv    Initialization vector buffer.
 * @param size  Size of the initialization vector.
 */
void cry_gcm_iv_set(struct cry_gcm_ctx *ctx, const unsigned char *iv,
                    unsigned int size);

/**
 * Encryption function.
 *
 * @param ctx   GCM context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
void cry_gcm_encrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

/**
 * Decryption function.
 *
 * @param ctx   GCM context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
void cry_gcm_decrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

/**
 * Set the additional authentication data.
 *
 * This must me called before the encrypt (decrypt) function (data_size == 0).
 *
 * @param ctx   GCM context.
 * @param aad   Additional authentication data buffer.
 * @param size  Size of the additional auth data.
 *              Constraint: auth_size % GCM_BLOCK_SIZE == 0.
 */
void cry_gcm_update(struct cry_gcm_ctx *ctx, const unsigned char *aad,
                    unsigned int size);

/**
 * Generate the GCM mac (GMAC).
 *
 * @param ctx   GCM context.
 * @param mac   Message authentication code output buffer.
 * @param size  Should be <= GCM_BLOCK_SIZE (16).
 */
void cry_gcm_digest(struct cry_gcm_ctx *ctx, unsigned char *mac,
                    unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_GCM_H_ */

