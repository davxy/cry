#include "mpi_pvt.h"

#ifdef CRY_MPI_DIV_SMALL

int cry_mpi_div_abs(cry_mpi *rq, cry_mpi *rr, const cry_mpi *a,
                    const cry_mpi *b)
{
    cry_mpi ta, tb, tq, q;
    int res, n;

    /* is divisor zero ? */
    if (cry_mpi_is_zero(b))
        return -1;

    /* if a < b then q=0, r=a */
    if (cry_mpi_cmp_abs(a, b) < 0) {
        if (rr)
            res = cry_mpi_copy(rr, a);
        else
            res = 0;
        if (rq)
            cry_mpi_zero(rq);
        return res;
    }

    if ((res = cry_mpi_init_list(&ta, &tb, &tq, &q, NULL)) != 0)
        return res;

    cry_mpi_set_int(&tq, 1);
    n = cry_mpi_count_bits(a) - cry_mpi_count_bits(b);
    if (((res = cry_mpi_abs(&ta, a)) != 0) ||
        ((res = cry_mpi_abs(&tb, b)) != 0) ||
        ((res = cry_mpi_shl(&tb, &tb, n)) != 0) ||
        ((res = cry_mpi_shl(&tq, &tq, n)) != 0))
        goto cleanup;

#if 0
    int k = 0;
    for ( ; n >= 0; n -= k) {
        if (cry_mpi_cmp(&tb, &ta) <= 0) {
            if (k) {
                if ((res = cry_mpi_shr(&tq, &tq, k)) != 0)
                    goto cleanup;
                k = 0;
            }

            if (((res = cry_mpi_sub(&ta, &ta, &tb)) != 0) ||
                ((res = cry_mpi_add(&q, &q, &tq)) != 0))
                goto cleanup;
        }
        k = MAX(cry_mpi_count_bits(&tb) - cry_mpi_count_bits(&ta), 1);
        if ((res = cry_mpi_shr(&tb, &tb, k)) != 0)
            goto cleanup;
    }
#else
    while (n-- >= 0) {
        if (cry_mpi_cmp(&tb, &ta) <= 0) {
            if (((res = cry_mpi_sub(&ta, &ta, &tb)) != 0) ||
                ((res = cry_mpi_add(&q, &q, &tq)) != 0))
                goto cleanup;
        }
        if (((res = cry_mpi_shr(&tb, &tb, 1)) != 0) ||
            ((res = cry_mpi_shr(&tq, &tq, 1)) != 0))
            goto cleanup;
    }
#endif

    /* now q == quotient and ta == remainder */
    if (rq) {
        cry_mpi_swap(rq, &q);
        rq->sign = 0;
    }
    if (rr) {
        cry_mpi_swap(rr, &ta);
        rr->sign = 0;
    }
cleanup:
    cry_mpi_clear_list(&ta, &tb, &tq, &q, NULL);
    return res;
}

#else /* !CRY_DIV_SMALL */

/*
 * HAC pp.598 Algorithm 14.20
 *
 * Note that the description in HAC is horribly incomplete.
 * For example, it doesn't consider the case where digits
 * are removed from 'x' in the inner loop. It also doesn't
 * consider the case that y has fewer than three digits, etc..
 *
 * The overall algorithm is as described as 14.20 from HAC
 * but fixed to treat these cases.
 */

#define MP_MASK ((cry_mpi_digit)-1)

int cry_mpi_div_abs(cry_mpi *c, cry_mpi *d, const cry_mpi *a,
                    const cry_mpi *b)
{
    cry_mpi q, x, y, t1, t2;
    int res, n, t, i, norm;

    /* is divisor zero ? */
    if (cry_mpi_is_zero(b))
        return -1;

    /* if a < b then q=0, r = a */
    if (cry_mpi_cmp_abs(a, b) < 0) {
        if (d)
            res = cry_mpi_copy(d, a);
        else
            res = 0;
        if (c)
            cry_mpi_zero(c);
        return res;
    }

    if ((res = cry_mpi_init_size(&q, a->used + 2)) != 0)
        return res;
    cry_mpi_set_used(&q, a->used + 2);

    if ((res = cry_mpi_init_size(&t1, 8)) != 0)
        goto LBL_Q;
    if ((res = cry_mpi_init_size(&t2, 8)) != 0)
        goto LBL_T1;
    if ((res = cry_mpi_init_copy(&x, a)) != 0)
        goto LBL_T2;
    if ((res = cry_mpi_init_copy(&y, b)) != 0)
        goto LBL_X;

    /* fix the sign */
    x.sign = y.sign = 0;

    /* normalize both x and y, ensure that y >= b/2, [b == 2**DIGIT_BIT] */
    norm = cry_mpi_count_bits(&y) % CRY_MPI_DIGIT_BITS;
    if (norm < (int)(CRY_MPI_DIGIT_BITS-1)) {
        norm = (CRY_MPI_DIGIT_BITS-1) - norm;
        if ((res = cry_mpi_shl(&x, &x, norm)) != 0)
            goto LBL_Y;
        if ((res = cry_mpi_shl(&y, &y, norm)) != 0)
            goto LBL_Y;
    } else {
        norm = 0;
    }

    /* note hac does 0 based, so if used==5 then its 0,1,2,3,4, e.g. use 4 */
    n = x.used - 1;
    t = y.used - 1;

    /* while (x >= y*b**n-t) do { q[n-t] += 1; x -= y*b**{n-t} } */
    if ((res = cry_mpi_shld(&y, n - t)) != 0) { /* y = y*b**{n-t} */
        goto LBL_Y;
    }

    while (cry_mpi_cmp(&x, &y) != -1) {
        ++(q.data[n - t]);
        if ((res = cry_mpi_sub(&x, &x, &y)) != 0) {
            goto LBL_Y;
        }
    }

    /* reset y by shifting it back down */
    cry_mpi_shrd(&y, n - t);

    /* Step 3. for i from n down to (t + 1) */
    for (i = n; i >= (t + 1); i--) {
        if (i > x.used) {
            continue;
        }

        /*
         * Step 3.1
         * if xi == yt then set q{i-t-1} to b-1,
         * else set q{i-t-1} to (xi*b + x{i-1})/yt
         */
        if (x.data[i] == y.data[t]) {
            q.data[i - t - 1] = (cry_mpi_digit)-1;
        } else {
            cry_mpi_dword tmp;

            tmp = ((cry_mpi_dword) x.data[i])
                        << ((cry_mpi_dword) CRY_MPI_DIGIT_BITS);
            tmp |= ((cry_mpi_dword) x.data[i - 1]);
            tmp /= ((cry_mpi_dword) y.data[t]);
            if (tmp > (cry_mpi_dword) MP_MASK)
                tmp = MP_MASK;
            q.data[i - t - 1] =
                (cry_mpi_digit) (tmp & (cry_mpi_dword) (MP_MASK));
        }

        /*
         * while (q{i-t-1} * (yt * b + y{t-1})) > xi * b**2 + xi-1 * b + xi-2
         *  do q{i-t-1} -= 1;
         */
        q.data[i - t - 1] = (q.data[i - t - 1] + 1) & MP_MASK;
        do {
            q.data[i - t - 1] = (q.data[i - t - 1] - 1) & MP_MASK;

            /* find left hand */
            cry_mpi_zero(&t1);
            t1.data[0] = (t - 1 < 0) ? 0 : y.data[t - 1];
            t1.data[1] = y.data[t];
            t1.used = 2;
            if ((res = cry_mpi_mul_dig(&t1, &t1, q.data[i - t - 1])) != 0) {
                goto LBL_Y;
            }

            /* find right hand */
            t2.data[0] = (i - 2 < 0) ? 0 : x.data[i - 2];
            t2.data[1] = (i - 1 < 0) ? 0 : x.data[i - 1];
            t2.data[2] = x.data[i];
            t2.used = 3;
        } while (cry_mpi_cmp_abs(&t1, &t2) == 1);

        /*
         * Step 3.3
         * x = x - q{i-t-1} * y * b**{i-t-1}
         */
        if ((res = cry_mpi_mul_dig(&t1, &y, q.data[i - t - 1])) != 0) {
            goto LBL_Y;
        }

        if ((res = cry_mpi_shld (&t1, i - t - 1)) != 0) {
            goto LBL_Y;
        }

        if ((res = cry_mpi_sub (&x, &x, &t1)) != 0) {
            goto LBL_Y;
        }

        /* if x < 0 then { x = x + y*b**{i-t-1}; q{i-t-1} -= 1; } */
        if (x.sign == 1) {
            if ((res = cry_mpi_copy (&t1, &y)) != 0) {
                goto LBL_Y;
            }
            if ((res = cry_mpi_shld (&t1, i - t - 1)) != 0) {
                goto LBL_Y;
            }
            if ((res = cry_mpi_add (&x, &x, &t1)) != 0) {
                goto LBL_Y;
            }

            q.data[i - t - 1] = (q.data[i - t - 1] - 1UL) & MP_MASK;
        }
    }

    /*
     * Now q is the quotient and x is the remainder
     * [which we have to normalize]
     */

    if (c) {
        cry_mpi_adjust(&q);
        cry_mpi_swap(&q, c);
        c->sign = 0;
    }

    if (d) {
        cry_mpi_shr(&x, &x, norm);
        cry_mpi_swap(&x, d);
        d->sign = 0;
    }

    res = 0;

LBL_Y:cry_mpi_clear (&y);
LBL_X:cry_mpi_clear (&x);
LBL_T2:cry_mpi_clear (&t2);
LBL_T1:cry_mpi_clear (&t1);
LBL_Q:cry_mpi_clear (&q);
    return res;
}

#endif /* CRY_DIV_SMALL */
