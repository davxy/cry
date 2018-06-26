/*
 * Copyright (c) 2013-2017, Davide Galassi. All rights reserved.
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

#ifndef TEST_MPI_MPI_TEST_H_
#define TEST_MPI_MPI_TEST_H_

#include "test.h"
#include <cry/mpi.h>


#define MPI_BUF_LEN   4
extern cry_mpi *g_mpi_buf;
#define g_mpi0 (&g_mpi_buf[0])
#define g_mpi1 (&g_mpi_buf[1])
#define g_mpi2 (&g_mpi_buf[2])
#define g_mpi3 (&g_mpi_buf[3])


#define LONG_VALUE	0x123456789abcdef
extern const char g_long_value_bin[8];

extern const char g_a8_bin[1];
extern const char g_b8_bin[1];
extern const char g_c8_bin[1];
extern const char g_a8_b8_add_bin[1];
extern const char g_a8_c8_add_bin[2];

extern const char g_a6400_bin[800];
extern const char g_b6400_bin[800];
extern const char g_a6400_b6400_mul_bin[1600];

void mpi_setup(void);
void mpi_teardown(void);
#define MPI_RUN(test) RUNX(test, mpi_setup, mpi_teardown)

/*
 * Subtests
 */
void mpi_core_test(void);
void mpi_cmp_test(void);
void mpi_abs_test(void);
void mpi_add_test(void);
void mpi_mul_test(void);

#endif /* TEST_MPI_MPI_TEST_H_ */
