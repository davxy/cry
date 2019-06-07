#include "test.h"
#include <cry/crc.h>

#define MSG     ((unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define MSGLEN  (strlen((char *)MSG))

#define CRC16_CCITT 0xad3b
#define CRC16_IBM   0xfe85
#define CRC32_ETH   0Xabf77822

/*
 * CRC16 CCITT
 */

static void crc16_ccitt_wrp_test(void)
{
    uint16_t crc;

    crc = cry_crc16_ccitt(MSG, MSGLEN);
    ASSERT_EQ(crc, CRC16_CCITT);
}

static void crc16_ccitt_int_test(void)
{
    uint16_t crc;
    cry_crc16_ctx ctx;

    cry_crc16_ccitt_init(&ctx);
    cry_crc16_update(&ctx, MSG, MSGLEN);
    crc = cry_crc16_final(&ctx);
    ASSERT_EQ(crc, CRC16_CCITT);
}

static void crc16_ccitt_arr_test(void)
{
    uint16_t crc;
    unsigned char out[2];
    cry_crc16_ctx ctx;

    cry_crc16_ccitt_init(&ctx);
    cry_crc16_update(&ctx, MSG, MSGLEN);
    cry_crc16_digest(&ctx, out);
    crc = ((uint16_t)out[0] << 8) | out[1];
    ASSERT_EQ(crc, CRC16_CCITT);
}

/*
 * CRC16 IBM
 */

static void crc16_ibm_wrp_test(void)
{
    uint16_t crc;

    crc = cry_crc16_ibm(MSG, MSGLEN);
    ASSERT_EQ(crc, CRC16_IBM);
}

static void crc16_ibm_int_test(void)
{
    uint16_t crc;
    cry_crc16_ctx ctx;

    cry_crc16_ibm_init(&ctx);
    cry_crc16_update(&ctx, MSG, MSGLEN);
    crc = cry_crc16_final(&ctx);
    ASSERT_EQ(crc, CRC16_IBM);
}

static void crc16_ibm_arr_test(void)
{
    uint16_t crc;
    unsigned char out[2];
    cry_crc16_ctx ctx;

    cry_crc16_ibm_init(&ctx);
    cry_crc16_update(&ctx, MSG, MSGLEN);
    cry_crc16_digest(&ctx, out);
    crc = ((uint16_t)out[0] << 8) | out[1];
    ASSERT_EQ(crc, CRC16_IBM);
}

/*
 * CRC16 DNP
 */

/* CEI EN 60870-5-1 reference pattern */
static const unsigned char en_60870_5_1_pattern[] = {
    0x00, 0x00, 0x00, 0x28, 0x30, 0x00, 0x3B, 0xE1,
    0x58, 0x0F, 0x12, 0xA7, 0x46, 0x1B, 0x01, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00
};

struct binary_test {
    char                *name;
    const unsigned char *pattern;
    size_t               pattern_size;
    unsigned short       expected;
};

static const struct binary_test brs[] = {
    {
        "CEI EN 60870-5-1 reference pattern",
        en_60870_5_1_pattern,
        sizeof(en_60870_5_1_pattern),
        0xea02
    }
};

static void crc16_dnp_test(void)
{
    int i;
    uint16_t crc;

    for(i = 0; i < sizeof(brs)/sizeof(*brs); ++i) {
        crc = cry_crc16_dnp(brs[i].pattern, brs[i].pattern_size);
        ASSERT_EQ(crc, brs[i].expected);
    }
}

/*
 * CRC32 ETH
 */


static void crc32_eth_wrp_test(void)
{
    uint32_t crc;

    crc = cry_crc32_eth(MSG, MSGLEN);
    ASSERT_EQ(crc, CRC32_ETH);
}

static void crc32_eth_int_test(void)
{
    uint32_t crc;
    cry_crc32_ctx ctx;

    cry_crc32_eth_init(&ctx);
    cry_crc32_update(&ctx, MSG, MSGLEN);
    crc = cry_crc32_final(&ctx);
    ASSERT_EQ(crc, CRC32_ETH);
}

static void crc32_eth_arr_test(void)
{
    uint32_t crc;
    unsigned char out[4];
    cry_crc32_ctx ctx;

    cry_crc32_eth_init(&ctx);
    cry_crc32_update(&ctx, MSG, MSGLEN);
    cry_crc32_digest(&ctx, out);
    crc = ((uint32_t)out[0] << 24) | ((uint32_t)out[1] << 16) |
          ((uint32_t)out[2] << 8)  | out[3];
    ASSERT_EQ(crc, CRC32_ETH);
}

/*
 * SUBTESTS
 */

static struct test_case crc16_ccitt_tests[] = {
    { "CRC16 CCITT (wrp)", crc16_ccitt_wrp_test },
    { "CRC16 CCITT (int)", crc16_ccitt_int_test },
    { "CRC16 CCITT (arr)", crc16_ccitt_arr_test },
};

static struct test_case crc16_ibm_tests[] = {
    { "CRC16 IBM (wrp)", crc16_ibm_wrp_test },
    { "CRC16 IBM (int)", crc16_ibm_int_test },
    { "CRC16 IBM (arr)", crc16_ibm_arr_test },
};

static struct test_case crc16_dnp_tests[] = {
    { "CRC16 DNP (wrp)", crc16_dnp_test },
};

static struct test_case crc32_eth_tests[] = {
    { "CRC32 ETH (wrp)", crc32_eth_wrp_test },
    { "CRC32 ETH (int)", crc32_eth_int_test },
    { "CRC32 ETH (arr)", crc32_eth_arr_test },
};


void crc_test(void)
{
    int i;

    TRACE("* CRC16-CCITT\n");
    for (i = 0; i < ARLEN(crc16_ccitt_tests); i++)
        run(crc16_ccitt_tests[i].name, crc16_ccitt_tests[i].func, NULL, NULL);
    TRACE("* CRC16-IBM\n");
    for (i = 0; i < ARLEN(crc16_ibm_tests); i++)
        run(crc16_ibm_tests[i].name, crc16_ibm_tests[i].func, NULL, NULL);
    TRACE("* CRC16-DNP\n");
    for (i = 0; i < ARLEN(crc16_dnp_tests); i++)
        run(crc16_dnp_tests[i].name, crc16_dnp_tests[i].func, NULL, NULL);
    TRACE("* CRC32-ETH\n");
    for (i = 0; i < ARLEN(crc32_eth_tests); i++)
        run(crc32_eth_tests[i].name, crc32_eth_tests[i].func, NULL, NULL);
    TRACE("\n");
}
