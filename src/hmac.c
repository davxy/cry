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

#include "cry/hmac.h" 
#include <string.h>

#define BLOCK_SIZE  64

void cry_hmac_init(cry_hmac_ctx *ctx, void *hash_ctx,
                   cry_hash_itf *hash_itf, size_t hash_len,
                   const unsigned char *key, size_t key_len)
{
    ctx->hash_ctx = hash_ctx;
    ctx->hash_itf = hash_itf;
    ctx->hash_len = hash_len;
    ctx->key = key;
    ctx->key_len = key_len;
}

int cry_hmac(cry_hmac_ctx *ctx, unsigned char *mac, const unsigned char *in,
             size_t insize)
{
    int i;
    const unsigned char *key;
    size_t key_len;
    unsigned char hkey[BLOCK_SIZE];
    unsigned char pad[BLOCK_SIZE];
    const cry_hash_itf *hash = ctx->hash_itf;
 
    if (ctx->key_len <= BLOCK_SIZE) {
        key = ctx->key;
        key_len = ctx->key_len;
    } else {
        key = hkey;
        key_len = ctx->hash_len;
        if (hash->init)
            hash->init(ctx->hash_ctx);
        hash->update(ctx->hash_ctx, ctx->key, ctx->key_len);
        hash->digest(ctx->hash_ctx, hkey);
        if (hash->clean)
            hash->clean(ctx->hash_ctx);
    }

    memset(pad, 0x36, BLOCK_SIZE);
    for (i = 0; i < key_len; i++)
        pad[i] ^= key[i];
    if (hash->init)
        hash->init(ctx->hash_ctx);
    hash->update(ctx->hash_ctx, pad, BLOCK_SIZE);
    hash->update(ctx->hash_ctx, in, insize);
    hash->digest(ctx->hash_ctx, mac);
    if (hash->clean)
        hash->clean(ctx->hash_ctx);

    memset(pad, 0x5C, BLOCK_SIZE);
    for (i = 0; i < key_len; i++)
        pad[i] ^= key[i];
    if (hash->init)
        hash->init(ctx->hash_ctx);
    hash->update(ctx->hash_ctx, pad, BLOCK_SIZE);
    hash->update(ctx->hash_ctx, mac, ctx->hash_len);
    hash->digest(ctx->hash_ctx, mac);
    if (hash->clean)
        hash->clean(ctx->hash_ctx);
    return 0;
}

