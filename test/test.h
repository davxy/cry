#ifndef CRY_TEST_H_
#define CRY_TEST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int g_fails;
extern unsigned char g_buf[BUFSIZ];


typedef void (* test_func_t)(void);

struct test_case {
    const char *name;
    test_func_t func;
};

typedef void (* dispatch_func_t)(int argc, char *argv[]);


void func_test(const char *datafile, dispatch_func_t dispatch);

void asc_to_raw(const char *asc, size_t size, unsigned char *raw);

int raw_init(unsigned char *raw, unsigned int rawlen, const char *asc);


#define ARLEN(ar) (sizeof(ar)/sizeof(ar[0]))


void run(const char *name, void (* test)(void),
         void (* setup)(void), void (* teardown)(void));

#define ASSERT(test) do { \
    if ((test) == 0) { \
        g_fails++; \
        printf("!!! ASSERTION FAIL (%s:%d): %s\n", \
                __FILE__, __LINE__, #test); \
        return; \
    } \
    } while (0)

#define ASSERT_EQ(i1, i2) \
    ASSERT((i1) == (i2))

#define ASSERT_NE(i1, i2) \
    ASSERT((i1) != (i2))

#define ASSERT_EQ_BUF(b1, b2, len) \
    ASSERT(memcmp(b1, b2, len) == 0)

#define ASSERT_NE_BUF(b1, b2, len) \
    ASSERT(memcmp(b1, b2, len) != 0)

#define ASSERT_EQ_MPI(mpi, bin, siz) do { \
    ASSERT_EQ(cry_mpi_count_bytes(mpi), siz); \
    ASSERT_EQ(BUFSIZ >= siz, 1); \
    cry_mpi_store_bin(mpi, (char *)g_buf, BUFSIZ, 0); \
    ASSERT(memcmp((char *)g_buf, bin, siz) == 0); \
    } while (0)

#define ASSERT_OK(e) \
    ASSERT_EQ((e), 0)


#define PRINT_HEX(msg, buf, siz) do { \
    size_t __siz = siz; \
    unsigned char *__p = buf; \
    if (msg) printf("%s: ", msg); \
    while (__siz-- > 0) printf("%02x", *__p++); \
    printf("\n"); \
    } while(0)

#define PRINT_ASC(msg, buf, siz) \
    printf("%s: %.*s\n", msg, (int)(siz), buf)

#define PRINT_MPI(msg, mpi, rad) do { \
    printf("%s:\t", msg); \
    cry_mpi_print(mpi, rad); \
    } while(0)

/* Constant seed to make "rand" results predictable */
#define RAND_SEED_RAW  "RANDSEED"
#define RAND_SEED_SIZ  (sizeof(RAND_SEED_RAW) - 1)

#endif /* _TEST_H_ */
