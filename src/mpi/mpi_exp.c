#include "mpi_pvt.h"

int cry_mpi_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e)
{
    return cry_mpi_mod_exp(r, b, e, NULL);
}
