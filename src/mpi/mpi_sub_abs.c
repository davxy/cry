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

int cry_mpi_sub_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    unsigned int min, max, i;
    cry_mpi_digit c, t1, t2, *rp, *ap, *bp;

    if (cry_mpi_cmp_abs(a, b) < 0)
        return -1; /* negative results are not allowed */

    max = a->used;
    min = b->used;

    if (r->alloc < max) {
        if (cry_mpi_grow(r, max) != 0)
            return -1;
    }
    r->used = max;
    r->sign = 0;

    ap = a->data;
    bp = b->data;
    rp = r->data;
    c = 0;
    for (i = 0; i < min; i++) {
        t1 = *ap++;
        t2 = *bp++;
        *rp++ = (t1 - t2 - c);
        if (t1 != t2) /* if are equal the carry is unchanged */
            c = (t1 < t2);
    }

    /*
     * If there is a borrow, decrease blocks until one does not
     * reverse rollover
     */
    for ( ; i < max && c; i++) {
        c = (*ap == 0);
        *rp++ = *ap++ - 1;
    }
    if (c)
        return -1; /* negative result not allowed */
    for ( ; i < max; i++)
        *rp++ = *ap++;
    cry_mpi_adjust(r); /* Adjust used counter */
    return 0;
}

