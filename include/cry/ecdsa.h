/*
 * Copyright (c) 2013-2018, Davide Galassi. All rights reserved.
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
 * @file    cry_ecdsa.h
 * @brief   Elliptic Curve Digital Signature Algorithm
 */

#ifndef _CRY_ECDSA_H_
#define _CRY_ECDSA_H_

#include "cry/ec.h"

struct cry_ecdsa_ctx {
    cry_ec  ec; /**< Elliptic curve parameters */
    cry_mpi d;  /**< Private signing key */
    cry_ecp q;  /**< Public verification key */
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

int cry_ecdsa_keygen(const cry_ec *ec, cry_mpi *d, cry_ecp *q);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_ECDSA_H_ */

