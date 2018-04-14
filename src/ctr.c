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

#include "cry/ctr.h"
#include "memxor.h"
#include "misc.h"
#include <string.h>

#define CTR_NBLOCKS     4

void cry_ctr_key_set(struct cry_ctr_ctx *ctr, const unsigned char *key,
                     unsigned int size)
{
    void *ciph = ctr->ciph_ctx;
    cry_ciph_key_set_f key_set = ctr->ciph_itf->key_set;

    key_set(ciph, key, size);
}

void cry_ctr_iv_set(struct cry_ctr_ctx *ctr, const unsigned char *iv,
                    unsigned int size)
{
    size = CRY_MIN(CRY_CTR_BLOCK_SIZE, size);
    memcpy(ctr->v, iv, size);
    memset(ctr->v + size, 0, CRY_CTR_BLOCK_SIZE - size);
}

void cry_ctr_encrypt(struct cry_ctr_ctx *ctr, unsigned char *dst,
                     const unsigned char *src, unsigned int len)
{
    unsigned char buffer[CRY_CTR_BLOCK_SIZE * CTR_NBLOCKS];
    void *ciph = ctr->ciph_ctx;
    cry_ciph_encrypt_f encrypt = ctr->ciph_itf->encrypt;

    if (src != dst) {
        if (len == CRY_CTR_BLOCK_SIZE) {
            encrypt(ciph, dst, ctr->v, CRY_CTR_BLOCK_SIZE);
            CRY_INCREMENT_BE(ctr->v, CRY_CTR_BLOCK_SIZE);
            cry_memxor(dst, src, CRY_CTR_BLOCK_SIZE);
        } else {
            unsigned left;
            unsigned char *p;

            for (p = dst, left = len;
                 left >= CRY_CTR_BLOCK_SIZE;
                 left -= CRY_CTR_BLOCK_SIZE, p += CRY_CTR_BLOCK_SIZE) {
                memcpy (p, ctr->v, CRY_CTR_BLOCK_SIZE);
                CRY_INCREMENT_BE(ctr->v, CRY_CTR_BLOCK_SIZE);
            }

            encrypt(ciph, dst, dst, len - left);
            cry_memxor(dst, src, len - left);

            if (left != 0) {
                encrypt(ciph, buffer, ctr->v, CRY_CTR_BLOCK_SIZE);
                CRY_INCREMENT_BE(ctr->v, CRY_CTR_BLOCK_SIZE);
                cry_memxor3(dst + len - left, src + len - left, buffer, left);
            }
        }
    } else {
        if (len <= CRY_CTR_BLOCK_SIZE) {
            encrypt(ciph, buffer, ctr->v, CRY_CTR_BLOCK_SIZE);
            CRY_INCREMENT_BE(ctr->v, CRY_CTR_BLOCK_SIZE);
            cry_memxor3(dst, src, buffer, len);
        } else {
            unsigned chunk = CRY_CTR_BLOCK_SIZE * CTR_NBLOCKS;

            for ( ; len >= chunk;
                    len -= chunk, src += chunk, dst += chunk) {
                unsigned n;
                unsigned char *p;

                for (n = 0, p = buffer; n < CTR_NBLOCKS;
                     n++, p += CRY_CTR_BLOCK_SIZE) {
                    memcpy(p, ctr->v, CRY_CTR_BLOCK_SIZE);
                    CRY_INCREMENT_BE(ctr->v, CRY_CTR_BLOCK_SIZE);
                }
                encrypt(ciph, buffer, buffer, chunk);
                cry_memxor(dst, buffer, chunk);
            }

            if (len != 0) {
                /* Final, possibly partial, blocks */
                for (chunk = 0; chunk < len; chunk += CRY_CTR_BLOCK_SIZE) {
                    memcpy (buffer + chunk, ctr->v, CRY_CTR_BLOCK_SIZE);
                    CRY_INCREMENT_BE(ctr->v, CRY_CTR_BLOCK_SIZE);
                }
                encrypt(ciph, buffer, buffer, chunk);
                cry_memxor3(dst, src, buffer, len);
            }
        }
    }
}

