#include <cry/rsa.h>
#include <cry/prng.h>
#include <string.h>
#include <stdlib.h>
#include "misc.h"


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

static int padding_add(unsigned char *dst, size_t dlen,
                       const unsigned char *src, size_t slen,
                       int padding, int sign)
{
    int res = 0;
    unsigned char *p = dst;
    size_t j;

    if (slen > (dlen - 11))
        return -1;

    *(p++) = 0;

    /* pad out with non-zero random data */
    j = dlen - 3 - slen;

    switch (padding) {
        case CRY_RSA_PADDING_PKCS_V15:
            if (sign == 0) {
                *(p++) = 2;     /* Block Type */
                res = nozero_rand(p, j);
            } else {
                *(p++) = 1;     /* Block Type */
                memset(p, 0xFF, j);
            }
            break;
        case CRY_RSA_PADDING_PKCS_V21:
            /* not implemented yet */
            res = -1;
            break;
        default:
            break;
    }
    if (res != 0)
        return res;
    p += j;
    *(p++) = '\0';
    memcpy(p, src, slen);
    return 0;
}

static int padding_del(unsigned char *dst, size_t dlen,
                       const unsigned char *src, size_t slen,
                       int padding, int sign)
{
    size_t i;

    (void)padding; /* unused */

    if (src[0] != 0 || (sign != 0 && src[1] != 0x01) ||
            (sign == 0 && src[1] != 0x02))
        /* Unrecognized block type */
        return -1;

    /*
     * Find next 0 byte after padding type byte;
     * this signifies start of data
     */
    for (i = 2; i < slen && src[i] != 0; i++) {
        if (sign != 0 && src[i] != 0xFF)
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

static int encrypt_block(cry_rsa_ctx *ctx, unsigned char *out,
                         const unsigned char *in, size_t inlen,
                         int sign)
{
    int res;
    unsigned char *buf;
    size_t mod_len;
    cry_mpi c, m;
    cry_mpi *key;

    key = (sign == 0) ? &ctx->e : &ctx->d;
    mod_len = cry_mpi_count_bytes(&ctx->n);
    buf = malloc(mod_len);
    if (buf == NULL)
        return -1;

    if ((res = cry_mpi_init_list(&c, &m, (cry_mpi *)NULL)) != 0)
        goto e;

    if ((res = padding_add(buf, mod_len, in, inlen, ctx->padding, sign)) != 0)
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

static int decrypt_block(cry_rsa_ctx *ctx, unsigned char *out,
                         const unsigned char *in, size_t inlen,
                         int sign)
{
    int res = 0;
    unsigned char *buf;
    size_t mod_len;
    cry_mpi c, m;
    cry_mpi *key;

    key = (sign == 0) ? &ctx->d : &ctx->e;
    mod_len = cry_mpi_count_bytes(&ctx->n);
    if (inlen != mod_len)
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

    res = padding_del(out, mod_len, buf, mod_len, ctx->padding, sign);

e1: cry_mpi_clear_list(&c, &m, (cry_mpi *)NULL);
e:  free(buf);
    return res;
}


static int encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                   const unsigned char *in, size_t inlen, int sign)
{
    int res;
    size_t mod_siz, block_siz;

    *out = NULL;
    *outlen = 0;
    mod_siz = cry_mpi_count_bytes(&ctx->n);

    while (inlen) {
        *out = realloc(*out, *outlen + mod_siz);
        if (*out == NULL) {
            res = -1;
            break;
        }

        block_siz = (inlen < mod_siz - 11) ? inlen : (mod_siz - 11);

        res = encrypt_block(ctx, *out + *outlen, in, block_siz, sign);
        if (res != 0)
            break;

        *outlen += mod_siz;
        inlen -= block_siz;
        in += block_siz;
    }

    if (res != 0) {
        if (*out != NULL) {
            free(*out);
            *out = NULL;
        }
        *outlen = 0;
    }
    return res;
}

static int decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                   const unsigned char *in, size_t inlen, int sign)
{
    int res;
    size_t mod_siz;

    *out = NULL;
    *outlen = 0;
    mod_siz = cry_mpi_count_bytes(&ctx->n);

    while (inlen > 0) {
        if (inlen < mod_siz) {
            /* Input must be an even multiple of key modulus */
            res = -1;
            break;
        }

        *out = realloc(*out, *outlen + mod_siz);

        res = decrypt_block(ctx, *out, in, mod_siz, sign);
        if (res < 0)
            break;
        *outlen += res;
        inlen -= mod_siz;
        in += mod_siz;
        res = 0; /* decrypt block returns the number of decrypted bytes */
    }

    if (res != 0) {
        if (*out != NULL) {
            free(*out);
            *out = NULL;
        }
        *outlen = 0;
    }
    return res;
}


int cry_rsa_encrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen)
{
    return encrypt(ctx, out, outlen, in, inlen, 0);
}

int cry_rsa_decrypt(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                    const unsigned char *in, size_t inlen)
{
    return decrypt(ctx, out, outlen, in, inlen, 0);
}


int cry_rsa_sign(cry_rsa_ctx *ctx, unsigned char **out, size_t *outlen,
                 const unsigned char *in, size_t inlen)
{
    return encrypt(ctx, out, outlen, in, inlen, 1);
}

int cry_rsa_verify(cry_rsa_ctx *ctx, const unsigned char *sig, size_t siglen,
                   const unsigned char *in, size_t inlen)
{
    int res;
    unsigned char *out;
    size_t outlen;

    res = decrypt(ctx, &out, &outlen, sig, siglen, 1);
    if (res == 0) {
        if (outlen != inlen || memcmp(out, in, inlen) != 0)
            res = -1;
        free(out);
    }
    return res;
}


#define MAX_ITER    10000

int cry_rsa_keygen(cry_rsa_ctx *ctx, size_t bits)
{
    int res;
    cry_mpi phi, p1, q1, one;
    cry_mpi_digit one_dig = 1;
    size_t hbits = bits >> 1;
    unsigned int i, j;

    one.alloc = 1;
    one.used = 1;
    one.sign = 0;
    one.data = &one_dig;

    if ((res = cry_mpi_init_list(&p1, &q1, &phi, (cry_mpi *)NULL)) != 0)
        goto e;
    i = MAX_ITER;
    if ((res = cry_mpi_prime(&ctx->p, hbits, &i)) != 0)
        goto e;
    for (j = 0; j < MAX_ITER; j++) {
        i = MAX_ITER;
        if ((res = cry_mpi_prime(&ctx->q, hbits, &i)) != 0)
            goto e;
        if (cry_mpi_cmp(&ctx->q, &ctx->p) != 0)
            break;
    }
    if ((res = cry_mpi_mul(&ctx->n, &ctx->p, &ctx->q)) != 0)
        goto e;

    if ((res = cry_mpi_sub(&p1, &ctx->p, &one)) != 0)
        goto e;
    if ((res = cry_mpi_sub(&q1, &ctx->q, &one)) != 0)
        goto e;
    if ((res = cry_mpi_mul(&phi, &p1, &q1)) != 0)
        goto e;

    /* Find key */
    for (i = 0; i < MAX_ITER; i++) {
        cry_mpi_rand(&ctx->e, bits);
        if ((res = cry_mpi_inv(&ctx->d, &ctx->e, &phi)) == 0)
            break;
    }
e:  cry_mpi_clear_list(&p1, &q1, &phi, (cry_mpi *)NULL);
    return res;
}

int cry_rsa_init(cry_rsa_ctx *ctx, int padding)
{
    ctx->padding = padding;
    return cry_mpi_init_list(&ctx->d, &ctx->e, &ctx->n, &ctx->p, &ctx->q,
                             (cry_mpi *)NULL);
}

void cry_rsa_clear(cry_rsa_ctx *ctx)
{
    ctx->padding = 0;
    cry_mpi_clear_list(&ctx->d, &ctx->e, &ctx->n, &ctx->p, &ctx->q,
                       (cry_mpi *)NULL);
}
