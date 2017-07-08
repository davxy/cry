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

#include "cry/aes.h"

static void cry_aes_xxx_encrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key,
                                unsigned int keysize)
{
    struct cry_aes_ctx ctx;

    cry_aes_key_set(&ctx, key, keysize);
    cry_aes_encrypt(&ctx, dst, src, size);
}

static void cry_aes_xxx_decrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key,
                                unsigned int keysize)
{
    struct cry_aes_ctx ctx;

    cry_aes_key_set(&ctx, key, keysize);
    cry_aes_decrypt(&ctx, dst, src, size);
}

void cry_aes_128_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_encrypt(dst, src, size, key, 16);
}

void cry_aes_128_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_decrypt(dst, src, size, key, 16);
}

void cry_aes_192_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_encrypt(dst, src, size, key, 24);
}

void cry_aes_192_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_decrypt(dst, src, size, key, 24);
}

void cry_aes_256_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_encrypt(dst, src, size, key, 32);
}

void cry_aes_256_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_decrypt(dst, src, size, key, 32);
}

