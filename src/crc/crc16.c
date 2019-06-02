#include <cry/crc.h>

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
        ctx->crc ^= (uint16_t) ~0U;
    if (ctx->flags & CRY_CRC_FLAG_SWAP)
        ctx->crc = (uint16_t)(ctx->crc << 8U) | (ctx->crc >> 8U);
    return ctx->crc;
}

void cry_crc16_digest(cry_crc16_ctx *ctx, unsigned char *out)
{
    uint16_t crc;

    crc = cry_crc16_final(ctx);
    out[0] = (unsigned char)(crc >> 8);
    out[1] = (unsigned char)(crc & 0xFF);
}
