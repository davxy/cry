#include <cry/cfb.h>
#include "memxor.h"
#include "misc.h"
#include <string.h>

void cry_cfb_init(struct cry_cfb_ctx *ctx, void *ciph_ctx,
                  const struct cry_ciph_itf *ciph_itf)
{
    memset(ctx->v, 0, sizeof(ctx->v));
    ctx->ciph_ctx = ciph_ctx;
    ctx->ciph_itf = ciph_itf;
}

void cry_cfb_key_set(struct cry_cfb_ctx *ctx, const unsigned char *key,
                     size_t size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_key_set_f key_set = ctx->ciph_itf->key_set;

    key_set(ciph, key, size);
}

void cry_cfb_iv_set(struct cry_cfb_ctx *ctx, const unsigned char *iv,
                    size_t size)
{
    ctx->vlen  = CRY_MIN(CRY_CFB_BLOCK_MAX, size);
    memcpy(ctx->v, iv, ctx->vlen);
    memset(ctx->v + ctx->vlen, 0, CRY_CFB_BLOCK_MAX - ctx->vlen);
}

void cry_cfb_crypt(struct cry_cfb_ctx *ctx, unsigned char *dst,
                   const unsigned char *src, size_t len)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;
    size_t n;

    for (n = len; n >= ctx->vlen; n -= ctx->vlen) {
        encrypt(ciph, ctx->v, ctx->v, ctx->vlen);
        cry_memxor2(dst, src, ctx->v, ctx->vlen);
        memcpy(ctx->v, dst, ctx->vlen);
        src += ctx->vlen;
        dst += ctx->vlen;
    }

    if (n != 0) {
        encrypt(ciph, ctx->v, ctx->v, ctx->vlen);
        cry_memxor2(dst, src, ctx->v, n);
        memcpy(ctx->v, dst, ctx->vlen);
    }
}

void cry_cfb8_encrypt(struct cry_cfb_ctx *ctx, unsigned char *dst,
                      const unsigned char *src, size_t len)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;
    size_t pos;
    unsigned char buf[CRY_CFB_BLOCK_MAX * 2];

    memcpy(buf, ctx->v, ctx->vlen);
    pos = 0;
    while (len != 0) {
        if (pos == ctx->vlen) {
            memcpy(buf, buf + ctx->vlen, ctx->vlen);
            pos = 0;
        }
        encrypt(ciph, ctx->v, buf + pos, ctx->vlen);
        buf[pos + ctx->vlen] = *(dst++) = *(src++) ^ ctx->v[0];
        len--;
        pos++;
    }
    memcpy(ctx->v, buf + pos, ctx->vlen);
}

void cry_cfb8_decrypt(struct cry_cfb_ctx *ctx, unsigned char *dst,
                      const unsigned char *src, size_t len)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;
    size_t i = 0;
    unsigned char buf[CRY_CFB_BLOCK_MAX * 2];
    unsigned char obuf[CRY_CFB_BLOCK_MAX * 2];

    memcpy(buf, ctx->v, ctx->vlen);
    memcpy(buf + ctx->vlen, src, len < ctx->vlen ? len : ctx->vlen);

    while (len != 0) {
        for (i = 0; i < len && i < ctx->vlen; i++)
            encrypt(ciph, obuf + i, buf + i, ctx->vlen);
        cry_memxor2(dst, src, obuf, i);
        len -= i;
        src += i;
        dst += i;
        memcpy(buf, buf + ctx->vlen, ctx->vlen);
        memcpy(buf + ctx->vlen, src, len < ctx->vlen ? len : ctx->vlen);
    }
    memcpy(ctx->v, buf + i, ctx->vlen);
}
