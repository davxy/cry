/*
 * Weak fallback random functions
 */

#include <cry/prng.h>
#include "misc.h"
#include <stdint.h>
#include <string.h>

static uint32_t z[4];
static int init = 0;

#define RAND_GET(r) do { \
    ((r) = ((z[0] << 6) ^ z[0]) >> 13); \
    (z[0] = ((z[0] & 4294967294UL) << 18) ^ (r)); \
    ((r) = ((z[1] << 2) ^ z[1]) >> 27); \
    (z[1] = ((z[1] & 4294967288UL) << 2) ^ (r)); \
    ((r) = ((z[2] << 13) ^ z[2]) >> 21); \
    (z[2] = ((z[2] & 4294967280UL) << 7) ^ (r)); \
    ((r) = ((z[3] << 3) ^ z[3]) >> 12); \
    (z[3] = ((z[3] & 4294967168UL) << 13) ^ (r)); \
    ((r) = (z[0] ^ z[1] ^ z[2] ^ z[3])); \
    } while(0)

int cry_prng_init(const unsigned char *seed, size_t seed_siz)
{
    init = 1;
    uint32_t fallback[4] = {1,2,3,4};

    if (!seed) {
        seed = (unsigned char *)fallback;
        seed_siz = sizeof(fallback);
    }
    memcpy(z, seed, CRY_MIN(seed_siz, sizeof(z)));
    return 0;
}

int cry_prng_rand(unsigned char *buf, size_t siz)
{
    int i, iter = siz / 4;
    uint32_t *buf32 = (uint32_t *) buf;
    uint32_t r;

    if (!init)
        cry_prng_init(NULL, 0);

    for (i = 0; i < iter; i++, buf32++)
        RAND_GET(*buf32);


    if ((iter = (siz & 0x03)) != 0) {
        RAND_GET(r);
        buf = (unsigned char *)buf32;
        for (i = 0; i < iter; i++, r >>= 8)
            *buf++ = (unsigned char)r;
    }
    return 0;
}

