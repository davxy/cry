#include "test.h"
#include <cry/aes.h>
#include <cry/cbc.h>


static const cry_ciph_itf aes_itf = {
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
    .decrypt = (cry_ciph_decrypt_f) cry_aes_decrypt,
};

static void aes_xxx_cbc_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key, size_t ksize,
                                const unsigned char *iv)
{
    cry_cbc_ctx ctx;
    cry_aes_ctx aes_ctx;

    cry_cbc_init(&ctx, &aes_ctx, &aes_itf);
    cry_cbc_key_set(&ctx, key, ksize);
    cry_cbc_iv_set(&ctx, iv, 16);
    cry_cbc_encrypt(&ctx, dst, src, size);
}

static void aes_xxx_cbc_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key, size_t ksize,
                                const unsigned char *iv)
{
    cry_cbc_ctx ctx;
    cry_aes_ctx aes_ctx;

    cry_cbc_init(&ctx, &aes_ctx, &aes_itf);
    cry_cbc_key_set(&ctx, key, ksize);
    cry_cbc_iv_set(&ctx, iv, 16);
    cry_cbc_decrypt(&ctx, dst, src, size);
}

static void aes_128_cbc_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_cbc_encrypt(dst, src, size, key, 16, iv);
}

static void aes_128_cbc_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_cbc_decrypt(dst, src, size, key, 16, iv);
}

static void aes_192_cbc_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_cbc_encrypt(dst, src, size, key, 24, iv);
}

static void aes_192_cbc_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_cbc_decrypt(dst, src, size, key, 24, iv);
}

static void aes_256_cbc_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_cbc_encrypt(dst, src, size, key, 32, iv);
}

static void aes_256_cbc_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_cbc_decrypt(dst, src, size, key, 32, iv);
}


static void dispatch(int argc, char *argv[])
{
    unsigned char key[32];
    unsigned char iv[32];
    unsigned char in[32];
    unsigned char out[32];
    unsigned char out2[32];

    asc_to_raw(argv[1], strlen(argv[1]), key);
    asc_to_raw(argv[2], 32, iv);
    asc_to_raw(argv[3], 32, in);
    asc_to_raw(argv[4], 32, out);

    if (strcmp("aes_128_cbc_encrypt", argv[0]) == 0)
        aes_128_cbc_encrypt(out2, in, 16, key, iv);
    if (strcmp("aes_128_cbc_decrypt", argv[0]) == 0)
        aes_128_cbc_decrypt(out2, in, 16, key, iv);
    if (strcmp("aes_192_cbc_encrypt", argv[0]) == 0)
        aes_192_cbc_encrypt(out2, in, 16, key, iv);
    if (strcmp("aes_192_cbc_decrypt", argv[0]) == 0)
        aes_192_cbc_decrypt(out2, in, 16, key, iv);
    if (strcmp("aes_256_cbc_encrypt", argv[0]) == 0)
        aes_256_cbc_encrypt(out2, in, 16, key, iv);
    if (strcmp("aes_256_cbc_decrypt", argv[0]) == 0)
        aes_256_cbc_decrypt(out2, in, 16, key, iv);
    ASSERT_EQ_BUF(out, out2, 16);
}


void cbc_test(void)
{
    func_test("AES-CBC NIST KAT", "aes_cbc.data", dispatch);
}

