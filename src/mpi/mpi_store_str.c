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

#define RAW_TO_ASC_CHAR(c) \
        (((c) < 10) ? ('0' + c) : ('a' + ((c) - 10)))

static int cry_mpi_store_str_hex(const cry_mpi *a, char *s, size_t size)
{
    size_t i, j, k = 0;
    unsigned char c;
    int incomplete = 0, started = 0;
    cry_mpi_digit dig;

    if (k == size - 1) {
        s[k] = 0;
        return -1;
    }

    if (a->sign)
        s[k++] = '-';

    for (i = a->used; i > 0; i--) {
        dig = a->data[i - 1];
        /* store one nibble at a time */
        for (j = 0; j < 2 * CRY_MPI_DIGIT_BYTES; j++) {
            if (k == size - 1) {
                incomplete = 1;
                goto e;
            }
            /* get high nibble */
            c = (unsigned char)(dig >> (CRY_MPI_DIGIT_BITS - 4));
            if (c != 0 || started) {
                s[k++] = RAW_TO_ASC_CHAR(c);
                started = 1;
            }
            dig <<= 4;
        }
    }
e:  if (incomplete)
        s[k - 1] = '*';
    s[k] = '\0';
    return 0;
}

int cry_mpi_store_str(const cry_mpi *a, unsigned int radix, char *s,
                      size_t size)
{
    int ret;
    cry_mpi tmp, rad, rem;
    unsigned int i = 0, j, d;

    if (radix < 2 || radix > 16 || size == 0)
        return -1;

    if (a->used == 0) {
        s[i++] = '0';
        return 0;
    }

    if (radix == 16)
        return cry_mpi_store_str_hex(a, s, size);

    if ((ret = cry_mpi_init_list(&tmp, &rad, &rem, 0)) != 0) {
        return ret;
    }

    if ((ret = cry_mpi_copy(&tmp, a)) != 0)
        return ret;
    cry_mpi_set_int(&rad, radix);

    while (!cry_mpi_is_zero(&tmp) && i < size) {
        if ((ret = cry_mpi_div_abs(&tmp, &rem, &tmp, &rad)) != 0)
            return ret;
        d = rem.used ? rem.data[0] : 0;
        s[i++] = RAW_TO_ASC_CHAR(d);
    }
    if (a->sign)
        s[i++] = '-';
    s[i] = '\0';

    /* string invert */
    i--;
    j = 0;
    while (j < i) {
        s[j] ^= s[i];
        s[i] ^= s[j];
        s[j] ^= s[i];
        j++;
        i--;
    }
}

