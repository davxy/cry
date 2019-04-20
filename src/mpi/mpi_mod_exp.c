#include "mpi_pvt.h"

/*
 * Modular exponentiation
 */
int cry_mpi_mod_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e,
                    const cry_mpi *m)
{
    int res = 0;
    size_t bits, i, w;
    cry_mpi_digit d;
    cry_mpi exp, tmp;

    if (cry_mpi_is_neg(e) || (m != NULL && cry_mpi_is_neg(m)))
        return -1;

    if ((res = cry_mpi_init_list(&tmp, &exp, NULL)) != 0)
        return res;

    if (cry_mpi_is_odd(e))
        res = cry_mpi_copy(&exp, b);
    else
        res = cry_mpi_set_int(&exp, 1);
    if (res != 0 || (res = cry_mpi_copy(&tmp, b)) != 0)
        goto e;

    bits = cry_mpi_count_bits(e);
    d = 1;
    w = 0;
    for (i = 1; i < bits; i++) {
        if ((res = cry_mpi_sqr(&tmp, &tmp)) != 0 ||
            (m != NULL && (res = cry_mpi_mod(&tmp, &tmp, m)) != 0))
            goto e;
        if (d != ((cry_mpi_digit)1 << (CRY_MPI_DIGIT_BITS-1))) {
            d <<= 1;
        } else {
            d = 1;
            w++;
        }
        if ((e->data[w] & d) != 0) {
            if ((res = cry_mpi_mul(&exp, &exp, &tmp)) != 0 ||
                (m != NULL && (res = cry_mpi_mod(&exp, &exp, m)) != 0))
                goto e;
        }
    }

    cry_mpi_swap(r, &exp);
e:  cry_mpi_clear_list(&tmp, &exp, (cry_mpi *) NULL);
    return res;
}
