#include <cry/rsa.h>
#include <cry/prng.h>
#include <string.h>
#include <stdlib.h>
#include "misc.h"


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

static int nozero_rand(unsigned char *dst, size_t n)
{
    int res;

    if ((res = cry_prng_aes_rand(dst, n)) < 0)
        return res;
    while (n-- > 0) {
        if (dst[n] == 0)
            dst[n] = (n != 0) ? n : 1;
    }
    return res;
}

static int rsa_padding_add(unsigned char *dst, size_t dlen,
                           const unsigned char *src, size_t slen,
                           int padding)
{
    int res = 0;
    unsigned char *p = dst;
    size_t j;

    if (slen > (dlen - 11))
        return -1;

    *(p++) = 0;
    *(p++) = 2;     /* Public Key BT (Block Type) */

    /* pad out with non-zero random data */
    j = dlen - 3 - slen;

    switch (padding) {
    case CRY_RSA_PADDING_PKCS1:
        res = nozero_rand(p, j);
        break;
    case CRY_RSA_PADDING_PKCS1_OEAP:
        /* not implemented yet */
        res = -1;
        break;
    case CRY_RSA_PADDING_PKCS1_PSS:
        memset(p, 0xFF, j);
        break;
    default:
        res = -1;
        break;
    }
    if (res != 0)
        return res;
    p += j;
    *(p++) = '\0';
    memcpy(p, src, slen);
    return 0;
}

static int rsa_padding_del(unsigned char *dst, size_t dlen,
                           const unsigned char *src, size_t slen,
                           int padding)
{
    size_t i;

    if (src[0] != 0 || src[1] != 0x02)
        /* Unrecognized block type */
        return -1;
    /*
     * Find next 0 byte after padding type byte;
     * this signifies start of data
     */
    for (i = 2; i < slen && src[i] != 0; i++) {
        if (padding == CRY_RSA_PADDING_PKCS1_PSS && src[i] != 0xFF)
            return -1;
    }
    if(i == slen)
        return -1;
    i++; /* skip zero */
    if (slen - i < dlen)
        dlen = slen - i;
    memcpy(dst, src + i, dlen);
    return dlen;
}

static int cry_rsa_encrypt_block(cry_rsa_ctx *ctx, unsigned char *out,
                                 const unsigned char *in, size_t in_siz)
{
    int res;
    unsigned char *buf;
    size_t mod_len;
    cry_mpi c, m;
    cry_mpi *key;

    key = (ctx->padding == CRY_RSA_PADDING_PKCS1_PSS) ? &ctx->d : &ctx->e;
    mod_len = cry_mpi_count_bytes(&ctx->n);
    buf = malloc(mod_len);
    if (buf == NULL)
        return -1;

    if ((res = cry_mpi_init_list(&c, &m, (cry_mpi *)NULL)) != 0)
        goto e;

    if ((res = rsa_padding_add(buf, mod_len, in, in_siz, ctx->padding)) != 0)
        goto e1;

    if ((res = cry_mpi_load_bin(&m, buf, mod_len)) != 0)
        goto e1;
    if ((res = cry_mpi_mod_exp(&c, &m, key, &ctx->n)) != 0)
        goto e1;

    res = cry_mpi_store_bin(&c, out, mod_len, 1);

e1: cry_mpi_clear_list(&c, &m, (cry_mpi *)NULL);
e:  free(buf);
    return res;
}

static int cry_rsa_decrypt_block(cry_rsa_ctx *ctx, unsigned char *out,
                                 const unsigned char *in, size_t in_siz)
{
    int res = 0;
    unsigned char *buf;
    size_t mod_len;
    cry_mpi c, m;
    cry_mpi *key;

    key = (ctx->padding == CRY_RSA_PADDING_PKCS1_PSS) ? &ctx->e : &ctx->d;

    mod_len = cry_mpi_count_bytes(&ctx->n);
    if (in_siz != mod_len)
        return -1;
    buf = malloc(mod_len);
    if (buf == NULL)
        return -1;

    if ((res = cry_mpi_init_list(&c, &m, (cry_mpi *)NULL)) != 0)
        goto e;

    if ((res = cry_mpi_load_bin(&c, in, mod_len)) != 0)
        goto e1;
    if ((res = cry_mpi_mod_exp(&m, &c, key, &ctx->n)) != 0)
        goto e1;
    if ((res = cry_mpi_store_bin(&m, buf, mod_len, 1)) != 0)
        goto e1;

    res = rsa_padding_del(out, mod_len, buf, mod_len, ctx->padding);

e1: cry_mpi_clear_list(&c, &m, (cry_mpi *)NULL);
e:  free(buf);
    return res;
}

/*
 * Encrypts a sequence of characters
 */
int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *out_siz,
                    const unsigned char *in, size_t in_siz)
{
    int res;
    size_t mod_siz, block_siz;

    *out = NULL;
    *out_siz = 0;
    mod_siz = cry_mpi_count_bytes(&ctx->n);

    while (in_siz) {
        *out = realloc(*out, *out_siz + mod_siz);
        if (*out == NULL) {
            res = -1;
            break;
        }

        block_siz = (in_siz < mod_siz - 11) ? in_siz : (mod_siz - 11);

        res = cry_rsa_encrypt_block(ctx, *out + *out_siz, in, block_siz);
        if (res != 0)
            break;

        *out_siz += mod_siz;
        in_siz -= block_siz;
        in += block_siz;
    }

    if (res != 0) {
        if (*out != NULL) {
            free(*out);
            *out = NULL;
        }
        *out_siz = 0;
    }
    return res;
}

/*
 * Decrypts a sequence of characters
 */
int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *out_siz,
                    const unsigned char *in, size_t in_siz)
{
    int res;
    size_t mod_siz;

    *out = NULL;
    *out_siz = 0;
    mod_siz = cry_mpi_count_bytes(&ctx->n);

    while (in_siz > 0) {
        if (in_siz < mod_siz) {
            /* Input must be an even multiple of key modulus */
            res = -1;
            break;
        }

        *out = realloc(*out, *out_siz + mod_siz);

        res = cry_rsa_decrypt_block(ctx, *out, in, mod_siz);
        if (res < 0)
            break;
        *out_siz += res;
        in_siz -= mod_siz;
        in += mod_siz;
        res = 0; /* decrypt block returns the number of decrypted bytes */
    }

    if (res != 0) {
        if (*out != NULL) {
            free(*out);
            *out = NULL;
        }
        *out_siz = 0;
    }
    return res;
}

#define MAX_ITER    10000

int cry_rsa_keygen(cry_rsa_ctx *ctx, size_t bits)
{
    int res;
    cry_mpi phi, p, q, p1, q1, one;
    cry_mpi_digit one_dig = 1;
    size_t hbits = bits >> 1;
    unsigned int i;


    if ((res = cry_mpi_init_list(&ctx->d, &ctx->e, &ctx->n,
                                 (cry_mpi *)NULL)) != 0)
        return res;
    if ((res = cry_mpi_init_list(&p, &q, &p1, &q1, &phi,
                                 (cry_mpi *)NULL)) != 0)
        goto e; /* FIXME: shall release only d,e,m */
    i = MAX_ITER;
    if ((res = cry_mpi_prime(&p, hbits, &i)) != 0)
        goto e;
    i = MAX_ITER;
    if ((res = cry_mpi_prime(&q, hbits, &i)) != 0)
        goto e;
    if ((res = cry_mpi_mul(&ctx->n, &p, &q)) != 0)
        goto e;

    one.alloc = 1;
    one.used = 1;
    one.sign = 0;
    one.data = &one_dig;

    if ((res = cry_mpi_sub(&p1, &p, &one)) != 0)
        goto e;
    if ((res = cry_mpi_sub(&q1, &q, &one)) != 0)
        goto e;
    if ((res = cry_mpi_mul(&phi, &p1, &q1)) != 0)
        goto e;

    /* Find key */
    for (i = 0; i < MAX_ITER; i++) {
        cry_mpi_rand(&ctx->e, bits);
        if ((res = cry_mpi_inv(&ctx->d, &ctx->e, &phi)) == 0)
            break;
    }
e:  cry_mpi_clear_list(&p, &q, &p1, &q1, &phi, (cry_mpi *)NULL);
    if (res != 0)
        cry_mpi_clear_list(&ctx->d, &ctx->e, &ctx->n, (cry_mpi *)NULL);
    return res;
}

int cry_rsa_init(cry_rsa_ctx *ctx, int padding)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->padding = padding;
    return 0;
}
