#include "mpi_pvt.h"

int cry_mpi_abs(cry_mpi *d, const cry_mpi *s)
{
    int res;

    if (d != s) {
        if ((res = cry_mpi_copy(d, s)) != 0)
            return res;
    }
    d->sign = 0;
    return 0;
}

