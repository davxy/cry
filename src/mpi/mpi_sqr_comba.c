#include "mpi_pvt.h"


int cry_mpi_sqr_comba(cry_mpi *r, const cry_mpi *a)
{
    int res;
    size_t ix, iy, iz, tx, ty, pa;
    cry_mpi_digit c0, c1, c2, *tmpx, *tmpy;
    cry_mpi tmp, *dst;

    pa = a->used + a->used;
    if (r == a) {
        if ((res = cry_mpi_init_size(&tmp, pa)) != 0)
            return res;
        dst = &tmp;
    } else {
        if ((res = cry_mpi_grow(r, pa)) != 0)
            return res;
        dst = r;
    }
    cry_mpi_set_used(dst, pa);;

    c1 = c2 = 0;
    for (ix = 0; ix < pa; ix++) {
        /* Get offsets into the two bignums */
        ty = CRY_MIN(ix, a->used - 1);
        tx = ix - ty;

        /* Setup temporary aliases */
        tmpx = a->data + tx;
        tmpy = a->data + ty;

        /* This is the number of times the loop will iterate, essentially its
         * while (tx++ < a->used && ty-- >= 0) */
        iy = CRY_MIN(a->used - tx, ty + 1);

        /* For squaring tx can never be equal ty.
         * We halve the distance since they approach at a rate of 2x and we
         * have to roudnd because odd cases need to be executed. */
        iy = CRY_MIN(iy, (ty - tx + 1) >> 1);

        /* Shift accumulator right */
        c0 = c1;
        c1 = c2;
        c2 = 0;

        /* Execute loop */
        for (iz = 0; iz < iy; iz++) {
            MULADD(*tmpx, *tmpy);
            tmpx++;
            tmpy--;
        }
        /* FIXME: cannot work... this generates souble shifts... */
        /* Double the inner product */
        c2 = (c2 << 1) | (c1 >> (CRY_MPI_DIGIT_BITS - 1));
        c1 = (c1 << 1) | (c0 >> (CRY_MPI_DIGIT_BITS - 1));
        c0 <<= 1;

        /* Even columns have the square term in them */
        if ((ix & 1) == 0) {
            tmpx = a->data + (ix >> 1);
            MULADD(*tmpx, *tmpx);
        }

        /* Store term */
        dst->data[ix] = c0;
    }

    if (r != dst) {
        cry_mpi_swap(r, dst);
        cry_mpi_clear(dst);
    }
    cry_mpi_adjust(r);
    return res;
}
