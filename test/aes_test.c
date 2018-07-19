#include "test.h"
#include <cry/aes.h>

#define KEY "mYv3rysekRe7k3y!xtendedFora3s256"
#define MSG "CRY is free soft"
#define C128 "\xcb\xcd\x85\x43\xe6\xd6\xc1\x91\x99\xf1\x33\xab\xcf\xb0\x45\xb3"
#define C192 "\x92\xa2\xb3\x87\xba\x93\x31\xb4\x7c\x88\x4c\x68\x51\x5b\xcc\x4c"
#define C256 "\xf1\xbd\x71\x68\x1a\xaf\xda\x2a\x11\x57\x4c\xe5\x5b\x51\x40\x21"
#define LEN 16

static void aes_128(void)
{
    cry_aes_128_encrypt(buf, MSG, LEN, KEY);
    PRINT_HEX("ciphertext", buf, LEN);
    ASSERT_EQ_BUF(buf, C128, LEN);
    cry_aes_128_decrypt(buf, buf, LEN, KEY);
    PRINT_ASC("cleartext ", buf, LEN);
    ASSERT_EQ_BUF(buf, MSG, LEN);
}


static void aes_192(void)
{
    cry_aes_192_encrypt(buf, MSG, LEN, KEY);
    PRINT_HEX("ciphertext", buf, LEN);
    PRINT_HEX("should be ", C192, LEN);
    ASSERT_EQ_BUF(buf, C192, LEN);
    cry_aes_192_decrypt(buf, buf, LEN, KEY);
    PRINT_ASC("cleartext ", buf, LEN);
    ASSERT_EQ_BUF(buf, MSG, LEN);
}

static void aes_256(void)
{
    cry_aes_256_encrypt(buf, MSG, LEN, KEY);
    PRINT_HEX("ciphertext", buf, LEN);
    ASSERT_EQ_BUF(buf, C256, LEN);
    cry_aes_256_decrypt(buf, buf, LEN, KEY);
    PRINT_ASC("cleartext ", buf, LEN);
    ASSERT_EQ_BUF(buf, MSG, LEN);
}

void aes_test(void)
{
    RUN(aes_128);
    RUN(aes_192);
    RUN(aes_256);
}
