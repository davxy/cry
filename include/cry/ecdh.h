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
 * @file    cry_ecdh.h
 * @brief   Elliptic Curve Diffie Hellman
 */

#ifndef _CRY_ECDH_
#define _CRY_ECDH_

#include "cry/ec.h"

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

#endif /* _CRY_ECDH_ */

