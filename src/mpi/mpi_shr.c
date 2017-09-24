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

void cry_mpi_shrd(cry_mpi *a, int n)
{
    int x;

    if (n <= 0)
        return;

    if (a->used <= n) {
        cry_mpi_zero(a);
        return;
    }

    {
        cry_mpi_digit *bottom, *top;

        bottom = a->data;
        top = a->data + n;
        for (x = 0; x < (a->used - n); x++)
            *bottom++ = *top++;

        /* zero the top digits */
        for ( ; x < a->used; x++)
            *bottom++ = 0;
    }
    /* remove excess digits */
    a->used -= n;
}

/*
 * Store quotient in c, optional remainder in d
 */
int cry_mpi_shr(cry_mpi *c, const cry_mpi *a, int n)
{
    cry_mpi_digit D, r, rr;
    int x, res;

    /* if the shift count is <= 0 then we do not work */
    if (n <= 0) {
        res = cry_mpi_copy(c, a);
        return res;
    }

    /* copy */
    if ((res = cry_mpi_copy(c, a)) != 0)
        return res;
    /* 'a' should not be used after here - it might be the same as 'd' */

    /* shift by as many digits in the bit count */
    if (n >= CRY_MPI_DIGIT_BITS)
        cry_mpi_shrd(c, n / CRY_MPI_DIGIT_BITS);

    /* shift any bit count < CRY_MPI_DIGIT_BITS */
    D = n % CRY_MPI_DIGIT_BITS;
    if (D) {
        cry_mpi_digit *tmpc, mask, shift;

        mask = (((cry_mpi_digit)1) << D) - 1;
        shift = CRY_MPI_DIGIT_BITS - D;
        tmpc = c->data + (c->used - 1);
        r = 0;  /* carry */
        for (x = c->used - 1; x >= 0; x--) {
            /* get the lower bits of this word in a temp */
            rr = *tmpc & mask;
            /* shift the current word and mix in the carry bits from the prev */
            *tmpc = (*tmpc >> D) | (r << shift);
            --tmpc;
            /* set carry to the carry bits of the current word found above */
            r = rr;
        }
    }
    cry_mpi_adjust(c);
    return 0;
}

