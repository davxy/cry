#include <cry/ecdsa.h>
#include "mpi/mpi_pvt.h"

#define CHK0(exp) CRY_CHK(exp, e0)
#define CHK1(exp) CRY_CHK(exp, e1)

int cry_ecdsa_sign(cry_ecdsa_ctx *ctx, cry_ecdsa_signature *sign,
                   const unsigned char *in, size_t len)
{
    int res;
    cry_mpi k, z;
    cry_ecp X;

    if ((res = cry_mpi_init_list(&k, &z, (cry_mpi *)NULL)) != 0)
        return res;

    CHK0(cry_ecp_init(&X));

    /* This should be a random number between 0 and n-1 */
#if 0
    unsigned char K[] = {
        0x9E, 0x56, 0xF5, 0x09, 0x19, 0x67, 0x84, 0xD9, 0x63, 0xD1, 0xC0,
        0xA4, 0x01, 0x51, 0x0E, 0xE7, 0xAD, 0xA3, 0xDC, 0xC5, 0xDE, 0xE0,
        0x4B, 0x15, 0x4B, 0xF6, 0x1A, 0xF1, 0xD5, 0xA6, 0xDE, 0xCE
    };
    CHK1(cry_mpi_load_bin(&k, K, sizeof(K)));
#else
    CHK1(cry_mpi_rand_range(&k, &ctx->ec.n));
#endif

    CHK1(cry_mpi_copy(&X.x, &ctx->ec.g.x));
    CHK1(cry_mpi_copy(&X.y, &ctx->ec.g.y));

    CHK1(cry_ecp_mul(&X, &X, &k, &ctx->ec));

    /* r = x mod n */
    CHK1(cry_mpi_mod(&sign->r, &X.x, &ctx->ec.n));

    /* z is the L_n leftmost bits of hash - cannot be longer than n */
    if (ctx->ec.n.used * CRY_MPI_DIGIT_BYTES < len)
        len = ctx->ec.n.used * CRY_MPI_DIGIT_BYTES;
    CHK1(cry_mpi_load_bin(&z, in, len));

    /* s = (inv(k) * (z + r*d_a)) mod n */
    CHK1(cry_mpi_inv(&k, &k, &ctx->ec.n));
    CHK1(cry_mpi_copy(&sign->s, &ctx->d));
    CHK1(cry_mpi_mul(&sign->s, &sign->s, &sign->r));
    CHK1(cry_mpi_add(&sign->s, &sign->s, &z));
    CHK1(cry_mpi_mul(&sign->s, &sign->s, &k));
    CHK1(cry_mpi_mod(&sign->s, &sign->s, &ctx->ec.n));

e1: cry_ecp_clear(&X);
e0: cry_mpi_clear_list(&k, &z, (cry_mpi *)NULL);
    return 0;
}

int cry_ecdsa_verify(cry_ecdsa_ctx *ctx, const cry_ecdsa_signature *sign,
                     const unsigned char *in, size_t len)
{
    int res;
    cry_mpi z, w;
    cry_ecp G, Q;

    if ((res = cry_mpi_init_list(&z, &w, (cry_mpi *)NULL)) != 0)
        return res;

    CHK0(cry_ecp_init_list(&G, &Q, (cry_ecp *)NULL));

    /* w = inv(s) mod n */
    CHK1(cry_mpi_copy(&w, &sign->s));
    CHK1(cry_mpi_inv(&w, &w, &ctx->ec.n));

    /* z is the L_n leftmost bits of the hash, eventually truncate */
    if (ctx->ec.n.used * CRY_MPI_DIGIT_BYTES < len)
        len = ctx->ec.n.used * CRY_MPI_DIGIT_BYTES;
    CHK1(cry_mpi_load_bin(&z, in, len));

    /* u1 = (z * w) mod n */
    CHK1(cry_mpi_mul(&z, &z, &w));
    CHK1(cry_mpi_mod(&z, &z, &ctx->ec.n));

    /* u2 = (r * w) mod n */
    CHK1(cry_mpi_mul(&w, &sign->r, &w));
    CHK1(cry_mpi_mod(&w, &w, &ctx->ec.n));

    /* (x1, y1) = u1 * G + u2 * Q */
    CHK1(cry_ecp_mul(&G, &ctx->ec.g, &z, &ctx->ec));
    CHK1(cry_ecp_mul(&Q, &ctx->q, &w, &ctx->ec));
    CHK1(cry_ecp_add(&G, &G, &Q, &ctx->ec));

    /* r = x1 mod n */
    CHK1(cry_mpi_mod(&G.x, &G.x, &ctx->ec.n));

    res = (cry_mpi_cmp_abs(&G.x, &sign->r) == 0) ? 0 : -1;

e1: cry_ecp_clear_list(&G, &Q, (cry_ecp *)NULL);
e0: cry_mpi_clear_list(&z, &w, &G.x, &G.y, &Q.x, &Q.y, (cry_mpi *)NULL);
    return res;
}

int cry_ecdsa_keygen(const cry_ecp_grp *ec, cry_mpi *d, cry_ecp *q)
{
    int res;

    if ((res = cry_mpi_rand_range(d, &ec->p)) == 0)
        res = cry_ecp_mul(q, &ec->g, d, ec);
    return res;
}
