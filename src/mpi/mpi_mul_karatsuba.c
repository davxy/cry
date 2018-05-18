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
#include "misc.h"


int cry_mpi_mul_karatsuba(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int B, hB, res;
    cry_mpi x0, x1, y0, y1, z0, z1, z2;

    /* minimum number of digits */
    B = CRY_MIN(a->used, b->used);
    /* divide by two */
    hB = B >> 1;
    B = hB << 1;

    /* init copy all the temporaries */
    if ((res = cry_mpi_init_size(&x0, hB)) < 0)
        goto e0;
    if ((res = cry_mpi_init_size(&x1, a->used - hB)) < 0)
        goto e1;
    if ((res = cry_mpi_init_size(&y0, hB)) < 0)
        goto e2;
    if ((res = cry_mpi_init_size(&y1, b->used - hB)) < 0)
        goto e3;

    if ((res = cry_mpi_init_size(&z0, B)) < 0)
        goto e4;
    if ((res = cry_mpi_init_size(&z1, B)) < 0)
        goto e5;
    if ((res = cry_mpi_init_size(&z2, B)) < 0)
        goto e6;

    /* Shift the digits */
    x0.used = y0.used = hB;
    x1.used = a->used - hB;
    y1.used = b->used - hB;

    {
        register int x;
        register cry_mpi_digit *tmpa, *tmpb, *tmpx, *tmpy;

        /*
         * We copy the digits directly instead of using higher level functions
         * since we also need to shift digits.
         */
        tmpa = a->data;
        tmpb = b->data;

        tmpx = x0.data;
        tmpy = y0.data;

        /* TODO: use memcpy? */
        for (x = 0; x < hB; x++) {
            *tmpx++ = *tmpa++;
            *tmpy++ = *tmpb++;
        }

        tmpx = x1.data;
        for (x = hB; x < a->used; x++)
            *tmpx++ = *tmpa++;

        tmpy = y1.data;
        for (x = hB; x < b->used; x++)
            *tmpy++ = *tmpb++;

    }

    /*
     * Only need to clamp the lower words since by definition the upper
     * words x1/y1 must have a known number of digits
     */
    cry_mpi_adjust(&x0);
    cry_mpi_adjust(&y0);

    if ((res = cry_mpi_mul_abs(&z0, &x0, &y0)) < 0) /* z0 = x0*y0 */
        goto e7;
    if ((res = cry_mpi_mul_abs(&z2, &x1, &y1)) < 0) /* z2 = x1*y1 */
        goto e7;
    /* Use x0 for temporary storage */
    if ((res = cry_mpi_add(&z1, &x1, &x0)) < 0) /* z1 = x1+x0 */
        goto e7;
    if ((res = cry_mpi_add(&x0, &y1, &y0)) < 0) /* x0 = y1+y0 */
        goto e7;
    if ((res = cry_mpi_mul_abs(&z1, &z1, &x0)) < 0) /* z1 = (x1+x0)(y1+y0) */
        goto e7;
    if ((res = cry_mpi_add(&x0, &z0, &z2)) < 0) /* x0 = z0+z2 */
        goto e7;
    if ((res = cry_mpi_sub(&z1, &z1, &x0)) < 0) /* z1=(x1+x0)*(y1+y0)-(z0+z2) */
        goto e7;

    if ((res = cry_mpi_shld(&z1, hB)) < 0)
        goto e7;
    if ((res = cry_mpi_shld(&z2, B)) < 0)
        goto e7;

    if ((res = cry_mpi_add(&z1, &z0, &z1)) < 0)
        goto e7;
    if ((res = cry_mpi_add(r, &z1, &z2)) < 0)   /* r = z2<<B + z1<<hB + z0 */
        goto e7;

e7: cry_mpi_clear(&z2);
e6: cry_mpi_clear(&z1);
e5: cry_mpi_clear(&z0);
e4: cry_mpi_clear(&y1);
e3: cry_mpi_clear(&y0);
e2: cry_mpi_clear(&x1);
e1: cry_mpi_clear(&x0);
e0: return res;
}
