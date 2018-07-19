#include "mpi_pvt.h"

/*
 * Signed addition
 */
int cry_mpi_sub(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int ret, rsign;

    if (a->sign != b->sign) {
        rsign = a->sign;
        ret = cry_mpi_add_abs(r, a, b);
    } else {
        switch (cry_mpi_cmp_abs(a, b)) {
            case 1:  /* a > b */
                rsign = a->sign;
                ret = cry_mpi_sub_abs(r, a, b);
                break;
            case -1: /* a < b */
                rsign = 1 - b->sign;
                ret = cry_mpi_sub_abs(r, b, a);
                break;
            default:
                rsign = 0;
                cry_mpi_zero(r);
                ret = 0;
                break;
        }
    }
    if (ret == 0)
        r->sign = rsign;
    return ret;
}
