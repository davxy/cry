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

#include <cry.h>
#include <stdio.h>

static void add_test(void)
{
    cry_mpi a, b, r;

    cry_mpi_init_int(&a, 0x1234);
    cry_mpi_init_int(&b, 0x4321);
    cry_mpi_add(&r, &a, &b);
    cry_mpi_print(&r);
    cry_mpi_clear(&a);
    cry_mpi_clear(&b);
}

static void init_test(void)
{
    cry_mpi a, b;

    cry_mpi_init(&a);
    cry_mpi_print(&a);
    cry_mpi_set_int(&a, 0x1234);
    cry_mpi_print(&a);
    cry_mpi_clear(&a);

    cry_mpi_init_int(&a, 0x12345678);
    cry_mpi_init_copy(&b, &a);
    cry_mpi_print(&b);
    cry_mpi_clear(&a);
    cry_mpi_clear(&b);

    cry_mpi_init_size(&a, 1);
    cry_mpi_init(&b);
    printf("with init_size(1) alloc=%d\n", a.alloc);
    printf("with init() alloc= %d\n", b.alloc);
}

int main(void)
{
    init_test();
    add_test();
    
    return 0;
}
