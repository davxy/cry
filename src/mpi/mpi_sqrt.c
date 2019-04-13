#include "mpi_pvt.h"

/*
 * Integer square root Babylonian Method.
 *
 * The integer square root of a positive integer n is the greatest integer S
 * such that:
 *
 *   0 <= S <= sqrt(n).
 *
 * Given an approximation x of the real square root sqrt(n):
 *   1. if x > sqrt(n) then n/x < sqrt(n);
 *   2. if x < sqrt(n) then n/x > sqrt(n).
 * So the average of x and n/x is expected to provide a better approximation.
 *
 * Note that because of integer arithmetic, for integer square root S we're
 * allowed to use only the first case.
 *
 *  n/x     S          x
 *   |------|----------|
 *
 * For the proofs below, we assume S as the real square root sqrt(n)
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Proposition #1
 *   if S <= x then n/x <= S
 *
 * Proof.
 *   If S <= x then n = SS <= Sx
 *   Dividing by x we get n/x <= S
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Proposition #2
 *   S - n/x <= x - S
 *
 * Proof.
 *  S - n/x <= x - s
 *  2S - n/x - x <= 0
 *  2Sx - n - x^2 <= 0
 *  x^2 - 2Sx + S^2 > 0
 *  x >= [2S +/- sqrt(4S^2 - 4S^2)] / 2
 *  x >= S
 *  Thus the method converges to S until x >= S
 *
 * The two propositions suggests the termination criterion.
 * The method terminates as soon as the new approximation is not greater than
 * the previous one over x.
 */
int cry_mpi_sqrt(cry_mpi *r, const cry_mpi *a)
{
    int res;
    cry_mpi t1, t2;

    if (a->sign != 0)
        return -1;

    if (cry_mpi_is_zero(a) != 0) {
        cry_mpi_zero(r);
        return 0;
    }

    if ((res = cry_mpi_init_list(&t1, &t2, (cry_mpi *) NULL)) != 0)
        return res;
    if ((res = cry_mpi_copy(&t1, a)) != 0)
        goto e;

    /* First coarse approximation */
    if ((res = cry_mpi_shrd(&t1, t1.used/2)) != 0)
        goto e;

    /* t1 > 0 */
    if ((res = cry_mpi_div(&t2, NULL, a, &t1)) != 0)
        goto e;
    if ((res = cry_mpi_add(&t1, &t1, &t2)) != 0)
        goto e;
    if ((res = cry_mpi_shr(&t1, &t1, 1)) != 0)
        goto e;

    do {
        /* t1 >= sqrt(a) >= t2 */
        if ((res = cry_mpi_div(&t2, NULL, a, &t1)) != 0)
            goto e;
        if ((res = cry_mpi_add(&t1, &t1, &t2)) != 0)
            goto e;
        if ((res = cry_mpi_shr(&t1, &t1, 1)) != 0)
            goto e;
    } while (cry_mpi_cmp_abs(&t1, &t2) > 0);

    cry_mpi_swap(r, &t1);

e:  cry_mpi_clear_list(&t1, &t2, (cry_mpi *) NULL);
    return res;
}
