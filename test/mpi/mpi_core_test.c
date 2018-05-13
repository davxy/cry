#include "mpi_test.h"

static cry_mpi a, b;

static void long_init(void)
{
    cry_mpi_init_int(&a, 0x123456789abcdef);

    ASSERT_EQ_MPI(&a, 16, "123456789abcdef");
    cry_mpi_clear(&a);
}

static void zero_init(void)
{
    cry_mpi_zero(&a);

    ASSERT_EQ(cry_mpi_is_zero(&a), 1);
    cry_mpi_clear(&a);
}

static void zero_set(void)
{
    cry_mpi_init(&a);
    cry_mpi_set_int(&a, 0);

    ASSERT_EQ(cry_mpi_is_zero(&a), 1);
    cry_mpi_clear(&a);
}

static void copy(void)
{
    ASSERT_OK(cry_mpi_init_str(&a, 16, "12345678"));

    ASSERT_OK(cry_mpi_init_copy(&b, &a));
    ASSERT_EQ_MPI(&b, 16, "12345678");

    cry_mpi_clear(&a);
    cry_mpi_clear(&b);
}

static void copy_grow(void)
{
    ASSERT_OK(cry_mpi_init_str(&a, 16, "123456782983641298734187253123129834"));

    ASSERT_OK(cry_mpi_init_copy(&b, &a));
    ASSERT_EQ_MPI(&b, 16, "123456782983641298734187253123129834");

    cry_mpi_clear(&a);
    cry_mpi_clear(&b);
}

static void count_bits(void)
{
    ASSERT_OK(cry_mpi_init_str(&a, 16, "1"));

    ASSERT_EQ(cry_mpi_count_bits(&a), 1);
}

static void count_bits_full(void)
{
    ASSERT_OK(cry_mpi_init_str(&a, 16, "ffffffffffffffff"));

    ASSERT_EQ(cry_mpi_count_bits(&a), sizeof(cry_mpi_digit)*8);
}

void mpi_core_test(void)
{
    RUN(zero_init);
    RUN(zero_set);
    RUN(long_init);
    RUN(copy);
    RUN(copy_grow);
    RUN(count_bits);
    RUN(count_bits_full);
}

