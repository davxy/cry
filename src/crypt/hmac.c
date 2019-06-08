#include <cry/hmac.h>
#include <string.h>

#define BLOCK_SIZE  CRY_HMAC_BLOCK_MAX

void cry_hmac_init(cry_hmac_ctx *ctx, void *hash_ctx,
                   const cry_hash_itf *hash_itf, size_t hash_len,
                   const unsigned char *key, size_t key_len)
{
    size_t i;
    unsigned char pad[BLOCK_SIZE];

    ctx->hash_ctx = hash_ctx;
    ctx->hash_itf = hash_itf;
    ctx->hash_len = hash_len;

    if (key_len <= BLOCK_SIZE) {
        memcpy(ctx->key, key, key_len);
        ctx->key_len = key_len;
    } else {
        if (hash_itf->init)
            hash_itf->init(ctx->hash_ctx);
        hash_itf->update(ctx->hash_ctx, key, key_len);
        hash_itf->digest(ctx->hash_ctx, ctx->key);
        if (hash_itf->clean)
            hash_itf->clean(ctx->hash_ctx);
        ctx->key_len = ctx->hash_len;
    }

    memset(pad, 0x36, BLOCK_SIZE);
    for (i = 0; i < ctx->key_len; i++)
        pad[i] ^= ctx->key[i];
    if (hash_itf->init)
        hash_itf->init(ctx->hash_ctx);
    hash_itf->update(ctx->hash_ctx, pad, BLOCK_SIZE);
}

void cry_hmac_update(cry_hmac_ctx *ctx, const unsigned char *in, size_t in_len)
{
    ctx->hash_itf->update(ctx->hash_ctx, in, in_len);
}

void cry_hmac_digest(cry_hmac_ctx *ctx, unsigned char *mac)
{
    size_t i;
    unsigned char pad[BLOCK_SIZE];
    const cry_hash_itf *hash_itf = ctx->hash_itf;

    hash_itf->digest(ctx->hash_ctx, mac);
    if (hash_itf->clean)
        hash_itf->clean(ctx->hash_ctx);

    memset(pad, 0x5C, BLOCK_SIZE);
    for (i = 0; i < ctx->key_len; i++)
        pad[i] ^= ctx->key[i];
    if (hash_itf->init)
        hash_itf->init(ctx->hash_ctx);
    hash_itf->update(ctx->hash_ctx, pad, BLOCK_SIZE);
    hash_itf->update(ctx->hash_ctx, mac, ctx->hash_len);
    hash_itf->digest(ctx->hash_ctx, mac);
    if (hash_itf->clean)
        hash_itf->clean(ctx->hash_ctx);
}
