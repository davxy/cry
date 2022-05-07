#include <cry/mpi.h>

#ifndef CRY_MPI_FIXED_WIDTH
static const cry_mpi_digit g_zero_digit = 0;
static const cry_mpi_digit g_one_digit = 1;
static const cry_mpi_digit g_three_digit = 3;
#endif

const cry_mpi g_zero = {
    .sign = 0,
    .used = 1,
    .alloc = 0, /* prevents dynamic release */
#ifdef CRY_MPI_FIXED_WIDTH
    .data[0] = 0,
#else
    .data = (cry_mpi_digit *)&g_zero_digit
#endif
};

const cry_mpi g_one = {
    .sign = 0,
    .used = 1,
    .alloc = 0, /* prevents dynamic release */
#ifdef CRY_MPI_FIXED_WIDTH
    .data[0] = 1,
#else
    .data = (cry_mpi_digit *)&g_one_digit
#endif
};

const cry_mpi g_three = {
    .sign = 0,
    .used = 1,
    .alloc = 0, /* prevents dynamic release */
#ifdef CRY_MPI_FIXED_WIDTH
    .data[0] = 3,
#else
    .data = (cry_mpi_digit *)&g_three_digit
#endif
};
