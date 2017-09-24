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

#include "mpi_pvt.h"

/*
 * Signed addition
 */
int cry_mpi_sub(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int ret, rsign;

    if (a->sign != b->sign) {
        rsign = a->sign;
        ret = cry_mpi_add_abs(r, a, b);
    } else {
        switch (cry_mpi_cmp_abs(a, b)) {
            case 1:  /* a > b */
                rsign = a->sign;
                ret = cry_mpi_sub_abs(r, a, b);
                break;
            case -1: /* a < b */
                rsign = 1 - b->sign;
                ret = cry_mpi_sub_abs(r, b, a);
                break;
            default:
                rsign = 0;
                cry_mpi_zero(r);
                ret = 0;
                break;
        }
    }
    if (ret == 0)
        r->sign = rsign;
    return ret;
}

