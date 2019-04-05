#include "test.h"
#include <cry/md5.h>

struct md5_param {
    size_t len;
    unsigned char *data;
    unsigned char hash[CRY_MD5_DIGEST_SIZE];
};

static void param_init(struct md5_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->len = strlen(argv[0]) >> 1;
    par->data = malloc(par->len);
    par->len = raw_init(par->data, par->len, argv[0]);
    raw_init(par->hash, CRY_MD5_DIGEST_SIZE, argv[1]);
}

static void md5_digest(int argc, char *argv[])
{
    struct md5_param par;
    unsigned char out[CRY_MD5_DIGEST_SIZE];

    param_init(&par, argc, argv);
    cry_md5(out, par.data, par.len);
    ASSERT_EQ_BUF(out, par.hash, CRY_MD5_DIGEST_SIZE);
    free(par.data);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;
    ASSERT(argc == 2);

    if (strcmp(test, "md5_digest") == 0)
        md5_digest(argc, argv);
    else
        printf("Test '%s' not defined\n", test);
}

void md5_test(void)
{
    printf("* MD5 RFC 1321 Vectors\n");
    func_test("md5_test.data", dispatch);
    printf("\n");
}
