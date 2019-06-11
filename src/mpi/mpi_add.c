#include "mpi_pvt.h"

int cry_mpi_add(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int ret, rsign;

    CRY_ASSERT2(r != NULL);
    CRY_ASSERT2(a != NULL);
    CRY_ASSERT2(b != NULL);

    if (a->sign == b->sign) {
        rsign = a->sign;
        ret = cry_mpi_add_abs(r, a, b);
    } else {
        switch (cry_mpi_cmp_abs(a, b)) {
        case 1:  /* a > b */
            rsign = a->sign;
            ret = cry_mpi_sub_abs(r, a, b);
            break;
        case -1: /* a < b */
            rsign = b->sign;
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
