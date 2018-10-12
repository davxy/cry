/*
 * The following algorithm is not secure and is provided as an
 * historical reference only.
 *
 * The Hill cipher is a polygraphic substitution cipher based on linear
 * algebra.
 *
 * Given a nxn matrix A that is invertible modulo 256. To encrypt a message
 * each cleartext block of n octets is treated as an n elements vector
 * and left multiplied by A.
 * To decrypt the message each ciphertext block of n octets is instead left
 * multiplied by the inverse matrix A^-1.
 *
 * Given the cleartext x=<x1,...,xn> and the ciphertext y=<x1...yn> 
 * Encryption function: E(x) = A * x = y
 * Encryption function: D(y) = A^-1 * y = x
 *
 * To be invertible modulo 256, the matrix determinant det should be
 * a unit (invertible) modulo 256. Thus there should be a value idet such
 * that det*idet = 1 (mod 256).
 * From theory We know that idet exists iff gcd(det,256)= 1 and, since
 * 256=2^8, this is true whenever the determinant value is odd.
 */

#ifndef CRY_HILL_H_
#define CRY_HILL_H_

#include <stddef.h>

#define CRY_HILL_KEYDIM_MAX 10
#define CRY_HILL_KEYLEN_MAX (CRY_HILL_KEYDIM_MAX*CRY_HILL_KEYDIM_MAX)

#define CRY_HILL_KEYGEN_TRIALS 100


struct cry_hill_ctx {
    unsigned char key[CRY_HILL_KEYLEN_MAX];
    unsigned char ikey[CRY_HILL_KEYLEN_MAX];
    size_t n;
};

typedef struct cry_hill_ctx cry_hill_ctx;

int cry_hill_init(struct cry_hill_ctx *ctx, const unsigned char *key,
                  const unsigned char *ikey, size_t keylen);

void cry_hill_encrypt(struct cry_hill_ctx *ctx, unsigned char *out,
                      const unsigned char *in, size_t len);

void cry_hill_decrypt(struct cry_hill_ctx *ctx, unsigned char *out,
                      const unsigned char *in, size_t len);

#endif /* CRY_HILL_H_ */
