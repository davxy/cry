#include "mpi_test.h"
#include <stdlib.h>


cry_mpi *g_mpi_buf;

void mpi_setup(void)
{
    g_mpi_buf = malloc(sizeof(cry_mpi)*MPI_BUF_LEN);
    cry_mpi_init_list(g_mpi0, g_mpi1, g_mpi2, g_mpi3, NULL);
}

void mpi_teardown(void)
{
    cry_mpi_clear_list(g_mpi0, g_mpi1, g_mpi2, g_mpi3, NULL);
    free(g_mpi_buf);
}


void mpi_test(void)
{
    RUN(mpi_core_test);
    RUN(mpi_cmp_test);
    RUN(mpi_abs_test);
    RUN(mpi_add_test);
    RUN(mpi_sub_test);
    RUN(mpi_shl_test);
    RUN(mpi_shr_test);
//    RUN(mpi_mul_test);
}
