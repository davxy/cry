/**
 * @file    ctr.h
 *
 * @brief   CTR block cipher mode of operation.
 *
 * Turns a block cipher into a stream cipher. It generates the next
 * keystream block by encrypting successive values of a "counter". The
 * counter can be any function which produces a sequence which is
 * guaranteed not to repeat for a long time, although an actual
 * increment-by-one counter is the simplest and most popular.
 */

#ifndef CRY_CTR_H_
#define CRY_CTR_H_

#include "cry/ciph.h"

/** Block size. */
#define CRY_CTR_BLOCK_MAX    16

/**
 * CTR context.
 */
struct cry_ctr_ctx {
    /** Cipher context. */
    void                      *ciph_ctx;
    /** Cipher interface. */
    const struct cry_ciph_itf *ciph_itf;
    /** Counter. */
    unsigned char              ctr[CRY_CTR_BLOCK_MAX];
    /** Counter length */
    unsigned int               ctrlen;
};

typedef struct cry_ctr_ctx cry_ctr_ctx;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Context initialization.
 *
 * @param ctx       CTR context.
 * @param ciph_ctx  Cipher context.
 * @param ciph_itf  Cipher interface.
 */
void cry_ctr_init(struct cry_ctr_ctx *ctx, void *ciph_ctx,
                  const struct cry_ciph_itf *ciph_itf);

/**
 * Set the cipher key in the cipher context.
 *
 * @param ctx   CTR context.
 * @param key   Cipher key.
 * @param size  Size of cipher key.
 */
void cry_ctr_key_set(struct cry_ctr_ctx *ctx, const unsigned char *key,
                     unsigned int size);

/**
 * Set the initialization vector in the CTR context.
 *
 * @param ctx   CTR context.
 * @param iv    Initialization vector.
 * @param size  Size of initialization vector (<= CRY_CTR_BLOCK_SIZE).
 */
void cry_ctr_iv_set(struct cry_ctr_ctx *ctx, const unsigned char *iv,
                    unsigned int size);

/**
 * Encryption function.
 *
 * @param ctx   CTR context.
 * @param dst   Destination pointer (cleartext).
 * @param src   Source pointer (ciphertext).
 * @param size  Size of ciphertext.
 */
void cry_ctr_encrypt(struct cry_ctr_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

#ifdef __cplusplus
}
#endif

/**
 * The decrypt is equal to the encrypt function.
 */
#define cry_ctr_decrypt cry_ctr_encrypt

#endif /* CRY_CTR_H_ */
