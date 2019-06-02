#include "mpi_pvt.h"

/*
 * The number of less significant zero-bits.
 * Used by the binary gcd algorithm.
 */
static size_t cry_mpi_lsb(struct cry_mpi *x)
{
    size_t i, j, count = 0;

    for (i = 0; i < x->used; i++) {
        for (j = 0; j < CRY_MPI_DIGIT_BITS; j++, count++)
            if (((x->data[i] >> j) & 1) != 0)
                break;
    }
    return count;
}

/*
 * Greatest common divisor.
 * Implemented using the binary (Stein's) algorithm)
 */
int cry_mpi_gcd(cry_mpi *r, const cry_mpi *x, const cry_mpi *y)
{
    int res;
    cry_mpi tx, ty;
    size_t lz, lzt;

    if ((res = cry_mpi_init_list(&tx, &ty, (cry_mpi *)NULL)) != 0)
        return res;

    if ((res = cry_mpi_copy(&tx, x)) != 0 ||
        (res = cry_mpi_copy(&ty, y)) != 0)
        goto e;

    lz  = cry_mpi_lsb(&tx);
    lzt = cry_mpi_lsb(&ty);
    if (lzt < lz)
        lz = lzt;

    if ((res = cry_mpi_shr(&tx, &tx, lz)) != 0 ||
        (res = cry_mpi_shr(&ty, &ty, lz)) != 0)
        goto e;

    while (!cry_mpi_is_zero(&tx)) {
        if ((res = cry_mpi_shr(&tx, &tx, cry_mpi_lsb(&tx))) != 0 ||
            (res = cry_mpi_shr(&ty, &ty, cry_mpi_lsb(&ty))) != 0)
            break;

        if (cry_mpi_cmp_abs(&tx, &ty) >= 0) {
            if ((res = cry_mpi_sub_abs(&tx, &tx, &ty)) != 0 ||
                (res = cry_mpi_shr(&tx, &tx, 1)) != 0)
                break;
        } else {
            if ((res = cry_mpi_sub_abs(&ty, &ty, &tx)) != 0 ||
                (res = cry_mpi_shr(&ty, &ty, 1)) != 0)
                break;
        }
    }
    res = cry_mpi_shl(&ty, &ty, lz);
    cry_mpi_swap(r, &ty);
e:  cry_mpi_clear_list(&tx, &ty, (cry_mpi *)NULL);
    return res;
}
