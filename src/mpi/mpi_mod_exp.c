#include "mpi_pvt.h"

/*
 * Modular exponentiation
 */
int cry_mpi_mod_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e,
                    const cry_mpi *m)
{
    int res = 0;
    size_t bits;
    cry_mpi exp;

    if (cry_mpi_is_neg(e) || (m != NULL && cry_mpi_is_neg(m)))
        return -1;

    res = cry_mpi_init_int(&exp, 1);
    bits = cry_mpi_count_bits(e);
    while (bits-- > 0) {
        if ((res = cry_mpi_sqr(&exp, &exp)) != 0 ||
            (m != NULL && (res = cry_mpi_mod(&exp, &exp, m)) != 0))
            goto e;
        if (cry_mpi_is_bit_set(e, bits) != 0) {
            if ((res = cry_mpi_mul(&exp, &exp, b)) != 0 ||
                (m != NULL && (res = cry_mpi_mod(&exp, &exp, m)) != 0))
                goto e;
        }
    }

    cry_mpi_swap(r, &exp);
e:  cry_mpi_clear(&exp);
    return res;
}
