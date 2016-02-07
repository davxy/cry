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
#include <cry/sum.h>

#define MSG "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CS8 0xdf
#define LRC 0x21

void sum_test(void)
{
    unsigned char cs;

    TRACE("msg: %s\n", MSG);

    cs = cry_cs8(MSG, strlen(MSG));
    TRACE("cs8 = 0x%02x\n", cs);
    ASSERT_EQ(cs, CS8);

    cs = cry_lrc(MSG, strlen(MSG));
    TRACE("lrc = 0x%02x\n", cs);
    ASSERT_EQ(cs, LRC);
}
