#include "cry/affine.h"
#include "misc.h"
#include <string.h>

long cry_inverse(unsigned long a, unsigned long m)
{
    long r0, r1, s0, s1, q;

    r0 = a;
    r1 = m;
    s0 = 1;
    s1 = 0;
    while (r1 != 0) {
        /* r2 = r0 - q1-r1 */
        q = r0 / r1;
        r0 %= r1;
        CRY_SWAP(r0, r1);
        /* s2 = s0 - q1*s1 */
        s0 = s0 - q*s1;
        CRY_SWAP(s0, s1);
    }
    return s0;
}

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

    memset(ctx, 0, sizeof(*ctx));
    if (keylen > CRY_AFFINE_KEYMAX)
        keylen = CRY_AFFINE_KEYMAX;
    memcpy(ctx->keya, keya, keylen);
    memcpy(ctx->keyb, keyb, keylen);
    ctx->keylen = keylen;
    /* Compute key inverse */
    for (i = 0; i < keylen; i++) {
        ctx->inva[i] = cry_inverse(ctx->keya[i], 256);
        /* Check */
    }
    return 0;
}

