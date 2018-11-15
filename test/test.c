#include "test.h"

void version_test(void);
void memxor_test(void);
void base64_test(void);
void mpi_test(void);
void hmac_test(void);
void des_test(void);
void aes_test(void);
void hill_test(void);
void affine_test(void);


static int g_runs;
int g_fails;
int g_verbose;
unsigned char g_buf[BUFSIZ];

static const char *g_test_str[] = {
    "version",
    "memxor",
    "base64",
    "mpi",
    "hmac",
    "des",
    "aes",
    "hill",
    "affine",
};

static char g_test_skip[ARLEN(g_test_str)];


static test_func_t g_test_func[] = {
    version_test,
    memxor_test,
    base64_test,
    mpi_test,
    hmac_test,
    des_test,
    aes_test,
    hill_test,
    affine_test,
#if 0
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

#define NTESTS  ARLEN(g_test_func)



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
    int fails;

    file = fopen(datafile, "r");
    if (file == NULL) {
        printf("Error: data file \"%s\" not found\n", datafile);
        return;
    }

    while (feof(file) == 0) {
        /* Test Name */
        if ((ret = get_line(file, argbuf, sizeof(argbuf))) != 0)
            break;
        if (*argbuf == '#' || *argbuf == '\0')
            continue;
        left = sizeof(argbuf);
        if (g_verbose != 0)
            printf("    %s\n", argbuf);
        /* Collect test function name and parameters */
        cnt = strlen(argbuf) + 1;
        curr = argbuf + cnt;
        left = sizeof(argbuf) - cnt;
        i = 0;
        while ((ret = get_line(file, curr, left)) == 0) {
            if (strlen(curr) == 0)
                break; /* last parameter read */
            params[i] = curr;
            cnt = strlen(params[i]) + 1;
            curr += cnt;
            left -= cnt;
            i++;
        }
        fails = g_fails;
        dispatch(i, params);
        g_runs++;
        if (g_fails != fails)
            printf("      %s\n", argbuf);
    }
    fclose(file);
}


void run(const char *name, void (* test)(void),
         void (* setup)(void), void (* teardown)(void))
{
    int fails;

    g_runs++;
    if (g_verbose != 0)
        printf("    %s\n", name);
    if (setup != NULL)
        setup();
    fails = g_fails;
    test();
    if (g_fails != fails)
        printf("      %s\n", name);
    if (teardown != NULL)
        teardown();
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


#define EMPTY_STRING "NULL"

int raw_init(unsigned char *raw, unsigned int rawlen, const char *asc)
{
    int len;

    if (strcmp(asc, EMPTY_STRING) == 0)
        return 0;
    if (strcmp(asc, "-1") == 0)
        return -1;
    len = strlen(asc);
    if (rawlen < (len >> 1))
        len = rawlen << 1;
    asc_to_raw(asc, len, raw);
    return len >> 1;
}


static void help(const char *arg)
{
    if (arg != NULL)
        printf("Error: unsupported option '%s'\n", arg);
    printf("\nUsage: test [options] [tests]\n");
    printf("  -h    help\n");
    printf("  -l    show cases\n");
    printf("  -v    verbose\n");
    printf("\n");
    exit(0);
}

static void show_cases(void)
{
    int i;

    printf("\nTest cases:\n");
    for (i = 0; i < NTESTS; i++)
        printf("  %s\n", g_test_str[i]);
    printf("\n");
    exit(0);
}

static void parse_args(int argc, char *argv[])
{
    int i = 1, j;

    while (i < argc) {
        if (argv[i][0] != '-')
            break;
        if (strcmp(argv[i], "-v") == 0)
            g_verbose = 1;
        else if (strcmp(argv[i], "-h") == 0)
            help(NULL);
        else if (strcmp(argv[i], "-l") == 0)
            show_cases();
        else
            help(argv[i]);
        i++;
    }
    /* Eventually disable some tests */
    if (i < argc) {
        memset(g_test_skip, 1, sizeof(g_test_skip));
        do {
            for (j = 0; j < NTESTS; j++) {
                if (strcmp(argv[i], g_test_str[j]) == 0)
                    g_test_skip[j] = 0;
            }
            i++;
        } while (i < argc);
    }
}

int main(int argc, char *argv[])
{
    int i;

    /* Reset global variables, just in case... */
    g_runs = 0;
    g_fails = 0;
    g_verbose = 0;
    memset(g_test_skip, 0, sizeof(g_test_skip));
    /* Parse arguments */
    parse_args(argc, argv);

    printf("\nCRY(T_T)EST\n\n");

    for (i = 0; i < NTESTS; i++) {
        if (g_test_skip[i] == 0)
            g_test_func[i]();
    }

    printf("\n");
    printf("|| Tests: %d\n", g_runs);
    printf("|| Fails: %d\n", g_fails);
    printf("\n-------------------------------------\n\n");

    return (g_fails != 0);
}
