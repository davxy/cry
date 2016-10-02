/*
 * Copyright (c) 2013-2016, Davide Galassi. All rights reserved.
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

static void crc16_ccitt_test(void)
{
    uint16_t crc;
#define CRC16_CCITT 0xad3b

    crc = cry_crc16_ccitt(MSG, strlen(MSG));
    TRACE("crc16-ccitt = 0x%04x\n", crc);
    ASSERT_EQ(crc, CRC16_CCITT);
}

static void crc16_ibm_test(void)
{
    uint16_t crc;
#define CRC16_IBM   0xfe85

    crc = cry_crc16_ibm(MSG, strlen(MSG));
    TRACE("crc16-ibm   = 0x%04x\n", crc);
    ASSERT_EQ(crc, CRC16_IBM);
}

static void crc32_eth_test(void)
{
    uint32_t crc;
#define CRC32_ETH   0Xabf77822

    crc = cry_crc32_eth(MSG, strlen(MSG));
    TRACE("crc32-eth   = 0x%08x\n", crc);
    ASSERT_EQ(crc, CRC32_ETH);
}

void crc_test(void)
{
    TRACE("msg: %s\n", MSG);
    RUN(crc16_ccitt_test);
    RUN(crc16_ibm_test);
    RUN(crc32_eth_test);
}

