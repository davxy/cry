#include "mpi_pvt.h"

int cry_mpi_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e)
{
    int res;
    int sign = b->sign ? cry_mpi_is_odd(e) : 0;
    cry_mpi t, c, one;

    if ((res = cry_mpi_init_list(&t, &one, &c, NULL)) != 0)
        return res;
    
    if ((res = cry_mpi_copy(&c, e)) != 0)
        goto e;

    one.data[0] = 1;
    one.used = 1;
    one.sign = 0;
    t.data[0] = 1;
    t.used = 1;
    t.sign = 0;
    while (c.used != 0) {
        if ((res = cry_mpi_mul_abs(&t, &t, b)) != 0)
            break;
        if ((res = cry_mpi_sub_abs(&c, &c, &one)) != 0)
            break;
    }

    cry_mpi_swap(&t, r);
    r->sign = sign;
e:  cry_mpi_clear_list(&t, &one, &c, NULL);
    return 0;
}

