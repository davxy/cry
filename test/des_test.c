/*
 * Copyright (c) 2013-2015, Davide Galassi. All rights reserved.
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
#include <cry/des.h>

#define KEY   "12345678abcdefghILMNOPQR"
#define MSG   "CRY is free soft"
#define LEN   16
#define CDES  "\x04\xcf\xf8\x8a\xd8\x95\x79\x5c\x2e\x8d\xf1\xd9\xf5\xbd\x94\x54"
#define CTDES "\x93\x7c\x8d\xf0\xa1\xa4\x77\x61\x11\x4d\xb3\x17\x71\xdb\x10\x19"

static cry_des_ctx des;

static void single(void)
{
    memcpy(buf, MSG, LEN); /* test in place */
    cry_des_key_set(&des, KEY, 8);
    cry_des_encrypt(&des, buf, buf, LEN);
    ASSERT(memcmp(buf, CDES, LEN) == 0);
    print_hex(buf, LEN);
    cry_des_decrypt(&des, buf, buf, LEN);
    ASSERT(memcmp(buf, MSG, LEN) == 0);
    TRACE("%.*s\n", LEN, buf);
}

static void triple(void)
{
    memcpy(buf, MSG, LEN); /* test in place */
    cry_des_key_set(&des, KEY, 24);
    cry_des_encrypt(&des, buf, buf, LEN);
    ASSERT(memcmp(buf, CTDES, LEN) == 0);
    print_hex(buf, LEN);
    cry_des_decrypt(&des, buf, buf, LEN);
    ASSERT(memcmp(buf, MSG, LEN) == 0);
    TRACE("%.*s\n", LEN, buf);
}

void des_test(void)
{
    RUN(single);
    RUN(triple);
}

