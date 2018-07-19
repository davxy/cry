#include "cry/cbc.h"
#include "cry/ciph.h"
#include "memxor.h"
#include "misc.h"
#include <string.h>
#include <stdlib.h>

void cry_cbc_key_set(struct cry_cbc_ctx *ctx, const unsigned char *key,
                     unsigned int size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_key_set_f key_set = ctx->ciph_itf->key_set;

    key_set(ciph, key, size);
}

void cry_cbc_iv_set(struct cry_cbc_ctx *ctx, const unsigned char *iv,
                    unsigned int size)
{
    size = CRY_MIN(CRY_CBC_BLOCK_SIZE, size);
    memcpy(ctx->v, iv, size);
    memset(ctx->v + size, 0, CRY_CBC_BLOCK_SIZE - size);
}

void cry_cbc_encrypt(struct cry_cbc_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctx->ciph_itf->encrypt;

    size &= ~(CRY_CBC_BLOCK_SIZE - 1);

    while (size != 0) {
        cry_memxor(ctx->v, src, CRY_CBC_BLOCK_SIZE);
        encrypt(ciph, dst, ctx->v, CRY_CBC_BLOCK_SIZE);
        memcpy(ctx->v, dst, CRY_CBC_BLOCK_SIZE);

        src += CRY_CBC_BLOCK_SIZE;
        dst += CRY_CBC_BLOCK_SIZE;
        size -= CRY_CBC_BLOCK_SIZE;
    }
}

#define CBC_BUFFER_LIMIT  512

void cry_cbc_decrypt(struct cry_cbc_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size)
{
    void *ciph = ctx->ciph_ctx;
    cry_ciph_encrypt_f decrypt = ctx->ciph_itf->decrypt;

    size &= ~(CRY_CBC_BLOCK_SIZE - 1);

    if (src != dst) {
        decrypt(ciph, dst, src, size);
        cry_memxor(dst, ctx->v, CRY_CBC_BLOCK_SIZE);
        cry_memxor(dst + CRY_CBC_BLOCK_SIZE, src, size - CRY_CBC_BLOCK_SIZE);
        memcpy(ctx->v, src + size - CRY_CBC_BLOCK_SIZE, CRY_CBC_BLOCK_SIZE);
    } else {
        /* 
         * For in-place CBC, we decrypt into a temporary buffer of size
         * at most CBC_BUFFER_LIMIT, and process that amount of data at
         * a time.
         */
        unsigned char initial_iv[CRY_CBC_BLOCK_SIZE];
        unsigned char fallback[CRY_CBC_BLOCK_SIZE];   /* if malloc fails */
        unsigned char *buffer;
        unsigned int buffer_size = CRY_MIN(size, CBC_BUFFER_LIMIT);

        buffer = malloc(buffer_size);
        if (buffer == NULL) {
            buffer = fallback;
            buffer_size = CRY_CBC_BLOCK_SIZE;
        }

        for ( ; size > buffer_size;
                size -= buffer_size, src += buffer_size, dst += buffer_size) {
            decrypt(ciph, buffer, src, buffer_size);
            memcpy(initial_iv, ctx->v, CRY_CBC_BLOCK_SIZE);
            memcpy(ctx->v, src + buffer_size - CRY_CBC_BLOCK_SIZE,
                   CRY_CBC_BLOCK_SIZE);
            cry_memxor3(dst + CRY_CBC_BLOCK_SIZE, buffer + CRY_CBC_BLOCK_SIZE,
                        src, buffer_size - CRY_CBC_BLOCK_SIZE);
            cry_memxor3(dst, buffer, initial_iv, CRY_CBC_BLOCK_SIZE);
        }

        decrypt(ciph, buffer, src, size);
        memcpy(initial_iv, ctx->v, CRY_CBC_BLOCK_SIZE);
        /* Copies last block */
        memcpy(ctx->v, src + size - CRY_CBC_BLOCK_SIZE, CRY_CBC_BLOCK_SIZE);
        /* Writes all but first block, reads all but last block. */
        cry_memxor3(dst + CRY_CBC_BLOCK_SIZE, buffer + CRY_CBC_BLOCK_SIZE,
                    src, size - CRY_CBC_BLOCK_SIZE);
        /* Writes first block. */
        cry_memxor3(dst, buffer, initial_iv, CRY_CBC_BLOCK_SIZE);

        if (buffer != fallback)
            free(buffer);
    }
}
