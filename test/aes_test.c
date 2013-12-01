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
#include <stdio.h>
#include <string.h>

int main(void)
{
    char buf[128];
    char *msg = "CRY is free software: you can redistribute it and/or modify";
    char key[] = {  0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9,10,11,12,13,14,15,
                   16,17,18,19,20,21,22,23,
                   24,25,26,27,28,29,30,31  };
    int msglen = strlen(msg);

    printf("Msg len: %d\n", msglen);

    printf("AES-128\n");
    memset(buf, 0, sizeof(buf));
    cry_aes_128_encrypt(buf, msg, msglen, key);
    cry_aes_128_decrypt(buf, buf, msglen, key);
    printf("%.*s\n", msglen, buf);

    printf("AES-192\n");
    memset(buf, 0, sizeof(buf));
    cry_aes_192_encrypt(buf, msg, msglen, key);
    cry_aes_192_decrypt(buf, buf, msglen, key);
    printf("%.*s\n", msglen, buf);

    printf("AES-256\n");
    memset(buf, 0, sizeof(buf));
    cry_aes_256_encrypt(buf, msg, msglen, key);
    cry_aes_256_decrypt(buf, buf, msglen, key);
    printf("%.*s\n", msglen, buf);

    return 0;
}

