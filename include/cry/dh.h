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
 * @file    cry_dh.h
 * @brief   Diffie-Hellman key exchange algorithm.
 */

#ifndef _CRY_DH_
#define _CRY_DH_

#include "cry/mpi.h"

/** Diffie-Hellman algorithm context */
struct cry_dh_ctx {
    cry_mpi p;    /**< Shared modulus */
    cry_mpi g;    /**< Shared base */
    cry_mpi e;    /**< Secret exponent */
    cry_mpi Y;    /**< First step token */
    cry_mpi Z;    /**< Shared secret */
};

typedef struct cry_dh_ctx cry_dh_ctx;

#ifdef __cplusplus
extern "C"{
#endif

int cry_dh_init(cry_dh_ctx *ctx);

void cry_dh_clear(cry_dh_ctx *ctx);

int cry_dh_agree(cry_dh_ctx *ctx);

int cry_dh_finalize(cry_dh_ctx *ctx);

int cry_dh_get_tok(cry_dh_ctx *ctx, unsigned char *out, size_t out_len);

int cry_dh_set_tok(cry_dh_ctx *ctx, unsigned char *in, size_t in_len);

int cry_dh_get_key(cry_dh_ctx *ctx, unsigned char *out, size_t out_len);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_DH_ */

