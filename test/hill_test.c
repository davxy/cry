#include "test.h"
#include <cry/hill.h>

#define RAW_MAX    101

struct hill_param {
    unsigned char key[RAW_MAX];
    unsigned char ikey[RAW_MAX];
    unsigned char src[RAW_MAX];
    unsigned char dst[RAW_MAX];
    size_t        keylen;
    size_t        ikeylen;
    size_t        srclen;
    size_t        dstlen;
};

static void param_init(struct hill_param *par, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->keylen  = raw_init(par->key, sizeof(par->key), argv[0]);
    par->ikeylen = raw_init(par->ikey, sizeof(par->ikey), argv[1]);
    par->srclen  = raw_init(par->src, sizeof(par->src), argv[2]);
    par->dstlen  = raw_init(par->dst, sizeof(par->dst), argv[3]);
}

static void hill_encrypt(int argc, char *argv[])
{
    struct cry_hill_ctx ctx;
    struct hill_param par;
    unsigned char dst[32];
    unsigned char *key, *ikey;

    ASSERT(argc == 4);
    param_init(&par, argv);

    key  = (par.keylen != 0)  ? par.key : NULL;
    ikey = (par.ikeylen != 0) ? par.ikey : NULL;
    cry_hill_init(&ctx, key, ikey, par.keylen);
    cry_hill_encrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.dstlen);
}

static void hill_decrypt(int argc, char *argv[])
{
    struct cry_hill_ctx ctx;
    struct hill_param par;
    unsigned char dst[32];
    unsigned char *key, *ikey;

    ASSERT(argc == 4);
    param_init(&par, argv);

    key  = (par.keylen != 0)  ? par.key : NULL;
    ikey = (par.ikeylen != 0) ? par.ikey : NULL;
    cry_hill_init(&ctx, key, ikey, par.keylen);
    cry_hill_decrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.dstlen);
}


static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;

    if (strcmp(test, "hill_encrypt") == 0)
        hill_encrypt(argc, argv);
    else if (strcmp(test, "hill_decrypt") == 0)
        hill_decrypt(argc, argv);
    else
        printf("Test '%s' not defined\n", test);
}


void hill_test(void)
{
    printf("* HILL Test\n");
    func_test("hill_test.data", dispatch);
    printf("\n");
}
