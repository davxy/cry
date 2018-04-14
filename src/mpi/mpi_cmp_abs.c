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
 * Compare two big numbers absolute values.
 */
int cry_mpi_cmp_abs(const cry_mpi *a, const cry_mpi *b)
{
    size_t i;
    cry_mpi_digit *ap, *bp;

    if (a->used < b->used)
        return -1;
    else if (a->used > b->used)
        return 1;

    i = a->used;
    ap = &a->data[i - 1];
    bp = &b->data[i - 1];
    while (i-- > 0) {
       if (*ap < *bp)
           return -1;
       if (*ap > *bp)
           return 1;
       ap--;
       bp--;
    }
    return 0;
}

