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
#include <cry/cbc.h>
#include <cry/aes.h>

static const struct cry_ciph_itf aes_itf = {
    .init = NULL,
    .clean = NULL,
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
    .decrypt = (cry_ciph_decrypt_f) cry_aes_decrypt
};

void cry_aes_128_cbc_encrypt(unsigned char *dst, const unsigned char *src,
                             unsigned int size, const unsigned char *key,
                             const unsigned char *iv)
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
                             unsigned int size, const unsigned char *key,
                             const unsigned char *iv)
{
    struct cry_aes_ctx aes;
    struct cry_cbc_ctx cbc;

    cbc.ciph_itf = &aes_itf;
    cbc.ciph_ctx = &aes;
    cry_cbc_key_set(&cbc, key, 16);
    cry_cbc_iv_set(&cbc, iv, 16);
    cry_cbc_decrypt(&cbc, dst, src, size);
}

const char key[] = {
    0xc2,0x86,0x69,0x6d,0x88,0x7c,0x9a,0xa0,
    0x61,0x1b,0xbb,0x3e,0x20,0x25,0xa4,0x5a
};

const char iv[] = {
    0x56,0x2e,0x17,0x99,0x6d,0x09,0x3d,0x28,
    0xdd,0xb3,0xba,0x69,0x5a,0x2e,0x6f,0x58
};

const char plaintext[] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f
};

const char ciphertext[] = {
    0xd2,0x96,0xcd,0x94,0xc2,0xcc,0xcf,0x8a,
    0x3a,0x86,0x30,0x28,0xb5,0xe1,0xdc,0x0a,
    0x75,0x86,0x60,0x2d,0x25,0x3c,0xff,0xf9,
    0x1b,0x82,0x66,0xbe,0xa6,0xd6,0x1a,0xb1
};

#define LEN sizeof(plaintext)

void cbc_test(void)
{
    memcpy(buf, plaintext, LEN);
    cry_aes_128_cbc_encrypt(buf, buf, LEN, key, iv);
    PRINT_HEX("ciphertext", buf, LEN);
    ASSERT_EQ_BUF(buf, ciphertext, LEN);
    cry_aes_128_cbc_decrypt(buf, buf, LEN, key, iv);
    PRINT_HEX("plaintext ", buf, LEN);
    ASSERT_EQ_BUF(buf, plaintext, LEN);
}

