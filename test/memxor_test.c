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
#include <string.h>


void cry_memxor(unsigned char *dst, const unsigned char *src,
                unsigned int size);

void cry_memxor3(unsigned char *dst, const unsigned char *src1,
                 const unsigned char *src2, unsigned int size);


void memxor_test(void)
{
    const unsigned char *a = "0123456789ABCDEF";
    const unsigned char *b = "abcdefghijklmnop";
    size_t n = strlen(a);
    
    memcpy(buf, a, n);
    cry_memxor(buf, b, n);

    PRINT_HEX("res", buf, n);
}

