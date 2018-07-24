#include "test.h"
#include <cry/md5.h>

static char *input[] = {
    "This file is part of CRY software.",
    "CRY is free software",
};

static const char md5_hash[] = {
    0x48,0x6a,0xc6,0x00,0x0e,0xf1,0x6f,0x00,
    0x12,0x0d,0x02,0x46,0xeb,0x4d,0xa6,0x69
};

void md5_test(void)
{
    struct cry_md5_ctx md5;
    unsigned int len, i;

    cry_md5_init(&md5);
    for (i = 0; i < ARLEN(input); i++) {
        len = strlen(input[i]);
        PRINT_ASC("input", input[i], len);
        cry_md5_update(&md5, input[i], len);
    }
    cry_md5_digest(&md5, buf);

    PRINT_HEX("md5", buf, 16);
    ASSERT_EQ_BUF(buf, md5_hash, 16);
}
