#include "test.h"
#include <cry/affine.h>

#define RAW_MAX    100

struct affine_param {
    unsigned char keya[CRY_AFFINE_KEYMAX];
    unsigned char keyb[CRY_AFFINE_KEYMAX];
    unsigned char src[CRY_AFFINE_KEYMAX];
    unsigned char dst[CRY_AFFINE_KEYMAX];
    size_t keylen;
    size_t srclen;
    size_t dstlen;
};

static void param_init(struct affine_param *par, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->keylen = raw_init(par->keya, sizeof(par->keya), argv[0]);
    par->keylen = raw_init(par->keyb, sizeof(par->keyb), argv[1]);
    par->srclen = raw_init(par->src, sizeof(par->src), argv[2]);
    par->dstlen = raw_init(par->dst, sizeof(par->dst), argv[3]);
}

static void affine_encrypt(int argc, char *argv[])
{
    struct cry_affine_ctx ctx;
    struct affine_param par;
    unsigned char dst[32];

    ASSERT(argc == 4);
    param_init(&par, argv);

    cry_affine_init(&ctx, par.keya, par.keyb, par.keylen);
    cry_affine_encrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.dstlen);
}

static void affine_decrypt(int argc, char *argv[])
{
    struct cry_affine_ctx ctx;
    struct affine_param par;
    unsigned char dst[32];

    ASSERT(argc == 4);
    param_init(&par, argv);

    cry_affine_init(&ctx, par.keya, par.keyb, par.keylen);
    cry_affine_decrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.dstlen);
}


static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;

    if (strcmp(test, "affine_encrypt") == 0)
        affine_encrypt(argc, argv);
    else if (strcmp(test, "affine_decrypt") == 0)
        affine_decrypt(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void affine_test(void)
{
    struct cry_affine_ctx ctx;
    unsigned char keya[] = { 1, 1, 1 };
    unsigned char keyb[] = { 19, 10, 201 };
    unsigned char ciphertext[100] = {0};
    unsigned char cleartext[100] = {0};

    cry_affine_init(&ctx, keya, keyb, sizeof(keya));
    cry_affine_encrypt(&ctx, ciphertext, (unsigned char *)"HelloWorld", 10);
    cry_affine_decrypt(&ctx, cleartext, ciphertext, 10);

    TRACE("* Affine\n");
    func_test("affine_test.data", dispatch);
    TRACE("\n");
}
