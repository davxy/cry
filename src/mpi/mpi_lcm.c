#include "mpi_pvt.h"

/*
 * Least Common Multiple (lcm = x*y/gcd)
 */
int cry_mpi_lcm(cry_mpi *r, const cry_mpi *x, const cry_mpi *y)
{
    int res;
    cry_mpi gcm, prod;

    if ((res = cry_mpi_init_list(&gcm, &prod, NULL)) != 0)
        return res;

    if ((res = cry_mpi_gcd(&gcm, x, y)) == 0)
        if ((res = cry_mpi_mul(&prod, x, y)) == 0)
            res = cry_mpi_div(r, 0, &prod, &gcm);
    cry_mpi_clear_list(&gcm, &prod, NULL);
    return res;
}
