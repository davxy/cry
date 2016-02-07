/*
 * Copyright (c) 2013-2016, Davide Galassi. All rights reserved.
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
#include "misc.h"

#ifdef CRY_MPI_MUL_COMBA

#define MULADD(i, j) do {                               \
   cry_mpi_dword t;                                     \
   t = (cry_mpi_dword)c0 + ((cry_mpi_dword)(i)) * ((cry_mpi_dword)(j)); \
   c0 = t;                                              \
   t = (cry_mpi_dword)c1 + (t >> CRY_MPI_DIGIT_BITS);   \
   c1 = t;                                              \
   c2 += t >> CRY_MPI_DIGIT_BITS;                       \
   } while (0);


int cry_mpi_mul_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int res, ix, iy, iz, tx, ty, pa;
    cry_mpi_digit c0, c1, c2, *tmpx, *tmpy;
    cry_mpi tmp, *dst;
    size_t digs = a->used + b->used;

    c0 = c1 = c2 = 0;
    pa = CRY_MIN(digs, a->used + b->used);

    if (r == a || r == b) {
        if ((res = cry_mpi_init_size(&tmp, pa)) != 0)
            return res;
        dst = &tmp;
    } else {
        cry_mpi_grow(r, pa);
        dst = r;
    }
    cry_mpi_set_used(dst, pa);

    for (ix = 0; ix < pa; ix++) {

        /* get offsets into the two bignums */
        ty = CRY_MIN(ix, b->used - 1);
        tx = ix - ty;

        /* setup temporary aliases */
        tmpx = a->data + tx;
        tmpy = b->data + ty;

        /*
         * this is the number of times the loop will iterate, essentially its
         * while (tx++ < a->used && ty-- >= 0
         */
        iy = CRY_MIN(a->used - tx, ty + 1);

        /* shift accumulator right */
        c0 = c1;
        c1 = c2;
        c2 = 0;

        /* execute loop */
        for (iz = 0; iz < iy; iz++) {
            MULADD(*tmpx, *tmpy);
            tmpx++;
            tmpy--;
        }

        /* store term */
        dst->data[ix] = c0;
    }

    if (r != dst) {
        cry_mpi_swap(r, dst);
        cry_mpi_clear(dst);
    }
    cry_mpi_adjust(r);
    return 0;
}

#else /* !CRY_MPI_MUL_COMBA */

int cry_mpi_mul_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    cry_mpi t;
    int res, pa, pb, i, j;
    cry_mpi_digit u, tmpx, *tmpt, *tmpy;
    cry_mpi_dword dw;
    size_t digs = a->used + b->used;

    if ((res = cry_mpi_init_size(&t, digs)) != 0)
        return res;
    cry_mpi_set_used(&t, digs);

    /* compute the digits of the product directly */
    pa = a->used;
    /* iterate through every digit in the first operand */
    for (i = 0; i < pa; i++) {
        /* limit ourselves to making digs digits of output */
        pb = CRY_MIN(b->used, digs - i);
        if (pb < 0)
            break;
        /* copy of the digit to be used within the nested loop */
        tmpx = a->data[i];
        /* an alias for the destination shifted i places */
        tmpt = t.data + i;
        /* an alias for the digits of b */
        tmpy = b->data;

        /* set initial carry to zero */
        u = 0;
        /* compute the columns of the output and propagate the carry */
        for (j = 0; j < pb; j++) {
            /* compute the column as a cry_mpi_dword */
            dw = (cry_mpi_dword)*tmpt +
                 (cry_mpi_dword)tmpx * (cry_mpi_dword)(*tmpy++) +
                 (cry_mpi_dword)u;
            /* the new column is the lower part of the result */
            *tmpt++ = (cry_mpi_digit)(dw & (cry_mpi_digit)-1);
            /* get the carry digit from the result */
            u = (cry_mpi_digit)(dw >> CRY_MPI_DIGIT_BITS);
        }
        /* set the carry if it fits within the required digs */
        if (i + j < digs)
            *tmpt = u;
    }
    cry_mpi_adjust(&t);
    cry_mpi_swap(&t, r);
    cry_mpi_clear(&t);
    return 0;
}

#endif /* CRY_MPI_MUL_COMBA */
