#include "test.h"

int test_runs;
int test_fails;
int test_level;
int test_cont;
int test_stop;
unsigned char buf[BUFSIZ];


void run(const char *name, void (* test)(void),
         void (* setup)(void), void (* teardown)(void))
{
    int i;

    test_runs++;
    if (test_level == 0)
        printf("\n");
    for (i = 0; i < test_level; i++)
        printf("---");
    printf("> %s\n", name);
    test_level++;
    if (setup != NULL)
        setup();
    test();
    if (teardown != NULL)
        teardown();
    test_level--;
}


#define TEST_WRAP(name) \
    void name ## _test(void); \
    void name ## _wrap(void) { RUN( name ## _test ); }

TEST_WRAP(version)
TEST_WRAP(memxor)
TEST_WRAP(base64)
TEST_WRAP(mpi)
#if 0
TEST_WRAP(des)
TEST_WRAP(aes)
TEST_WRAP(cbc)
TEST_WRAP(ctr)
TEST_WRAP(gcm)
TEST_WRAP(crc)
TEST_WRAP(md5)
TEST_WRAP(sha256)
TEST_WRAP(cmac)
TEST_WRAP(sum)
TEST_WRAP(rsa)
TEST_WRAP(rand)
TEST_WRAP(dh)
TEST_WRAP(dsa)
TEST_WRAP(ecp)
TEST_WRAP(ecdsa)
TEST_WRAP(ecdh)
#endif

struct test_def {
    const char *name;
    void (*func)(void);
};

#define TEST_ELEM(name) { #name , name ## _wrap }

static struct test_def tests[] = {
    TEST_ELEM(version),
    TEST_ELEM(memxor),
    TEST_ELEM(base64),
    TEST_ELEM(mpi),
#if 0
    TEST_ELEM(des),
    TEST_ELEM(aes),
    TEST_ELEM(cbc),
    TEST_ELEM(ctr),
    TEST_ELEM(gcm),
    TEST_ELEM(crc),
    TEST_ELEM(md5),
    TEST_ELEM(sha256),
    TEST_ELEM(cmac),
    TEST_ELEM(sum),
    TEST_ELEM(rsa),
    TEST_ELEM(rand),
    TEST_ELEM(dh),
    TEST_ELEM(dsa),
    TEST_ELEM(ecp),
    TEST_ELEM(ecdsa),
    TEST_ELEM(ecdh),
#endif
};

#define TESTS_NUM   (sizeof(tests)/sizeof(*tests))


int main(int argc, char *argv[])
{
    int i, j;

    /* Reset test variables */
    test_runs = 0;
    test_fails = 0;
    test_level = 0;
    test_cont = 0;
    test_stop = 0;

    printf("\nCRY(T_T)EST\n");

    if (argc == 1) {
        for (j = 0; j < TESTS_NUM; j++) {
            tests[j].func();
            if (test_stop != 0)
                break;
        }
    } else {
        for (i = 1; i < argc; i++) {
            for (j = 0; j < TESTS_NUM; j++) {
                if (strcmp(argv[i], tests[j].name) == 0) {
                    tests[j].func();
                    if (test_stop != 0) {
                        i = argc;
                        break;
                    }
                }
            }
        }
    }

    printf("\n");
    printf("|| Tests: %d\n", test_runs);
    printf("|| Fails: %d\n", test_fails);
    printf("\n-------------------------------------\n\n");

    return (test_fails != 0);
}
