#include "mpi_pvt.h"

/*
 * Initialize a big number from a null terminated ASCII string.
 */
int cry_mpi_init_str(cry_mpi *a, unsigned int radix, const char *s)
{
    int res;

    res = cry_mpi_init(a);
    if (res == 0) {
        res = cry_mpi_load_str(a, radix, s);
        if (res != 0)
            cry_mpi_clear(a);
    }
    return res;
}

