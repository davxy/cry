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

/**
 * @file    cry_mpi.h
 * @brief   Multiple precision integers.
 */

#ifndef _CRY_MPI_H_
#define _CRY_MPI_H_

#include <stddef.h>

typedef unsigned char cry_mpi_digit;

struct cry_mpi {
    int           sign;
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

int cry_mpi_init_bin(cry_mpi *a, const void *b, unsigned int size);

int cry_mpi_load_bin(cry_mpi *a, const void *b, unsigned int size);

int cry_mpi_store_bin(const cry_mpi *a, void *b, unsigned int size, int pad);

int cry_mpi_init_str(cry_mpi *a, unsigned int radix, const char *s);

int cry_mpi_load_str(cry_mpi *a, unsigned int radix, const char *s);

int cry_mpi_store_str(const cry_mpi *a, unsigned int radix, char *s,
                      size_t size);

int cry_mpi_init_list(cry_mpi *a, ...);

void cry_mpi_clear_list(cry_mpi *a, ...);

/*
 *  Arithmetic
 */

int cry_mpi_abs(cry_mpi *d, const cry_mpi *s);

int cry_mpi_add_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_sub_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_div_abs(cry_mpi *q, cry_mpi *r, const cry_mpi *a,
                    const cry_mpi *b);

int cry_mpi_add(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_sub(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_mul(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_div(cry_mpi *q, cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_shl(cry_mpi *c, const cry_mpi *a, int n);

int cry_mpi_shr(cry_mpi *c, const cry_mpi *a, int n);

/*
 * More advanced arithmetic algorithms
 */

int cry_mpi_gcd(cry_mpi *r, const cry_mpi *x, const cry_mpi *y);

/*
 * Utilities
 */

int cry_mpi_cmp_abs(const cry_mpi *a, const cry_mpi *b);

int cry_mpi_cmp(const cry_mpi *a, const cry_mpi *b);

void cry_mpi_print(const cry_mpi *a, unsigned int radix);

#define cry_mpi_zero(a) do { \
    (a)->sign = 0;           \
    (a)->used = 0;           \
    } while(0)

size_t cry_mpi_count_bits(const cry_mpi *a);

#define cry_mpi_count_bytes(a) \
    ((cry_mpi_count_bits(a) + 7) / 8)

#define cry_mpi_swap(a, b) do { \
    cry_mpi __t = *(a);         \
    *(a) = *(b);                \
    *(b) = __t;                 \
    } while(0)

#define cry_mpi_is_zero(a) \
    ((a)->used == 0)

#define cry_mpi_is_odd(a) \
    (!cry_mpi_is_zero(a) && ((a)->data[0] & 1))

#define cry_mpi_is_even(a) \
    (!cry_mpi_is_odd(a))

#ifdef __cplusplus
}
#endif

#endif /* _CRY_MPI_H_ */

