/**
 * @file    cry_sha256.h
 * @brief   Secure Hash Algorithm (256)
 */

#ifndef _CRY_SHA256_H_
#define _CRY_SHA256_H_

#include <stdint.h>
#include <stddef.h>

#define CRY_SHA256_DIGEST_SIZE 32

struct cry_sha256_ctx {
    unsigned char   data[64];
    uint32_t        datalen;
    uint64_t        bitlen;
    uint32_t        state[8];
};

typedef struct cry_sha256_ctx cry_sha256_ctx;

#ifdef __cplusplus
extern "C" {
#endif

void cry_sha256_init(struct cry_sha256_ctx *ctx);

void cry_sha256_update(struct cry_sha256_ctx *ctx, const unsigned char *data,
                       size_t size);

void cry_sha256_digest(struct cry_sha256_ctx *ctx, unsigned char *digest);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_SHA256_H */

