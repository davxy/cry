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

#include "cry/ecp.h"
#include "mpi/mpi_pvt.h" /* CRY_MPI_DIGIT_BITS */

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_mpi *a, const cry_mpi *p)
{
    int i, j, res, paf = 1;
    struct cry_ecp dp, r;

    if ((res = cry_mpi_init_list(&dp.x, &dp.y, &r.x, &r.y, 0)) != 0)
        return res;

    if ((res = cry_mpi_copy(&dp.x, &p1->x)) != 0)
        goto e;
    if ((res = cry_mpi_copy(&dp.y, &p1->y)) != 0)
        goto e;

    for (i = 0; i < k->used; i++) {
        for (j = 0; j < CRY_MPI_DIGIT_BITS; j++) {
            if (k->data[i] & (1UL << j)) {
                if (!paf) {
                    if ((res = cry_ecp_add(&r, &r, &dp, p)) != 0)
                        goto e;
                } else {
                    paf = 0; /* First addition */
                    if ((res = cry_mpi_copy(&r.x, &dp.x)) != 0)
                        goto e;
                    if ((res = cry_mpi_copy(&r.y, &dp.y)) != 0)
                        goto e;
                }
            }
            if ((res = cry_ecp_dbl(&dp, &dp, a, p)) != 0)
                goto e;
        }
    }

    cry_ecp_swap(pr, &r);
e:  cry_mpi_clear_list(&dp.x, &dp.y, &r.x, &r.y, 0);
    return res;
}

