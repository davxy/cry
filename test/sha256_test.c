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
#include <cry/sha256.h>

static char *input[] = {
    "This file is part of CRY software.",
    "CRY is free software",
};

static const char sha256_hash[] = {
    0x0f,0x02,0x31,0x2e,0x48,0xa2,0xdd,0x86,
    0x97,0xf2,0xa1,0xd5,0xd4,0x29,0xb6,0x22,
    0x20,0x00,0x67,0xaf,0xc1,0x31,0x82,0x4d,
    0x57,0x30,0x75,0xda,0xa5,0x82,0x54,0x99
};

void sha256_test(void)
{
    struct cry_sha256_ctx sha256;
    unsigned int len, i;

    cry_sha256_init(&sha256);
    for (i = 0; i < ARLEN(input); i++) {
        len = strlen(input[i]);
        PRINT_ASC("input", input[i], len);
        cry_sha256_update(&sha256, input[i], len);
    }
    cry_sha256_digest(&sha256, buf);

    PRINT_HEX("sha256", buf, 32);
    ASSERT_EQ_BUF(buf, sha256_hash, 32);
}

