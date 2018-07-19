#include "test.h"
#include <cry/des.h>

#define KEY   "12345678abcdefghILMNOPQR"
#define MSG   "CRY is free soft"
#define LEN   16
#define CDES  "\x04\xcf\xf8\x8a\xd8\x95\x79\x5c\x2e\x8d\xf1\xd9\xf5\xbd\x94\x54"
#define CTDES "\x93\x7c\x8d\xf0\xa1\xa4\x77\x61\x11\x4d\xb3\x17\x71\xdb\x10\x19"

static cry_des_ctx des;

static void single(void)
{
    memcpy(buf, MSG, LEN); /* test in place */
    cry_des_key_set(&des, KEY, 8);
    cry_des_encrypt(&des, buf, buf, LEN);
    PRINT_HEX("ciphertext", buf, LEN);
    ASSERT_EQ_BUF(buf, CDES, LEN);
    cry_des_decrypt(&des, buf, buf, LEN);
    PRINT_ASC("cleartext ", buf, LEN);
    ASSERT_EQ_BUF(buf, MSG, LEN);
}

static void triple(void)
{
    memcpy(buf, MSG, LEN); /* test in place */
    cry_des_key_set(&des, KEY, 24);
    cry_des_encrypt(&des, buf, buf, LEN);
    PRINT_HEX("ciphertext", buf, LEN);
    ASSERT_EQ_BUF(buf, CTDES, LEN);
    cry_des_decrypt(&des, buf, buf, LEN);
    PRINT_ASC("cleartext ", buf, LEN);
    ASSERT_EQ_BUF(buf, MSG, LEN);
}

void des_test(void)
{
    RUN(single);
    RUN(triple);
}

