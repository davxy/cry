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
 * Compare two big numbers values.
 */
int cry_mpi_cmp(const cry_mpi *a, const cry_mpi *b)
{
    int ret;

    if (a->sign > b->sign)
        ret = -1;  /* a is negative and b is positive */
    else if (a->sign < b->sign)
        ret = 1;   /* a is positive and b is negative */
    else {
        ret = cry_mpi_cmp_abs(a, b);
        if (a->sign == 1)
            ret = -ret; /* negative numbers */
    }
    return ret;
}

