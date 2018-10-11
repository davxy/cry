#include "misc.h"

unsigned long cry_long_inv(unsigned long val, unsigned long mod)
{
    unsigned long r0, r1, q;
    long s0, s1;

    r0 = val;
    r1 = mod;
    s0 = 1;
    s1 = 0;
    while (r1 != 0) {
        /* r2 = r0 - q*r1 */
        q = r0 / r1;
        r0 %= r1;
        CRY_SWAP(r0, r1);
        /* s2 = s0 - q*s1 */
        s0 -= q*s1;
        CRY_SWAP(s0, s1);
    }
    if (r0 != 1)
        s0 = 0;
    else if (s0 < 0)
        s0 += mod;
    return (unsigned long)s0;
}
