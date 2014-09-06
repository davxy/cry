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

#include <cry/cbc.h>
#include <cry/aes.h>
#include <stdio.h>
#include <string.h>

static const struct cry_ciph_itf aes_itf = {
    .init = NULL,
    .clean = NULL,
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
    .decrypt = (cry_ciph_decrypt_f) cry_aes_decrypt
};

void cry_aes_128_cbc_encrypt(unsigned char *dst, const unsigned char *src,
                             unsigned int size, unsigned char *key,
                             unsigned char *iv)
{
    struct cry_aes_ctx aes;
    struct cry_cbc_ctx cbc;

    cbc.ciph_itf = &aes_itf;
    cbc.ciph_ctx = &aes;
    cry_cbc_key_set(&cbc, key, 16);
    cry_cbc_iv_set(&cbc, iv, 16);
    cry_cbc_encrypt(&cbc, dst, src, size);
}

void cry_aes_128_cbc_decrypt(unsigned char *dst, const unsigned char *src,
                             unsigned int size, unsigned char *key,
                             unsigned char *iv)
{
    struct cry_aes_ctx aes;
    struct cry_cbc_ctx cbc;

    cbc.ciph_itf = &aes_itf;
    cbc.ciph_ctx = &aes;
    cry_cbc_key_set(&cbc, key, 16);
    cry_cbc_iv_set(&cbc, iv, 16);
    cry_cbc_decrypt(&cbc, dst, src, size);
}

int main(void)
{
    char buf[128];
    char *msg = "CRY is free software: you can redistribute it and/or modify";
    char key[] = {  0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9,10,11,12,13,14,15 };
    char iv[] =  {  0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9,10,11,12,13,14,15 };

    int msglen = strlen(msg);

    printf("Msg len: %d\n", msglen);

    printf("AES-128-CBC\n");
    memset(buf, 0, sizeof(buf));
    cry_aes_128_cbc_encrypt(buf, msg, msglen, key, iv);
    cry_aes_128_cbc_decrypt(buf, buf, msglen, key, iv);
    printf("%.*s\n", msglen, buf);

    return 0;
}

