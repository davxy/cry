#include "mpi_test.h"


static void mpi_add_no_expand(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 1));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_b6400_bin, 1));

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ_MPI(g_mpi2, g_a8_b8_add_bin, sizeof(g_a8_b8_add_bin));
}

static void mpi_add_with_swap(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 1));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_b6400_bin, 32));

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ_MPI(g_mpi2, g_a8_b256_add_bin, sizeof(g_a8_b256_add_bin));
}

static void mpi_add_expand_no_final_carry(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 128));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 64));

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ_MPI(g_mpi2, g_a1024_a512_add_bin, sizeof(g_a1024_a512_add_bin));
}

static void mpi_add_expand_final_carry(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 128));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 128));

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ_MPI(g_mpi2, g_a1024_a1024_add_bin, sizeof(g_a1024_a1024_add_bin));
}

static void mpi_add_diff_sign_first_neg_and_bigger(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 33));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 32));
    g_mpi0->sign = 1;

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ(cry_mpi_is_neg(g_mpi2), 1);
    ASSERT_EQ_MPI(g_mpi2, g_a264_a256_add_sign_bin,
                   sizeof(g_a264_a256_add_sign_bin));
}

static void mpi_add_diff_sign_first_pos_and_bigger(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 33));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 32));
    g_mpi1->sign = 1;

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ(cry_mpi_is_neg(g_mpi2), 0);
    ASSERT_EQ_MPI(g_mpi2, g_a264_a256_add_sign_bin,
                   sizeof(g_a264_a256_add_sign_bin));
}

static void mpi_add_diff_sign_first_neg_and_smaller(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 32));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 33));
    g_mpi0->sign = 1;

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ(cry_mpi_is_neg(g_mpi2), 0);
    ASSERT_EQ_MPI(g_mpi2, g_a264_a256_add_sign_bin,
                   sizeof(g_a264_a256_add_sign_bin));
}

static void mpi_add_diff_sign_first_pos_and_smaller(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 32));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 33));
    g_mpi1->sign = 1;

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ(cry_mpi_is_neg(g_mpi2), 1);
    ASSERT_EQ_MPI(g_mpi2, g_a264_a256_add_sign_bin,
                   sizeof(g_a264_a256_add_sign_bin));
}

static void mpi_add_diff_sign_equal_magnitude(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 32));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 32));
    g_mpi0->sign = 1;

    ASSERT_OK(cry_mpi_add(g_mpi2, g_mpi0, g_mpi1));

    ASSERT_EQ(cry_mpi_is_pos(g_mpi2), 1);
    ASSERT_EQ(cry_mpi_is_zero(g_mpi2), 1);
}

/* Entry point */
void mpi_add_test(void)
{
    MPI_RUN(mpi_add_no_expand);
    MPI_RUN(mpi_add_with_swap);
    MPI_RUN(mpi_add_expand_no_final_carry);
    MPI_RUN(mpi_add_expand_final_carry);
    MPI_RUN(mpi_add_diff_sign_first_neg_and_bigger);
    MPI_RUN(mpi_add_diff_sign_first_pos_and_bigger);
    MPI_RUN(mpi_add_diff_sign_first_neg_and_smaller);
    MPI_RUN(mpi_add_diff_sign_first_pos_and_smaller);
    MPI_RUN(mpi_add_diff_sign_equal_magnitude);
}
