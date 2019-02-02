#include <cry/affine.h>
#include "misc.h"
#include <string.h>

void cry_affine_encrypt(struct cry_affine_ctx *ctx, unsigned char *out,
                      const unsigned char *in, size_t len)
{
    size_t i, k;

    k = 0;
    for (i = 0; i < len; i++) {
        out[i] = in[i]*ctx->keya[k] + ctx->keyb[k];
        k++;
        if (k == ctx->keylen)
            k = 0;
    }
}

void cry_affine_decrypt(struct cry_affine_ctx *ctx, unsigned char *out,
                      const unsigned char *in, size_t len)
{
    size_t i, k;

    k = 0;
    for (i = 0; i < len; i++) {
        out[i] = (in[i] - ctx->keyb[k]) * ctx->inva[k];
        k++;
        if (k == ctx->keylen)
            k = 0;
    }
}

int cry_affine_init(struct cry_affine_ctx *ctx, const unsigned char *keya,
                    const unsigned char *keyb, size_t keylen)
{
    size_t i;
    int res = 0;

    memset(ctx, 0, sizeof(*ctx));
    if (keylen > CRY_AFFINE_KEYMAX)
        keylen = CRY_AFFINE_KEYMAX;
    memcpy(ctx->keya, keya, keylen);
    memcpy(ctx->keyb, keyb, keylen);
    ctx->keylen = keylen;
    /* Compute key inverse */
    for (i = 0; i < keylen; i++) {
        /* sould be odd  to have gcd(256, val)=1 */
        if ((ctx->keya[i] & 1) != 1) {
            res = -1;
            break;
        }
        ctx->inva[i] = (unsigned char)cry_long_inv(ctx->keya[i], 256);
    }
    return res;
}
