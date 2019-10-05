#include "test.h"
#include <cry/utils.h>
#include <string.h>


struct aes_wrap_param {
    size_t keylen;
    size_t srclen;
    size_t dstlen;
    unsigned char key[32];
    unsigned char src[40];
    unsigned char dst[40];
};

static void param_init(struct aes_wrap_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->keylen = raw_init(par->key, sizeof(par->key), argv[0]);
    par->srclen = raw_init(par->src, sizeof(par->src), argv[1]);
    par->dstlen = raw_init(par->dst, sizeof(par->dst), argv[2]);
    ASSERT_EQ(par->srclen + 8, par->dstlen);
}


static void aes_wrap(int argc, char *argv[])
{
    int res;
    struct aes_wrap_param par;
    unsigned char dst[40];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_aes_wrap(dst, par.src, par.srclen, par.key, par.keylen);
    ASSERT_EQ_BUF(dst, par.dst, par.dstlen);

    res = cry_aes_unwrap(dst, par.dst, par.srclen, par.key, par.keylen);
    ASSERT_EQ(res, 0);
    ASSERT_EQ_BUF(dst, par.src, par.srclen);
}

static void aes_wrap_error(void)
{
    int res;
    unsigned char dst[16], src[24], key[16];

    memset(src, 0, sizeof(src));
    memset(dst, 0, sizeof(dst));
    memset(key, 0, sizeof(key));

    res = cry_aes_unwrap(dst, src, sizeof(dst), key, sizeof(key));
    ASSERT_EQ(res, -1);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;

    if (strcmp(test, "aes_wrap") == 0)
        aes_wrap(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void utils_test(void)
{
    TRACE("* AES Key Wrap\n");
    func_test("aes_wrap_test.data", dispatch);
    RUN(aes_wrap_error);
    TRACE("\n");
}
