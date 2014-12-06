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

#include "mpi_pvt.h"

/*
 * Initialize a big number from a null terminated ASCII string.
 */
int cry_mpi_init_str(cry_mpi *a, unsigned int radix, const char *s)
{
    int res;

    res = cry_mpi_init(a);
    if (res == 0) {
        res = cry_mpi_load_str(a, radix, s);
        if (res != 0)
            cry_mpi_clear(a);
    }
    return res;
}

