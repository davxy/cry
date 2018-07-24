#include "mpi_test.h"
#include "mpi/mpi_pvt.h"


static void shld_dummy(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    ASSERT_OK(cry_mpi_shld(g_mpi0, 0));

    ASSERT_EQ_MPI(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit));
}

static void shld_expand(void)
{
    char *raw;
    size_t siz;

    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));
    siz = sizeof(cry_mpi_digit) * 10;
    ASSERT_NE(raw = calloc(1, siz), NULL);
    memcpy(raw, g_a6400_bin, sizeof(cry_mpi_digit));

    ASSERT_OK(cry_mpi_shld(g_mpi0, 9));

    ASSERT_EQ_MPI(g_mpi0, raw, siz);

    free(raw);
}

static void shl_one_digit(void)
{
    char *raw;
    size_t siz;

    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));
    siz = sizeof(cry_mpi_digit) * 2;
    ASSERT_NE(raw = calloc(1, siz), NULL);
    memcpy(raw, g_a6400_bin, sizeof(cry_mpi_digit));

    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, 8*sizeof(cry_mpi_digit)));

    ASSERT_EQ_MPI(g_mpi1, raw, siz);

    free(raw);
}

static void shl_expand(void)
{
    char *raw;
    size_t siz, bits;

    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));
    siz = sizeof(cry_mpi_digit) * 10;
    ASSERT_NE(raw = calloc(1, siz), NULL);
    memcpy(raw, g_a6400_bin, sizeof(cry_mpi_digit));

    bits = 8 * sizeof(cry_mpi_digit) * 9;
    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, bits));

    ASSERT_EQ_MPI(g_mpi1, raw, siz);

    free(raw);
}

static void shl_not_digit_multiple(void)
{
    long val;

    ASSERT_OK(cry_mpi_set_int(g_mpi0, A32_VAL));

    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, 1));

    ASSERT_OK(cry_mpi_get_int(g_mpi1, &val));

    ASSERT_EQ(val, 0x1925c4172);
}

static void shl_not_digit_multiple_carry(void)
{
    const char *raw = "\x01\x92\x5c\x41\x73\x21\x08\xd4\x20";

    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(cry_mpi_digit)));

    ASSERT_OK(cry_mpi_shl(g_mpi1, g_mpi0, 1));

    ASSERT_EQ_MPI(g_mpi1, raw, 9);
}


void mpi_shl_test(void)
{
    MPI_RUN(shld_dummy);
    MPI_RUN(shld_expand);
    MPI_RUN(shl_one_digit);
    MPI_RUN(shl_expand);
    MPI_RUN(shl_not_digit_multiple);
    MPI_RUN(shl_not_digit_multiple_carry);
}
