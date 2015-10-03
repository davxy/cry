/*
 * Copyright (c) 2013-2015, Davide Galassi. All rights reserved.
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
 * Modular inverse
 *
 * Classic extended Euclidean algorithm.
 */
int cry_mpi_inv(cry_mpi *r, const cry_mpi *a, const cry_mpi *m)
{
    int ret = 0;
    cry_mpi r0, r1, s0, s1, q;

    if ((ret = cry_mpi_init_list(&r0, &r1, &s0, &s1, &q, 0)) != 0)
        return ret;

    cry_mpi_copy(&r0, a);
    cry_mpi_copy(&r1, m);
    cry_mpi_set_int(&s0, 1);
    cry_mpi_set_int(&s1, 0);

    while (!cry_mpi_is_zero(&r1)) {

        /* r2 = r0 - q1*r1 */
        cry_mpi_div(&q, &r0, &r0, &r1);
        cry_mpi_swap(&r0, &r1);

        /* s2 = s0 - q1*s1 */
        cry_mpi_mul(&q, &q, &s1);
        cry_mpi_sub(&s0, &s0, &q);
        cry_mpi_swap(&s0, &s1);

        cry_mpi_zero(&q);
    }

    /* Check that the GCD is 1, necessary condition */
    if (r0.used == 1 && r0.sign == 0 && r0.data[0] == 1) {
        cry_mpi_swap(r, &s0);
        if (r->sign)
            cry_mpi_mod(r, r, m);
    } else {
        cry_mpi_zero(&s0);
        ret = -1;
    }
    cry_mpi_clear_list(&r0, &r1, &s0, &s1, 0);
    return ret;
}

