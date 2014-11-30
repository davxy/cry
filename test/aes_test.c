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
#include <cry/aes.h>

#define MSG "CRY is free software: you can redistribute it and/or modify it"
#define KEY "0123456789abcdefABCDEF9876543210"

void aes_test(void)
{
    char buf[128];
    int msglen = strlen(MSG);

    TRACE("Msg len: %d\n", msglen);

    TRACE("AES-128\n");
    memset(buf, 0, sizeof(buf));
    cry_aes_128_encrypt(buf, MSG, msglen, KEY);
    print_hex(buf, msglen);
    cry_aes_128_decrypt(buf, buf, msglen, KEY);
    print_hex(buf, msglen);
    TRACE("%.*s\n", msglen, buf);

    TRACE("AES-192\n");
    memset(buf, 0, sizeof(buf));
    cry_aes_192_encrypt(buf, MSG, msglen, KEY);
    cry_aes_192_decrypt(buf, buf, msglen, KEY);
    TRACE("%.*s\n", msglen, buf);

    TRACE("AES-256\n");
    memset(buf, 0, sizeof(buf));
    cry_aes_256_encrypt(buf, MSG, msglen, KEY);
    cry_aes_256_decrypt(buf, buf, msglen, KEY);
    TRACE("%.*s\n", msglen, buf);
}

