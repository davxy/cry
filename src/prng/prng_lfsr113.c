/*
 * Pierre L'Ecuyer LFSR113  32-bits random number generator.
 * Source: http://www.iro.umontreal.ca/~lecuyer/myftp/papers/tausme2.ps
 *
 * The generator has four 32-bit components combined by a bitwise xor.
 * Its period length is approximatively 2^113.
 *
 * The initial seeds z1, z2, z3, z4 MUST be larger than 1, 7, 15, and 127
 * respectively.
 * Default seed: 987654321.
 */

#include <cry/prng.h>
#include "misc.h"
#include <stdint.h>
#include <string.h>

#define DEFAULT_SEED 987654321

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
} while (0)

int cry_prng_lfsr113_init(const unsigned char *seed, size_t seed_siz)
{
    unsigned char *p = (unsigned char *)z;
    size_t i;

    memset(z, 0, seed_siz);
    for (i = 0; i < seed_siz; i++)
        p[i % sizeof(z)] ^= seed[i];
    if (z[0] < 1)
        z[0] = DEFAULT_SEED;
    if (z[1] < 7)
        z[1] = DEFAULT_SEED;
    if (z[2] < 15)
        z[2] = DEFAULT_SEED;
    if (z[3] < 127)
        z[3] = DEFAULT_SEED;
    init = 1;
    return 0;
}

int cry_prng_lfsr113_rand(unsigned char *buf, size_t siz)
{
    size_t i;
    uint32_t r;

    if (!init)
        cry_prng_lfsr113_init(NULL, 0);

    for (i = 0; i < siz/4; i++) {
        RAND_GET(r);
        *buf++ = (unsigned char)(r >> 24);
        *buf++ = (unsigned char)(r >> 16);
        *buf++ = (unsigned char)(r >> 8);
        *buf++ = (unsigned char)(r);
    }

    if ((i = (siz & 0x03)) != 0) {
        RAND_GET(r);
        while (i-- > 0) {
            *buf++ = (unsigned char)(r >> 24);
            r <<= 8;
        }
    }
    return 0;
}
