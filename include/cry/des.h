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
 * @file    cry_des.h
 * @brief   DES block cipher.
 */

#ifndef _CRY_DES_H_
#define _CRY_DES_H_

#include <string.h>

/** DES algorithm key size in octets (64 bit) */
#define CRY_DES_BLOCK_SIZE  8


/** DES context */
struct cry_des_ctx {
    unsigned int  keylen; /* 8 for normal DES, 24 for triple DES */
    unsigned char key[3*CRY_DES_BLOCK_SIZE];
};

typedef struct cry_des_ctx cry_des_ctx;


#define cry_des_key_set(ctx, _key, _keylen) do { \
    (ctx)->keylen = _keylen; \
    memcpy((ctx)->key, _key, _keylen); \
    } while(0)

void cry_des_encrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

void cry_des_decrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

#endif /* _CRY_DES_H_ */

