#include "test.h"
#include <cry/arc4.h>


struct arc4_param {
    size_t keylen;
    size_t buflen;
    unsigned char key[32];
    unsigned char src[512];
    unsigned char dst[512];
};

static void param_init(struct arc4_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->keylen = raw_init(par->key, sizeof(par->key), argv[0]);
    par->buflen = raw_init(par->src, sizeof(par->src), argv[1]);
    par->buflen = raw_init(par->dst, sizeof(par->dst), argv[2]);
}


static void arc4_crypt(int argc, char *argv[])
{
    cry_arc4_ctx ctx;
    struct arc4_param par;
    unsigned char dst[512];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_arc4_init(&ctx);
    cry_arc4_key_set(&ctx, par.key, par.keylen);
    cry_arc4_crypt(&ctx, dst, par.src, par.buflen);
    cry_arc4_clear(&ctx);

    ASSERT_EQ_BUF(dst, par.dst, par.buflen);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;

    if (strcmp(test, "arc4_crypt") == 0)
        arc4_crypt(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void arc4_test(void)
{
    TRACE("* ARC4 IETF RFC6229 Vectors\n");
    func_test("arc4_test.data", dispatch);
    TRACE("\n");
}
