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

/*
 * Weak fallback random functions
 */

#include <time.h>
#include <stdint.h>

static uint32_t z1, z2, z3, z4;

#define RAND_GET(r) do { \
    ((r) = ((z1 << 6) ^ z1) >> 13); \
    (z1 = ((z1 & 4294967294UL) << 18) ^ (r)); \
    ((r) = ((z2 << 2) ^ z2) >> 27); \
    (z2 = ((z2 & 4294967288UL) << 2) ^ (r)); \
    ((r) = ((z3 << 13) ^ z3) >> 21); \
    (z3 = ((z3 & 4294967280UL) << 7) ^ (r)); \
    ((r) = ((z4 << 3) ^ z4) >> 12); \
    (z4 = ((z4 & 4294967168UL) << 13) ^ (r)); \
    ((r) = (z1 ^ z2 ^ z3 ^ z4)); \
    } while(0)

int __attribute__((weak)) cry_rand_init(void)
{
    z1 = z2 = z3 = z4 = time(NULL);
    return 0;
}

int __attribute__((weak)) cry_rand(unsigned char *buf, unsigned int siz)
{
    static int init = 0;
    int i, iter = siz / 4;
    uint32_t *buf32 = (uint32_t *) buf;
    uint32_t r;

    if (!init) {
        init = 1;
        cry_rand_init();
    }

    for (i = 0; i < iter; i++, buf32++)
        RAND_GET(*buf32);

    if (iter = (siz & 0x03)) {
        RAND_GET(r);
        buf = (unsigned char *)buf32;
        for (i = 0; i < iter; i++, r >>= 8)
            *buf++ = (unsigned char)r;
    }
    return 0;
}

