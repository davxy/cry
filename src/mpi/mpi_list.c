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
#include <stdarg.h>

int cry_mpi_init_list(cry_mpi *mp, ...)
{
    int res = 0, n = 0;
    cry_mpi *curr = mp;
    va_list args;

    va_start(args, mp);
    while (curr != NULL) {
        if (cry_mpi_init(curr) == 0) {
            n++;
            curr = va_arg(args, cry_mpi *);
        } else {
            /* on error undo all the initialized variables */
            va_end(args); /* end the current list */
            curr = mp;
            va_start(args, mp);
            while (n--) {
                cry_mpi_clear(curr);
                curr = va_arg(args, cry_mpi *);
            }
            res = -1;
            break;
        }
    }
    va_end(args);
    return res;
}

void cry_mpi_clear_list(cry_mpi *mp, ...)
{
    cry_mpi *curr = mp;
    va_list args;

    va_start(args, mp);
    while (curr != NULL) {
        cry_mpi_clear(curr);
        curr = va_arg(args, cry_mpi *);
    }
    va_end(args);
}

