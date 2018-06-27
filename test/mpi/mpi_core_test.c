#include "mpi_test.h"


static void count_bits(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(g_a6400_bin)));

    ASSERT_EQ(cry_mpi_count_bits(g_mpi0), 6400);
}


static void count_bytes(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(g_a6400_bin)));

    ASSERT_EQ(cry_mpi_count_bytes(g_mpi0), 800);
}

static void init_size(void)
{
    cry_mpi tmp;

    cry_mpi_init_size(&tmp, 1);

    ASSERT_EQ(tmp.alloc, 1);

    cry_mpi_clear(&tmp);
}

/*
 * Get/Set int
 */

static void set_zero(void)
{
    ASSERT_OK(cry_mpi_set_int(g_mpi0, 0));

    ASSERT_EQ(cry_mpi_is_zero(g_mpi0), 1);
}

static void set_pos_int(void)
{
    ASSERT_OK(cry_mpi_set_int(g_mpi0, A32_VAL));

    ASSERT_EQ_MPI(g_mpi0, g_a6400_bin, cry_mpi_count_bytes(g_mpi0));
}

static void get_pos_int(void)
{
    long val;

    ASSERT_OK(cry_mpi_set_int(g_mpi0, A32_VAL));

    ASSERT_OK(cry_mpi_get_int(g_mpi0, &val));

    ASSERT_EQ(val, A32_VAL);
}

static void set_neg_int(void)
{
    cry_mpi_set_int(g_mpi0, -A32_VAL);

    ASSERT_EQ(cry_mpi_is_neg(g_mpi0), 1);
    ASSERT_EQ_MPI(g_mpi0, g_a6400_bin, cry_mpi_count_bytes(g_mpi0));
}

static void get_neg_int(void)
{
    long val;

    ASSERT_OK(cry_mpi_set_int(g_mpi0, -A32_VAL));

    ASSERT_OK(cry_mpi_get_int(g_mpi0, &val));

    ASSERT_EQ(val, -A32_VAL);
}

static void init_int(void)
{
    cry_mpi tmp;

    ASSERT_OK(cry_mpi_init_int(&tmp, A32_VAL));

    cry_mpi_clear(&tmp);
}

/*
 * Get/Set bin
 */

static void set_bin(void)
{
    long val;

    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 4));

    ASSERT_OK(cry_mpi_get_int(g_mpi0, &val));
    ASSERT_EQ(val, A32_VAL);
}

static void get_bin(void)
{
    ASSERT_OK(cry_mpi_set_int(g_mpi0, A32_VAL));

    ASSERT_OK(cry_mpi_store_bin(g_mpi0, buf, BUFSIZ, 0));
    ASSERT_EQ(memcmp(buf, g_a6400_bin, 4), 0);
}

static void bin_init(void)
{
    cry_mpi tmp;
    long val;

    ASSERT_OK(cry_mpi_init_bin(&tmp, g_a6400_bin, 4));

    ASSERT_OK(cry_mpi_get_int(&tmp, &val));
    ASSERT_EQ(val, A32_VAL);

    cry_mpi_clear(&tmp);
}

/*
 * Copy
 */


static void copy_over_smaller(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, sizeof(g_a6400_bin)));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, 16));

    ASSERT_OK(cry_mpi_copy(g_mpi1, g_mpi0));

    ASSERT_EQ_MPI(g_mpi1, g_a6400_bin, sizeof(g_a6400_bin));
}

static void copy_over_bigger(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 32));
    ASSERT_OK(cry_mpi_load_bin(g_mpi1, g_a6400_bin, sizeof(g_a6400_bin)));

    ASSERT_OK(cry_mpi_copy(g_mpi1, g_mpi0));

    ASSERT_EQ_MPI(g_mpi1, g_a6400_bin, 32);
}

static void copy_over_same(void)
{
    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 32));

    ASSERT_OK(cry_mpi_copy(g_mpi0, g_mpi0));
}

static void copy_init(void)
{
    cry_mpi tmp;

    ASSERT_OK(cry_mpi_load_bin(g_mpi0, g_a6400_bin, 32));

    ASSERT_OK(cry_mpi_init_copy(&tmp, g_mpi0));

    ASSERT_EQ_MPI(&tmp, g_a6400_bin, 32);
    cry_mpi_clear(&tmp);
}


void mpi_core_test(void)
{
    MPI_RUN(count_bits);
    MPI_RUN(count_bytes);
    MPI_RUN(set_zero);
    MPI_RUN(init_size);
    /* Int */
    MPI_RUN(set_pos_int);
    MPI_RUN(get_pos_int);
    MPI_RUN(set_neg_int);
    MPI_RUN(get_neg_int);
    MPI_RUN(init_int);
    /* Bin */
    MPI_RUN(bin_init);
    MPI_RUN(set_bin);
    MPI_RUN(get_bin);
    /* Copy */
    MPI_RUN(copy_over_smaller);
    MPI_RUN(copy_over_bigger);
    MPI_RUN(copy_over_same);
    MPI_RUN(copy_init);
}
