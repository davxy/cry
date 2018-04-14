/*
 * Copyright (c) 2013-2018, Davide Galassi. All rights reserved.
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

#include "cry/hash.h"

#ifndef _CRY_HMAC_H_
#define _CRY_HMAC_H_

#define CRY_HMAC_HASH_BLOCK_MAX     64

struct cry_hmac_ctx {
    void                *hash_ctx;
    const cry_hash_itf  *hash_itf;
    size_t              hash_len;
    const unsigned char *key;
    size_t              key_len;
};

typedef struct cry_hmac_ctx cry_hmac_ctx;

void cry_hmac_init(cry_hmac_ctx *ctx, void *hash_ctx,
                   cry_hash_itf *hash_itf, size_t hash_len,
                   const unsigned char *key, size_t key_len);

int cry_hmac(cry_hmac_ctx *ctx, unsigned char *mac, const unsigned char *in,
             size_t insize);

#endif /* _CRY_HMAC_H_ */

