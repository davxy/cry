#include "test.h"
#include <cry/aes.h>
#include <cry/cbc.h>
#include <cry/ctr.h>
#include <cry/gcm.h>


static const cry_ciph_itf aes_itf = {
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
    .decrypt = (cry_ciph_decrypt_f) cry_aes_decrypt,
};

struct aes_param {
    unsigned int  keylen;
    unsigned int  ivlen;
    unsigned int  aadlen;
    unsigned int  srclen;
    unsigned int  dstlen;
    unsigned int  maclen;
    unsigned char key[32];
    unsigned char iv[16];
    unsigned char mac[16];
    unsigned char aad[128];
    unsigned char src[128];
    unsigned char dst[128];
};

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

static void param_init(struct aes_param *par, int argc, char *argv[])
{
    int i = 0;

    memset(par, 0, sizeof(*par));
    par->keylen = raw_init(par->key, sizeof(par->key), argv[i++]);
    if (argc >= 4) {
        /* CBC, CTR, GCM */
        par->ivlen = raw_init(par->iv, sizeof(par->iv), argv[i++]);
        if (argc == 6) {
            /* GCM */
            par->aadlen = raw_init(par->aad, sizeof(par->aad), argv[i++]);
            par->maclen = raw_init(par->mac, sizeof(par->mac), argv[i++]);
        }
    }
    par->srclen = raw_init(par->src, sizeof(par->src), argv[i++]);
    par->dstlen = raw_init(par->dst, sizeof(par->dst), argv[i++]);
    if (par->dstlen != -1)
        ASSERT_EQ(par->srclen, par->dstlen);
}

static void aes_ecb_encrypt(int argc, char *argv[])
{
    cry_aes_ctx ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_aes_key_set(&ctx, par.key, par.keylen);
    cry_aes_encrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.srclen);
}

static void aes_ecb_decrypt(int argc, char *argv[])
{
    cry_aes_ctx ctx;
    struct aes_param par;
    unsigned char dst[32];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_aes_key_set(&ctx, par.key, par.keylen);
    cry_aes_decrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.srclen);
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
    cry_cbc_encrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.srclen);
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
    cry_cbc_decrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.srclen);
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
    cry_ctr_encrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.srclen);
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
    cry_ctr_decrypt(&ctx, dst, par.src, par.srclen);

    ASSERT_EQ_BUF(dst, par.dst, par.srclen);
}

static void aes_gcm_encrypt(int argc, char *argv[])
{
    cry_gcm_ctx ctx;
    cry_aes_ctx aes_ctx;
    struct aes_param par;
    unsigned char dst[128];
    unsigned char mac[16];

    ASSERT(argc == 6);
    param_init(&par, argc, argv);

    cry_gcm_init(&ctx, &aes_ctx, &aes_itf);
    cry_gcm_key_set(&ctx, par.key, par.keylen);
    cry_gcm_iv_set(&ctx, par.iv, 16);
    cry_gcm_update(&ctx, par.aad, par.aadlen);
    cry_gcm_encrypt(&ctx, dst, par.src, par.srclen);
    cry_gcm_digest(&ctx, mac, par.maclen);

    ASSERT_EQ_BUF(dst, par.dst, par.srclen);
    ASSERT_EQ_BUF(mac, par.mac, par.maclen);
}

static void aes_gcm_decrypt(int argc, char *argv[])
{
    cry_gcm_ctx ctx;
    cry_aes_ctx aes_ctx;
    struct aes_param par;
    unsigned char dst[128];
    unsigned char mac[16];

    ASSERT(argc == 6);
    param_init(&par, argc, argv);

    cry_gcm_init(&ctx, &aes_ctx, &aes_itf);
    cry_gcm_key_set(&ctx, par.key, par.keylen);
    cry_gcm_iv_set(&ctx, par.iv, 16);
    cry_gcm_update(&ctx, par.aad, par.aadlen);
    cry_gcm_decrypt(&ctx, dst, par.src, par.srclen);
    cry_gcm_digest(&ctx, mac, par.maclen);

    if (par.dstlen == par.srclen) {
        ASSERT_EQ_BUF(mac, par.mac, par.maclen);
        ASSERT_EQ_BUF(dst, par.dst, par.dstlen);
    } else {
        ASSERT_NE_BUF(mac, par.mac, par.maclen);
    }
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
    else if (strcmp(test, "aes_gcm_encrypt") == 0)
        aes_gcm_encrypt(argc, argv);
    else if (strcmp(test, "aes_gcm_decrypt") == 0)
        aes_gcm_decrypt(argc, argv);
    else
        fprintf(stdout, "Test '%s' not defined\n", test);
}

void aes_test(void)
{
    fprintf(stdout, "* AES NIST AESAVS KAT\n");
    func_test("aes_test.data", dispatch);
    fprintf(stdout, "* AES GCM NIST Encrypt Validation (iv,txt,aad,mac)\n");
    func_test("aes_gcm_en_test.data", dispatch);
    fprintf(stdout, "* AES GCM NIST Decrypt Validation (iv,txt,aad,mac)\n");
    func_test("aes_gcm_de_test.data", dispatch);
    fprintf(stdout, "* AES-192 GCM NIST Encrypt Validation (iv,txt,aad,mac)\n");
    func_test("aes_gcm_en_192_test.data", dispatch);
    fprintf(stdout, "\n");
}
