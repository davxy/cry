#include "test.h"
#include <cry/sha1.h>

struct sha1_param {
    size_t len;
    unsigned char *data;
    unsigned char hash[CRY_SHA1_DIGEST_SIZE];
};

static void param_init(struct sha1_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->len = strlen(argv[0]) >> 1;
    par->data = malloc(par->len);
    par->len = raw_init(par->data, par->len, argv[0]);
    raw_init(par->hash, CRY_SHA1_DIGEST_SIZE, argv[1]);
}

static void sha1_digest(int argc, char *argv[])
{
    struct sha1_param par;
    unsigned char out[CRY_SHA1_DIGEST_SIZE];

    param_init(&par, argc, argv);
    cry_sha1(out, par.data, par.len);
    ASSERT_EQ_BUF(out, par.hash, CRY_SHA1_DIGEST_SIZE);
    free(par.data);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;
    ASSERT(argc == 2);

    if (strcmp(test, "sha1_digest") == 0)
        sha1_digest(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void sha1_test(void)
{
    TRACE("* SHA1 NIST CAVS Vectors\n");
    func_test("sha1_test.data", dispatch);
    TRACE("\n");
}
