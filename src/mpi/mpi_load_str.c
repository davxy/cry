#include "mpi_pvt.h"

#define ASC_TO_RAW_CHAR(c) \
        (('0' <= (c) && (c) <= '9') ?  ((c) - '0') : \
         ('a' <= (c) && (c) <= 'z') ? (((c) - 'a') + 10) : \
         ('A' <= (c) && (c) <= 'Z') ? (((c) - 'A') + 10) : 0)

int cry_mpi_load_str(cry_mpi *a, unsigned int radix, const char *s)
{
    int ret = 0, sign;
    unsigned long l;
    cry_mpi tmp, base;

    /* max base 36 ('z') */
    if (radix > 36)
        return -1;

    cry_mpi_zero(a);

    /* Sign (optional) */
    if (*s == '-') {
        sign = 1;
        s++;
    } else {
        sign = 0;
        if (*s == '+')
            s++;
    }

    if (cry_mpi_init_int(&base, (long)radix) < 0)
        return -1;
    if (cry_mpi_init_int(&tmp, 1) < 0) {
        cry_mpi_clear(&base);
        return -1;
    }

    /* Skip leading zeros */
    while (*s == '0')
        s++;

    while ((l = *s) != '\0') {
        l = ASC_TO_RAW_CHAR(l);
        if (radix <= l)
            break;

        tmp.data[0] = l;
        if (cry_mpi_mul(a, a, &base) < 0)
            break;
        if (cry_mpi_add(a, a, &tmp) < 0)
            break;
        s++; /* In the end, just if no errors are detected */
    }
    if (*s != '\0') { /* If all the string has not been fed */
        cry_mpi_clear(a);
        ret = -1;
    }
    a->sign = sign;
    cry_mpi_clear(&base);
    cry_mpi_clear(&tmp);
    return ret;
}

