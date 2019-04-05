#include "test.h"
#include <cry/md5.h>

struct md5_param {
    size_t len;
    unsigned char *data;
    unsigned char hash[CRY_MD5_DIGEST_SIZE];
};

static void param_init(struct md5_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->len = strlen(argv[0]) >> 1;
    par->data = malloc(par->len);
    par->len = raw_init(par->data, par->len, argv[0]);
    raw_init(par->hash, CRY_MD5_DIGEST_SIZE, argv[1]);
}

static void cry_md5(unsigned char *out, unsigned char *data, size_t len)
{
    struct cry_md5_ctx ctx;

    cry_md5_init(&ctx);
    cry_md5_update(&ctx, data, len);
    cry_md5_digest(&ctx, out);
}

static void md5_digest(int argc, char *argv[])
{
    struct md5_param par;
    unsigned char out[CRY_MD5_DIGEST_SIZE];

    param_init(&par, argc, argv);
    cry_md5(out, par.data, par.len);
    ASSERT_EQ_BUF(out, par.hash, CRY_MD5_DIGEST_SIZE);
    free(par.data);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;
    ASSERT(argc == 2);

    if (strcmp(test, "md5_digest") == 0)
        md5_digest(argc, argv);
    else
        printf("Test '%s' not defined\n", test);
}

static char *input[] = {
    "This file is part of CRY software.",
    "CRY is free software",
};

static const char md5_hash[] = {
    0x48,0x6a,0xc6,0x00,0x0e,0xf1,0x6f,0x00,
    0x12,0x0d,0x02,0x46,0xeb,0x4d,0xa6,0x69
};

static void test_old(void)
{
    struct cry_md5_ctx md5;
    size_t len, i;
    unsigned char buf[16];

    cry_md5_init(&md5);
    for (i = 0; i < ARLEN(input); i++) {
        len = strlen(input[i]);
        PRINT_ASC("input", input[i], len);
        cry_md5_update(&md5, (unsigned char *)input[i], len);
    }
    cry_md5_digest(&md5, buf);

    PRINT_HEX("md5", buf, 16);
    ASSERT_EQ_BUF(buf, md5_hash, 16);
}

void md5_test(void)
{
    test_old();
    printf("* MD5 RFC 1321 Vectors\n");
    func_test("md5_test.data", dispatch);
    printf("\n");
}
