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
 * Least Common Multiple (lcm = x*y/gcd)
 */
int cry_mpi_lcm(cry_mpi *r, const cry_mpi *x, const cry_mpi *y)
{
    int res;
    cry_mpi gcm, prod;

    if ((res = cry_mpi_init_list(&gcm, &prod, 0)) != 0)
        return res;

    if ((res = cry_mpi_gcd(&gcm, x, y)) == 0)
        if ((res = cry_mpi_mul(&prod, x, y)) == 0)
            res = cry_mpi_div(r, 0, &prod, &gcm);
    cry_mpi_clear_list(&gcm, &prod, 0);
    return res;
}

