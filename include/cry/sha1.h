/**
 * @file    sha1.h
 * @brief   SHA1 message digest algorithm
 */

#ifndef CRY_SHA1_H_
#define CRY_SHA1_H_

#include <stdint.h>
#include <stddef.h>

#define CRY_SHA1_DIGEST_SIZE 20

struct cry_sha1_ctx {
    unsigned char data[64]; /**< Data block being processed */
    uint32_t len[2];        /**< Number of data bytes processed */
    uint32_t state[5];      /**< Intermediate digest state */
};

typedef struct cry_sha1_ctx cry_sha1_ctx;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize SHA-1 context.
 *
 * @param ctx   SHA-1 context to be initialized
 */
void cry_sha1_init(cry_sha1_ctx *ctx);

/**
 * Clear SHA-1 context.
 *
 * @param ctx   SHA-1 context to be cleared
 */
void cry_sha1_clear(cry_sha1_ctx *ctx);

/**
 * SHA-1 process buffer.
 *
 * @param ctx   SHA-1 context.
 * @param data  Buffer holding the data.
 * @param len   Length of the input data.
 */
void cry_sha1_update(cry_sha1_ctx *ctx, const unsigned char *data, size_t len);

/**
 * Final digest.
 *
 * @param ctx   SHA-1 context
 * @param out   SHA-1 result
 */
void cry_sha1_digest(cry_sha1_ctx *ctx, unsigned char *out);

/**
 * SHA-1 computation of a given input.
 *
 * @param out   SHA-1 result buffer (shall be at least 20 bytes).
 * @param data  Input data buffer.
 * @param len   Input data length.
 */
void cry_sha1(unsigned char *out, const unsigned char *data, size_t len);


#ifdef __cplusplus
}
#endif

#endif /* CRY_SHA1_H_ */
