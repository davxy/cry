#include "mpi_pvt.h"
#include "misc.h"
#include <cry/config.h>

#define KARATSUBA_CUTOFF 64 /* TODO: take in consideration the digit size */

int cry_mpi_sqr(cry_mpi *r, const cry_mpi *a)
{
    if (cry_mpi_is_zero(a)) {
        cry_mpi_zero(r);
        return 0;
    }

#ifdef CRY_MPI_MUL_KARATSUBA
    if (a->used > KARATSUBA_CUTOFF)
        return cry_mpi_sqr_karatsuba(r, a);
#endif

#ifdef CRY_MPI_MUL_COMBA
    return cry_mpi_sqr_comba(r, a);
#else
    return cry_mpi_sqr_baseline(r, a);
#endif
}
