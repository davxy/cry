/**
 * @file    gcm.h
 * @brief   GCM block cipher mode of operation.
 */

#ifndef CRY_GCM_H_
#define CRY_GCM_H_

#include "cry/ciph.h"

/** Size of a GCM block. */
#define CRY_GCM_BLOCK_SIZE      16

/** GCM context. */
struct cry_gcm_ctx {
    /** Block cipher context. */
    void                      *ciph_ctx;
    /** Block cipher interface. */
    const struct cry_ciph_itf *ciph_itf;
    /** Initialization vector */
    unsigned char              iv[CRY_GCM_BLOCK_SIZE];
    /** Counter. */
    unsigned char              ctr[CRY_GCM_BLOCK_SIZE];
    /** Hashing state */
    unsigned char              hs[CRY_GCM_BLOCK_SIZE];
    /** Hashing subkey. */
    unsigned char              key[CRY_GCM_BLOCK_SIZE];
    /** Authenticated data length. */
    unsigned int               auth_len;
    /** Cipher data length. */
    unsigned int               ciph_len;
};

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Context initialization.
 *
 * @param ctx       GCM context.
 * @param ciph_ctx  Cipher context.
 * @param ciph_itf  Cipher interface.
 */
void cry_gcm_init(struct cry_gcm_ctx *ctx, void *ciph_ctx,
                  const struct cry_ciph_itf *ciph_itf);

/**
 * Set the key in the gcm and in the cipher context.
 *
 * @param ctx   GCM context.
 * @param key   Cipher key.
 * @param size  Size of cipher key.
 */
void cry_gcm_key_set(struct cry_gcm_ctx *ctx, const unsigned char *key,
                     unsigned int size);

/**
 * Set of the initialization vector.
 *
 * @param ctx   GCM context.
 * @param iv    Initialization vector buffer.
 * @param size  Size of the initialization vector.
 */
void cry_gcm_iv_set(struct cry_gcm_ctx *ctx, const unsigned char *iv,
                    unsigned int size);

/**
 * Encryption function.
 *
 * @param ctx   GCM context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
void cry_gcm_encrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

/**
 * Decryption function.
 *
 * @param ctx   GCM context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
void cry_gcm_decrypt(struct cry_gcm_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

/**
 * Set the additional authentication data.
 *
 * This must me called before the encrypt (decrypt) function (data_size == 0).
 *
 * @param ctx   GCM context.
 * @param aad   Additional authentication data buffer.
 * @param size  Size of the additional auth data.
 *              Constraint: auth_size % GCM_BLOCK_SIZE == 0.
 */
void cry_gcm_update(struct cry_gcm_ctx *ctx, const unsigned char *aad,
                    unsigned int size);

/**
 * Generate the GCM MAC (GMAC).
 *
 * @param ctx   GCM context.
 * @param mac   Message authentication code output buffer.
 * @param size  Should be <= GCM_BLOCK_SIZE (16).
 */
void cry_gcm_digest(struct cry_gcm_ctx *ctx, unsigned char *mac,
                    unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* CRY_GCM_H_ */
