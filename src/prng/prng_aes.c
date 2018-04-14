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
#include "cry/aes.h"
#include "misc.h"
#include <string.h>

#define BLOCK_SIZ 16

struct prng_bc_ctx {
    cry_aes_ctx     aes;
    unsigned char   ctr[16];
    unsigned char   lo[16];
    unsigned int    ix;
};

static struct prng_bc_ctx prng_ctx;
static int init = 0;

int cry_prng_init(const unsigned char *seed, size_t seed_siz)
{
    size_t sz;
    unsigned char key[BLOCK_SIZ] = {0};

    sz = CRY_MIN(seed_siz, BLOCK_SIZ);
    memcpy(key, seed, sz);
    cry_aes_key_set(&prng_ctx.aes, key, BLOCK_SIZ);

    seed += sz;
    seed_siz -= sz;

    sz = CRY_MIN(seed_siz, BLOCK_SIZ);
    memset(prng_ctx.ctr, 0, BLOCK_SIZ);
    memcpy(prng_ctx.ctr, seed, sz);

    prng_ctx.ix = 0;
    init = 1;
    return 0;
}

#define CTR_INC(ctr) do { \
    size_t __i = BLOCK_SIZ; \
    while (__i--) \
        if (++ctr[__i]) break; \
    } while (0)

int cry_prng_rand(unsigned char *buf, size_t siz)
{
    if (!init)
        cry_prng_init(NULL, 0);

    /* eventually store the previous call residual */
    for ( ; prng_ctx.ix && siz; siz--) {
        *buf++ = prng_ctx.lo[prng_ctx.ix++];
        prng_ctx.ix %= BLOCK_SIZ;
    }

    /* main loop */
    while (siz >= BLOCK_SIZ) {
        cry_aes_encrypt(&prng_ctx.aes, buf, prng_ctx.ctr, BLOCK_SIZ);
        CTR_INC(prng_ctx.ctr);
        buf += BLOCK_SIZ;
        siz -= BLOCK_SIZ;
    }

    /* there is some other data to give, generates residual */
    if (siz) {
        cry_aes_encrypt(&prng_ctx.aes, prng_ctx.lo, prng_ctx.ctr, BLOCK_SIZ);
        CTR_INC(prng_ctx.ctr);
        prng_ctx.ix = siz;
        while (siz--)
            buf[siz] = prng_ctx.lo[siz];
    }
    return 0;
}

