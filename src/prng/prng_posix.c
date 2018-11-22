#include <cry/prng.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static int urand_fd = -1;

int cry_prng_init(const unsigned char *seed, size_t seed_siz)
{
    urand_fd = open("/dev/urandom", O_RDONLY);
    if (urand_fd < 0)
        return -1;
    return 0;
}

int cry_prng_rand(unsigned char *buf, size_t siz)
{
    int n;

    if (urand_fd == -1 && cry_prng_init(NULL, 0) != 0)
        return -1;

    while (siz) {
        if ((n = read(urand_fd, buf, siz)) < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        buf += n;
        siz -= n;
    }
    return 0;
}
