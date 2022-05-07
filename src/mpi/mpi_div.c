#include "mpi_pvt.h"

int cry_mpi_div(cry_mpi *q, cry_mpi *r, const cry_mpi *a,
                const cry_mpi *b)
{
    int res, sign = a->sign ^ b->sign;

    if ((res = cry_mpi_div_abs(q, r, a, b)) != 0) {
        return res;
    }

    if (sign != 0 && !cry_mpi_is_zero(r)) {
        if (q != NULL) {
            q->sign = 1;
            res = cry_mpi_sub(q, q, &g_one);
        }
        if (r != NULL) {
            res = cry_mpi_sub_abs(r, b, r);
        }
    }
    return res;
}
