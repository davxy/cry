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
 * @file    cry_ciph.h
 * @brief   Generic cipher interface
 */

#ifndef _CRY_CIPH_H_
#define _CRY_CIPH_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Context initialization function pointer type.
 *
 * @param ctx   Cipher context
 */
typedef void (* cry_ciph_init_f)(void *ctx);

/**
 * Context cleanup function pointer type.
 *
 * @param ctx   Cipher context
 */
typedef void (* cry_ciph_clean_f)(void *ctx);

/**
 * Key set function pointer type.
 *
 * @param ctx   Cipher context.
 * @param key   Cipher key.
 * @param size  Key size.
 */
typedef void (* cry_ciph_key_set_f)(void *ctx, const unsigned char *key,
                                    unsigned int size);

/**
 * Encrypt function pointer type.
 *
 * @param ctx   Cipher context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
typedef void (* cry_ciph_encrypt_f)(void *ctx, unsigned char *dst,
                                    const unsigned char *src,
                                    unsigned int size);

/**
 * Decrypt function pointer type.
 *
 * @param ctx   Cipher context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
typedef void (* cry_ciph_decrypt_f)(void *ctx, unsigned char *dst,
                                    const unsigned char *src,
                                    unsigned int size);

#ifdef __cplusplus
}
#endif

/**
 * Cipher algorithm generic interface.
 */
struct cry_ciph_itf {
    cry_ciph_init_f       init;
    cry_ciph_clean_f      clean;
    cry_ciph_key_set_f    key_set;
    cry_ciph_encrypt_f    encrypt;
    cry_ciph_decrypt_f    decrypt;
};

typedef struct cry_ciph_itf cry_ciph_itf;


#endif /* _CRY_CIPH_H_ */

