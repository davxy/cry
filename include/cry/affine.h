/*
 * The following algorithm is not secure and is provided as an
 * historical reference only.
 * The following cipher mix together the affine and the substitution
 * ciphers.
 *
 * Affine cipher encryption function:
 *  E(x) = (ax + b)
 *  D(y) = (y - b)*a^-1
 * To allow a correct decryption is fundamental that gcd(a,256)=1
 * and that is the case for all the odd numbers less than 256.
 *
 * To exploit the properties of the vigenere cipher we make a list of
 * numbers 'a's and 'b's to be used as keys for the affine cipher.
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
