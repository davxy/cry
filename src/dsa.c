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

#include "cry/dsa.h"
#include "mpi/mpi_pvt.h"

#define CHK(exp) do { if ((res = (exp)) != 0) goto e; } while (0)

/*
 * c = rand()
 * k = (c mod (q-1)) + 1
 */
static int secret_gen(cry_mpi *k, const cry_mpi *q)
{
    int res;
    cry_mpi t, one;
    cry_mpi_digit dig = 1;

    one.sign = 0;
    one.used = 1;
    one.alloc = 1;
    one.data = &dig;

    if ((res = cry_mpi_init(&t)) != 0)
        return res;

    CHK(cry_mpi_copy(&t, q));
    CHK(cry_mpi_sub(&t, &t, &one));
    CHK(cry_mpi_rand(k, cry_mpi_count_bits(q)));
    CHK(cry_mpi_mod(k, k, &t));
    CHK(cry_mpi_add(k, k, &one));
e:  cry_mpi_clear(&t);
    return res;
}

int cry_dsa_sign(cry_dsa_ctx *ctx, cry_dsa_signature *sign,
                 const unsigned char *in, size_t len)
{
    int res;
    cry_mpi k, z;

    if ((res = cry_mpi_init_list(&k, &z, NULL)) != 0)
        return res;

    /* k = c mod (q-1) + 1 */
    CHK(secret_gen(&k, &ctx->q));

    /* r = (g^k mod p) mod q */
    CHK(cry_mpi_mod_exp(&sign->r, &ctx->g, &k, &ctx->p));
    CHK(cry_mpi_mod(&sign->r, &sign->r, &ctx->q));

    /* z = buf truncated to the size of q */
    /* TODO: double check... the book do strange stuff here */
    if (ctx->q.used * CRY_MPI_DIGIT_BYTES < len)
        len = ctx->q.used * CRY_MPI_DIGIT_BYTES;
    CHK(cry_mpi_load_bin(&z, in, len));

    /* s = (inv(k) * (z + xr)) mod q */
    CHK(cry_mpi_inv(&k, &k, &ctx->q));
    CHK(cry_mpi_copy(&sign->s, &ctx->pvt));
    CHK(cry_mpi_mul(&sign->s, &sign->s, &sign->r));
    CHK(cry_mpi_add(&sign->s, &sign->s, &z));
    CHK(cry_mpi_mul(&sign->s, &sign->s, &k));
    CHK(cry_mpi_mod(&sign->s, &sign->s, &ctx->q));

e:  cry_mpi_clear_list(&k, &z, NULL);
    return res;
}

int cry_dsa_verify(cry_dsa_ctx *ctx, const cry_dsa_signature *sign,
                   const unsigned char *in, size_t len)
{
    int res;
    cry_mpi z, w, u1, u2;

    if ((res = cry_mpi_init_list(&z, &w, &u1, &u2, NULL)) != 0)
        return res;

    /* w = inv(s) mod q */
    CHK(cry_mpi_copy(&w, &sign->s));
    CHK(cry_mpi_inv(&w, &w, &ctx->q));

    /* z = buf truncated to the size of q */
    if (ctx->q.used * CRY_MPI_DIGIT_BYTES < len)
        len = ctx->q.used * CRY_MPI_DIGIT_BYTES;
    CHK(cry_mpi_load_bin(&z, in, len));

    /* u1 = (z * w) mod q */
    CHK(cry_mpi_mul(&z, &z, &w));
    CHK(cry_mpi_mod(&z, &z, &ctx->q));

    /* u2 = (r * w) mod q */
    CHK(cry_mpi_mul(&w, &sign->r, &w));
    CHK(cry_mpi_mod(&w, &w, &ctx->q));

    /* v = (((g^u1) mod p * (y^u2) mod p) mod p) mod q */
    CHK(cry_mpi_mod_exp(&u1, &ctx->g, &z, &ctx->p));
    CHK(cry_mpi_mod_exp(&u2, &ctx->pub, &w, &ctx->p));
    CHK(cry_mpi_mul(&u1, &u1, &u2));
    CHK(cry_mpi_mod(&u1, &u1, &ctx->p));
    CHK(cry_mpi_mod(&u1, &u1, &ctx->q));

    /* Check to see if v and sig match */
    res = (cry_mpi_cmp_abs(&u1, &sign->r) == 0) ? 0 : -1;
e:  cry_mpi_clear_list(&z, &w, &u1, &u2, NULL);
    return res;
}

