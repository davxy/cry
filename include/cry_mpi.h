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

/**
 * @file    cry_mpi.h
 * @brief   Multiple precision integers.
 */

#ifndef _CRY_MPI_H_
#define _CRY_MPI_H_

#include <stddef.h>

typedef unsigned char cry_mpi_digit;

struct cry_mpi {
    size_t        used;
    size_t        alloc;
    cry_mpi_digit *data;
};

typedef struct cry_mpi cry_mpi;

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Construction
 */

int cry_mpi_init(cry_mpi *a);

void cry_mpi_clear(cry_mpi *a);

int cry_mpi_copy(cry_mpi *d, const cry_mpi *s);

int cry_mpi_init_copy(cry_mpi *d, const cry_mpi *s);

int cry_mpi_init_size(cry_mpi *a, unsigned int size);

int cry_mpi_init_int(cry_mpi *a, long i);

void cry_mpi_set_int(cry_mpi *a, long i);

/*
 *  Arithmetic
 */

int cry_mpi_add(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

/*
 * Utilities
 */

void cry_mpi_print(const cry_mpi *a);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_MPI_H_ */

