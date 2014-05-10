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

#include "cry_aes.h"
#include <string.h>

static void xor_128(unsigned char *o, const unsigned char *a,
                    const unsigned char *b)
{
    size_t i;

    for (i = 0; i < 16; i++)
        o[i] = a[i] ^ b[i];
}

static void cry_aes_cbc_compute_subkeys(struct cry_aes_ctx *aes,
                                        unsigned char *k1,
                                        unsigned char *k2)
{
    unsigned char blank[16] = {0};
    unsigned char msb;
    size_t i;

    cry_aes_encrypt(aes, k1, blank, 16);

    msb = (k1[0] & 0x80);
    for (i = 0; i < 16; i++) {
        k1[i] <<= 1;
        if (i < 15 && ((k1[i + 1] & 0x80) != 0))
            k1[i] |= 1;
    }
    if (msb != 0)
        k1[15] ^= 0x87;

    msb = (k1[0] & 0x80);
    for (i = 0; i < 16; i++) {
        k2[i] = k1[i] << 1;
        if (i < 15 && ((k1[i + 1] & 0x80) != 0))
            k2[i] |= 1;
    }
    if (msb != 0)
        k2[15] ^= 0x87;
}

static void padding(unsigned char *pad, const unsigned char *last,
                    size_t size)
{
    size_t i;

    for (i = 0; i < 16; i++) {
        if (i < size)
            pad[i] = last[i];
        else if (i == size)
            pad[i] = 0x80;
        else
            pad[i] = 0x00;
    }
}

void cry_cmac_digest(unsigned char *mac, const unsigned char *input,
                     size_t size, const unsigned char *key, size_t keysize)
{
    struct cry_aes_ctx aes;
    unsigned char block[16], last[16], padded[16];
    unsigned char k1[16], k2[16];
    size_t n, i;
    int flag;

    cry_aes_key_set(&aes, key, keysize);

    cry_aes_cbc_compute_subkeys(&aes, k1, k2);

    n = (size + 15) / 16;  /* n is number of rounds */
    if (n == 0) {
        n = 1;
        flag = 0;
    } else {
        if ((size % 16) == 0)
            flag = 1; /* last block is a complete block */
        else
            flag = 0; /* last block is not complete block */
    }

    if (flag) {
        /* last block is complete block */
        xor_128(last, &input[16*(n-1)], k1);
    } else {
        padding(padded, &input[16*(n-1)], size % 16);
        xor_128(last, padded, k2);
    }

    memset(mac, 0, 16);
    for (i = 0; i < (n - 1); i++) {
        xor_128(block, mac, &input[16*i]);
        cry_aes_encrypt(&aes, mac, block, 16);
    }

    xor_128(block, mac, last);
    cry_aes_encrypt(&aes, mac, block, 16);
}

