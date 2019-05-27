#include <cry/hill.h>
#include <cry/prng.h>
#include "misc.h"
#include <string.h>


static void mtx_minor(unsigned char *out, unsigned char *in,
                      size_t i, size_t j, size_t n)
{
    size_t r, c, h, k;

    h = 0;
    for (r = 0; r < n; r++) {
        if (r == i)
            continue;
        k = 0;
        for (c = 0; c < n; c++) {
            if (c == j)
                continue;
            out[h*(n-1) + k] = in[r*n + c];
            k++;
        }
        h++;
    }
}

static unsigned char mtx_det(unsigned char *mtx, size_t n)
{
    size_t i;
    long det;
    unsigned char b[CRY_HILL_KEYLEN_MAX];

    if (n > 2) {
        det = 0;
        for (i = 0; i < n; i++) {
            mtx_minor(b, mtx, 0, i, n);
            det += mtx[i] * ((i & 1) ? -1 : 1) * mtx_det(b, n-1);
            det %= 256;
            if (det < 0)
                det += 256;
        }
    } else if (n == 2) {
        det = mtx[0]*mtx[3] - mtx[1]*mtx[2];
        det %= 256;
        if (det < 0)
            det += 256;
    } else if (n == 1) {
        det = mtx[0];
    } else { /* n == 0 */
        det = 1;
    }
    return (unsigned char)det;
}

#define SWAP(a, b) do { a ^= b; b ^= a; a ^= b; } while (0)

static void mtx_transpose(unsigned char *out, unsigned char *in, size_t n)
{
    size_t i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            out[i*n+j] = in[j*n+i];
    }
}

static void mtx_invert(unsigned char *imtx, unsigned char *mtx,
                       size_t n, unsigned char idet)
{
    unsigned char t[CRY_HILL_KEYLEN_MAX]; /* Transpose */
    unsigned char b[CRY_HILL_KEYLEN_MAX]; /* Minor */
    size_t i, j;
    long c;

    mtx_transpose(t, mtx, n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            mtx_minor(b, t, i, j, n);
            c = mtx_det(b, n-1);
            if (((i+j) & 1))
                c = -c;
            c *= idet;
            c %= 256;
            if (c < 0)
                c += 256;
            imtx[i*n + j] = (unsigned char)c;
        }
    }
}

static void mtx_vec_mul(unsigned char *r, const unsigned char *m,
                        const unsigned char *v, size_t n)
{
    size_t i, j;
    long res;

    for (i = 0; i < n; i++) {
        res = 0;
        for (j = 0; j < n; j++)
            res += m[i*n + j] * v[j];
        res %= 256;
        if (res < 0)
            res += 256;
        r[i] = (unsigned char)res;
    }
}

static size_t keylen_trim(size_t *keylen)
{
    size_t n, z;

    if (*keylen > CRY_HILL_KEYLEN_MAX)
        *keylen = CRY_HILL_KEYLEN_MAX;
    for (n = 1; (z=n*n) < *keylen; n++) {
    }
    if (z > *keylen) {
        n--;
        *keylen = n*n;
    }
    return n;
}

static void hill_operate(struct cry_hill_ctx *ctx, unsigned char *out,
                         const unsigned char *in, size_t len,
                         const unsigned char *key)
{
    unsigned char v[CRY_HILL_KEYDIM_MAX];

    size_t n;

    while (len > 0) {
        n = CRY_MIN(ctx->n, len);
        memcpy(v, in, n);
        if (n < ctx->n)
            memset(v + n, 0, ctx->n - n);
        mtx_vec_mul(out, key, v, ctx->n);
        out += n;
        in  += n;
        len -= n;
    }
}

void cry_hill_encrypt(struct cry_hill_ctx *ctx, unsigned char *out,
                  const unsigned char *in, size_t len)
{
    hill_operate(ctx, out, in, len, ctx->key);
}


void cry_hill_decrypt(struct cry_hill_ctx *ctx, unsigned char *out,
                      const unsigned char *in, size_t len)
{
    hill_operate(ctx, out, in, len, ctx->ikey);
}


static int keygen(unsigned char *key, unsigned char *ikey, size_t keylen,
                  unsigned int trials)
{
    int res = -1;
    unsigned char det, idet;
    size_t n;

    n = keylen_trim(&keylen);

    do {
        if (trials > 0) {
            cry_prng_aes_rand(key, keylen);
            trials--;
        }

        det = mtx_det(key, n);
        /*
         * Determinant should be non-zero (to be invertible) and
         * should be odd, since if gcd(det, 256)=1 -> is invertible
         */
        if ((det & 1) != 0) {
            idet = (unsigned char) cry_long_inv(det, 256);
            mtx_invert(ikey, key, n, idet);
            res = 0;
            trials = 0;
        }
    } while (trials > 0);

    return res;
}

int cry_hill_init(struct cry_hill_ctx *ctx, const unsigned char *key,
              const unsigned char *ikey, size_t keylen)
{
    int res = 0;
    unsigned int trials = 0;

    memset(ctx, 0, sizeof(*ctx));

    ctx->n = keylen_trim(&keylen);

    if (key != NULL)
        memcpy(ctx->key, key, keylen);
    else
        trials = CRY_HILL_KEYGEN_TRIALS;

    if (ikey == NULL)
        res = keygen(ctx->key, ctx->ikey, keylen, trials);
    else if (key != NULL)
        memcpy(ctx->ikey, ikey, keylen);
    else
        res = -1;
    return res;
}
