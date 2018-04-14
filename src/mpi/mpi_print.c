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

#include "cry/mpi.h"
#include <stdio.h>
#include <stdlib.h>

void cry_mpi_print(const cry_mpi *a, unsigned int radix)
{
    char *s;
    size_t n = BUFSIZ;
    char fallback[16];
    int ret;

    if ((s = malloc(BUFSIZ)) == NULL) {
        s = fallback;
        n = sizeof(fallback);
    }

    if ((ret = cry_mpi_store_str(a, radix, s, n)) == 0)
        printf("%s\n", s);
    else
        printf("cry_mpi_print unexpected error (%d)\n", ret);

    if (s != fallback)
        free(s);
}

