#include "test.h"
#include <cry/trivium.h>


struct trivium_param {
    size_t keylen;
    size_t ivlen;
    size_t buflen;
    unsigned char key[10];
    unsigned char iv[10];
    unsigned char buf[512];
};


static void param_init(struct trivium_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->keylen = raw_init(par->key, sizeof(par->key), argv[0]);
    par->ivlen = raw_init(par->iv, sizeof(par->iv), argv[1]);
    par->buflen = raw_init(par->buf, sizeof(par->buf), argv[2]);
}


static void trivium_operate(int argc, char *argv[])
{
    cry_trivium_ctx ctx;
    struct trivium_param par;
    unsigned char src[512] = {0};
    unsigned char dst[512];

    ASSERT(argc == 4);
    param_init(&par, argc, argv);

    cry_trivium_init(&ctx);
    cry_trivium_key_set(&ctx, par.key, par.keylen);
    cry_trivium_iv_set(&ctx, par.iv, par.ivlen);
    cry_trivium_encrypt(&ctx, dst, src, 512);

    ASSERT_EQ_BUF(dst, par.buf, 512);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;

    if (strcmp(test, "trivium_operate") == 0)
        trivium_operate(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void trivium_test(void)
{
    TRACE("* TRIVIUM ECRYPT Vectors\n");
    func_test("trivium_ecrypt.data", dispatch);
    TRACE("\n");
}
