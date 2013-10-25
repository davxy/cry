/*
 * Copyright (c) 2013, Davide Galassi. All rights reserved.
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

#include <cry.h>
#include <string.h>
#include <stdio.h>

#define TXT     "HelloWorldFromDesAlgo"
#define KEY     "12345678abcdefghILMNOPQR"

int main(void)
{
    char buf[64] = {0};
    cry_des_ctx des;
    int len = strlen(TXT);
    int pad = 8 - len % 8;

    printf("Single DES\n");
    memcpy(buf, TXT, len);
    memset(buf + len, pad, pad);
    len += pad; /* loose the original len info */
    cry_des_init(&des, KEY, 8);
    cry_des_encrypt(&des, buf, buf, len);
    cry_des_decrypt(&des, buf, buf, len);
    len -= buf[len - 1];
    printf("%.*s\n", len, buf);

    printf("Triple DES\n");
    memcpy(buf, TXT, len);
    memset(buf + len, pad, pad);
    len += pad; /* loose the original len info */
    cry_des_init(&des, KEY, 24);
    cry_des_encrypt(&des, buf, buf, len);
    cry_des_decrypt(&des, buf, buf, len);
    len -= buf[len - 1];
    printf("%.*s\n", len, buf);

    return 0;
}

