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

int cry_mpi_load_str(cry_mpi *a, const char *s)
{
    int ret = 0, sign;
    unsigned long l, radix;
    cry_mpi tmp, base;

    cry_mpi_zero(a);

    /* Sign (optional) */
    if (*s == '-') {
        sign = 1;
        s++;
    } else {
        sign = 0;
        if (*s == '+')
            s++;
    }

    /* Radix (optional) */
    if (*s == '0' && *(s+1) == 'x') {
        radix = 16;
        s += 2;
    } else if (*s == '0' && *(s+1) == 'b') {
        radix = 2;
        s += 2;
    } else if (*s == '0') {
        radix = 8;
        s++;
    } else {
        radix = 10;
    }

    if (cry_mpi_init_int(&base, (long)radix) < 0)
        return -1;
    if (cry_mpi_init_int(&tmp, 1) < 0) {
        cry_mpi_clear(&base);
        return -1;
    }

    /* Skip leading zeros */
    while (*s == '0')
        s++;

    while ((l = *s) != '\0') {
        if ('0' <= l && l <= '9')
            l -= '0';
        else if ('A' <= l && l <= 'Z')
            l = l - 'A' + 10;
        else if ('a' <= l && l <= 'z')
            l = l - 'a' + 10;
        else
            l = (unsigned long)-1; /* force check failure */
        if (radix <= l)
            break;

        tmp.data[0] = l;
        if (cry_mpi_mul(a, a, &base) < 0)
            break;
        if (cry_mpi_add(a, a, &tmp) < 0)
            break;
        s++; /* In the end, just if no errors are detected */
    }
    if (*s != '\0') { /* If all the string has not been fed */
        cry_mpi_clear(a);
        ret = -1;
    }
    a->sign = sign;
    cry_mpi_clear(&base);
    cry_mpi_clear(&tmp);
    return ret;
}

/*
 * Initialize a big number from a null terminated ASCII string.
 */
int cry_mpi_init_str(cry_mpi *a, const char *s)
{
    int res;

    res = cry_mpi_init(a);
    if (res == 0) {
        res = cry_mpi_load_str(a, s);
        if (res != 0)
            cry_mpi_clear(a);
    }
    return res;
}

