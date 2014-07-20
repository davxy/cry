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

#include "cry_mpi_pvt.h"
#include <stdlib.h>
#include <stdio.h>


/* Digits allocation quantum */
#define MPI_PREC    8

int cry_mpi_grow(cry_mpi *a, unsigned int digs)
{
    cry_mpi_digit *tmp;

    if (digs <= a->alloc)
        return 0;

    /* ensure there are always at least MPI_PREC digits extra on top */
    digs += (MPI_PREC * 2) - (digs % MPI_PREC);
    /* reallocate the array */
    tmp = (cry_mpi_digit *) realloc(a->data, sizeof(cry_mpi_digit) * digs);
    if (tmp == NULL)
        return -1;
    a->data = tmp;

    /* zero excess digits */
    a->alloc = digs;

    return 0;
}

int cry_mpi_init(cry_mpi *a)
{
    /* allocate required memory and clear it */
    a->data = (cry_mpi_digit *) malloc(sizeof(cry_mpi_digit) * MPI_PREC);
    if (a->data == NULL)
        return -1;

    /* finalize the initialization */
    a->used = 0;
    a->alloc = MPI_PREC;
    a->sign = 0;
    return 0;
}

/*
 * Init an cry_mpi for a given size
 */
int cry_mpi_init_size(cry_mpi *a, unsigned int size)
{
    a->data = malloc(sizeof(cry_mpi_digit) * size);
    if (a->data == NULL)
        return -1;

    a->used = 0;
    a->alloc = size;
    a->sign = 0;
    return 0;
}

/*
 * Initialize a big number and assign it an integer value.
 */
int cry_mpi_init_int(cry_mpi *a, long i)
{
    int res;

    if ((res = cry_mpi_init(a)) != 0)
        return res;
    cry_mpi_set_int(a, i);
    return 0;
}

void cry_mpi_set_int(cry_mpi *a, long i)
{
    if (i < 0) {
        a->sign = 1;
        i = -i;
    } else {
        a->sign = 0;
    }

    while (i != 0) {
        a->data[a->used++] = (unsigned char) i;
        i >>= 8;
    }
}

void cry_mpi_clear(cry_mpi *a)
{
    if (a->data != NULL)
        free(a->data);
    /* reset the members to make debugging easier */
    a->data = NULL;
    a->alloc = 0;
    a->used = 0;
    a->sign = 0;
}

int cry_mpi_copy(cry_mpi *d, const cry_mpi *s)
{
    int res;
    unsigned int i;

    /* if dst == src do nothing */
    if (d == s)
        return 0;

    /* grow dest */
    if (d->alloc < s->used) {
        res = cry_mpi_grow(d, s->used);
        if (res != 0)
            return res;
    }

    /* zero dest and copy the parameters over */
    for (i = 0; i < s->used; i++)
        d->data[i] = s->data[i];
    d->used = s->used;
    d->sign = s->sign;
    return 0;
}

int cry_mpi_init_copy(cry_mpi *d, const cry_mpi *s)
{
    int res;

    if ((res = cry_mpi_init(d)) != 0)
        return res;
    if ((res = cry_mpi_copy(d, s)) != 0)
        cry_mpi_clear(d);
    return res;
}

void cry_mpi_print(const cry_mpi *a)
{
    int i;

    if (a->used == 0) {
        printf("00\n");
        return;
    }    

    i = a->used;
    while (i-- > 0)
        printf("%02x", a->data[i]);
    printf("\n");
}

