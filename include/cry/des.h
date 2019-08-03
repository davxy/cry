/**
 * @file    des.h
 * @brief   DES block cipher.
 */

#ifndef CRY_DES_H_
#define CRY_DES_H_

#include <string.h>

/** DES algorithm key size in octets (64 bit) */
#define CRY_DES_BLOCK_SIZE  8

/** DES context */
struct cry_des_ctx {
    size_t keylen;    /* 8 for DES, 24 for TDES (EDE) */
    unsigned char key[3*CRY_DES_BLOCK_SIZE];
};

typedef struct cry_des_ctx cry_des_ctx;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set the DES key.
 *
 * @param ctx   DES context.
 * @param key   Pointer to the key.
 * @param size  Key size in bytes.
 *              Shall be 8 for DES or 24 for TDES (EDE).
 */
void cry_des_key_set(cry_des_ctx *ctx, const unsigned char *key,
                     size_t size);

/**
 * Encrypt the data pointed by src into dst.
 * The function works in-place so dst can be equal to src.
 *
 * @param ctx   DES context.
 * @param dst   Destination buffer (ciphertext).
 * @param src   Source buffer (cleartext).
 * @param size  Size of the data. Must be a multiple of 8.
 */
void cry_des_encrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size);

/**
 * Decrypt the data pointed by src into dst.
 * The function works in-place so dst can be equal to src.
 *
 * @param ctx   DES context.
 * @param dst   Destination buffer (cleartext).
 * @param src   Source buffer (ciphertext).
 * @param size  Size of the data. Must be a multiple of 16.
 */
void cry_des_decrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* CRY_DES_H_ */
