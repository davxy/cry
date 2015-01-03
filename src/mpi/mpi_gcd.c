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

#include "mpi_pvt.h"

/*
 * The number of less significant zero-bits.
 * Used by the binary gcd algorithm.
 */
static unsigned int cry_mpi_lsb(struct cry_mpi *x)
{
    int i, j, count = 0;

    for (i = 0; i < x->used; i++) {
        for (j = 0; j < CRY_MPI_DIGIT_BITS; j++, count++)
            if (((x->data[i] >> j) & 1) != 0)
                return count;
    }
    return 0;
}

/*
 * Greatest common divisor.
 * Implemented using the binary (Stein's) algorithm)
 */
int cry_mpi_gcd(cry_mpi *r, const cry_mpi *x, const cry_mpi *y)
{
    cry_mpi tx, ty;
    int res, lz, lzt;

    if ((res = cry_mpi_init_list(&tx, &ty, 0)) != 0)
        return res;

    if ((res = cry_mpi_copy(&tx, x)) != 0 ||
        (res = cry_mpi_copy(&ty, y)) != 0)
        goto e;

    lz  = cry_mpi_lsb(&tx);
    lzt = cry_mpi_lsb(&ty);
    if (lzt < lz)
        lz = lzt;

    if ((res = cry_mpi_shr(&tx, &tx, lz)) != 0 ||
        (res = cry_mpi_shr(&ty, &ty, lz)) != 0)
        goto e;

    while (!cry_mpi_is_zero(&tx)) {
        if ((res = cry_mpi_shr(&tx, &tx, cry_mpi_lsb(&tx))) != 0 ||
            (res = cry_mpi_shr(&ty, &ty, cry_mpi_lsb(&ty))) != 0)
            break;

        if (cry_mpi_cmp_abs(&tx, &ty) >= 0) {
            if ((res = cry_mpi_sub_abs(&tx, &tx, &ty)) != 0 ||
                (res = cry_mpi_shr(&tx, &tx, 1)) != 0)
                break;
        } else {
            if ((res = cry_mpi_sub_abs(&ty, &ty, &tx)) != 0 ||
                (res = cry_mpi_shr(&ty, &ty, 1)) != 0)
                break;
        }
    }
    res = cry_mpi_shl(&ty, &ty, lz);
    cry_mpi_swap(r, &ty);
e:  cry_mpi_clear_list(&tx, &ty, 0);
    return res;
}

