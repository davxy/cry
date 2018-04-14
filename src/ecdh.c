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

#include "cry/ecdh.h"

int cry_ecdh_init(cry_ecdh_ctx *ctx)
{
    cry_ec_init(&ctx->ec);
    cry_mpi_init(&ctx->d);
    cry_ecp_init(&ctx->q);
    cry_ecp_init(&ctx->z);
    return 0;
}

void cry_ecdh_clear(cry_ecdh_ctx *ctx)
{
    cry_ec_clear(&ctx->ec);
    cry_mpi_clear(&ctx->d);
    cry_ecp_clear(&ctx->q);
    cry_ecp_clear(&ctx->z);
}

int cry_ecdh_agree(cry_ecdh_ctx *ctx)
{
    return cry_ecp_mul(&ctx->q, &ctx->ec.g, &ctx->d, &ctx->ec.a, &ctx->ec.p);
}

int cry_ecdh_final(cry_ecdh_ctx *ctx)
{
    return cry_ecp_mul(&ctx->z, &ctx->q, &ctx->d, &ctx->ec.a, &ctx->ec.p);
}

