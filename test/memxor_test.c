#include "test.h"
#include "memxor.h"


#define XOR_RES \
        "\x51\x53\x51\x57\x51\x53\x51\x5f\x51\x53\x2a\x2e\x2e\x2a\x2a\x36"

void memxor_test(void)
{
    const char *a = "0123456789ABCDEF";
    const char *b = "abcdefghijklmnop";
    size_t n = strlen(a);
    
    memcpy(buf, a, n);
    cry_memxor(buf, (unsigned char *)b, n);

    ASSERT_EQ_BUF(buf, XOR_RES, n);
}
