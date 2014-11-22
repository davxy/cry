/*
 * Copyright (c) 2013-2014, Davide Galassi. All rights reserved.
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

int cry_mpi_div_abs(cry_mpi *q, cry_mpi *r, const cry_mpi *a,
                    const cry_mpi *b)
{
    int res;
    cry_mpi tq, tr, one;

    if ((res = cry_mpi_init_list(&tq, &tr, &one, NULL)) < 0)
        return res;

    one.data[0] = 1;
    one.used = 1;
    one.sign = 0;

    cry_mpi_copy(&tr, a);
    while (cry_mpi_cmp_abs(b, &tr) <= 0) {
        if ((res = cry_mpi_sub_abs(&tr, &tr, b)) != 0 ||
            (res = cry_mpi_add_abs(&tq, &tq, &one)) != 0)
            goto e;
    }

    if (q)
        cry_mpi_swap(&tq, q);
    if (r)
        cry_mpi_swap(&tr, r);
e:  cry_mpi_clear_list(&tq, &tr, &one, NULL);
    return res;
}

