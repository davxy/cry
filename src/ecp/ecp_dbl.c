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

#include "cry/ecp.h"

#define CHK(exp) do { if ((res = (exp)) != 0) goto e; } while (0)

int cry_ecp_dbl(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *a,
                const cry_mpi *p)
{
    int res;
    cry_mpi num, den, lam;
    cry_ecp r;

    if ((res = cry_mpi_init_list(&num, &den, &lam, &r.x, &r.y, NULL)) != 0)
        return res;

    CHK(cry_mpi_set_int(&num, 3));        /* num = 3 */
    CHK(cry_mpi_mul(&num, &num, &p1->x)); /* num = 3 * x1 */
    CHK(cry_mpi_mul(&num, &num, &p1->x)); /* num = 3 * x1^2 */
    CHK(cry_mpi_add(&num, &num, a));      /* num = 3 * x1^2 + a */

    CHK(cry_mpi_set_int(&den, 2));        /* den =  2 */
    CHK(cry_mpi_mul(&den, &den, &p1->y)); /* den =  2 * y1 */
    CHK(cry_mpi_inv(&den, &den, p));      /* den = (2 * y1)^(-1) (mod p) */

    CHK(cry_mpi_mul(&lam, &num, &den));   /* lam = num / den */

    CHK(cry_mpi_sqr(&r.x, &lam));         /* x =  lam^2 */
    CHK(cry_mpi_sub(&r.x, &r.x, &p1->x)); /* x =  lam^2 - x1 */
    CHK(cry_mpi_sub(&r.x, &r.x, &p1->x)); /* x =  lam^2 - 2*x1 */
    CHK(cry_mpi_mod(&r.x, &r.x, p));      /* x = (lam^2 - 2*x1) % p */

    CHK(cry_mpi_sub(&r.y, &p1->x, &r.x)); /* y =   x1 - x */
    CHK(cry_mpi_mul(&r.y, &r.y, &lam));   /* y =  (x1 - x) * lam */
    CHK(cry_mpi_sub(&r.y, &r.y, &p1->y)); /* y =  (x1 - x) * lam - y1 */
    CHK(cry_mpi_mod(&r.y, &r.y, p));      /* y = ((x1 - x) * lam - y1) % p */

    cry_ecp_swap(pr, &r);
e:  cry_mpi_clear_list(&num, &den, &lam, &r.x, &r.y, NULL);
    return res;
}

