#include <windows.h>
#include <wincrypt.h>

static HCRYPTPROV hProvider = 0;

int cry_prng_init(const unsigned char *seed, size_t seed_siz)
{
    if (!CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        return -1;
    }
    return 0;
}

int cry_prng_rand(unsigned char *buf, size_t siz)
{
    if (!hProvider && cry_prng_init(NULL, 0) != 0)
        return -1;
    if (!CryptGenRandom(hProvider, siz, buf))
        return -1;
    return 0;
}
