/**
 * @file    cbc.h
 * @brief   CBC block cipher mode of operation.
 */

#ifndef CRY_CBC_H_
#define CRY_CBC_H_

#include <cry/ciph.h>

/** CBC block size. */
#define CRY_CBC_BLOCK_MAX   16


/** CBC context structure. */
struct cry_cbc_ctx {
    /** Cipher context. */
    void                      *ciph_ctx;
    /** Cipher interface. */
    const struct cry_ciph_itf *ciph_itf;
    /** Counter */
    unsigned char              ctr[CRY_CBC_BLOCK_MAX];
    /** Counter length */
    unsigned int               ctrlen;
};

typedef struct cry_cbc_ctx cry_cbc_ctx;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Context initialization.
 *
 * @param ctx       CBC context.
 * @param ciph_ctx  Cipher context.
 * @param ciph_itf  Cipher interface.
 */
void cry_cbc_init(struct cry_cbc_ctx *ctx, void *ciph_ctx,
                  const cry_ciph_itf *ciph_itf);

/**
 * Set the cipher key.
 *
 * @param ctx   CBC context.
 * @param key   Cipher key.
 * @param size  Cipher key size.
 */
void cry_cbc_key_set(struct cry_cbc_ctx *ctx, const unsigned char *key,
                     unsigned int size);

/**
 * Set the initialization vector.
 *
 * @param ctx   CBC context.
 * @param iv    Initialization vector.
 * @param size  Initialization vector size.
 */
void cry_cbc_iv_set(struct cry_cbc_ctx *ctx, const unsigned char *iv,
                    unsigned int size);

/**
 * Encryption function.
 *
 * @param ctx   CBC context.
 * @param dst   Destination buffer (ciphertext).
 * @param src   Source buffer (cleartext).
 * @param size  Size should be a multiple of CBC_BLOCK_SIZE
 */
void cry_cbc_encrypt(struct cry_cbc_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

/**
 * Decryption function.
 *
 * @param ctx   CBC context.
 * @param dst   Destination buffer (cleartext).
 * @param src   Source buffer (ciphertext).
 * @param size  Size should be a multiple of CBC_BLOCK_SIZE.
 */
void cry_cbc_decrypt(struct cry_cbc_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* CRY_CBC_H_ */
