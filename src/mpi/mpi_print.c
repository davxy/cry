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

#include "cry/mpi.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Print utility function
 */
static void cry_mpi_print_slow(const cry_mpi *x, unsigned int radix)
{
    cry_mpi tmp, rad, rem;
    unsigned int j, i = 0, strsiz = 0;
    char *str = NULL;

    if (radix < 2 || radix > 16) {
        printf("Error: cry_mpi_print: radix must be between 2 and 16\n");
        return;
    }

    if (x->used == 0) {
        printf("0");
        return;
    }

    if (cry_mpi_init_list(&tmp, &rad, &rem, 0) != 0) {
        printf("cry_mpi_init_list error\n");
        return;
    }

    if (cry_mpi_copy(&tmp, x) < 0)
        printf("Error: cry_mpi_print\n");
    cry_mpi_set_int(&rad, radix);

    while (!cry_mpi_is_zero(&tmp)) {
        if (cry_mpi_div_abs(&tmp, &rem, &tmp, &rad) != 0)
            printf("Error: cry_mpi_print\n");
        if (i == strsiz) {
            strsiz += BUFSIZ;
            str = realloc(str, strsiz);
            if (str == NULL) {
                printf("Error: cry_mpi_print: reallocating string buffer\n");
                return;
            }
        }
        j = rem.data ? rem.data[0] : 0;
        if (j < 10)
            str[i] = '0' + j;
        else
            str[i] = 'A' + j - 10;
        i++;
    }
    if (x->sign)
        printf("-");
    strsiz = i;
    i--;
    j = 0;
    while (j < i) {
        str[j] ^= str[i];
        str[i] ^= str[j];
        str[j] ^= str[i];
        j++;
        i--;
    }
    printf("%.*s\n", strsiz, str);
    if (str)
        free(str);
}

void cry_mpi_print(const cry_mpi *a, unsigned int radix)
{
    int i;

    if (radix != 16) {
        cry_mpi_print_slow(a, radix);
        return;
    }

    /* Quick and dirty */

    if (a->used == 0) {
        printf("00\n");
        return;
    }    

    i = a->used;
    if (a->sign)
        printf("-");
    printf("0x");
    while (i-- > 0)
        printf("%02x", a->data[i]);
    printf("\n");
}

