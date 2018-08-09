#include "test.h"
#include <cry/base64.h>

#define MSG "Hello World"
#define MLEN strlen(MSG)
#define ELEN 16

void base64_test(void)
{
    char *cbuf = (char *)g_buf;

    ASSERT(cry_base64_encode(MSG, strlen(MSG), cbuf) == ELEN);
    PRINT_ASC("encoded", cbuf, ELEN);
    ASSERT_EQ_BUF(cbuf, "SGVsbG8gV29ybGQ=", ELEN);

    ASSERT(cry_base64_decode(cbuf, ELEN, cbuf) == MLEN);
    PRINT_ASC("decoded", cbuf, MLEN);
    ASSERT_EQ_BUF(g_buf, MSG, MLEN);
}
