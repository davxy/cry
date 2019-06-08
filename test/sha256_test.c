#include "test.h"
#include <cry/sha256.h>

struct sha256_param {
    size_t len;
    unsigned char *data;
    unsigned char hash[CRY_SHA256_DIGEST_SIZE];
};

static void param_init(struct sha256_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->len = strlen(argv[0]) >> 1;
    par->data = malloc(par->len);
    par->len = raw_init(par->data, par->len, argv[0]);
    raw_init(par->hash, CRY_SHA256_DIGEST_SIZE, argv[1]);
}

static void sha256_digest(int argc, char *argv[])
{
    struct sha256_param par;
    unsigned char out[CRY_SHA256_DIGEST_SIZE];

    param_init(&par, argc, argv);
    cry_sha256(out, par.data, par.len);
    ASSERT_EQ_BUF(out, par.hash, CRY_SHA256_DIGEST_SIZE);
    free(par.data);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;
    ASSERT(argc == 2);

    if (strcmp(test, "sha256_digest") == 0)
        sha256_digest(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void sha256_test(void)
{
    TRACE("* SHA256 NIST CAVS Vectors\n");
    func_test("sha256_test.data", dispatch);
    TRACE("\n");
}
