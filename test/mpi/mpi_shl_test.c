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
#include "mpi/mpi_pvt.h"


static void shld_dummy(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    ASSERT_OK(cry_mpi_shld(g_mpi0, 0));
    cry_mpi_print(g_mpi0, 16);
}

static void shld_expand(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    ASSERT_OK(cry_mpi_shld(g_mpi0, g_mpi0->alloc + 1));
    cry_mpi_print(g_mpi0, 16);
}

static void shl_one_digit(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, 8*sizeof(cry_mpi_digit)));
    cry_mpi_print(g_mpi1, 16);

    //ASSERT_EQ_MPI(g_mpi2, g_a8_b8_sub_bin, sizeof(g_a8_b8_sub_bin));
}

static void shl_expand(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    size_t bits = 8*sizeof(cry_mpi_digit)*(g_mpi0->alloc + 1);
    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, bits));
    cry_mpi_print(g_mpi1, 16);

    //ASSERT_EQ_MPI(g_mpi2, g_a8_b8_sub_bin, sizeof(g_a8_b8_sub_bin));
}

static void shl_not_digit_multiple(void)
{
    long val;
    ASSERT_OK(cry_mpi_set_int(g_mpi0, A32_VAL));

    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, 1));

    ASSERT_OK(cry_mpi_get_int(g_mpi1, &val));

    ASSERT_EQ(val, 0x1925c4172);
}

static void shl_not_digit_multiple_carry(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, 1));
    cry_mpi_print(g_mpi1, 16);
}


void mpi_shl_test(void)
{
    MPI_RUN(shld_dummy);
    MPI_RUN(shld_expand);
    MPI_RUN(shl_one_digit);
    MPI_RUN(shl_expand);
    MPI_RUN(shl_not_digit_multiple);
    MPI_RUN(shl_not_digit_multiple_carry);
}
