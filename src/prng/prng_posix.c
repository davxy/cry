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

#include "cry/prng.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static int urand_fd = -1;

int cry_prng_init(const unsigned char *seed, size_t seed_siz)
{
    urand_fd = open("/dev/urandom", O_RDONLY);
    if (urand_fd < 0)
        return -1;
    return 0;
}

int cry_prng_rand(unsigned char *buf, size_t siz)
{
    int n;

    if (urand_fd == -1 && cry_prng_init(NULL, 0) != 0)
        return -1;

    while (siz) {
        if ((n = read(urand_fd, buf, siz)) < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        buf += n;
        siz -= n;
    }
    return 0;
}

