/**
 * @file    trivium.h
 * @brief   Trivium stream cipher.
 *
 * Trivium is a synchronous stream cipher designed to provide a flexible
 * trade-off between speed and gate count in hardware, and reasonably efficient
 * software implementation.
 *
 * The cipher makes use of a 80-bit key and 80-bit initialisation vector (IV);
 * its secret state has 288 bits, consisting of three interconnected non-linear
 * feedback shift registers (NLFSR) of length 93, 84 and 111 bits, respectively.
 *
 * The cipher operation consists of two phases: the key and IV set-up and the
 * keystream generation. Initialisation is very similar to keystream generation
 * and requires 1152 steps of the clocking procedure of Trivium. The keystream
 * is generated by repeatedly clocking the cipher, where in each clock cycle
 * three state bits are updated using a non-linear feedback function, and one
 * bit of keystream is produced and output. The cipher specification states
 * that 264 keystream bits can be generated from each key/IV pair.
 *
 * For more information about refer to the eSTREAM project page:
 * http://www.ecrypt.eu.org/stream/e2-trivium.html
 */

#ifndef CRY_TRIVIUM_H_
#define CRY_TRIVIUM_H_

#include <stddef.h>
#include <stdint.h>

#define CRY_TRIVIUM_KEYLEN  10  /* 80-bit */
#define CRY_TRIVIUM_IVLEN   10  /* 80-bit */

struct cry_trivium_ctx {
    size_t  keylen;
    size_t  ivlen;
    uint8_t s[40];
    uint8_t key[CRY_TRIVIUM_KEYLEN];
};

typedef struct cry_trivium_ctx cry_trivium_ctx;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Context initialization.
 *
 * @param ctx       Trivium context.
 */
void cry_trivium_init(cry_trivium_ctx *ctx);

/**
 * Set the cipher key.
 *
 * @param ctx   Trivium context.
 * @param key   Cipher key.
 * @param size  Cipher key size.
 */
void cry_trivium_key_set(cry_trivium_ctx *ctx, const unsigned char *key,
                         size_t size);

/**
 * Set the initialization vector.
 *
 * @param ctx   Trivium context.
 * @param iv    Initialization vector.
 * @param size  Initialization vector size.
 */
void cry_trivium_iv_set(cry_trivium_ctx *ctx, const unsigned char *iv,
                        size_t size);

/**
 * Encryption function.
 *
 * @param ctx   Trivium context.
 * @param dst   Destination buffer (ciphertext).
 * @param src   Source buffer (cleartext).
 * @param size  Number of bytes.
 */
void cry_trivium_encrypt(cry_trivium_ctx *ctx, unsigned char *dst,
                         const unsigned char *src, size_t size);

/**
 * Decryption function.
 *
 * @param ctx   Trivium context.
 * @param dst   Destination buffer (cleartext).
 * @param src   Source buffer (ciphertext).
 * @param size  Number of bytes.
 */
void cry_trivium_decrypt(cry_trivium_ctx *ctx, unsigned char *dst,
                         const unsigned char *src, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* CRY_TRIVIUM_H_ */
