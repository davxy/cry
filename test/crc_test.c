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
#include <cry/crc.h>

#define MSG "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

void crc_test(void)
{
    TRACE("Msg: %s\n", MSG);
    TRACE("crc16-ccitt = 0x%04x\n", cry_crc16_ccitt(MSG, strlen(MSG)));
    TRACE("crc16-ibm   = 0x%04x\n", cry_crc16_ibm(MSG, strlen(MSG)));
}

