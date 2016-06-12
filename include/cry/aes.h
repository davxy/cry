/*
 * Copyright (c) 2013-2016, Davide Galassi. All rights reserved.
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
 * @file    cry_aes.h
 * @brief   AES block cipher.
 */

#ifndef _CRY_AES_H_
#define _CRY_AES_H_

#include <stddef.h>

#define CRY_AES_BLOCK_SIZE  16

struct cry_aes_ctx {
    unsigned char ks[60][4]; /** Key schedule */
    unsigned int  nr;        /** Number of rounds */
};

typedef struct cry_aes_ctx cry_aes_ctx;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Set the AES key.
 *
 * @param ctx   AES context.
 * @param key   Pointer to the key.
 * @param size  Key size. Must be 16 for AES-128, 24 for AES-192, 32 for AES-256
 */
void cry_aes_key_set(struct cry_aes_ctx *ctx, const unsigned char *key,
                     size_t size);

/**
 * Encrypt the data pointed by src into dst.
 *
 * @param ctx   AES context.
 * @param dst   Destination buffer (ciphertext).
 * @param src   Source buffer (cleartext).
 * @param size  Size of the data. Must be a multiple of 16.
 */
void cry_aes_encrypt(struct cry_aes_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size);

/**
 * Decrypt the data pointed by src into dst.
 * The function works in-place so dst can be equal to src.
 *
 * @param ctx   AES context.
 * @param dst   Destination buffer (cleartext).
 * @param src   Source buffer (ciphertext).
 * @param size  Size of the data. Must be a multiple of 16.
 */
void cry_aes_decrypt(struct cry_aes_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size);

/*
 * Wrappers
 */

void cry_aes_128_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key);

void cry_aes_128_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key);

void cry_aes_192_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key);

void cry_aes_192_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key);

void cry_aes_256_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key);

void cry_aes_256_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_AES_H_ */

