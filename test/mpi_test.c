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

    ASSERT_EQ(argc, 2);
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

    ASSERT_EQ(argc, 2);
    res = atoi(argv[1]);

    ASSERT(cry_mpi_load_str(g_mpi0, 16, argv[0]) == res);
    if (res == 0) {
        ASSERT(cry_mpi_store_str(g_mpi0, 16, (char *)g_buf, BUFSIZ) == 0);
        ASSERT(strcmp((char *)g_buf, argv[0]) == 0);
    }
}


static void dispatch(int argc, char *argv[])
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
    else
        ASSERT(0);

    mpi_teardown();
}



void mpi_test(void)
{
    func_test("MPI", "mpi.data", dispatch);
}
