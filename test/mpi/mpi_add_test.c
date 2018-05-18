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


void mpi_add_test(void)
{
    struct cry_mpi a, b, r;

    ASSERT_OK(cry_mpi_init_list(&a, &b, &r, NULL));

    ASSERT_OK(cry_mpi_set_int(&a, 0x12));
    ASSERT_OK(cry_mpi_set_int(&b, 0x21));
    ASSERT_OK(cry_mpi_add(&r, &a, &b));

    ASSERT_OK(cry_mpi_store_str(&r, 16, (char *)buf, BUFSIZ));
    ASSERT_EQ(strcmp((char *)buf, "33"), 0);

    cry_mpi_clear_list(&a, &b, &r, NULL);
}
