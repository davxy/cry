#include "test.h"

int test_runs;
int test_fails;
int test_level;
int test_cont;
int test_stop;
unsigned char buf[BUFSIZ];

static int get_line(FILE *f, char *buf, size_t len)
{
    char *ret;

    ret = fgets(buf, len, f);
    if (ret == NULL)
        return -1;
    if (strlen(buf) && buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = '\0';
    if (strlen(buf) && buf[strlen(buf)-1] == '\r')
        buf[strlen(buf)-1] = '\0';
    return 0;
}

void func_test(const char *name, const char *datafile,
               void (*dispatch)(int argc, char *argv[]))
{
    FILE *file;
    int ret, cnt;
    char *params[50];
    int i;
    char *curr;
    size_t left;

    fprintf(stdout, ">>> %s\n", name);

    file = fopen(datafile, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: data file \"%s\" not found", datafile);
        return;
    }

    while (feof(file) == 0) {
        curr = (char *)buf;
        left = BUFSIZ;
        /* Test Name */
        if ((ret = get_line(file, (char*)buf, left)) != 0)
            break;
        fprintf(stdout, "    %s\n", buf);
        /* Collect test function name and parameters */
        i = 0;
        while ((ret = get_line(file, curr, left)) == 0) {
            if (strlen(curr) == 0)
                break; /* last parameter read */
            params[i] = curr;
            cnt = strlen(params[i])+1;
            curr += cnt;
            left -= cnt;
            i++;
        }
        dispatch(i, params);
    }
}


void asc_to_raw(const char *asc, size_t size, unsigned char *raw)
{
    unsigned char c;
    size_t i = 0;
    size_t j = 0;

    while (j < size) {
        c = asc[j];
        if ('0' <= c && c <= '9')
            raw[i] = (c - '0') << 4;
        else if ('a' <= c && c <= 'f')
            raw[i] = ((c - 'a') + 10) << 4;
        else if ('A' <= c && c <= 'F')
            raw[i] = ((c - 'A') + 10) << 4;
        else
            raw[i] = 0;   /* Fallback for no-ascii values. */
        j++;

        c = asc[j];
        if ('0' <= c && c <= '9')
            raw[i] |= (c - '0');
        else if ('a' <= c && c <= 'f')
            raw[i] |= ((c - 'a') + 10);
        else if ('A' <= c && c <= 'F')
            raw[i] |= ((c - 'A') + 10);
        else
            raw[i] = 0;   /* Fallback for no-ascii values. */
        j++;
        i++;
    }
}








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
TEST_WRAP(aes)
TEST_WRAP(cbc)
TEST_WRAP(ctr)
TEST_WRAP(gcm)
#if 0
TEST_WRAP(des)
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
    TEST_ELEM(aes),
    TEST_ELEM(cbc),
    TEST_ELEM(ctr),
    TEST_ELEM(gcm),
#if 0
    TEST_ELEM(des),
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
