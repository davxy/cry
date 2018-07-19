/**
 * @file    cry_ciph.h
 * @brief   Generic cipher interface
 */

#ifndef _CRY_CIPH_H_
#define _CRY_CIPH_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Context initialization function pointer type.
 *
 * @param ctx   Cipher context
 */
typedef void (* cry_ciph_init_f)(void *ctx);

/**
 * Context cleanup function pointer type.
 *
 * @param ctx   Cipher context
 */
typedef void (* cry_ciph_clean_f)(void *ctx);

/**
 * Key set function pointer type.
 *
 * @param ctx   Cipher context.
 * @param key   Cipher key.
 * @param size  Key size.
 */
typedef void (* cry_ciph_key_set_f)(void *ctx, const unsigned char *key,
                                    unsigned int size);

/**
 * Encrypt function pointer type.
 *
 * @param ctx   Cipher context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
typedef void (* cry_ciph_encrypt_f)(void *ctx, unsigned char *dst,
                                    const unsigned char *src,
                                    unsigned int size);

/**
 * Decrypt function pointer type.
 *
 * @param ctx   Cipher context.
 * @param dst   Destination buffer.
 * @param src   Source buffer.
 * @param size  Size of source/destination buffers.
 */
typedef void (* cry_ciph_decrypt_f)(void *ctx, unsigned char *dst,
                                    const unsigned char *src,
                                    unsigned int size);

#ifdef __cplusplus
}
#endif

/**
 * Cipher algorithm generic interface.
 */
struct cry_ciph_itf {
    cry_ciph_init_f       init;
    cry_ciph_clean_f      clean;
    cry_ciph_key_set_f    key_set;
    cry_ciph_encrypt_f    encrypt;
    cry_ciph_decrypt_f    decrypt;
};

typedef struct cry_ciph_itf cry_ciph_itf;


#endif /* _CRY_CIPH_H_ */

