#include "mpi_test.h"

static cry_mpi a, b, r;
static size_t siz;


static void setup(void)
{
    memset(buf, 0, BUFSIZ);
    ASSERT_OK(cry_mpi_init_list(&a, &b, &r, NULL));
}

static void teardown(void)
{
    cry_mpi_clear_list(&a, &b, &r, NULL);
}



static void simple(void)
{
    ASSERT_OK(cry_mpi_set_int(&a, 0x02));
    ASSERT_OK(cry_mpi_set_int(&b, 0x03));

    ASSERT_OK(cry_mpi_mul(&r, &a, &b));

    cry_mpi_store_bin(&r, buf, BUFSIZ, 0);
    ASSERT_EQ(memcmp(buf, "\x06", 1), 0);
}


#define DIGIT_MAX_BIN "\xff\xff\xff\xff\xff\xff\xff\xff"
#define DIGIT_MAX_SIZ 8
#define DIGIT_MAX_MUL_BIN \
        "\xff\xff\xff\xff\xff\xff\xff\xfe\x00\x00\x00\x00\x00\x00\x00\x01"
#define DIGIT_MAX_MUL_SIZ \
        (sizeof(DIGIT_MAX_MUL_BIN)-1)

static void digit_max(void)
{
    ASSERT_OK(cry_mpi_load_bin(&a, DIGIT_MAX_BIN, DIGIT_MAX_SIZ));
    ASSERT_OK(cry_mpi_load_bin(&b, DIGIT_MAX_BIN, DIGIT_MAX_SIZ));

    ASSERT_OK(cry_mpi_mul(&r, &a, &b));

    siz = cry_mpi_store_bin(&r, (char *)buf, BUFSIZ, 0);
    ASSERT_EQ(siz, DIGIT_MAX_MUL_SIZ);
    ASSERT_EQ(memcmp(buf, DIGIT_MAX_MUL_BIN, DIGIT_MAX_MUL_SIZ), 0);
}

static void by_zero(void)
{
    ASSERT_OK(cry_mpi_load_bin(&a, DIGIT_MAX_BIN, DIGIT_MAX_SIZ));
    cry_mpi_zero(&b);

    ASSERT_OK(cry_mpi_mul(&r, &a, &b));

    ASSERT_EQ(cry_mpi_is_zero(&r), 1);
}





static void karatsuba(void)
{
    cry_mpi c, d;

    ASSERT_OK(cry_mpi_init_list(&c, &d, NULL));
    ASSERT_OK(cry_mpi_load_bin(&a, g_a6400_bin, sizeof(g_a6400_bin)));
    ASSERT_OK(cry_mpi_load_bin(&b, g_b6400_bin, sizeof(g_b6400_bin)));
    ASSERT_OK(cry_mpi_load_bin(&c, g_a6400_b6400_mul_bin, sizeof(g_a6400_b6400_mul_bin)));
    ASSERT_OK(cry_mpi_mul_karatsuba(&d, &a, &b));

    ASSERT_EQ(cry_mpi_cmp(&c, &d), 0);

    cry_mpi_clear_list(&a, &b, &c, &d, NULL);
}

static void comba(void)
{
    cry_mpi c, d;

    ASSERT_OK(cry_mpi_init_list(&c, &d, NULL));
    ASSERT_OK(cry_mpi_load_bin(&a, g_a6400_bin, sizeof(g_a6400_bin)));
    ASSERT_OK(cry_mpi_load_bin(&b, g_b6400_bin, sizeof(g_b6400_bin)));
    ASSERT_OK(cry_mpi_load_bin(&c, g_a6400_b6400_mul_bin, sizeof(g_a6400_b6400_mul_bin)));
    ASSERT_OK(cry_mpi_mul_comba(&d, &a, &b));

    ASSERT_EQ(cry_mpi_cmp(&c, &d), 0);

    cry_mpi_clear_list(&a, &b, &c, &d, NULL);
}



#define MYRUN(test) RUNX(test, setup, teardown)

void mpi_mul_test(void)
{
    MYRUN(simple);
    MYRUN(digit_max);
    MYRUN(by_zero);
    MYRUN(karatsuba);
    MYRUN(comba);
}
