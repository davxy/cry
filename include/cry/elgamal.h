/**
 * @file    elgamal.h
 * @brief   Elgamal digital signature.
 */

#ifndef CRY_ELGAMAL_H_
#define CRY_ELGAMAL_H_

#include <cry/mpi.h>

/**
 * Elgamal context
 */
struct cry_elgamal_ctx {
    cry_mpi p;  /**< Prime */
    cry_mpi g;  /**< Group generator */
    cry_mpi y;  /**< g^x mod p */
    cry_mpi x;  /**< secret exponent */
};

#endif /* CRY_ELGAMAL_H_ */
