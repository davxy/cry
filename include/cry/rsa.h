/**
 * @file    rsa.h
 * @brief   RSA cipher.
 */

#ifndef CRY_RSA_H_
#define CRY_RSA_H_

#include <cry/mpi.h>

/** Schoolbook RSA */
#define CRY_RSA_PADDING_NONE        0
/** PKCS#1 v1.5 padding */
#define CRY_RSA_PADDING_PKCS_V15    1
/** PKCS#1 v2.0 padding */
#define CRY_RSA_PADDING_PKCS_V21    2

/**
 * Structure used for both private and public key.
 */
struct cry_rsa_ctx {
    cry_mpi n;      /**< Modulus */
    cry_mpi e;      /**< Public exponent */
    cry_mpi d;      /**< Private exponent */
    cry_mpi p;      /**< First secret prime factor */
    cry_mpi q;      /**< Second secret prime factor */
    char padding;   /**< Padding mode */
};

typedef struct cry_rsa_ctx cry_rsa_ctx;

#ifdef __cplusplus
extern "C" {
#endif

int cry_rsa_init(cry_rsa_ctx *ctc, int padding);

void cry_rsa_clear(cry_rsa_ctx *ctx);

int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen);

int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen);

int cry_rsa_sign(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                 const unsigned char *in, size_t inlen);

/**
 * RSA signature verification.
 *
 * Returns 1 on signature verification success, 0 on verification failure, <0 on error.
 */
int cry_rsa_verify(cry_rsa_ctx *ctx, const unsigned char *sig, size_t siglen,
                   const unsigned char *in, size_t inlen);

/**
 * Generate RSA key pair.
 *
 * If pubblic key exponent parameter is set to 0 then the exponent will be
 * randomly generated.
 *
 * @param ctx   RSA context that will hold the key.
 * @param bits  Size of public key in bits.
 * @param e     public exponent (e.g. 65537).
 */
int cry_rsa_keygen(cry_rsa_ctx *ctx, size_t bits, long e);


#ifdef __cplusplus
}
#endif

#endif /* CRY_RSA_H_ */
