#include "mpi_test.h"

static void positive_abs(void)
{
    cry_mpi_set_int(g_mpi0, 123456);

    cry_mpi_abs(g_mpi0, g_mpi0);

    ASSERT_EQ(cry_mpi_is_pos(g_mpi0), 1);
}

static void negative_abs(void)
{
    cry_mpi_set_int(g_mpi0, -123456);

    cry_mpi_abs(g_mpi0, g_mpi0);

    ASSERT_EQ(cry_mpi_is_pos(g_mpi0), 1);
}

static void different_src_dst(void)
{
    cry_mpi_set_int(g_mpi0, -123456);

    cry_mpi_abs(g_mpi1, g_mpi0);

    ASSERT_EQ(cry_mpi_is_pos(g_mpi1), 1);
    ASSERT_EQ(cry_mpi_is_pos(g_mpi0), 0);
}


void mpi_abs_test(void)
{
    MPI_RUN(positive_abs);
    MPI_RUN(negative_abs);
    MPI_RUN(different_src_dst);
}
