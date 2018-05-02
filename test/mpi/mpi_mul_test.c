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

#include "test.h"
#include <cry/mpi.h>

static cry_mpi a, b, r;
static size_t siz;


static void setup(void)
{
    memset(buf, 0, BUFSIZ);
    ASSERT_OK(cry_mpi_init_list(&a, &b, &r, NULL));
}

static void teardown(void)
{
    cry_mpi_clear_list(&a, &b, &r, NULL);
}



void mpi_mul_simple(void)
{
    ASSERT_OK(cry_mpi_set_int(&a, 0x02));
    ASSERT_OK(cry_mpi_set_int(&b, 0x03));

    ASSERT_OK(cry_mpi_mul(&r, &a, &b));

    cry_mpi_store_bin(&r, buf, BUFSIZ, 0);
    ASSERT_EQ(memcmp(buf, "\x06", 1), 0);
}


#define DIGIT_MAX_BIN "\xff\xff\xff\xff\xff\xff\xff\xff"
#define DIGIT_MAX_SIZ 8
#define DIGIT_MAX_MUL_BIN \
        "\xff\xff\xff\xff\xff\xff\xff\xfe\x00\x00\x00\x00\x00\x00\x00\x01"
#define DIGIT_MAX_MUL_SIZ \
        (sizeof(DIGIT_MAX_MUL_BIN)-1)

void mpi_mul_digit_max(void)
{
    ASSERT_OK(cry_mpi_load_bin(&a, DIGIT_MAX_BIN, DIGIT_MAX_SIZ));
    ASSERT_OK(cry_mpi_load_bin(&b, DIGIT_MAX_BIN, DIGIT_MAX_SIZ));

    ASSERT_OK(cry_mpi_mul(&r, &a, &b));

    siz = cry_mpi_store_bin(&r, (char *)buf, BUFSIZ, 0);
    ASSERT_EQ(siz, DIGIT_MAX_MUL_SIZ);
    ASSERT_EQ(memcmp(buf, DIGIT_MAX_MUL_BIN, DIGIT_MAX_MUL_SIZ), 0);
}


#define MYRUN(test) RUNX(test, setup, teardown)

void mpi_mul_test(void)
{
    MYRUN(mpi_mul_simple);
    MYRUN(mpi_mul_digit_max);
}
