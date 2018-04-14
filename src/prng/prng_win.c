/*
 * Copyright (c) 2013-2018, Davide Galassi. All rights reserved.
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

static HCRYPTPROV hProvider = 0;

int cry_prng_init(const unsigned char *seed, size_t seed_siz)
{
    if (!CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        return -1;
    }
    return 0;
}

int cry_prng_rand(unsigned char *buf, size_t siz)
{
    if (!hProvider && cry_prng_init(NULL, 0) != 0)
        return -1;
    if (!CryptGenRandom(hProvider, siz, buf))
        return -1;
    return 0;
}

