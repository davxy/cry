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
#include "memxor.h"


#define XOR_RES \
        "\x51\x53\x51\x57\x51\x53\x51\x5f\x51\x53\x2a\x2e\x2e\x2a\x2a\x36"

void memxor_test(void)
{
    const char *a = "0123456789ABCDEF";
    const char *b = "abcdefghijklmnop";
    size_t n = strlen(a);
    
    memcpy(buf, a, n);
    cry_memxor(buf, (unsigned char *)b, n);

    ASSERT_EQ_BUF(buf, XOR_RES, n);
}

