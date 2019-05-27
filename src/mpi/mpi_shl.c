#include "mpi_pvt.h"

/*
 * Shift left by a certain amount of digits
 */
int cry_mpi_shld(cry_mpi *a, size_t n)
{
    int res;
    size_t x;
    cry_mpi_digit *top, *bottom;

    if (n == 0 || cry_mpi_is_zero(a) != 0)
        return 0;

    /* grow to fit the new digits */
    if (a->alloc < (a->used + n)) {
        if ((res = cry_mpi_grow(a, a->used + n)) != 0)
            return res;
    }

    a->used += n;
    top = a->data + a->used - 1;
    bottom = (a->data + a->used - 1) - n;
    for (x = a->used - 1; x >= n; x--)
        *top-- = *bottom--;

    /* zero the lower digits */
    top = a->data;
    for (x = 0; x < n; x++)
        *top++ = 0;
    return 0;
}

/*
 * Shift left by a certain bit count
 */
int cry_mpi_shl(cry_mpi *c, const cry_mpi *a, size_t n)
{
    cry_mpi_digit d;
    int res;

    /* copy */
    if (a != c) {
        if ((res = cry_mpi_copy(c, a)) != 0)
            return res;
    }

    if (n == 0 || cry_mpi_is_zero(a) != 0)
        return 0;

    if (c->alloc < (c->used + (n / CRY_MPI_DIGIT_BITS) + 1)) {
        if ((res = cry_mpi_grow(c,
                        c->used + (n / CRY_MPI_DIGIT_BITS) + 1)) != 0)
            return res;
    }

    /* shift by as many digits in the bit count */
    if (n >= CRY_MPI_DIGIT_BITS) {
        if ((res = cry_mpi_shld(c, n / CRY_MPI_DIGIT_BITS)) != 0)
            return res;
    }

    /* shift any bit count < DIGIT_BIT */
    d = n % CRY_MPI_DIGIT_BITS;
    if (d != 0) {
        cry_mpi_digit *tmpc, shift, mask, r, rr;
        size_t x;

        mask = (((cry_mpi_digit)1) << d) - 1;
        shift = CRY_MPI_DIGIT_BITS - d;
        tmpc = c->data;
        r = 0;  /* carry */
        for (x = 0; x < c->used; x++) {
            rr = (*tmpc >> shift) & mask;
            *tmpc = ((*tmpc << d) | r);
            ++tmpc;
            r = rr;
        }

        /* set final carry */
        if (r != 0)
            c->data[(c->used)++] = r;
    }
    cry_mpi_adjust(c);
    return 0;
}
