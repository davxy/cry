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

#include "cry_mpi.h"

int cry_mpi_grow(cry_mpi *a, unsigned int digs);

int cry_mpi_add(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    unsigned int min, max, i;
    cry_mpi_digit t, l, c, *rp, *ap, *bp;

    if (a->used < b->used) {
        const cry_mpi *t = a;
        a = b;
        b = t;
    }
    max = a->used;
    min = b->used;

    if (r->alloc < (max + 1)) {
        if (cry_mpi_grow(r, max + 1) != 0)
            return -1;
    }
    r->used = max;

    ap = a->data;
    bp = b->data;
    rp = r->data;

    c = 0;
    for (i = 0; i < min; i++) {
        t = (*ap++ + c);
        c = (t < c);  /* check for wrap, on overflow t is 0 */
        l = (t + *bp++);
        c += (l < t); /* check for wrap, if t is 0 then l >= t  */
        *rp++ = l;
    }

    for ( ; i < max && c; i++) {
        *rp = *ap++ + 1;
        c = (*rp++ == 0);
    }
    if (c) {
        *rp = 1;
        r->used++;
    } else {
        for ( ; i < max; i++)
            *rp++ = *ap++;
    }
    return 0;
}

