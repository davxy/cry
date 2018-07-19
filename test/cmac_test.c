#include "test.h"
#include <cry/cmac.h>

#define KEY "0123456789abcdef"
#define MSG "HelloWorld"
#define LEN strlen(MSG)

void cmac_test(void)
{
    cry_cmac_digest(buf, MSG, strlen(MSG), KEY, strlen(KEY));

    PRINT_HEX("cmac", buf, 16);
}
