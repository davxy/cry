/**
 * @file    cry_md5.h
 * @brief   MD5 message digest algorithm
 */

#ifndef _CRY_MD5_H_
#define _CRY_MD5_H_

#include <stddef.h>
#include <stdint.h>

/** MD5 hash size in octets. */
#define CRY_MD5_DIGEST_SIZE     16

/**
 * Data structure for MD5 (Message Digest) computation
 */
struct cry_md5_ctx {
    uint32_t      len[2];   /**< Data length in bits */
    uint32_t      buf[4];   /**< Hash buffer */
    unsigned char in[64];   /**< Partial input buffer */
};

typedef struct cry_md5_ctx cry_md5_ctx;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the MD5 context.
 *
 * @param ctx       MD5 context.
 */
void cry_md5_init(struct cry_md5_ctx *ctx);

/**
 * Add data to the MD5 hash.
 *
 * @param ctx       MD5 context.
 * @param data      Data array.
 * @param size      Data array size in octets.
 */
void cry_md5_update(struct cry_md5_ctx *ctx, const unsigned char *data,
                    size_t size);

/**
 * Finalize the MD5 hash.
 *
 * @param ctx       MD5 context.
 * @param digest    Destination buffer where the 16 octets hash will be stored.
 */
void cry_md5_digest(struct cry_md5_ctx *ctx, unsigned char *digest);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_MD5_H_ */

