/*
 * The following algorithm is not secure and is provided as an
 * historical reference only.
 *
 * The original affine cipher is a monoalphaberic substitution cipher where
 * each octet is encrypted using a simple linear function performed modulo
 * 256.
 *
 * Encryption function: E(x) = (ax + b) = y
 * Decryption function: D(y) = (y - b)*a^-1 = x
 *
 * Note that to be invertible the value 'a' should be chosen so that
 * gcd(a,256)=1 and, since 256=2^8, this is true whenever the value
 * of 'a' is odd.
 *
 * The provided implementation allows the usage of a list of 'a' values
 * and 'b' values, thus it can be more appropriately classified as a
 * polyalphabetic affine cipher.
 *
 * Given the two variable length 'keys' the implementation provides the
 * following well known ciphers:
 *  - Caesar     : keylen = 1, keya = {1}, keyb = {3}
 *  - Rot-X      : keylen = 1, keya = {1}, keyb = {X}
 *  - Affine     : keylen = 1, keya = {a}, keyb = {0}
 *  - Vigenere   : keylen > 1, keya = {1,...,1},   keyb = {b1,...,bn}
 *  - Poly-Affine: keylen > 1, keya = {a1,...,an}, keyb = {b1,...,bn}
 */

#ifndef CRY_AFFINE_H_
#define CRY_AFFINE_H_

#include <stddef.h>

#define CRY_AFFINE_KEYMAX 32

struct cry_affine_ctx {
    unsigned char keya[CRY_AFFINE_KEYMAX];
    unsigned char inva[CRY_AFFINE_KEYMAX];
    unsigned char keyb[CRY_AFFINE_KEYMAX];
    size_t keylen;
};

typedef struct cry_affine_ctx cry_affine_ctx;

int cry_affine_init(struct cry_affine_ctx *ctx, const unsigned char *keya,
                    const unsigned char *keyb, size_t keylen);

void cry_affine_encrypt(struct cry_affine_ctx *ctx, unsigned char *out,
                        const unsigned char *in, size_t len);

void cry_affine_decrypt(struct cry_affine_ctx *ctx, unsigned char *out,
                        const unsigned char *in, size_t len);

#endif /* CRY_AFFINE_H_ */
