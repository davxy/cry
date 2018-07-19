#include "cry/sum.h"

unsigned char cry_cs8(const unsigned char *in, size_t n)
{
    unsigned char cs = 0;

    while (n-- > 0)
        cs += *in++;
    return cs;
}

