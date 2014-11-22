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

#include <cry/mpi.h>
#include <stdio.h>

#define MPI_PRINT(a, msg) do { \
    printf("%s:\t", msg); \
    cry_mpi_print(a, 16); \
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

static void init_list_test(void)
{
    cry_mpi a, b, c;

    printf("> MPI init-list-test\n");

    if (cry_mpi_init_list(&a, &b, &c, NULL) != 0)
        printf("Error: mpi_init_list\n");
    cry_mpi_clear_list(&a, &b, &c, NULL);
}

static void init_bin_test(void)
{
    cry_mpi a;
    int i;
    const char mpi_be_data[] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
        0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
    };
    char buf[sizeof(mpi_be_data)];

    printf("> MPI init-bin-test\n");

    printf("load-bin:  ");
    for (i = 0; i < sizeof(mpi_be_data); i++)
        printf("%02x", mpi_be_data[i]);
    printf("\n");

    cry_mpi_init_bin(&a, mpi_be_data, sizeof(mpi_be_data));
    MPI_PRINT(&a, "a");

    cry_mpi_store_bin(&a, buf, sizeof(buf), 1);
    printf("store-bin (pad=1): ");
    for (i = 0; i < sizeof(buf); i++)
        printf("%02x", buf[i]);
    printf("\n");

    cry_mpi_store_bin(&a, buf, a.used, 0);
    printf("store-bin (pad=0): ");
    for (i = 0; i < a.used; i++)
        printf("%02x", buf[i]);
    printf("\n");
}

static void init_str_test(void)
{
   cry_mpi a;

   printf("> MPI init-str-test\n");

   cry_mpi_init_str(&a, "0x123456789abcdef");
   MPI_PRINT(&a, "a");
   cry_mpi_clear(&a);

   cry_mpi_init_str(&a, "-0x123456789a");
   MPI_PRINT(&a, "a");
   cry_mpi_clear(&a);
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

static void mul_test(void)
{
    cry_mpi a, b, r;

    printf("> MPI mul-test\n");

    cry_mpi_init_list(&a, &b, &r, NULL);
    cry_mpi_set_int(&a, 0x1234);
    cry_mpi_set_int(&b, 0x02);
    cry_mpi_mul(&r, &a, &b);
    MPI_PRINT(&a, "a");
    MPI_PRINT(&b, "b");
    MPI_PRINT(&r, "a * b");
    cry_mpi_clear_list(&a, &b, &r, NULL);
}

static void div_test(void)
{
    cry_mpi a, b, r, q;

    printf("> MPI div-test\n");

    cry_mpi_init_list(&a, &b, &q, &r, NULL);
    cry_mpi_set_int(&a, -7);
    cry_mpi_set_int(&b, 3);
    cry_mpi_div(&q, &r, &a, &b);
    MPI_PRINT(&a, "a");
    MPI_PRINT(&b, "b");
    MPI_PRINT(&q, "a / b");
    MPI_PRINT(&r, "a \% b");
    cry_mpi_clear_list(&a, &b, &q, &r, NULL);
}

int main(void)
{
    init_test();
    cmp_test();
    add_test();
    sub_test();
    init_bin_test();
    init_str_test();
    init_list_test();
    mul_test();
    div_test();
    
    return 0;
}
