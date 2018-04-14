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

/**
 * @file    cry_sha256.h
 * @brief   Secure Hash Algorithm (256)
 */

#ifndef _CRY_SHA256_H_
#define _CRY_SHA256_H_

#include <stdint.h>
#include <stddef.h>

#define CRY_SHA256_DIGEST_SIZE 32

struct cry_sha256_ctx {
    unsigned char   data[64];
    uint32_t        datalen;
    uint64_t        bitlen;
    uint32_t        state[8];
};

typedef struct cry_sha256_ctx cry_sha256_ctx;

#ifdef __cplusplus
extern "C" {
#endif

void cry_sha256_init(struct cry_sha256_ctx *ctx);

void cry_sha256_update(struct cry_sha256_ctx *ctx, const unsigned char *data,
                       size_t size);

void cry_sha256_digest(struct cry_sha256_ctx *ctx, unsigned char *digest);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_SHA256_H */

