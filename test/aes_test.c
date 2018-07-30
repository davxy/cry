#include "test.h"
#include <cry/aes.h>


static void aes_xxx_ecb_encrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key, size_t keysize)
{
    cry_aes_ctx ctx;

    cry_aes_key_set(&ctx, key, keysize);
    cry_aes_encrypt(&ctx, dst, src, size);
}

static void aes_xxx_ecb_decrypt(unsigned char *dst,
                                const unsigned char *src, size_t size,
                                const unsigned char *key, size_t keysize)
{
    cry_aes_ctx ctx;

    cry_aes_key_set(&ctx, key, keysize);
    cry_aes_decrypt(&ctx, dst, src, size);
}

static void aes_128_ecb_encrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key)
{
    aes_xxx_ecb_encrypt(dst, src, size, key, 16);
}

static void aes_128_ecb_decrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key)
{
    aes_xxx_ecb_decrypt(dst, src, size, key, 16);
}

static void aes_192_ecb_encrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key)
{
    aes_xxx_ecb_encrypt(dst, src, size, key, 24);
}

static void aes_192_ecb_decrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key)
{
    aes_xxx_ecb_decrypt(dst, src, size, key, 24);
}

static void aes_256_ecb_encrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key)
{
    aes_xxx_ecb_encrypt(dst, src, size, key, 32);
}

static void aes_256_ecb_decrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key)
{
    aes_xxx_ecb_decrypt(dst, src, size, key, 32);
}

static void dispatch(int argc, char *argv[])
{
    unsigned char key[32];
    unsigned char in[32];
    unsigned char out[32];
    unsigned char out2[32];

    asc_to_raw(argv[1], strlen(argv[1]), key);
    asc_to_raw(argv[2], 32, in);
    asc_to_raw(argv[3], 32, out);

    if (strcmp("aes_128_ecb_encrypt", argv[0]) == 0)
        aes_128_ecb_encrypt(out2, in, 16, key);
    else if (strcmp("aes_128_ecb_decrypt", argv[0]) == 0)
        aes_128_ecb_decrypt(out2, in, 16, key);
    else if (strcmp("aes_192_ecb_encrypt", argv[0]) == 0)
        aes_192_ecb_encrypt(out2, in, 16, key);
    else if (strcmp("aes_192_ecb_decrypt", argv[0]) == 0)
        aes_192_ecb_decrypt(out2, in, 16, key);
    else if (strcmp("aes_256_ecb_encrypt", argv[0]) == 0)
        aes_256_ecb_encrypt(out2, in, 16, key);
    else if (strcmp("aes_256_ecb_decrypt", argv[0]) == 0)
        aes_256_ecb_decrypt(out2, in, 16, key);
    ASSERT_EQ_BUF(out, out2, 16);
}


void aes_test(void)
{
    func_test("AES-ECB NIST KAT", "aes_ecb.data", dispatch);
}
