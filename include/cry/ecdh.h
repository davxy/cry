/**
 * @file    ecdh.h
 * @brief   Elliptic Curve Diffie Hellman
 */

#ifndef CRY_ECDH_
#define CRY_ECDH_

#include <cry/ec.h>

struct cry_ecdh_ctx {
    cry_ec      ec;  /**> Elliptic curve parameters */
    cry_mpi     d;   /**> Private key */
    cry_ecp     q;   /**> Public key */
    cry_ecp     z;   /**> Shared secret */
};

typedef struct cry_ecdh_ctx cry_ecdh_ctx;

#ifdef __cplusplus
extern "C"{
#endif

int cry_ecdh_init(cry_ecdh_ctx *ctx);

void cry_ecdh_clear(cry_ecdh_ctx *ctx);

int cry_ecdh_agree(cry_ecdh_ctx *ctx);

int cry_ecdh_final(cry_ecdh_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif /* CRY_ECDH_ */
