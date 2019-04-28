/**
 * @file    ecdsa.h
 * @brief   Elliptic Curve Digital Signature Algorithm
 */

#ifndef CRY_ECDSA_H_
#define CRY_ECDSA_H_

#include <cry/ecp.h>

struct cry_ecdsa_ctx {
    cry_ecp_grp  ec;    /**< Elliptic curve group parameters */
    cry_mpi d;          /**< Private signing key */
    cry_ecp q;          /**< Public verification key */
};

typedef struct cry_ecdsa_ctx cry_ecdsa_ctx;

struct cry_ecdsa_signature {
    cry_mpi r;
    cry_mpi s;
};

typedef struct cry_ecdsa_signature cry_ecdsa_signature;

#ifdef __cplusplus
extern "C"{
#endif

int cry_ecdsa_sign(cry_ecdsa_ctx *ctx, cry_ecdsa_signature *sign,
                   const unsigned char *in, size_t len);

int cry_ecdsa_verify(cry_ecdsa_ctx *ctx, const cry_ecdsa_signature *sign,
                     const unsigned char *in, size_t len);

int cry_ecdsa_keygen(const cry_ecp_grp *ec, cry_mpi *d, cry_ecp *q);

#ifdef __cplusplus
}
#endif

#endif /* CRY_ECDSA_H_ */
