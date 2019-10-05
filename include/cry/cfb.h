/**
 * @file    cfb.h
 *
 * @brief   CFB block cipher mode of operation.
 *
 * The Cipher Feedback (CFB) mode, a close relative of CBC, makes a block
 * cipher into a self-synchronizing stream cipher. Operation is very
 * similar; in particular, CFB decryption is almost identical to CBC
 * encryption performed in reverse.
 */

#ifndef CRY_CFB_H_
#define CRY_CFB_H_

#include <cry/ciph.h>

/** Block size. */
#define CRY_CFB_BLOCK_MAX    16

/**
 * CFB context.
 */
struct cry_cfb_ctx {
    /** Cipher context. */
    void *ciph_ctx;
    /** Cipher interface. */
    const struct cry_ciph_itf *ciph_itf;
    /** Initialization vector length */
    size_t vlen;
    /** Initialization vector data. */
    unsigned char v[CRY_CFB_BLOCK_MAX];
};

typedef struct cry_cfb_ctx cry_cfb_ctx;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Context initialization.
 *
 * @param ctx       CFB context.
 * @param ciph_ctx  Cipher context.
 * @param ciph_itf  Cipher interface.
 */
void cry_cfb_init(struct cry_cfb_ctx *ctx, void *ciph_ctx,
                  const struct cry_ciph_itf *ciph_itf);

/**
 * Context cleanup.
 *
 * @param ctx       CFB context.
 */
void cry_cfb_clear(struct cry_cfb_ctx *ctx);

/**
 * Set the cipher key in the cipher context.
 *
 * @param ctx   CFB context.
 * @param key   Cipher key.
 * @param size  Size of cipher key.
 */
void cry_cfb_key_set(struct cry_cfb_ctx *ctx, const unsigned char *key,
                     size_t size);

/**
 * Set the initialization vector in the CFB context.
 *
 * @param ctx   CFB context.
 * @param iv    Initialization vector.
 * @param size  Size of initialization vector (<= CRY_CFB_BLOCK_SIZE).
 */
void cry_cfb_iv_set(struct cry_cfb_ctx *ctx, const unsigned char *iv,
                    size_t size);

/**
 * Encrypt/Decrypt function.
 *
 * @param ctx   CFB context.
 * @param dst   Destination pointer.
 * @param src   Source pointer.
 * @param size  Size of source/destination.
 */
void cry_cfb_crypt(struct cry_cfb_ctx *ctx, unsigned char *dst,
                   const unsigned char *src, size_t size);

/**
 * Encryption function.
 *
 * @param ctx   CFB context.
 * @param dst   Destination pointer (ciphertext).
 * @param src   Source pointer (cleartext).
 * @param size  Size of cleartext.
 */
#define cry_cfb_encrypt cry_cfb_crypt

/**
 * Decryption function.
 *
 * @param ctx   CFB context.
 * @param dst   Destination pointer (cleartext).
 * @param src   Source pointer (ciphertext).
 * @param size  Size of ciphertext.
 */
#define cry_cfb_decrypt cry_cfb_crypt


/**
 * Encryption function.
 *
 * @param ctx   CFB context.
 * @param dst   Destination pointer (ciphertext).
 * @param src   Source pointer (cleartext).
 * @param size  Size of source/destination.
 */
void cry_cfb8_encrypt(struct cry_cfb_ctx *ctx, unsigned char *dst,
                      const unsigned char *src, size_t size);

/**
 * Decryption function.
 *
 * @param ctx   CFB context.
 * @param dst   Destination pointer (cleartext).
 * @param src   Source pointer (ciphertext).
 * @param size  Size of ciphertext.
 */
void cry_cfb8_decrypt(struct cry_cfb_ctx *ctx, unsigned char *dst,
                      const unsigned char *src, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* CRY_CFB_H_ */
