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

int cry_mpi_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e)
{
    int res;
    int sign = b->sign ? cry_mpi_is_odd(e) : 0;
    cry_mpi t, c, one;

    if ((res = cry_mpi_init_list(&t, &one, &c, NULL)) != 0)
        return res;
    
    if ((res = cry_mpi_copy(&c, e)) != 0)
        goto e;

    one.data[0] = 1;
    one.used = 1;
    one.sign = 0;
    t.data[0] = 1;
    t.used = 1;
    t.sign = 0;
    while (c.used != 0) {
        if ((res = cry_mpi_mul_abs(&t, &t, b)) != 0)
            break;
        if ((res = cry_mpi_sub_abs(&c, &c, &one)) != 0)
            break;
    }

    cry_mpi_swap(&t, r);
    r->sign = sign;
e:  cry_mpi_clear_list(&t, &one, &c, NULL);
    return 0;
}

