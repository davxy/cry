#include "mpi_pvt.h"

/*
 * Compare two big numbers values.
 */
int cry_mpi_cmp(const cry_mpi *a, const cry_mpi *b)
{
    int ret;

    if (a->sign > b->sign)
        ret = -1;  /* a is negative and b is positive */
    else if (a->sign < b->sign)
        ret = 1;   /* a is positive and b is negative */
    else {
        ret = cry_mpi_cmp_abs(a, b);
        if (a->sign == 1)
            ret = -ret; /* negative numbers */
    }
    return ret;
}
