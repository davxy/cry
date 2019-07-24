/**
 * @file    rsa.h
 * @brief   RSA cipher.
 */

#ifndef CRY_RSA_H_
#define CRY_RSA_H_

#include <cry/mpi.h>

/** PKCS1 v1.5 encryption padding */
#define CRY_RSA_PADDING_PKCS1           1
/** PKCS1 v2.0 (OEAP) encryption padding */
#define CRY_RSA_PADDING_PKCS1_OEAP      2
/** PKCS1 v2.1 (PSS) signature padding */
#define CRY_RSA_PADDING_PKCS1_PSS       3

/**
 * Structure used for both private and public key.
 */
struct cry_rsa_ctx {
    cry_mpi n;      /**< Modulus */
    cry_mpi e;      /**< Public exponent */
    cry_mpi d;      /**< Private exponent */
    int padding;    /**< Padding mode */
};

typedef struct cry_rsa_ctx cry_rsa_ctx;

#ifdef __cplusplus
extern "C" {
#endif

int cry_rsa_init(cry_rsa_ctx *ctc, int padding);

int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen);

int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen);

int cry_rsa_keygen(cry_rsa_ctx *ctx, size_t bits);


#ifdef __cplusplus
}
#endif

#endif /* CRY_RSA_H_ */
