/**
 * @file    aes.h
 * @brief   AES block cipher.
 */

#ifndef CRY_AES_H_
#define CRY_AES_H_

#include <stddef.h>
#include <stdint.h>

#define CRY_AES_BLOCK_SIZE      16

#define CRY_AES_MODE_ENCRYPT    1
#define CRY_AES_MODE_DECRYPT    2

struct cry_aes_ctx {
    uint32_t      keys[60];     /** Maximum size of key schedule */
    unsigned int  nr;           /** Number of rounds */
    int           mode;         /** Encrypt/decrypt */
};

typedef struct cry_aes_ctx cry_aes_ctx;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Set the AES key.
 *
 * @param ctx   AES context.
 * @param key   Pointer to the key.
 * @param size  Key size.
 *              Must be 16 for AES-128, 24 for AES-192, 32 for AES-256.
 */
void cry_aes_key_set(cry_aes_ctx *ctx, const unsigned char *key, size_t size);

/**
 * Encrypt the data pointed by src into dst.
 *
 * @param ctx   AES context.
 * @param dst   Destination buffer (ciphertext).
 * @param src   Source buffer (cleartext).
 * @param size  Size of the data. Must be a multiple of 16.
 */
void cry_aes_encrypt(cry_aes_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size);

/**
 * Decrypt the data pointed by src into dst.
 * The function works in-place so dst can be equal to src.
 *
 * @param ctx   AES context.
 * @param dst   Destination buffer (cleartext).
 * @param src   Source buffer (ciphertext).
 * @param size  Size of the data. Must be a multiple of 16.
 */
void cry_aes_decrypt(cry_aes_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* CRY_AES_H_ */
