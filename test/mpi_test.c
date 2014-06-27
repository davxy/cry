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

#define MPI_PRINT(a, msg) do { \
    printf("%s:\t", msg); \
    cry_mpi_print(a); \
    } while(0)

static void init_test(void)
{
    cry_mpi a, b;

    printf("> MPI init-test\n");

    cry_mpi_init(&a);
    MPI_PRINT(&a, "pre init");
    cry_mpi_set_int(&a, 0x1234);
    MPI_PRINT(&a, "post init");
    cry_mpi_clear(&a);

    cry_mpi_init_int(&a, 0x12345678);
    cry_mpi_init_copy(&b, &a);
    MPI_PRINT(&b, "after init-copy");
    cry_mpi_clear(&a);
    cry_mpi_clear(&b);

    cry_mpi_init_size(&a, 1);
    cry_mpi_init(&b);
    printf("with init_size(1) alloc=%d\n", a.alloc);
    printf("with init() alloc= %d\n", b.alloc);
}

static void cmp_test(void)
{
    cry_mpi a, b, r;

    printf("> MPI cmp-test\n");

    cry_mpi_init_int(&a, 0x1234);
    cry_mpi_init_int(&b, 0x4321);
    cry_mpi_init(&r);

    MPI_PRINT(&a, "a");
    MPI_PRINT(&b, "b");
    printf("cmp(a,b) res=%d\n", cry_mpi_cmp(&a, &b));
    printf("cmp(b,a) res=%d\n", cry_mpi_cmp(&b, &a));

    cry_mpi_clear(&a);
    cry_mpi_clear(&b);
    cry_mpi_clear(&r);

}

static void add_test(void)
{
    cry_mpi a, b, r;

    printf("> MPI add-test\n");

    cry_mpi_init_int(&a, 0x1234);
    cry_mpi_init_int(&b, 0x4321);
    cry_mpi_init(&r);

    cry_mpi_add(&r, &a, &b);

    MPI_PRINT(&a, "a");
    MPI_PRINT(&b, "b");
    MPI_PRINT(&r, "a + b");

    cry_mpi_clear(&a);
    cry_mpi_clear(&b);
    cry_mpi_clear(&r);
}

static void sub_test(void)
{
    cry_mpi a, b, r;

    printf("> MPI sub-test\n");

    cry_mpi_init_int(&a, 0x55555555);
    cry_mpi_init_int(&b, 0x1111);
    cry_mpi_init(&r);

    cry_mpi_sub(&r, &a, &b);

    MPI_PRINT(&a, "a");
    MPI_PRINT(&b, "b");
    MPI_PRINT(&r, "a - b");

    cry_mpi_clear(&a);
    cry_mpi_clear(&b);
    cry_mpi_clear(&r);
}



int main(void)
{
    init_test();
    cmp_test();
    add_test();
    sub_test();
    
    return 0;
}
