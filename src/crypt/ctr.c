#include <cry/ctr.h>
#include "memxor.h"
#include "misc.h"
#include <string.h>

#define BUF_BLOCKS  4
#define BUF_LEN     (CRY_CTR_BLOCK_SIZE * BUF_BLOCKS)

void cry_ctr_init(struct cry_ctr_ctx *ctx, void *ciph_ctx,
                  const struct cry_ciph_itf *ciph_itf)
{
    memset(ctx->ctr, 0, sizeof(ctx->ctr));
    ctx->ciph_ctx = ciph_ctx;
    ctx->ciph_itf = ciph_itf;
}

void cry_ctr_key_set(struct cry_ctr_ctx *ctx, const unsigned char *key,
                     unsigned int size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_key_set_f key_set = ctx->ciph_itf->key_set;

    key_set(ciph, key, size);
}

void cry_ctr_iv_set(struct cry_ctr_ctx *ctx, const unsigned char *iv,
                    unsigned int size)
{
    size = CRY_MIN(CRY_CTR_BLOCK_SIZE, size);
    memcpy(ctx->ctr, iv, size);
    memset(ctx->ctr + size, 0, CRY_CTR_BLOCK_SIZE - size);
}

void cry_ctr_encrypt(struct cry_ctr_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int len)
{
    unsigned char buf[BUF_LEN];
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;
    size_t n;
    unsigned char *p;

    if (src != dst) {
        p = dst;
        for (n = len; n >= CRY_CTR_BLOCK_SIZE; n -= CRY_CTR_BLOCK_SIZE) {
            memcpy(p, ctx->ctr, CRY_CTR_BLOCK_SIZE);
            CRY_INCREMENT_BE(ctx->ctr, CRY_CTR_BLOCK_SIZE);
            p += CRY_CTR_BLOCK_SIZE;
        }

        encrypt(ciph, dst, dst, len - n);
        cry_memxor(dst, src, len - n);

        if (n != 0) {
            encrypt(ciph, buf, ctx->ctr, CRY_CTR_BLOCK_SIZE);
            CRY_INCREMENT_BE(ctx->ctr, CRY_CTR_BLOCK_SIZE);
            cry_memxor2(dst + len - n, src + len - n, buf, n);
        }
    } else {
        while (len >= BUF_LEN) {
            p = buf;
            for (n = 0; n < BUF_BLOCKS; n++) {
                memcpy(p, ctx->ctr, CRY_CTR_BLOCK_SIZE);
                CRY_INCREMENT_BE(ctx->ctr, CRY_CTR_BLOCK_SIZE);
                p += CRY_CTR_BLOCK_SIZE;
            }
            encrypt(ciph, buf, buf, BUF_LEN);
            cry_memxor2(dst, src, buf, BUF_LEN);
            len -= BUF_LEN;
            src += BUF_LEN;
            dst += BUF_LEN;
        }

        if (len != 0) {
            p = buf;
            for (n = 0; n < len; n += CRY_CTR_BLOCK_SIZE) {
                memcpy(p, ctx->ctr, CRY_CTR_BLOCK_SIZE);
                CRY_INCREMENT_BE(ctx->ctr, CRY_CTR_BLOCK_SIZE);
                p += CRY_CTR_BLOCK_SIZE;
            }
            encrypt(ciph, buf, buf, n);
            cry_memxor2(dst, src, buf, len);
        }
    }
}
