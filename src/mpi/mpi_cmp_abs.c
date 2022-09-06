#include "mpi_pvt.h"

int cry_mpi_cmp_abs(const cry_mpi *a, const cry_mpi *b)
{
    size_t i;
    const cry_mpi_digit *ap, *bp;

    if (a->used < b->used)
        return -1;
    else if (a->used > b->used)
        return 1;

    i = a->used;
    ap = &a->data[i - 1];
    bp = &b->data[i - 1];
    while (i-- > 0) {
        if (*ap < *bp)
            return -1;
        if (*ap > *bp)
            return 1;
        ap--;
        bp--;
    }
    return 0;
}
