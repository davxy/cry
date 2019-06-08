#include "mpi_pvt.h"
#include <cry/config.h>

#ifdef CRY_MPI_INV_EUCLID

/*
 * Modular inverse.
 *
 * Classic extended Euclidean algorithm.
 */
int cry_mpi_inv(cry_mpi *r, const cry_mpi *a, const cry_mpi *m)
{
    int ret = 0;
    cry_mpi r0, r1, s0, s1, q;

    if ((ret = cry_mpi_init_list(&r0, &r1, &s0, &s1, &q,
                                 (cry_mpi *)NULL)) != 0)
        return ret;

    cry_mpi_copy(&r0, a);
    cry_mpi_copy(&r1, m);
    cry_mpi_set_int(&s0, 1);
    cry_mpi_set_int(&s1, 0);

    while (!cry_mpi_is_zero(&r1)) {
        /* r2 = r0 - q1*r1 */
        cry_mpi_div(&q, &r0, &r0, &r1);
        cry_mpi_swap(&r0, &r1);

        /* s2 = s0 - q1*s1 */
        cry_mpi_mul(&q, &q, &s1);
        cry_mpi_sub(&s0, &s0, &q);
        cry_mpi_swap(&s0, &s1);

        cry_mpi_zero(&q);
    }

    /* Check that the GCD is 1, necessary condition */
    if (r0.used == 1 && r0.sign == 0 && r0.data[0] == 1) {
        cry_mpi_swap(r, &s0);
        if (r->sign)
            cry_mpi_mod(r, r, m);
    } else {
        cry_mpi_zero(&s0);
        ret = -1;
    }
    cry_mpi_clear_list(&r0, &r1, &s0, &s1, &q, (cry_mpi *)NULL);
    return ret;
}

#else

/*
 * Modular inverse.
 *
 * Binary extended gcd algorithm (HAC 14.4.3).
 */
int cry_mpi_inv(cry_mpi *r, const cry_mpi *x, const cry_mpi *m)
{
    int res;
    cry_mpi TA, TU, U1, U2, TB, TV, V1, V2;

    if ((res = cry_mpi_init_list(&TA, &TU, &U1, &U2,
                                 &TB, &TV, &V1, &V2, (cry_mpi *)NULL)) != 0)
        return res;

    if ((res = cry_mpi_mod(&TA, x, m)) != 0)
        goto cleanup;
    cry_mpi_copy(&TU, &TA);
    cry_mpi_copy(&TB, m);
    cry_mpi_copy(&TV, m);

    cry_mpi_set_int(&U1, 1);
    cry_mpi_set_int(&U2, 0);
    cry_mpi_set_int(&V1, 0);
    cry_mpi_set_int(&V2, 1);

    do {
        while (cry_mpi_is_even(&TU)) {
            cry_mpi_shr(&TU, &TU, 1);
            if (cry_mpi_is_odd(&U1) || cry_mpi_is_odd(&U2)) {
                cry_mpi_add(&U1, &U1, &TB);
                cry_mpi_sub(&U2, &U2, &TA);
            }
            cry_mpi_shr(&U1, &U1, 1);
            cry_mpi_shr(&U2, &U2, 1);
        }
        while (cry_mpi_is_even(&TV)) {
            cry_mpi_shr(&TV, &TV, 1);
            if (cry_mpi_is_odd(&V1) || cry_mpi_is_odd(&V2)) {
                cry_mpi_add(&V1, &V1, &TB);
                cry_mpi_sub(&V2, &V2, &TA);
            }
            cry_mpi_shr(&V1, &V1, 1);
            cry_mpi_shr(&V2, &V2, 1);
        }
        if (cry_mpi_cmp(&TU, &TV) >= 0) {
            cry_mpi_sub(&TU, &TU, &TV);
            cry_mpi_sub(&U1, &U1, &V1);
            cry_mpi_sub(&U2, &U2, &V2);
        } else {
            cry_mpi_sub(&TV, &TV, &TU);
            cry_mpi_sub(&V1, &V1, &U1);
            cry_mpi_sub(&V2, &V2, &U2);
        }
    } while (TU.used != 0);

    while (V1.used && V1.sign)
        cry_mpi_add(&V1, &V1, m);

    while (cry_mpi_cmp(&V1, m) >= 0)
        cry_mpi_sub(&V1, &V1, m);

    cry_mpi_swap(r, &V1);
cleanup:
    cry_mpi_clear_list(&TA, &TB, &TU, &TV, &U1, &U2, &V1, &V2,
                       (cry_mpi *)NULL);
    return res;
}

#endif /* CRY_MPI_INV_EUCLID */
