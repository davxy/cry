/**
 * @file    ciph.h
 * @brief   Generic cipher interface
 */

#ifndef CRY_CIPH_H_
#define CRY_CIPH_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Context initialization.
 *
 * @param ctx   Cipher context
 */
typedef void (*cry_ciph_init_f)(void *ctx);

/**
 * Context cleanup.
 *
 * @param ctx   Cipher context
 */
typedef void (*cry_ciph_clear_f)(void *ctx);

/**
 * Key set.
 *
 * @param ctx   Cipher context.
 * @param key   Key.
 * @param size  Key size.
 */
typedef void (*cry_ciph_key_set_f)(void *ctx, const unsigned char *key,
                                   size_t size);

/**
 * Initialization vector set.
 *
 * @param ctx   Cipher context.
 * @param iv    Initialization vector.
 * @param size  Initialization vector length.
 */
typedef void (*cry_ciph_iv_set_f)(void *ctx, const unsigned char *iv,
                                  size_t size);

/**
 * Encrypt function pointer type.
 *
 * @param ctx   Cipher context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
typedef void (*cry_ciph_encrypt_f)(void *ctx, unsigned char *dst,
                                   const unsigned char *src,
                                   size_t size);

/**
 * Decrypt function pointer type.
 *
 * @param ctx   Cipher context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
typedef void (*cry_ciph_decrypt_f)(void *ctx, unsigned char *dst,
                                   const unsigned char *src,
                                   size_t size);

#ifdef __cplusplus
}
#endif

/**
 * Cipher algorithm generic interface.
 */
struct cry_ciph_itf {
    cry_ciph_init_f init;
    cry_ciph_clear_f clear;
    cry_ciph_key_set_f key_set;
    cry_ciph_iv_set_f iv_set;
    cry_ciph_encrypt_f encrypt;
    cry_ciph_decrypt_f decrypt;
};

typedef struct cry_ciph_itf cry_ciph_itf;


#endif /* CRY_CIPH_H_ */
