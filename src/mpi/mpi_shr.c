#include "mpi_pvt.h"

int cry_mpi_shrd(cry_mpi *a, int n)
{
    int x;
    cry_mpi_digit *bottom, *top;

    if (n < 0)
        return cry_mpi_shld(a, -n);
    if (n == 0 || cry_mpi_is_zero(a) != 0)
        return 0;

    if (a->used <= n) {
        cry_mpi_zero(a);
        return 0;
    }

    bottom = a->data;
    top = a->data + n;
    for (x = 0; x < (a->used - n); x++)
        *bottom++ = *top++;

    /* zero the top digits */
    for ( ; x < a->used; x++)
        *bottom++ = 0;

    /* remove excess digits */
    a->used -= n;
    return 0;
}

/*
 * Store quotient in c, optional remainder in d
 */
int cry_mpi_shr(cry_mpi *c, const cry_mpi *a, int n)
{
    cry_mpi_digit D, r, rr;
    int x, res;

    if (n < 0)
        return cry_mpi_shl(c, a, -n);

    /* copy */
    if (a != c) {
        if ((res = cry_mpi_copy(c, a)) != 0)
            return res;
    }

    if (n == 0 || cry_mpi_is_zero(a) != 0)
        return 0;

    /* shift by as many digits in the bit count */
    if (n >= CRY_MPI_DIGIT_BITS)
        cry_mpi_shrd(c, n / CRY_MPI_DIGIT_BITS);

    /* shift any bit count < CRY_MPI_DIGIT_BITS */
    D = n % CRY_MPI_DIGIT_BITS;
    if (D) {
        cry_mpi_digit *tmpc, mask, shift;

        mask = (((cry_mpi_digit)1) << D) - 1;
        shift = CRY_MPI_DIGIT_BITS - D;
        tmpc = c->data + (c->used - 1);
        r = 0;  /* carry */
        for (x = c->used - 1; x >= 0; x--) {
            /* get the lower bits of this word in a temp */
            rr = *tmpc & mask;
            /* shift the current word and mix in the carry bits from the prev */
            *tmpc = (*tmpc >> D) | (r << shift);
            --tmpc;
            /* set carry to the carry bits of the current word found above */
            r = rr;
        }
    }
    cry_mpi_adjust(c);
    return 0;
}
