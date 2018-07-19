#include "test.h"
#include <cry/crc.h>

#define MSG "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

static void crc16_ccitt_test(void)
{
    uint16_t crc;
#define CRC16_CCITT 0xad3b

    crc = cry_crc16_ccitt(MSG, strlen(MSG));
    TRACE("crc16-ccitt = 0x%04x\n", crc);
    ASSERT_EQ(crc, CRC16_CCITT);
}

static void crc16_ibm_test(void)
{
    uint16_t crc;
#define CRC16_IBM   0xfe85

    crc = cry_crc16_ibm(MSG, strlen(MSG));
    TRACE("crc16-ibm   = 0x%04x\n", crc);
    ASSERT_EQ(crc, CRC16_IBM);
}

static void crc32_eth_test(void)
{
    uint32_t crc;
#define CRC32_ETH   0Xabf77822

    crc = cry_crc32_eth(MSG, strlen(MSG));
    TRACE("crc32-eth   = 0x%08x\n", crc);
    ASSERT_EQ(crc, CRC32_ETH);
}

void crc_test(void)
{
    TRACE("msg: %s\n", MSG);
    RUN(crc16_ccitt_test);
    RUN(crc16_ibm_test);
    RUN(crc32_eth_test);
}

