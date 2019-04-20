#include "mpi_pvt.h"

int cry_mpi_div(cry_mpi *q, cry_mpi *r, const cry_mpi *a,
                const cry_mpi *b)
{
    int res, sign = a->sign ^ b->sign;

    if ((res = cry_mpi_div_abs(q, r, a, b)) != 0)
        return res;

    if (sign != 0) {
        if (q) {
            cry_mpi one;
            cry_mpi_digit one_dig = 1;

            one.sign = 0;
            one.used = one.alloc = 1;
            one.data = &one_dig;
            q->sign = 1;
            res = cry_mpi_sub(q, q, &one);
        }
        if (r)
            res = cry_mpi_sub_abs(r, b, r);
    }
    return res;
}
