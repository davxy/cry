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

#include "test.h"
#include <cry/mpi.h>

#define BUF_SIZ 512
unsigned char buf[BUF_SIZ];

#ifdef NDEBUG
# define MPI_PRINT(a, msg)
#else
# define MPI_PRINT(a, msg) do { \
            TRACE("%s:\t", msg); \
            cry_mpi_print(a, 16); \
            } while(0)
#endif

#define MPI_ASSERT_EQUAL(a, radix, string) do { \
    cry_mpi_store_str(a, radix, buf, sizeof(buf)); \
    ASSERT(strcmp(buf, string) == 0); \
    } while (0)


static void mpi_init_test(void)
{
    cry_mpi a, b;

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
    TRACE("with init_size(1) alloc=%d\n", a.alloc);
    TRACE("with init() alloc= %d\n", b.alloc);
}

static void init_list_test(void)
{
    cry_mpi a, b, c;

    if (cry_mpi_init_list(&a, &b, &c, NULL) != 0)
        TRACE("Error: mpi_init_list\n");
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

    TRACE("load-bin:  ");
    for (i = 0; i < sizeof(mpi_be_data); i++)
        TRACE("%02x", mpi_be_data[i]);
    TRACE("\n");

    cry_mpi_init_bin(&a, mpi_be_data, sizeof(mpi_be_data));
    MPI_PRINT(&a, "a");

    cry_mpi_store_bin(&a, buf, sizeof(buf), 1);
    TRACE("store-bin (pad=1): ");
    for (i = 0; i < sizeof(buf); i++)
        TRACE("%02x", buf[i]);
    TRACE("\n");

    cry_mpi_store_bin(&a, buf, a.used, 0);
    TRACE("store-bin (pad=0): ");
    for (i = 0; i < a.used; i++)
        TRACE("%02x", buf[i]);
    TRACE("\n");
}

static void init_str_test(void)
{
   cry_mpi a;

   cry_mpi_init_str(&a, 16, "123456789abcdef");
   MPI_PRINT(&a, "a");
   MPI_ASSERT_EQUAL(&a, 16, "123456789abcdef");
   cry_mpi_clear(&a);

   cry_mpi_init_str(&a, 16, "-123456789a");
   MPI_PRINT(&a, "a");
   MPI_ASSERT_EQUAL(&a, 16, "-123456789a");
   cry_mpi_clear(&a);
}

static void load_str_test(void)
{
    cry_mpi a;

    cry_mpi_init_str(&a, 16, "0123456789abcdef");
    cry_mpi_store_str(&a, 16, buf, 6);
    ASSERT(memcmp("1234*\x00", buf, 6) == 0);
}

static void abs_test(void)
{
    cry_mpi a, b;

    cry_mpi_init_str(&a, 16, "-123456789a");
    ASSERT(a.sign != 0);
    cry_mpi_init(&b);
    cry_mpi_abs(&b, &a);
    ASSERT(b.sign == 0);
}

static void cmp_test(void)
{
    cry_mpi a, b, r;

    cry_mpi_init_int(&a, 0x1234);
    cry_mpi_init_int(&b, 0x4321);
    cry_mpi_init(&r);

    MPI_PRINT(&a, "a");
    MPI_PRINT(&b, "b");
    TRACE("cmp(a,b) res=%d\n", cry_mpi_cmp(&a, &b));
    TRACE("cmp(b,a) res=%d\n", cry_mpi_cmp(&b, &a));

    cry_mpi_clear(&a);
    cry_mpi_clear(&b);
    cry_mpi_clear(&r);

}

static void add_test(void)
{
    cry_mpi a, b, r;

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

static void shl_test(void)
{
    cry_mpi a;

    cry_mpi_init_str(&a, 16, "0102030405060708090a0b0c0d0e0f");
    MPI_PRINT(&a, "a     ");
    cry_mpi_shl(&a, &a, 3);
    MPI_PRINT(&a, "a << 3");
    MPI_ASSERT_EQUAL(&a, 16, "81018202830384048505860687078");
}

static void shr_test(void)
{
    cry_mpi a;

    cry_mpi_init_str(&a, 16, "0102030405060708090a0b0c0d0e0f");
    MPI_PRINT(&a, "a     ");
    cry_mpi_shr(&a, &a, 3);
    MPI_PRINT(&a, "a >> 3");
    MPI_ASSERT_EQUAL(&a, 16, "20406080a0c0e10121416181a1c1");
}

static void exp_test(void)
{
    cry_mpi r, b, e;

    cry_mpi_init_str(&b, 10, "123");
    cry_mpi_init_str(&e, 10, "4");
    cry_mpi_init(&r);
    cry_mpi_exp(&r, &b, &e);
    MPI_PRINT(&r, "exp(a,b)");
    MPI_ASSERT_EQUAL(&r, 16, "da48871");
}

static void gcd_test(void)
{
    cry_mpi a, b, c;

    cry_mpi_init_str(&a, 10, "53667");
    cry_mpi_init_str(&b, 10, "25527");
    cry_mpi_init(&c);
    cry_mpi_gcd(&c, &a, &b);
    MPI_PRINT(&c, "gcd(a,b)");
    MPI_ASSERT_EQUAL(&c, 10, "201");
}

void mpi_test(void)
{
    RUN(mpi_init_test);
    RUN(abs_test);
    RUN(cmp_test);
    RUN(add_test);
    RUN(sub_test);
    RUN(init_bin_test);
    RUN(init_str_test);
    RUN(load_str_test);
    RUN(init_list_test);
    RUN(mul_test);
    RUN(div_test);
    RUN(shl_test);
    RUN(shr_test);
    RUN(gcd_test);
    RUN(exp_test);
}

