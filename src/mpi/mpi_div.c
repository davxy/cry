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

int cry_mpi_div(cry_mpi *q, cry_mpi *r, const cry_mpi *a,
                const cry_mpi *b)
{
    int res, sign = a->sign ^ b->sign;

    if ((res = cry_mpi_div_abs(q, r, a, b)) != 0)
        return res;

    if (sign) {
        if (q) {
            cry_mpi one;
            cry_mpi_digit one_dig = 1;

            one.sign = 0;
            one.used = one.alloc = 1;
            one.data = &one_dig;

            q->sign = 1;
            res = cry_mpi_sub(q, q, &one);
        }
        if (r)
            res = cry_mpi_sub_abs(r, b, r);
    }
    return res;
}

