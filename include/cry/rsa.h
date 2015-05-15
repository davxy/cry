/*
 * Copyright (c) 2013-2015, Davide Galassi. All rights reserved.
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
 * @file    cry_rsa.h
 * @brief   RSA cipher.
 */

#ifndef _CRY_RSA_H_
#define _CRY_RSA_H_

#include "cry/mpi.h"

#define CRY_RSA_FLAG_SIGN   (1 << 0)

/**
 * Structure used for both private and public key.
 */
struct cry_rsa_ctx {
    cry_mpi m;      /**< Public modulus */
    cry_mpi e;      /**< Public exponent */
    cry_mpi d;      /**< Private exponent */
    int     flags;  /**< Behaviour flags */
};

typedef struct cry_rsa_ctx cry_rsa_ctx;

#ifdef __cplusplus
extern "C"{
#endif

int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen);

int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_RSA_H_ */

