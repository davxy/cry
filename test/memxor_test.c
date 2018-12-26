#include "test.h"
#include <cry/memxor.h>
#include <string.h>

#define S1 "HeLLoWoRld01x"
#define S2 "0231487516295"
#define L1 (sizeof(S1)-1)
#define L2 (sizeof(S2)-1)

#define USED_MAX (L1+L2+1)


#define XOR_RES \
    "\x78\x57\x7f\x7d\x5b\x6f\x58\x67\x5d\x52\x02\x08\x4d"

#define XOR_RES1 \
    "\x78\x57\x7f\x7d\x5b\x6f\x58\x67\x5d\x52\x02\x08\x05"

#define XOR_RES2 \
    "\x78\x57\x7f\x01\x06\x0b\x06\x01\x09\x01\x07\x08\x03"


static void mem1_before_mem2(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1+1, S2, L2);

    cry_memxor(g_buf, g_buf+L1+1, L1);

    ASSERT_EQ_BUF(g_buf, XOR_RES, L1);
}

static void mem1_after_mem2(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1+1, S2, L2);

    cry_memxor(g_buf+L1+1, g_buf, L1);

    ASSERT_EQ_BUF(g_buf+L1+1, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1+1, S2, L2);

    cry_memxor2(g_buf, g_buf, g_buf+L1+1, L1);

    ASSERT_EQ_BUF(g_buf, XOR_RES, L1);
}

static void memxor2_mem1_after_mem2(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1+1, S2, L2);

    cry_memxor2(g_buf+L1+1, g_buf+L1+1, g_buf, L1);

    ASSERT_EQ_BUF(g_buf+L1+1, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2_overlap(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1-1, S2, L2);

    cry_memxor2(g_buf, g_buf, g_buf+L1-1, L1);

    ASSERT_EQ_BUF(g_buf, XOR_RES1, L1);
}

static void memxor2_mem1_after_mem2_overlap(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1-1, S2, L2);

    cry_memxor2(g_buf+L1-1, g_buf+L1-1, g_buf, L1);

    ASSERT_EQ_BUF(g_buf+L1-1, XOR_RES1, L1);
}

static void memxor2_mem1_before_mem2_dst_in_the_gap_after_mem1_end(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1+1, S2, L2);

    cry_memxor2(g_buf+L1, g_buf, g_buf+L1+1, L1);

    ASSERT_EQ_BUF(g_buf+L1, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2_dst_in_the_gap_before_mem1_end(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+L1+1, S2, L2);

    cry_memxor2(g_buf+L1-3, g_buf, g_buf+L1+1, L1);

    ASSERT_EQ_BUF(g_buf+L1-3, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2_dst_in_the_mid(void)
{
    memcpy(g_buf, S1, L1);
    memcpy(g_buf+3, S2, L2);

    cry_memxor2(g_buf+1, g_buf, g_buf+3, L1);

    ASSERT_EQ_BUF(g_buf+1, XOR_RES2, L1);
}


static void setup(void)
{
    memset(g_buf, 0, USED_MAX);
}


static struct test_case tests[] = {
    { "M1 < M2 and dst = M1", mem1_before_mem2 },
    { "M1 > M2 and dst = M1", mem1_after_mem2 },
    { "M1 < M2", memxor2_mem1_before_mem2 },
    { "M1 > M2", memxor2_mem1_after_mem2 },
    { "M1 < M2 and overlap", memxor2_mem1_before_mem2_overlap },
    { "M1 > M2 and overlap", memxor2_mem1_after_mem2_overlap },
    { "M1 < M2 and M1.end <= dst <= M2", memxor2_mem1_before_mem2_dst_in_the_gap_after_mem1_end },
    { "M1 < M2 and M1 <= dst <= M1.end", memxor2_mem1_before_mem2_dst_in_the_gap_before_mem1_end },
    { "M1 < M2 and M1.end <= dst <= M2", memxor2_mem1_before_mem2_dst_in_the_mid }
};

#define NTESTS (sizeof(tests)/sizeof(tests[0]))

void memxor_test(void)
{
    int i;

    printf("* XOR\n");
    ASSERT(USED_MAX < BUFSIZ);
    for (i = 0; i < NTESTS; i++)
        run(tests[i].name, tests[i].func, setup, NULL);
    printf("\n");
}
