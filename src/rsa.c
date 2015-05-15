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

#include "cry/rsa.h"
#include "cry/rand.h"
#include <string.h>
#include <stdlib.h>

/*
 * Compute c = m^e mod n.
 *
 * Each rsa_operate operation returns a result mod n.
 * This means that you can’t encrypt blocks larger than n without losing
 * information, so you need to chop the input up into blocks of length n
 * or less.
 * RSA works on blocks of data. Each block includes a header and some
 * padding (of at least 11 bytes), so the resulting input blocks are
 * modulus_length-11 bytes minimum. The header is pretty simple:
 * it’s a 0 byte, followed by a padding identifier of 0, 1, or 2.
 * For RSA encryption, always use padding identifier 2, which indicates
 * that the following bytes, up to the first 0 byte, are padding and
 * should be discarded. Everything following the first 0 byte, up to the
 * length of the modulus n in bytes, is data.
 * Note that RSA pads at the beginning of its block.
 * To encrypt a very small amount of data, or the non-aligned end of a
 * long block of data, you need to pad it to complicate brute-force attacks.
 */

#if 1

#include <stdio.h>

static int nozero_rand(unsigned char *dst, unsigned int n)
{
    int res, k;
    unsigned char buf[16];

    if ((res = cry_rand(dst, n)) < 0)
        return res;
    while (n-- > 0) {
        if (dst[n] == 0) {
            cry_rand(buf, sizeof(buf));
            for (k = 0; k < sizeof(buf); k++) {
                if (buf[k] != 0) {
                    dst[n] = buf[k];
                    break;
                }
            }
            if (k > 0) {
                fprintf(stderr, "FIXED AFTER: %d\n", k);
            }
            if (dst[n] == 0) {
                fprintf(stderr, "still 0... exiting\n");
                exit(0);
                return -1; /* No Way!!! Probability is ~0 */
            }
        }
    }
    return 0;
}

/*
 * Encrypts a sequence of characters
 */
int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen)
{
    cry_mpi c, m;
    int res, modulus_siz, block_siz, encrypt_siz = 0;
    unsigned char *padded_block, *tmp;

    *out = NULL;

    modulus_siz = cry_mpi_count_bytes(&ctx->m);
    padded_block = malloc(modulus_siz);
    if (!padded_block)
        return -1;

    if ((res = cry_mpi_init_list(&c, &m, 0)) != 0) {
        free(padded_block);
        return res;
    }

    while (inlen) {
        encrypt_siz += modulus_siz;
        block_siz = (inlen < modulus_siz - 11) ?
                    inlen : (modulus_siz - 11);
        memset(padded_block, 0, modulus_siz);
        memcpy(padded_block + (modulus_siz - block_siz), in, block_siz);
        padded_block[1] = 0x02; /* Block type */

        if (ctx->flags & CRY_RSA_FLAG_SIGN) {
            memset(padded_block + 2, 0xFF, modulus_siz - block_siz - 3);
        } else {
            if ((res = nozero_rand(padded_block + 2,
                            modulus_siz - block_siz - 3)) < 0)
                return res;
        }

        if ((res = cry_mpi_load_bin(&m, padded_block, modulus_siz)) != 0)
            break;
        if ((res = cry_mpi_mod_exp(&c, &m, &ctx->e, &ctx->m)) != 0)
            break;

        tmp = realloc(*out, encrypt_siz);
        if (tmp == NULL) {
            res = -1;
            break;
        }
        *out = tmp;

        if ((res = cry_mpi_store_bin(&c, *out + (encrypt_siz - modulus_siz),
                                     modulus_siz, 1)) < 0)
            break;

        inlen -= block_siz;
        in += block_siz;
    }

    free(padded_block);
    cry_mpi_clear_list(&c, &m, 0);
    *outlen = encrypt_siz;
    return res;
}

/*
 * Decrypts a sequence of characters
 */
int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen)
{
    cry_mpi c, m;
    int res, i, modulus_siz, out_siz = 0;
    unsigned char *padded_block, *tmp;

    *out = NULL;

    modulus_siz = cry_mpi_count_bytes(&ctx->m);
    padded_block = malloc(modulus_siz);
    if (!padded_block)
        return -1;

    if ((res = cry_mpi_init_list(&c, &m, 0)) != 0) {
        free(padded_block);
        return res;
    }

    while (inlen) {
        if (inlen < modulus_siz) {
            /* Input must be an even multiple of key modulus */
            res = -1;
            break;
        }

        if ((res = cry_mpi_load_bin(&c, in, modulus_siz)) != 0)
            break;
        if ((res = cry_mpi_mod_exp(&m, &c, &ctx->d, &ctx->m)) != 0)
            break;
        if (cry_mpi_store_bin(&m, padded_block, modulus_siz, 1) != modulus_siz)
            break;

        if (padded_block[1] > 0x02) {
            /* Decryption error or unrecognized block type */
            res = -1;
            break;
        }

        /*
         * Find next 0 byte after padding type byte; this signifies
         * start of data
         */
        i = 2;
        while (padded_block[i++])
            ;

        out_siz += modulus_siz - i;
        tmp = realloc(*out, out_siz);
        if (tmp == NULL) {
            res = -1;
            break;
        }
        *out = tmp;
        memcpy(*out + (out_siz - (modulus_siz - i)),
                padded_block + i, modulus_siz - i);

        inlen -= modulus_siz;
        in += modulus_siz;
    }

    free(padded_block);
    cry_mpi_clear_list(&c, &m, 0);
    *outlen = out_siz;
    return res;
}


#else

/* Textbook implementation */

static int rsa_operate(unsigned char *out, const unsigned char *in,
                       size_t siz, cry_mpi *exp, cry_mpi *mod)
{
    int ret;
    cry_mpi a;

    if ((ret = cry_mpi_init_bin(&a, in, siz)) != 0)
        return ret;
    if ((ret = cry_mpi_mod_exp(&a, &a, exp, mod)) == 0)
        ret = cry_mpi_store_bin(&a, out, a.used, 1);
    cry_mpi_clear(&a);
    return ret;
}

int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char *out,
                    const unsigned char *in, size_t siz)
{
    return rsa_operate(out, in, siz, &ctx->e, &ctx->m);
}

int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char *out,
                    const unsigned char *in, size_t siz)
{
    return rsa_operate(out, in, siz, &ctx->d, &ctx->m);
}

#endif

