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

#include "cry/crc.h"

void cry_crc32_init(struct cry_crc32_ctx *ctx, uint32_t start,
                    const uint32_t *tab, unsigned char flags)
{
    ctx->crc = start;
    ctx->tab = tab;
    ctx->flags = flags;
}

void cry_crc32_update(struct cry_crc32_ctx *ctx, 
                      const unsigned char *buf, size_t size)
{
	while (size--)
		ctx->crc = ctx->tab[(*buf++ ^ ctx->crc) & 0xFF] ^ (ctx->crc >> 8);
}

uint32_t cry_crc32_final(struct cry_crc32_ctx *ctx)
{
    if (ctx->flags & CRY_CRC_FLAG_COMPLEMENT)
        ctx->crc ^= ~0;
    if (ctx->flags & CRY_CRC_FLAG_SWAP)
        ctx->crc = (ctx->crc & 0x000000ff) << 24 |
                   (ctx->crc & 0x0000ff00) << 8 |
                   (ctx->crc & 0x00ff0000) >> 8 |
                   (ctx->crc & 0xff000000) >> 24;
    return ctx->crc;
}

