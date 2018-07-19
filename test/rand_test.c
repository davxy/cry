#include "test.h"
#include <cry/prng.h>
#include <time.h>

/* TODO: insert a real randomness test */

void rand_test(void)
{
    time_t t[4];

    t[0] = t[1] = t[2] = t[3] = time(NULL);
    ASSERT_OK(cry_prng_init((unsigned char *)t, sizeof(t)));
    ASSERT_OK(cry_prng_rand(buf, 64));
    PRINT_HEX("rand", buf, 64);
}
