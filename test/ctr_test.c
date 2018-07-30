#include "test.h"
#include <cry/ctr.h>
#include <cry/aes.h>


static const cry_ciph_itf aes_itf = {
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
};

static void aes_xxx_ctr_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key, size_t ksize,
                                const unsigned char *iv)
{
    cry_ctr_ctx ctx;
    cry_aes_ctx aes_ctx;

    cry_ctr_init(&ctx, &aes_ctx, &aes_itf);
    cry_ctr_key_set(&ctx, key, ksize);
    cry_ctr_iv_set(&ctx, iv, 16);
    cry_ctr_encrypt(&ctx, dst, src, size);
}

static void aes_xxx_ctr_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key, size_t ksize,
                                const unsigned char *iv)
{
    cry_ctr_ctx ctx;
    cry_aes_ctx aes_ctx;

    cry_ctr_init(&ctx, &aes_ctx, &aes_itf);
    cry_ctr_key_set(&ctx, key, ksize);
    cry_ctr_iv_set(&ctx, iv, 16);
    cry_ctr_decrypt(&ctx, dst, src, size);
}

static void aes_128_ctr_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_ctr_encrypt(dst, src, size, key, 16, iv);
}

static void aes_128_ctr_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_ctr_decrypt(dst, src, size, key, 16, iv);
}

static void aes_192_ctr_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_ctr_encrypt(dst, src, size, key, 24, iv);
}

static void aes_192_ctr_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_ctr_decrypt(dst, src, size, key, 24, iv);
}

static void aes_256_ctr_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_ctr_encrypt(dst, src, size, key, 32, iv);
}

static void aes_256_ctr_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key,
                                const unsigned char *iv)
{
    aes_xxx_ctr_decrypt(dst, src, size, key, 32, iv);
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

    if (strcmp("aes_128_ctr_encrypt", argv[0]) == 0)
        aes_128_ctr_encrypt(out2, in, 16, key, iv);
    if (strcmp("aes_128_ctr_decrypt", argv[0]) == 0)
        aes_128_ctr_decrypt(out2, in, 16, key, iv);
    if (strcmp("aes_192_ctr_encrypt", argv[0]) == 0)
        aes_192_ctr_encrypt(out2, in, 16, key, iv);
    if (strcmp("aes_192_ctr_decrypt", argv[0]) == 0)
        aes_192_ctr_decrypt(out2, in, 16, key, iv);
    if (strcmp("aes_256_ctr_encrypt", argv[0]) == 0)
        aes_256_ctr_encrypt(out2, in, 16, key, iv);
    if (strcmp("aes_256_ctr_decrypt", argv[0]) == 0)
        aes_256_ctr_decrypt(out2, in, 16, key, iv);
    ASSERT_EQ_BUF(out, out2, 16);
}


void ctr_test(void)
{
    func_test("AES-CTR NIST KAT", "aes_ctr.data", dispatch);
}
