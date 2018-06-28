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


static void mpi_sub_no_expand(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 1));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_b6400_bin, 1));

    ASSERT_OK(cry_mpi_sub(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ_MPI(g_mpi2, g_a8_b8_sub_bin, sizeof(g_a8_b8_sub_bin));
}


static void mpi_sub_carry_propagate(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 64));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 32));

    ASSERT_OK(cry_mpi_sub(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ_MPI(g_mpi2, g_a512_a256_sub_bin, sizeof(g_a512_a256_sub_bin));
}

static void mpi_sub_expand(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 128));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 32));

    ASSERT_OK(cry_mpi_sub(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ_MPI(g_mpi2, g_a1024_a256_sub_bin, sizeof(g_a1024_a256_sub_bin));
}

static void mpi_sub_abs_neg_res(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 1));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_b6400_bin, 1));

    ASSERT_NE(cry_mpi_sub_abs(g_mpi2, g_mpi1, g_mpi0), 0);
}

void mpi_sub_test(void)
{
    MPI_RUN(mpi_sub_no_expand);
    MPI_RUN(mpi_sub_expand);
    MPI_RUN(mpi_sub_carry_propagate);
    MPI_RUN(mpi_sub_abs_neg_res);
}
