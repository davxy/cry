#include "mpi_pvt.h"

/*
 * Modular exponentiation
 * TODO: start from MSB and double the result in place of b.
 * This saves a variable.
 */
int cry_mpi_mod_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e,
                    const cry_mpi *m)
{
    int i, j, res = 0;
    cry_mpi exp, tmp;

    if ((res = cry_mpi_init_int(&exp, 1)) != 0)
        return res;
    if ((res = cry_mpi_init_copy(&tmp, b)) != 0) {
        cry_mpi_clear(&exp);
        return res;
    }

    /* For each block */
    for (i = 0; i < e->used; i++) {
        for (j = 0; j < CRY_MPI_DIGIT_BITS; j++) {
            if (i == e->used - 1 && /* If is the last exponent block */
                (e->data[i] & ~((1UL << j) - 1)) == 0)
                break;
            if (e->data[i] & (1UL << j)) {
                if ((res = cry_mpi_mul(&exp, &exp, &tmp)) != 0 ||
                    (m && (res = cry_mpi_mod(&exp, &exp, m)) != 0))
                    goto e;
            }
            if ((res = cry_mpi_sqr(&tmp, &tmp)) != 0 ||
                (m && (res = cry_mpi_mod(&tmp, &tmp, m)) != 0))
                goto e;
        }
    }
    cry_mpi_swap(r, &exp);
e:  cry_mpi_clear_list(&tmp, &exp, NULL);
    return res;
}
