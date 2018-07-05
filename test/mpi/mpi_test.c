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

#include "mpi_test.h"
#include <stdlib.h>


cry_mpi *g_mpi_buf;

void mpi_setup(void)
{
    g_mpi_buf = malloc(sizeof(cry_mpi)*MPI_BUF_LEN);
    cry_mpi_init_list(g_mpi0, g_mpi1, g_mpi2, g_mpi3, NULL);
}

void mpi_teardown(void)
{
    cry_mpi_clear_list(g_mpi0, g_mpi1, g_mpi2, g_mpi3, NULL);
    free(g_mpi_buf);
}


void mpi_test(void)
{
    RUN(mpi_core_test);
    RUN(mpi_cmp_test);
    RUN(mpi_abs_test);
    RUN(mpi_add_test);
    RUN(mpi_sub_test);
    RUN(mpi_shl_test);
    RUN(mpi_shr_test);
//    RUN(mpi_mul_test);
}

