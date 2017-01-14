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
 * @file    cry_dsa.h
 * @brief   Digital Signature Algorithm
 */

#ifndef _CRY_DSA_H_
#define _CRY_DSA_H_

#include "cry/mpi.h"

struct cry_dsa_ctx {
    cry_mpi  g;
    cry_mpi  p;
    cry_mpi  q;
    cry_mpi  pvt; /**< Private signing key */
    cry_mpi  pub; /**< Public verification key */
};

typedef struct cry_dsa_ctx cry_dsa_ctx;

struct cry_dsa_signature {
    cry_mpi r;
    cry_mpi s;
};

typedef struct cry_dsa_signature cry_dsa_signature;

#ifdef __cplusplus
extern "C"{
#endif

int cry_dsa_sign(cry_dsa_ctx *ctx, cry_dsa_signature *sign,
                 const unsigned char *in, size_t len);

int cry_dsa_verify(cry_dsa_ctx *ctx, const cry_dsa_signature *sign,
                   const unsigned char *in, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_DSA_H_ */

