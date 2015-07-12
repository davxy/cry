/*
 * Copyright (c) 2013-2015, Davide Galassi. All rights reserved.
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

#include <windows.h>
#include <wincrypt.h>

static HCRYPTPROV hProvider;

int cry_rand_init(void)
{
    if (!CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        return -1;
    }
    return 0;
}

int cry_rand(unsigned char *buf, unsigned int siz)
{
    if (!hProvider)
        cry_rand_init();
    if (!CryptGenRandom(hProvider, siz, buf))
        return -1;
    return 0;
}

