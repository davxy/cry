#include "test.h"

int test_runs;
int test_fails;
int test_cont;
int test_stop;
unsigned char g_buf[BUFSIZ];


static int get_line(FILE *f, char *buf, size_t len)
{
    char *ret;

    *buf = '\0';
    ret = fgets(buf, len, f);
    if (ret == NULL)
        return -1;
    if (strlen(buf) && buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = '\0';
    if (strlen(buf) && buf[strlen(buf)-1] == '\r')
        buf[strlen(buf)-1] = '\0';
    return 0;
}

void func_test(const char *datafile, dispatch_func_t dispatch)
{
    FILE *file;
    int ret, cnt;
    char *params[50];
    int i;
    char *curr;
    size_t left;
    static char argbuf[BUFSIZ];

    file = fopen(datafile, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: data file \"%s\" not found", datafile);
        return;
    }

    while (feof(file) == 0) {
        /* Test Name */
        if ((ret = get_line(file, argbuf, sizeof(argbuf))) != 0)
            break;
        if (*argbuf == '#' || *argbuf == '\0')
            continue;
        fprintf(stdout, "    %s\n", argbuf);
        /* Collect test function name and parameters */
        i = 0;
        curr = argbuf;
        left = sizeof(argbuf);
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
        test_runs++;
    }
    fclose(file);
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
    test_runs++;
    printf("    %s\n", name);
    if (setup != NULL)
        setup();
    test();
    if (teardown != NULL)
        teardown();
}

void version_test(void);
void memxor_test(void);
void base64_test(void);
void mpi_test(void);
void aes_test(void);

static test_func_t tests[] = {
    version_test,
    memxor_test,
    base64_test,
    mpi_test,
    aes_test,
#if 0
    TEST_ELEM(cbc),
    TEST_ELEM(ctr),
    TEST_ELEM(gcm),
#endif
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
    int i;

    /* Reset test variables */
    test_runs = 0;
    test_fails = 0;
    test_cont = 0;
    test_stop = 0;

    printf("\nCRY(T_T)EST\n\n");

    for (i = 0; i < TESTS_NUM; i++) {
        tests[i]();
        if (test_stop != 0)
            break;
    }

    printf("\n");
    printf("|| Tests: %d\n", test_runs);
    printf("|| Fails: %d\n", test_fails);
    printf("\n-------------------------------------\n\n");

    return (test_fails != 0);
}
