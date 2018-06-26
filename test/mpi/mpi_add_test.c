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


static void mpi_add_no_expand(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a8_bin, sizeof(g_a8_bin)));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_b8_bin, sizeof(g_b8_bin)));

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    cry_mpi_store_bin(g_mpi2, buf, BUFSIZ, 0);
    ASSERT_EQ_BUF(buf, g_a8_b8_add_bin, sizeof(g_a8_b8_add_bin));
}


void mpi_add_test(void)
{
    MPI_RUN(mpi_add_no_expand);
}
