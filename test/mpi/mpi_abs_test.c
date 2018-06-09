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

static void positive_abs(void)
{
    cry_mpi_set_int(g_mpi0, 123456);

    cry_mpi_abs(g_mpi0, g_mpi0);

    ASSERT_EQ(cry_mpi_is_pos(g_mpi0), 1);
}

static void negative_abs(void)
{
    cry_mpi_set_int(g_mpi0, -123456);

    cry_mpi_abs(g_mpi0, g_mpi0);

    ASSERT_EQ(cry_mpi_is_pos(g_mpi0), 1);
}

static void different_src_dst(void)
{
    cry_mpi_set_int(g_mpi0, -123456);

    cry_mpi_abs(g_mpi1, g_mpi0);

    ASSERT_EQ(cry_mpi_is_pos(g_mpi1), 1);
    ASSERT_EQ(cry_mpi_is_pos(g_mpi0), 0);
}


void mpi_abs_test(void)
{
    MPI_RUN(positive_abs);
    MPI_RUN(negative_abs);
    MPI_RUN(different_src_dst);
}
