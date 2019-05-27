#include "mpi_pvt.h"
#include "misc.h"

static int mod_2e(cry_mpi *r, const cry_mpi *a, size_t e)
{
    int res;
    size_t x;

    if (e == 0) {
        cry_mpi_zero(r);
        return 0;
    }

    /* copy */
    if ((res = cry_mpi_copy(r, a)) != 0)
        return res;

    /* if the modulus is larger than the value than return */
    if (e >= a->used * CRY_MPI_DIGIT_BITS)
        return res;

    /* zero digits above the last digit of the modulus */
    x = (e / CRY_MPI_DIGIT_BITS) + (((e % CRY_MPI_DIGIT_BITS) == 0) ? 0 : 1);
    while (x < r->used)
        r->data[x++] = 0;

    /* clear the digit that is not completely outside/inside the modulus */
    r->data[e / CRY_MPI_DIGIT_BITS] &= (cry_mpi_digit) ((((cry_mpi_digit) 1) <<
                (((cry_mpi_digit) e) % CRY_MPI_DIGIT_BITS)) - ((cry_mpi_digit) 1));

    cry_mpi_adjust(r);
    return 0;
}


/*
 * Based on Tudor Jebelean "exact division" algorithm.
 */
static int div3(cry_mpi *a)
{
    int res;
    size_t i;
    cry_mpi q;
    cry_mpi_dword w, t;
    cry_mpi_digit b;

    /* b = 2**DIGIT_BIT / 3 */
    b = (((cry_mpi_dword)1) << CRY_MPI_DIGIT_BITS) / 3;

    if ((res = cry_mpi_init_size(&q, a->used)) != 0)
        return res;

    q.used = a->used;
    q.sign = a->sign;
    w = 0;
    i = a->used;
    while (i-- > 0) {
        w = (w << ((cry_mpi_dword)CRY_MPI_DIGIT_BITS)) | a->data[i];
        if (w >= 3) {
            /* multiply w by 1/3 */
            t = (w * ((cry_mpi_dword)b)) >> CRY_MPI_DIGIT_BITS;
            /* now subtract 3*w/3 from w, to get the remainder */
            w -= t+t+t;
            /*
             * fixup the remainder as required since the optimization
             * is not exact.
             */
            while (w >= 3) {
               t += 1;
               w -= 3;
            }
        } else {
            t = 0;
        }
        q.data[i] = (cry_mpi_digit)t;
    }

    cry_mpi_adjust(&q);
    cry_mpi_swap(&q, a);
    cry_mpi_clear(&q);
    return res;
}

/*
 * For nodes evaluation and interpolation the implementation uses the
 * operations sequence given by Bodrato's paper:
 * "Optimal Toom-Cook Multiplication for univariate and Multivariate
 *  Polynomials in Characteristic 2 and 0"
 */
int cry_mpi_mul_toom3(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int res;
    size_t B;
    cry_mpi w0, w1, w2, w3, w4, a0, a1, a2, b0, b1, b2, t1, t2;

    if (cry_mpi_is_zero(a) || cry_mpi_is_zero(b)) {
        cry_mpi_zero(r);
        return 0;
    }

    /* init temps */
    if ((res = cry_mpi_init_list(&w0, &w1, &w2, &w3, &w4,
                                 &a0, &a1, &a2, &b0, &b1,
                                 &b2, &t1, &t2, (cry_mpi *) NULL)) != 0) {
       return res;
    }

    /* B */
    B = CRY_MIN(a->used, b->used) / 3;

    /*
     * Splitting
     */

    /* a = a2 * B**2 + a1 * B + a0 */
    CRY_CHK(res = mod_2e(&a0, a, CRY_MPI_DIGIT_BITS * B), e);
    CRY_CHK(res = cry_mpi_copy(&a1, a), e);
    CRY_CHK(res = cry_mpi_shrd(&a1, B), e);
    CRY_CHK(res = mod_2e(&a1, &a1, CRY_MPI_DIGIT_BITS * B), e);
    CRY_CHK(res = cry_mpi_copy(&a2, a), e);
    CRY_CHK(res = cry_mpi_shrd(&a2, B*2), e);

    /* b = b2 * B**2 + b1 * B + b0 */
    CRY_CHK(res = mod_2e(&b0, b, CRY_MPI_DIGIT_BITS * B), e);
    CRY_CHK(res = cry_mpi_copy(&b1, b), e);
    CRY_CHK(res = cry_mpi_shrd(&b1, B), e);
    CRY_CHK(res = mod_2e(&b1, &b1, CRY_MPI_DIGIT_BITS * B), e);
    CRY_CHK(res = cry_mpi_copy(&b2, b), e);
    CRY_CHK(res = cry_mpi_shrd(&b2, B*2), e);

    /*
     * Evaluation (using Bodrato's steps)
     */

    /* t1 = a0 + a2 */
    CRY_CHK(res = cry_mpi_add(&t1, &a0, &a2), e);
    /* t2 = b0 + b2 */
    CRY_CHK(res = cry_mpi_add(&t2, &b0, &b2), e);

    /* w1 = ((a0 + a2) + a1)((b0 + b2) + b1) */
    CRY_CHK(res = cry_mpi_add(&w1, &t1, &a1), e);
    CRY_CHK(res = cry_mpi_add(&w2, &t2, &b1), e);
    CRY_CHK(res = cry_mpi_mul(&w1, &w1, &w2), e);

    /* w2 = ((a0 + a2) - a1)((b0 + b2) - b1) */
    CRY_CHK(res = cry_mpi_sub(&t1, &t1, &a1), e);
    CRY_CHK(res = cry_mpi_sub(&t2, &t2, &b1), e);
    CRY_CHK(res = cry_mpi_mul(&w2, &t1, &t2), e);

    /* t1 = 2*((a0+a2-a1) + a2) - a0 = a0 - a1 + 4a2 */
    CRY_CHK(res = cry_mpi_add(&t1, &t1, &a2), e);
    CRY_CHK(res = cry_mpi_shl(&t1, &t1, 1), e);
    CRY_CHK(res = cry_mpi_sub(&t1, &t1, &a0), e);
    /* t2 = 2*((b0+b2-b1) + b2) - b0 = b0 - 2a1 + 4a2 */
    CRY_CHK(res = cry_mpi_add(&t2, &t2, &b2), e);
    CRY_CHK(res = cry_mpi_shl(&t2, &t2, 1), e);
    CRY_CHK(res = cry_mpi_sub(&t2, &t2, &b0), e);
    /* w3 = t1 * t2 */
    CRY_CHK(res = cry_mpi_mul(&w3, &t1, &t2), e);

    /* w0 = a0 * b0 */
    CRY_CHK(res = cry_mpi_mul(&w0, &a0, &b0), e);
    /* w4 = a2 * b2 */
    CRY_CHK(res = cry_mpi_mul(&w4, &a2, &b2), e);

    /*
     * Interpolation
     *
     *  w0    1  0  0  0  0     r0
     *  w1    1  1  1  1  1     r1
     *  w2  = 1 -1  1 -1  1   x r2
     *  w3    1 -2  4 -8  16    r3
     *  w4    0  0  0  0  1     r4
     */

    /* w3 = (w3 - w1)/3 */
    CRY_CHK(res = cry_mpi_sub(&w3, &w3, &w1), e);
    CRY_CHK(res = div3(&w3), e);

    /* w1 = (w1 - w2)/2 */
    CRY_CHK(res = cry_mpi_sub(&w1, &w1, &w2), e);
    CRY_CHK(res = cry_mpi_shr(&w1, &w1, 1), e);

    /* w2 = w2 - w0 */
    CRY_CHK(res = cry_mpi_sub(&w2, &w2, &w0), e);

    /* w3 = (w2 - w3)/2 + 2*w4 */
    CRY_CHK(res = cry_mpi_sub(&w3, &w2, &w3), e);
    CRY_CHK(res = cry_mpi_shr(&w3, &w3, 1), e);
    CRY_CHK(res = cry_mpi_shl(&t1, &w4, 1), e);
    CRY_CHK(res = cry_mpi_add(&w3, &w3, &t1), e);

    /* w2 = w2 + w1 - w4 */
    CRY_CHK(res = cry_mpi_add(&w2, &w2, &w1), e);
    CRY_CHK(res = cry_mpi_sub(&w2, &w2, &w4), e);

    /* w1 = w1 - w3 */
    CRY_CHK(res = cry_mpi_sub(&w1, &w1, &w3), e);

    /*
     * Reconstruction
     */

    /* Shift wn by B*n */
    CRY_CHK(res = cry_mpi_shld(&w1, 1*B), e);
    CRY_CHK(res = cry_mpi_shld(&w2, 2*B), e);
    CRY_CHK(res = cry_mpi_shld(&w3, 3*B), e);
    CRY_CHK(res = cry_mpi_shld(&w4, 4*B), e);

    /* Add the parts */
    CRY_CHK(res = cry_mpi_add(r, &w0, &w1), e);
    CRY_CHK(res = cry_mpi_add(&t1, &w2, &w3), e);
    CRY_CHK(res = cry_mpi_add(&t1, &w4, &t1), e);
    CRY_CHK(res = cry_mpi_add(r, &t1, r), e);

e:  cry_mpi_clear_list(&w0, &w1, &w2, &w3, &w4,
                       &a0, &a1, &a2, &b0, &b1,
                       &b2, &t1, &t2, (cry_mpi *) NULL);
    return res;
}
