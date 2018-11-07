/**
 * @file    hmac.h
 * @brief   Hash MAC
 */

#include "cry/hash.h"

#ifndef _CRY_HMAC_H_
#define _CRY_HMAC_H_

#define CRY_HMAC_HASH_BLOCK_MAX     64

struct cry_hmac_ctx {
    void                *hash_ctx;
    const cry_hash_itf  *hash_itf;
    size_t               hash_len;
    const unsigned char *key;
    size_t               key_len;
};

typedef struct cry_hmac_ctx cry_hmac_ctx;

void cry_hmac_init(cry_hmac_ctx *ctx, void *hash_ctx,
                   const cry_hash_itf *hash_itf, size_t hash_len,
                   const unsigned char *key, size_t key_len);

int cry_hmac_digest(cry_hmac_ctx *ctx, unsigned char *mac,
                    const unsigned char *in, size_t insize);

#endif /* _CRY_HMAC_H_ */
