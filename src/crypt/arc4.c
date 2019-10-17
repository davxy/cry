#include <cry/arc4.h>
#include <string.h>
#include "misc.h"


void cry_arc4_init(cry_arc4_ctx *ctx)
{
    memset(ctx, 0, sizeof(*ctx));
}

void cry_arc4_clear(cry_arc4_ctx *ctx)
{
    cry_memset(ctx, 0, sizeof(*ctx));
}

void cry_arc4_key_set(cry_arc4_ctx *ctx, const unsigned char *key,
                      size_t size)
{
    size_t i, j;
    unsigned char a;
    unsigned int k;

    ctx->x = 0;
    ctx->y = 0;

    for (i = 0; i < 256; i++)
        ctx->s[i] = (unsigned char)i;

    j = k = 0;
    for (i = 0; i < 256; i++) {
        a = ctx->s[i];
        j = (unsigned char)(j + a + key[k]);
        ctx->s[i] = ctx->s[j];
        ctx->s[j] = a;
        k++;
        if (k == size)
            k = 0;
    }
}

void cry_arc4_crypt(cry_arc4_ctx *ctx, unsigned char *dst,
                    const unsigned char *src, size_t size)

{
    size_t i;
    unsigned char x, y, a, b;

    x = ctx->x;
    y = ctx->y;

    for (i = 0; i < size; i++) {
        x = (unsigned char)(x + 1);
        a = ctx->s[x];
        y = (unsigned char)(y + a);
        b = ctx->s[y];
        ctx->s[x] = b;
        ctx->s[y] = a;
        dst[i] = src[i] ^ ctx->s[(unsigned char)(a + b)];
    }

    ctx->x = x;
    ctx->y = y;
}
