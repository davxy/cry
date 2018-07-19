/**
 * @file    ec.h
 * @brief   Elliptic curve equation
 */

#ifndef _CRY_EC_H_
#define _CRY_EC_H_

#include "cry/ecp.h"

/*
 * Describes an elliptic curve equation:
 *   y^2 = (x^3 + ax + b) % p
 */
struct cry_ec {
    cry_mpi p;  /**< Modulus */
    cry_mpi a;  /**< x coefficient */
    cry_mpi b;  /**< Constant */
    cry_ecp g;  /**< Base point */
    cry_mpi n;  /**< n is prime and is the 'order' of 'g' */
};

typedef struct cry_ec cry_ec;

#define cry_ec_init(ec) \
        cry_mpi_init_list(&(ec)->p, &(ec)->a, &(ec)->b, \
                          &(ec)->n, &(ec)->g.x, &(ec)->g.y, NULL)

#define cry_ec_clear(ec) \
        cry_mpi_clear_list(&(ec)->p, &(ec)->a, &(ec)->b, \
                           &(ec)->n, &(ec)->g.x, &(ec)->g.y, NULL)

#ifdef __cplusplus
extern "C"{
#endif

int cry_ec_init_nist_p256(cry_ec *ec);

int cry_ec_set_nist_p256(cry_ec *ec);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_EC_H_ */

