#include "test.h"
#include "memxor.h"
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
    memcpy(buf, S1, L1);
    memcpy(buf+L1+1, S2, L2);

    cry_memxor(buf, buf+L1+1, L1);

    ASSERT_EQ_BUF(buf, XOR_RES, L1);
}

static void mem1_after_mem2(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+L1+1, S2, L2);

    cry_memxor(buf+L1+1, buf, L1);

    ASSERT_EQ_BUF(buf+L1+1, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+L1+1, S2, L2);

    cry_memxor2(buf, buf, buf+L1+1, L1);

    ASSERT_EQ_BUF(buf, XOR_RES, L1);
}

static void memxor2_mem1_after_mem2(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+L1+1, S2, L2);

    cry_memxor2(buf+L1+1, buf+L1+1, buf, L1);

    ASSERT_EQ_BUF(buf+L1+1, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2_overlap(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+L1-1, S2, L2);

    cry_memxor2(buf, buf, buf+L1-1, L1);

    ASSERT_EQ_BUF(buf, XOR_RES1, L1);
}

static void memxor2_mem1_after_mem2_overlap(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+L1-1, S2, L2);

    cry_memxor2(buf+L1-1, buf+L1-1, buf, L1);

    ASSERT_EQ_BUF(buf+L1-1, XOR_RES1, L1);
}

static void memxor2_mem1_before_mem2_dst_in_the_gap_after_mem1_end(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+L1+1, S2, L2);

    cry_memxor2(buf+L1, buf, buf+L1+1, L1);

    ASSERT_EQ_BUF(buf+L1, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2_dst_in_the_gap_before_mem1_end(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+L1+1, S2, L2);

    cry_memxor2(buf+L1-3, buf, buf+L1+1, L1);

    ASSERT_EQ_BUF(buf+L1-3, XOR_RES, L1);
}

static void memxor2_mem1_before_mem2_dst_in_the_mid(void)
{
    memcpy(buf, S1, L1);
    memcpy(buf+3, S2, L2);

    cry_memxor2(buf+1, buf, buf+3, L1);

    ASSERT_EQ_BUF(buf+1, XOR_RES2, L1);
}


static void setup(void)
{
    memset(buf, 0, USED_MAX);
}

#define MYRUN(test) RUNX(test, setup, NULL)

void memxor_test(void)
{
    /* tests assumptions */
    ASSERT(USED_MAX < BUFSIZ);

    MYRUN(mem1_before_mem2);
    MYRUN(mem1_after_mem2);
    MYRUN(memxor2_mem1_before_mem2);
    MYRUN(memxor2_mem1_after_mem2);
    MYRUN(memxor2_mem1_before_mem2_overlap);
    MYRUN(memxor2_mem1_after_mem2_overlap);
    MYRUN(memxor2_mem1_before_mem2_dst_in_the_gap_after_mem1_end);
    MYRUN(memxor2_mem1_before_mem2_dst_in_the_gap_before_mem1_end);
    MYRUN(memxor2_mem1_before_mem2_dst_in_the_mid);
}
