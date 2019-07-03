#include "misc.h"
#include <stdlib.h>


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


int cry_list_init(cry_list_elem_init_f init, cry_list_elem_clear_f clear,
                  void *first, va_list args)
{
    int res = 0, n = 0;
    void *curr = first;
    va_list copy;

    va_copy(copy, args);
    while (curr != NULL) {
        if ((res = init(curr)) == 0) {
            n++;
            curr = va_arg(args, void *);
        } else {
            /* on error undo all the initialized variables */
            va_end(args); /* end the current list */
            curr = first;
            while (n--) {
                clear(curr);
                curr = va_arg(copy, void *);
            }
            break;
        }
    }
    va_end(copy);
    return res;
}

void cry_list_clear(cry_list_elem_clear_f clear,
                    void *first, va_list args)
{
    void *curr = first;

    while (curr != NULL) {
        clear(curr);
        curr = va_arg(args, void *);
    }
}
