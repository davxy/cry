#include "cry/aes.h"

static void cry_aes_xxx_encrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key,
                                unsigned int keysize)
{
    struct cry_aes_ctx ctx;

    cry_aes_key_set(&ctx, key, keysize);
    cry_aes_encrypt(&ctx, dst, src, size);
}

static void cry_aes_xxx_decrypt(unsigned char *dst, const unsigned char *src,
                                size_t size, const unsigned char *key,
                                unsigned int keysize)
{
    struct cry_aes_ctx ctx;

    cry_aes_key_set(&ctx, key, keysize);
    cry_aes_decrypt(&ctx, dst, src, size);
}

void cry_aes_128_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_encrypt(dst, src, size, key, 16);
}

void cry_aes_128_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_decrypt(dst, src, size, key, 16);
}

void cry_aes_192_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_encrypt(dst, src, size, key, 24);
}

void cry_aes_192_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_decrypt(dst, src, size, key, 24);
}

void cry_aes_256_encrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_encrypt(dst, src, size, key, 32);
}

void cry_aes_256_decrypt(unsigned char *dst, const unsigned char *src,
                         size_t size, const unsigned char *key)
{
    cry_aes_xxx_decrypt(dst, src, size, key, 32);
}

