/*
 * Copyright (c) 2013-2017, Davide Galassi. All rights reserved.
 *
 * This file is part of CRY software.
 *
 * CRY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CRY; if not, see <http://www.gnu/licenses/>.
 */

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

