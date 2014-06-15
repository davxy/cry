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

#include "cry_crc.h"

void cry_crc16_init(struct cry_crc16_ctx *ctx, unsigned short start,
                    const unsigned short *tab, unsigned char flags)
{
    ctx->crc = start;
    ctx->tab = tab;
    ctx->flags = flags;
}

#include <stdint.h>

void cry_crc16_update(struct cry_crc16_ctx *ctx,
                      const unsigned char *ptr, size_t n)
{
    while (n--)
        ctx->crc = ctx->tab[(*ptr++ ^ ctx->crc) & 0xFF] ^ (ctx->crc >> 8);
}

unsigned short cry_crc16_final(struct cry_crc16_ctx *ctx)
{
    if (ctx->flags & CRY_CRC_FLAG_COMPLEMENT)
        ctx->crc ^= 0xffff;
    if (ctx->flags & CRY_CRC_FLAG_SWAP)
        ctx->crc = (ctx->crc << 8) | (ctx->crc >> 8);
    return (ctx->crc & 0Xffff);
}

