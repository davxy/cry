#include "mpi_test.h"

static cry_mpi a, b;

static void simple_cmp(void)
{
    cry_mpi_load_str(&a, 16, "12345678");
    cry_mpi_load_str(&b, 16, "12345678");

    ASSERT_EQ(cry_mpi_cmp(&a, &b), 0);
}

static void not_equal_same_len(void)
{
    cry_mpi_load_str(&a, 16, "12");
    cry_mpi_load_str(&b, 16, "21");

    ASSERT_EQ(cry_mpi_cmp(&a, &b) < 0, 1);
    ASSERT_EQ(cry_mpi_cmp(&b, &a) > 0, 1);
}

static void not_equal_not_same_len(void)
{
    cry_mpi_load_str(&a, 16, "12");
    cry_mpi_load_str(&b, 16, "12121212121212121212121212121121212121212121212");

    ASSERT_EQ(cry_mpi_cmp(&a, &b) < 0, 1);
    ASSERT_EQ(cry_mpi_cmp(&b, &a) > 0, 1);
}


static void setup(void)
{
    cry_mpi_init_list(&a, &b, NULL);
}

static void teardown(void)
{
    cry_mpi_clear_list(&a, &b, NULL);
}

#define MYRUN(test) RUNX(test, setup, teardown)

void mpi_cmp_test(void)
{
    MYRUN(simple_cmp);
    MYRUN(not_equal_same_len);
    MYRUN(not_equal_not_same_len);
}
