#include "mpi_test.h"


static void set_int(void)
{
    ASSERT_OK(cry_mpi_set_int(g_mpi0, LONG_VALUE));

    ASSERT_EQ_MPI(g_mpi0, g_long_value_bin);
}

#if 0
static void set_zero(void)
{
    ASSERT_OK(cry_mpi_set_int(g_mpi0, 0));

    ASSERT_EQ(cry_mpi_is_zero(g_mpi0), 1);
}

static void set_neg(void)
{
    cry_mpi_set_int(g_mpi0, -10);

    ASSERT_EQ(cry_mpi_is_neg(g_mpi0), 1);
}

static void copy(void)
{
    ASSERT_OK(cry_mpi_load_str(g_mpi0, 16, "12345678"));

    ASSERT_OK(cry_mpi_copy(g_mpi1, g_mpi0));

    //ASSERT_EQ_MPI(g_mpi1, 16, "12345678");
}

static void copy_grow(void)
{
    unsigned int words = g_mpi1->used;

    ASSERT_OK(cry_mpi_load_str(g_mpi0, 16, "123456782983641298734187253123129834"));

    ASSERT_OK(cry_mpi_copy(g_mpi1, g_mpi0));

    //ASSERT_EQ_MPI(g_mpi1, 16, "123456782983641298734187253123129834");
    ASSERT_EQ(g_mpi1->used > words, 1);
}

static void count_bits(void)
{
    ASSERT_OK(cry_mpi_load_str(g_mpi0, 16, "1"));

    ASSERT_EQ(cry_mpi_count_bits(g_mpi0), 1);
}

static void count_bits_full(void)
{
    ASSERT_OK(cry_mpi_load_str(g_mpi0, 16, "ffffffffffffffff"));

    ASSERT_EQ(cry_mpi_count_bits(g_mpi0), sizeof(cry_mpi_digit)*8);
}

#endif

void mpi_core_test(void)
{
    MPI_RUN(set_int);
#if 0
    MPI_RUN(set_zero);
    MPI_RUN(set_neg);
    MPI_RUN(copy);
    MPI_RUN(copy_grow);
    MPI_RUN(count_bits);
    MPI_RUN(count_bits_full);
#endif
}
