#include "mpi_pvt.h"

int cry_mpi_mul(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int res, sign;

    sign = a->sign ^ b->sign;
    res = cry_mpi_mul_abs(r, a, b);
    if (res == 0)
        r->sign = sign;
    return res;
}
