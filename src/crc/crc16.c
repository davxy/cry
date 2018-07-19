#include "cry/crc.h"

void cry_crc16_init(struct cry_crc16_ctx *ctx, uint16_t start,
                    const uint16_t *tab, unsigned char flags)
{
    ctx->crc = start;
    ctx->tab = tab;
    ctx->flags = flags;
}

void cry_crc16_update(struct cry_crc16_ctx *ctx,
                      const unsigned char *buf, size_t size)
{
    while (size--)
        ctx->crc = ctx->tab[(*buf++ ^ ctx->crc) & 0xFF] ^ (ctx->crc >> 8);
}

uint16_t cry_crc16_final(struct cry_crc16_ctx *ctx)
{
    if (ctx->flags & CRY_CRC_FLAG_COMPLEMENT)
        ctx->crc ^= ~0;
    if (ctx->flags & CRY_CRC_FLAG_SWAP)
        ctx->crc = (ctx->crc << 8) | (ctx->crc >> 8);
    return ctx->crc;
}

