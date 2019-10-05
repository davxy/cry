/**
 * @file    utils.h
 * @brief   Elliptic curve point arithmetic
 *
 */

#ifndef CRY_UTILS_H_
#define CRY_UTILS_H_

#include <stddef.h>

/**
 * Wrap keys with AES Key Wrap Algorithm (RFC-3394)
 *
 * If input length is n blocks then output length is n+1 blocks.
 *
 * @out     Cipher-text output.
 * @in      Plain-text input.
 * @len     Length of the plain-text (in).
 * @key     Key-Encryption Key (KEK).
 * @keylen  KEK length.
 * @return  0 on success, -1 on failure
 */
int cry_aes_wrap(unsigned char *out, const unsigned char *in, size_t len,
                 const unsigned char *key, size_t keylen);

/**
 * Unwrap keys with AES Key Wrap Algorithm (RFC-3394)
 *
 * If input length is n blocks then output length is n+1 blocks.
 *
 * @out     Plain-text output.
 * @in      Cipher-text input.
 * @len     Length of the plain-text (out).
 * @key     Key-Encryption Key (KEK).
 * @keylen  KEK length.
 * @return  0 on success, -1 on failure
 */
int cry_aes_unwrap(unsigned char *out, const unsigned char *in, size_t len,
                   const unsigned char *key, size_t keylen);

#endif /* CRY_UTILS_H_ */
