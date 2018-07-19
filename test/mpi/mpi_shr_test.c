#include "mpi_test.h"
#include "mpi/mpi_pvt.h"


static void shrd_dummy(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    cry_mpi_shrd(g_mpi0, 0);
    cry_mpi_print(g_mpi0, 16);
}

static void shrd_all(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    cry_mpi_shrd(g_mpi0, g_mpi0->used + 1);
    cry_mpi_print(g_mpi0, 16);
}

static void shr_one_digit(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 2*sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    ASSERT_OK(cry_mpi_shr(g_mpi1, g_mpi0, 8*sizeof(cry_mpi_digit)));
    cry_mpi_print(g_mpi1, 16);

    //ASSERT_EQ_MPI(g_mpi2, g_a8_b8_sub_bin, sizeof(g_a8_b8_sub_bin));
}

static void shr_all(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    cry_mpi_print(g_mpi0, 16);
    size_t bits = 8*sizeof(cry_mpi_digit)*(g_mpi0->used + 1);
    ASSERT_OK(cry_mpi_shr(g_mpi1, g_mpi0, bits));
    cry_mpi_print(g_mpi1, 16);

    //ASSERT_EQ_MPI(g_mpi2, g_a8_b8_sub_bin, sizeof(g_a8_b8_sub_bin));
}

static void shr_not_digit_multiple(void)
{
    long val;
    ASSERT_OK(cry_mpi_set_int(g_mpi0, A32_VAL));

    ASSERT_OK(cry_mpi_shr(g_mpi1, g_mpi0, 1));

    ASSERT_OK(cry_mpi_get_int(g_mpi1, &val));

    ASSERT_EQ(val, 0x6497105c);
}

static void shr_not_digit_multiple_carry(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 2*sizeof(cry_mpi_digit)));

    ASSERT_OK(cry_mpi_shr(g_mpi1, g_mpi0, 1));
    cry_mpi_print(g_mpi1, 16);
}


void mpi_shr_test(void)
{
    MPI_RUN(shrd_dummy);
    MPI_RUN(shrd_all);
    MPI_RUN(shr_one_digit);
    MPI_RUN(shr_all);
    MPI_RUN(shr_not_digit_multiple);
    MPI_RUN(shr_not_digit_multiple_carry);
}
