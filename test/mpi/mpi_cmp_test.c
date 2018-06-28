#include "mpi_test.h"

static void equal(void)
{
    cry_mpi_load_bin(g_mpi0, g_a6400_bin, 64);
    cry_mpi_load_bin(g_mpi1, g_a6400_bin, 64);

    ASSERT_EQ(cry_mpi_cmp(g_mpi0, g_mpi1), 0);
}

static void not_equal_same_len(void)
{
    cry_mpi_load_bin(g_mpi0, g_a6400_bin, 64);
    cry_mpi_load_bin(g_mpi1, g_b6400_bin, 64);

    ASSERT_EQ(cry_mpi_cmp(g_mpi0, g_mpi1) > 0, 1);
    ASSERT_EQ(cry_mpi_cmp(g_mpi1, g_mpi0) < 0, 1);
}

static void not_equal_same_len_neg(void)
{
    cry_mpi_load_bin(g_mpi0, g_a6400_bin, 64);
    cry_mpi_load_bin(g_mpi1, g_b6400_bin, 64);
    g_mpi0->sign = g_mpi1->sign = 1; /* Negative numbers */

    ASSERT_EQ(cry_mpi_cmp(g_mpi0, g_mpi1) < 0, 1);
    ASSERT_EQ(cry_mpi_cmp(g_mpi1, g_mpi0) > 0, 1);
}

static void not_equal_diff_len(void)
{
    cry_mpi_load_bin(g_mpi0, g_a6400_bin, 33);
    cry_mpi_load_bin(g_mpi1, g_a6400_bin, 32);

    ASSERT_EQ(cry_mpi_cmp(g_mpi0, g_mpi1) > 0, 1);
    ASSERT_EQ(cry_mpi_cmp(g_mpi1, g_mpi0) < 0, 1);
}

static void not_equal_sign(void)
{
    cry_mpi_load_bin(g_mpi0, g_a6400_bin, 32);
    cry_mpi_load_bin(g_mpi1, g_a6400_bin, 32);
    g_mpi1->sign = 1; /* Set mpi1 as negative */

    ASSERT_EQ(cry_mpi_cmp(g_mpi0, g_mpi1) > 0, 1);
    ASSERT_EQ(cry_mpi_cmp(g_mpi1, g_mpi0) < 0, 1);
}

void mpi_cmp_test(void)
{
    MPI_RUN(equal);
    MPI_RUN(not_equal_same_len);
    MPI_RUN(not_equal_same_len_neg);
    MPI_RUN(not_equal_diff_len);
    MPI_RUN(not_equal_sign);
}
