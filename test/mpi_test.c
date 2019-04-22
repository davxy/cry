#include "test.h"
#include <cry/mpi.h>
#include <stdlib.h>


cry_mpi *g_mpi_buf;
#define g_mpi0 (&g_mpi_buf[0])
#define g_mpi1 (&g_mpi_buf[1])
#define g_mpi2 (&g_mpi_buf[2])
#define g_mpi3 (&g_mpi_buf[3])
#define MPI_BUF_LEN   4

void mpi_setup(void)
{
    g_mpi_buf = malloc(sizeof(cry_mpi)*MPI_BUF_LEN);
    cry_mpi_init_list(g_mpi0, g_mpi1, g_mpi2, g_mpi3, NULL);
}

void mpi_teardown(void)
{
    cry_mpi_clear_list(g_mpi0, g_mpi1, g_mpi2, g_mpi3, NULL);
    free(g_mpi_buf);
}

static void mpi_init_size(int argc, char *argv[])
{
    cry_mpi tmp;

    ASSERT_EQ(argc, 1);
    ASSERT_OK(cry_mpi_init_size(&tmp, atol(argv[0])));

    ASSERT_EQ(tmp.alloc, atol(argv[0]));

    cry_mpi_clear(&tmp);
}

static void mpi_count_bits(int argc, char *argv[])
{
    ASSERT_EQ(argc, 2);
    ASSERT_OK(cry_mpi_load_str(g_mpi0, 16, argv[0]));

    ASSERT_EQ(cry_mpi_count_bits(g_mpi0), atol(argv[1]));
}

static void mpi_count_bytes(int argc, char *argv[])
{
    ASSERT_EQ(argc, 2);
    ASSERT_OK(cry_mpi_load_str(g_mpi0, 16, argv[0]));

    ASSERT_EQ(cry_mpi_count_bytes(g_mpi0), atol(argv[1]));
}

static void mpi_set_int(int argc, char *argv[])
{
    long l1, l2;

    ASSERT_EQ(argc, 1);
    l1 = atol(argv[0]);

    ASSERT_OK(cry_mpi_set_int(g_mpi0, l1));
    ASSERT_OK(cry_mpi_get_int(g_mpi0, &l2));

    ASSERT_EQ(l1, l2);
}

static void mpi_load_store_str(int argc, char *argv[])
{
    int res;

    ASSERT_EQ(argc, 3);
    res = atoi(argv[2]);

    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == res);
    if (res == 0) {
        ASSERT(cry_mpi_store_str(g_mpi0, 16, (char *)g_buf) == 0);
        ASSERT(strcmp((char *)g_buf, argv[1]) == 0);
    }
}

static void mpi_cmp(int argc, char *argv[])
{
    int res;

    ASSERT_EQ(argc, 3);
    res = atoi(argv[2]);

    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == 0);
    ASSERT(cry_mpi_load_str(g_mpi1, 16, argv[1]) == 0);

    ASSERT(cry_mpi_cmp(g_mpi0, g_mpi1) == res);
}

static void mpi_shl(int argc, char *argv[])
{
    int bits;

    ASSERT_EQ(argc, 3);
    bits = atoi(argv[1]);
    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == 0);

    ASSERT(cry_mpi_shl(g_mpi1, g_mpi0, bits) == 0);

    ASSERT(cry_mpi_store_str(g_mpi1, 16, (char *)g_buf) == 0);
    ASSERT(strcmp((char *)g_buf, argv[2]) == 0);
}

static void mpi_shr(int argc, char *argv[])
{
    int bits;

    ASSERT_EQ(argc, 3);
    bits = atoi(argv[1]);
    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == 0);

    ASSERT(cry_mpi_shr(g_mpi1, g_mpi0, bits) == 0);

    ASSERT(cry_mpi_store_str(g_mpi1, 16, (char *)g_buf) == 0);
    ASSERT(strcmp((char *)g_buf, argv[2]) == 0);
}

static void mpi_abs(int argc, char *argv[])
{
    ASSERT_EQ(argc, 2);

    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == 0);

    ASSERT(cry_mpi_abs(g_mpi1, g_mpi0) == 0);

    ASSERT(cry_mpi_store_str(g_mpi1, 16, (char *)g_buf) == 0);
    ASSERT(strcmp((char *)g_buf, argv[1]) == 0);
}

static void mpi_sqr(int argc, char *argv[])
{
    ASSERT_EQ(argc, 2);

    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == 0);

    ASSERT(cry_mpi_sqr(g_mpi1, g_mpi0) == 0);

    ASSERT(cry_mpi_store_str(g_mpi1, 16, (char *)g_buf) == 0);
    ASSERT(strcmp((char *)g_buf, argv[1]) == 0);
}

#define ERROR_FLAG 'Z'

static void check(int res, cry_mpi *num, char *res_str)
{
    if (*res_str != ERROR_FLAG) {
        ASSERT(res == 0);
        ASSERT(cry_mpi_store_str(num, 16, (char *)g_buf) == 0);
        ASSERT(strcmp((char *)g_buf, res_str) == 0);
    } else {
        ASSERT(atoi(res_str + 1) == res);
    }
}

typedef int (* binary_op_f)(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

static void mpi_binary_op(int argc, char *argv[], binary_op_f op)
{
    int res;

    ASSERT_EQ(argc, 3);

    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == 0);
    ASSERT(cry_mpi_load_str(g_mpi1, 16, argv[1]) == 0);

    res = op(g_mpi2, g_mpi0, g_mpi1);

    check(res, g_mpi2, argv[2]);
}

typedef int (* binary_mod_op_f)(cry_mpi *r, const cry_mpi *a,
        const cry_mpi *b, const cry_mpi *m);

static void mpi_binary_mod_op(int argc, char *argv[], binary_mod_op_f op)
{
    int res;

    ASSERT_EQ(argc, 4);

    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == 0);
    ASSERT(cry_mpi_load_str(g_mpi1, 16, argv[1]) == 0);
    ASSERT(cry_mpi_load_str(g_mpi2, 16, argv[2]) == 0);

    res = op(g_mpi3, g_mpi0, g_mpi1, g_mpi2);

    check(res, g_mpi3, argv[3]);
}

static void mpi_dispatch(int argc, char *argv[])
{
    char *test = *argv;

    mpi_setup();

    argv++;
    argc--;
    if (strcmp(test, "mpi_init_size") == 0)
        mpi_init_size(argc, argv);
    else if (strcmp(test, "mpi_count_bits") == 0)
        mpi_count_bits(argc, argv);
    else if (strcmp(test, "mpi_count_bytes") == 0)
        mpi_count_bytes(argc, argv);
    else if (strcmp(test, "mpi_set_int") == 0)
        mpi_set_int(argc, argv);
    else if (strcmp(test, "mpi_load_store_str") == 0)
        mpi_load_store_str(argc, argv);
    else if (strcmp(test, "mpi_abs") == 0)
        mpi_abs(argc, argv);
    else if (strcmp(test, "mpi_cmp") == 0)
        mpi_cmp(argc, argv);
    else if (strcmp(test, "mpi_shl") == 0)
        mpi_shl(argc, argv);
    else if (strcmp(test, "mpi_shr") == 0)
        mpi_shr(argc, argv);
    else if (strcmp(test, "mpi_add") == 0)
        mpi_binary_op(argc, argv, cry_mpi_add);
    else if (strcmp(test, "mpi_sub") == 0)
        mpi_binary_op(argc, argv, cry_mpi_sub);
    else if (strcmp(test, "mpi_mul_baseline") == 0)
        mpi_binary_op(argc, argv, cry_mpi_mul_baseline);
    else if (strcmp(test, "mpi_mul_comba") == 0)
        mpi_binary_op(argc, argv, cry_mpi_mul_comba);
    else if (strcmp(test, "mpi_mul_karatsuba") == 0)
        mpi_binary_op(argc, argv, cry_mpi_mul_karatsuba);
    else if (strcmp(test, "mpi_mul_toom3") == 0)
        mpi_binary_op(argc, argv, cry_mpi_mul_toom3);
    else if (strcmp(test, "mpi_exp") == 0)
        mpi_binary_op(argc, argv, cry_mpi_exp);
    else if (strcmp(test, "mpi_mod_exp") == 0)
        mpi_binary_mod_op(argc, argv, cry_mpi_mod_exp);
    else if (strcmp(test, "mpi_sqr") == 0)
        mpi_sqr(argc, argv);
    else
        printf("Test '%s' not defined\n", test);

    mpi_teardown();
}

void mpi_test(void)
{
    printf("* MPI\n");
    func_test("mpi_test.data", mpi_dispatch);
    printf("\n");
}
