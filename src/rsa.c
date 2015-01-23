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

#include "cry/rsa.h"
#include <string.h>

static int rsa_operate(unsigned char *out, unsigned char *in,
                       size_t siz, cry_mpi *exp, cry_mpi *mod)
{
    int ret;
    cry_mpi a;

    if ((ret = cry_mpi_init_bin(&a, in, siz)) != 0)
        return ret;
    if ((ret = cry_mpi_mod_exp(&a, &a, exp, mod)) == 0)
        ret = cry_mpi_store_bin(&a, out, siz, 1);
    cry_mpi_clear(&a);
    return ret;
}

int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char *out, unsigned char *in,
                    size_t siz)
{
    return rsa_operate(out, in, siz, &ctx->e, &ctx->m);
}

int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char *out, unsigned char *in,
                    size_t siz)
{
    return rsa_operate(out, in, siz, &ctx->d, &ctx->m);
}

