#include "cry/rsa.h"
#include "cry/prng.h"
#include <string.h>
#include <stdlib.h>

/*
 * Compute c = m^e mod n.
 *
 * Each rsa_operate operation returns a result mod n.
 * This means that you can't encrypt blocks larger than n without losing
 * information, so you need to chop the input up into blocks of length n
 * or less.
 * RSA works on blocks of data. Each block includes a header and some
 * padding (of at least 11 bytes), so the resulting input blocks are
 * modulus_length-11 bytes minimum. The header is pretty simple:
 * it's a 0 byte, followed by a padding identifier of 0, 1, or 2.
 * For RSA encryption, always use padding identifier 2, which indicates
 * that the following bytes, up to the first 0 byte, are padding and
 * should be discarded. Everything following the first 0 byte, up to the
 * length of the modulus n in bytes, is data.
 * Note that RSA pads at the beginning of its block.
 * To encrypt a very small amount of data, or the non-aligned end of a
 * long block of data, you need to pad it to complicate brute-force attacks.
 */

#include <stdio.h>

static int nozero_rand(unsigned char *dst, unsigned int n)
{
    int res, k;
    unsigned char buf[16];

    if ((res = cry_prng_rand(dst, n)) < 0)
        return res;
    while (n-- > 0) {
        if (dst[n] == 0) {
            cry_prng_rand(buf, sizeof(buf));
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
int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *out_siz,
                    const unsigned char *in, size_t in_siz)
{
    cry_mpi c, m;
    int res, mod_siz, block_siz;
    unsigned char *padded_block;

    *out = NULL;
    *out_siz = 0;

    mod_siz = cry_mpi_count_bytes(&ctx->m);
    padded_block = malloc(mod_siz);
    if (!padded_block)
        return -1;

    if ((res = cry_mpi_init_list(&c, &m, NULL)) != 0) {
        free(padded_block);
        return res;
    }

    while (in_siz) {
        *out_siz += mod_siz;
        block_siz = (in_siz < mod_siz - 11) ?
                    in_siz : (mod_siz - 11);
        memset(padded_block, 0, mod_siz);
        memcpy(padded_block + (mod_siz - block_siz), in, block_siz);
        padded_block[1] = 0x02; /* Block type */

        if (ctx->flags & CRY_RSA_FLAG_SIGN) {
            memset(padded_block + 2, 0xFF, mod_siz - block_siz - 3);
        } else {
            if ((res = nozero_rand(padded_block + 2,
                            mod_siz - block_siz - 3)) < 0)
                return res;
        }

        *out = realloc(*out, *out_siz);
        if (*out == NULL) {
            res = -1;
            break;
        }

        if ((res = cry_mpi_load_bin(&m, padded_block, mod_siz)) != 0)
            break;
        if ((res = cry_mpi_mod_exp(&c, &m, &ctx->e, &ctx->m)) != 0)
            break;
        if (cry_mpi_store_bin(&c, *out + (*out_siz - mod_siz),
                              mod_siz, 1) != mod_siz) {
            res = -1;
            break;
        }

        in_siz -= block_siz;
        in += block_siz;
    }

    free(padded_block);
    cry_mpi_clear_list(&c, &m, NULL);
    if (res != 0) {
        *out_siz = 0;
        *out = NULL;
    }
    return res;
}

/*
 * Decrypts a sequence of characters
 */
int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *out_siz,
                    const unsigned char *in, size_t in_siz)
{
    cry_mpi c, m;
    int res, i, mod_siz;
    unsigned char *padded_block;

    *out = NULL;
    *out_siz = 0;

    mod_siz = cry_mpi_count_bytes(&ctx->m);
    padded_block = malloc(mod_siz);
    if (!padded_block)
        return -1;

    if ((res = cry_mpi_init_list(&c, &m, NULL)) != 0) {
        free(padded_block);
        return res;
    }

    while (in_siz) {
        if (in_siz < mod_siz) {
            /* Input must be an even multiple of key modulus */
            res = -1;
            break;
        }

        if ((res = cry_mpi_load_bin(&c, in, mod_siz)) != 0)
            break;
        if ((res = cry_mpi_mod_exp(&m, &c, &ctx->d, &ctx->m)) != 0)
            break;
        if (cry_mpi_store_bin(&m, padded_block, mod_siz, 1) != mod_siz) {
            res = -1;
            break;
        }

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

        *out_siz += mod_siz - i;
        *out = realloc(*out, *out_siz);
        if (*out == NULL) {
            res = -1;
            break;
        }
        memcpy(*out + (*out_siz - (mod_siz - i)),
                padded_block + i, mod_siz - i);

        in_siz -= mod_siz;
        in += mod_siz;
    }

    free(padded_block);
    cry_mpi_clear_list(&c, &m, NULL);
    if (res != 0) {
        *out_siz = 0;
        *out = NULL;
    }
    return res;
}
