#include "test.h"
#include <cry/sum.h>

#define MSG "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CS8 0xdf
#define LRC 0x21

void sum_test(void)
{
    unsigned char cs;

    TRACE("msg: %s\n", MSG);

    cs = cry_cs8(MSG, strlen(MSG));
    TRACE("cs8 = 0x%02x\n", cs);
    ASSERT_EQ(cs, CS8);

    cs = cry_lrc(MSG, strlen(MSG));
    TRACE("lrc = 0x%02x\n", cs);
    ASSERT_EQ(cs, LRC);
}
