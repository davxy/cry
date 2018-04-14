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


#include "mpi_pvt.h"

/*
 * Modular exponentiation
 * TODO: start from MSB and double the result in place of b.
 * This saves a variable.
 */
int cry_mpi_mod_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e,
                    const cry_mpi *m)
{
    int i, j, res = 0;
    cry_mpi exp, tmp;

    if ((res = cry_mpi_init_int(&exp, 1)) != 0)
        return res;
    if ((res = cry_mpi_init_copy(&tmp, b)) != 0) {
        cry_mpi_clear(&exp);
        return res;
    }

    /* For each block */
    for (i = 0; i < e->used; i++) {
        for (j = 0; j < CRY_MPI_DIGIT_BITS; j++) {
            if (i == e->used - 1 && /* If is the last exponent block */
                (e->data[i] & ~((1UL << j) - 1)) == 0)
                break;
            if (e->data[i] & (1UL << j)) {
                if ((res = cry_mpi_mul(&exp, &exp, &tmp)) != 0 ||
                    (m && (res = cry_mpi_mod(&exp, &exp, m)) != 0))
                    goto e;
            }
            if ((res = cry_mpi_sqr(&tmp, &tmp)) != 0 ||
                (m && (res = cry_mpi_mod(&tmp, &tmp, m)) != 0))
                goto e;
        }
    }
    cry_mpi_swap(r, &exp);
e:  cry_mpi_clear_list(&tmp, &exp, NULL);
    return res;
}

