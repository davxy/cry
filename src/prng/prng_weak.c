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
    (z[0] = ((z[0] & 4294967294U) << 18) ^ (r)); \
    ((r) = ((z[1] << 2) ^ z[1]) >> 27); \
    (z[1] = ((z[1] & 4294967288U) << 2) ^ (r)); \
    ((r) = ((z[2] << 13) ^ z[2]) >> 21); \
    (z[2] = ((z[2] & 4294967280U) << 7) ^ (r)); \
    ((r) = ((z[3] << 3) ^ z[3]) >> 12); \
    (z[3] = ((z[3] & 4294967168U) << 13) ^ (r)); \
    ((r) = (z[0] ^ z[1] ^ z[2] ^ z[3])); \
    } while (0)

int cry_prng_weak_init(const unsigned char *seed, size_t seed_siz)
{
    uint32_t fallback[4] = { 0x12345678, 0x87654321, 0xabcdef09, 0x90fedcba };

    init = 1;
    if (!seed) {
        seed = (unsigned char *)fallback;
        seed_siz = sizeof(fallback);
    }
    memcpy(z, seed, CRY_MIN(seed_siz, sizeof(z)));
    return 0;
}

int cry_prng_weak_rand(unsigned char *buf, size_t siz)
{
    size_t i, iter = siz / 4;
    uint32_t *buf32 = (uint32_t *) buf;
    uint32_t r;

    if (!init)
        cry_prng_weak_init(NULL, 0);

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
