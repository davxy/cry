#include "test.h"
#include <cry/sha256.h>

static char *input[] = {
    "This file is part of CRY software.",
    "CRY is free software",
};

static const char sha256_hash[] = {
    0x0f,0x02,0x31,0x2e,0x48,0xa2,0xdd,0x86,
    0x97,0xf2,0xa1,0xd5,0xd4,0x29,0xb6,0x22,
    0x20,0x00,0x67,0xaf,0xc1,0x31,0x82,0x4d,
    0x57,0x30,0x75,0xda,0xa5,0x82,0x54,0x99
};

void sha256_test(void)
{
    struct cry_sha256_ctx sha256;
    unsigned int len, i;

    cry_sha256_init(&sha256);
    for (i = 0; i < ARLEN(input); i++) {
        len = strlen(input[i]);
        PRINT_ASC("input", input[i], len);
        cry_sha256_update(&sha256, input[i], len);
    }
    cry_sha256_digest(&sha256, buf);

    PRINT_HEX("sha256", buf, 32);
    ASSERT_EQ_BUF(buf, sha256_hash, 32);
}
