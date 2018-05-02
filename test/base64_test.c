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
#include <cry/base64.h>

#define MSG "Hello World"
#define MLEN strlen(MSG)
#define ELEN 16

void base64_test(void)
{
    char *cbuf = (char *)buf;

    ASSERT(cry_base64_encode(MSG, strlen(MSG), cbuf) == ELEN);
    PRINT_ASC("encoded", cbuf, ELEN);
    ASSERT_EQ_BUF(cbuf, "SGVsbG8gV29ybGQ=", ELEN);

    ASSERT(cry_base64_decode(cbuf, ELEN, cbuf) == MLEN);
    PRINT_ASC("decoded", cbuf, MLEN);
    ASSERT_EQ_BUF(buf, MSG, MLEN);
}

