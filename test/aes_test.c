#include "test.h"
#include <cry/aes.h>
#include <cry/cbc.h>
#include <cry/ctr.h>

static const cry_ciph_itf aes_itf = {
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
    .decrypt = (cry_ciph_decrypt_f) cry_aes_decrypt,
};

struct aes_param {
    unsigned int  keylen;
    unsigned int  len;
    unsigned char key[32];
    unsigned char iv[32];
    unsigned char src[32];
    unsigned char dst[32];
};

static void param_init(struct aes_param *par, int argc, char *argv[])
{
    int i = 0;

    par->keylen = strlen(argv[i]);
    asc_to_raw(argv[i], par->keylen, par->key);
    i++;
    if (argc == 4) {
        asc_to_raw(argv[i], 32, par->iv);
        i++;
    } else {
        memset(par->iv, 0, sizeof(par->iv));
    }
    par->len = strlen(argv[i]);
    asc_to_raw(argv[i], par->len, par->src);
    i++;
    asc_to_raw(argv[i], par->len, par->dst);
    par->keylen >>= 1;
    par->len >>= 1;
}

static void aes_ecb_encrypt(int argc, char *argv[])
{
    cry_aes_ctx ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_aes_key_set(&ctx, par.key, par.keylen);
    cry_aes_encrypt(&ctx, dst, par.src, par.len);

    ASSERT_EQ_BUF(dst, par.dst, par.len);
}

static void aes_ecb_decrypt(int argc, char *argv[])
{
    cry_aes_ctx ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_aes_key_set(&ctx, par.key, par.keylen);
    cry_aes_decrypt(&ctx, dst, par.src, par.len);

    ASSERT_EQ_BUF(dst, par.dst, par.len);
}

static void aes_cbc_encrypt(int argc, char *argv[])
{
    cry_cbc_ctx ctx;
    cry_aes_ctx aes_ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 4);
    param_init(&par, argc, argv);

    cry_cbc_init(&ctx, &aes_ctx, &aes_itf);
    cry_cbc_key_set(&ctx, par.key, par.keylen);
    cry_cbc_iv_set(&ctx, par.iv, 16);
    cry_cbc_encrypt(&ctx, dst, par.src, par.len);

    ASSERT_EQ_BUF(dst, par.dst, par.len);
}

static void aes_cbc_decrypt(int argc, char *argv[])
{
    cry_cbc_ctx ctx;
    cry_aes_ctx aes_ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 4);
    param_init(&par, argc, argv);

    cry_cbc_init(&ctx, &aes_ctx, &aes_itf);
    cry_cbc_key_set(&ctx, par.key, par.keylen);
    cry_cbc_iv_set(&ctx, par.iv, 16);
    cry_cbc_decrypt(&ctx, dst, par.src, par.len);

    ASSERT_EQ_BUF(dst, par.dst, par.len);
}

static void aes_ctr_encrypt(int argc, char *argv[])
{
    cry_ctr_ctx ctx;
    cry_aes_ctx aes_ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 4);
    param_init(&par, argc, argv);

    cry_ctr_init(&ctx, &aes_ctx, &aes_itf);
    cry_ctr_key_set(&ctx, par.key, par.keylen);
    cry_ctr_iv_set(&ctx, par.iv, 16);
    cry_ctr_encrypt(&ctx, dst, par.src, par.len);

    ASSERT_EQ_BUF(dst, par.dst, par.len);
}

static void aes_ctr_decrypt(int argc, char *argv[])
{
    cry_ctr_ctx ctx;
    cry_aes_ctx aes_ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 4);
    param_init(&par, argc, argv);

    cry_ctr_init(&ctx, &aes_ctx, &aes_itf);
    cry_ctr_key_set(&ctx, par.key, par.keylen);
    cry_ctr_iv_set(&ctx, par.iv, 16);
    cry_ctr_decrypt(&ctx, dst, par.src, par.len);

    ASSERT_EQ_BUF(dst, par.dst, par.len);
}


static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;

    if (strcmp(test, "aes_ecb_encrypt") == 0)
        aes_ecb_encrypt(argc, argv);
    else if (strcmp(test, "aes_ecb_decrypt") == 0)
        aes_ecb_decrypt(argc, argv);
    else if (strcmp(test, "aes_cbc_encrypt") == 0)
        aes_cbc_encrypt(argc, argv);
    else if (strcmp(test, "aes_cbc_decrypt") == 0)
        aes_cbc_decrypt(argc, argv);
    else if (strcmp(test, "aes_ctr_encrypt") == 0)
        aes_ctr_encrypt(argc, argv);
    else if (strcmp(test, "aes_ctr_decrypt") == 0)
        aes_ctr_decrypt(argc, argv);
    else
        fprintf(stdout, "Test '%s' not defined\n", test);
}

void aes_test(void)
{
    fprintf(stdout, "* AES NIST AESAVS KAT\n");
    func_test("aes_test.data", dispatch);
    fprintf(stdout, "\n");
}
