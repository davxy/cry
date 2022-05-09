#include "mpi_pvt.h"
#include "misc.h"


int cry_mpi_mul_baseline(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int res;
    cry_mpi t;
    size_t pa, pb, i, j;
    cry_mpi_digit u, tmpx, *tmpt;
    const cry_mpi_digit *tmpy;
    cry_mpi_dword dw;
    size_t digs;

    if (cry_mpi_is_zero(a) || cry_mpi_is_zero(b)) {
        cry_mpi_zero(r);
        return 0;
    }

    digs = a->used + b->used;
    if ((res = cry_mpi_init_size(&t, digs)) != 0)
        return res;
    cry_mpi_set_used(&t, digs);

    /* compute the digits of the product directly */
    pa = a->used;
    /* iterate through every digit in the first operand */
    for (i = 0; i < pa; i++) {
        pb = CRY_MIN(b->used, digs - i);
        /* copy of the digit to be used within the nested loop */
        tmpx = a->data[i];
        /* an alias for the destination shifted i places */
        tmpt = t.data + i;
        /* an alias for the digits of b */
        tmpy = b->data;

        /* set initial carry to zero */
        u = 0;
        /* compute the columns of the output and propagate the carry */
        for (j = 0; j < pb; j++) {
            /* compute the column as a cry_mpi_dword */
            dw = (cry_mpi_dword)*tmpt +
                 (cry_mpi_dword)tmpx * (cry_mpi_dword)(*tmpy++) +
                 (cry_mpi_dword)u;
            /* the new column is the lower part of the result */
            *tmpt++ = (cry_mpi_digit)(dw & CRY_MPI_DIGIT_MAX);
            /* get the carry digit from the result */
            u = (cry_mpi_digit)(dw >> CRY_MPI_DIGIT_BITS);
        }
        /* set the carry if it fits within the required digs */
        if (i + j < digs)
            *tmpt = u;
    }
    cry_mpi_adjust(&t);
    cry_mpi_swap(&t, r);
    cry_mpi_clear(&t);
    return 0;
}
