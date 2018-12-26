#include <cry/crc.h>

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
