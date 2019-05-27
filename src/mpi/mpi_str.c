#include "mpi_pvt.h"
#include <stdlib.h>


#define ASC_TO_RAW_CHAR(c) \
            (('0' <= (c) && (c) <= '9') ?  ((c) - '0') : \
             ('a' <= (c) && (c) <= 'z') ? (((c) - 'a') + 10) : \
             ('A' <= (c) && (c) <= 'Z') ? (((c) - 'A') + 10) : 0)

static int load_str_hex(cry_mpi *a, const char *s)
{
    int res;
    size_t i = 0, j = 0;
    unsigned char *bin;
    char first = 0;
    size_t siz, len;

    len = strlen(s);
    siz = len >> 1;
    if ((len & 0x01) != 0) {
        len--;
        siz++;
        first = s[j++];
    }
    bin = malloc(siz);
    if (bin == NULL)
        return -1;
    if (first != 0)
        bin[i++] = ASC_TO_RAW_CHAR(first);

    while (j < len) {
        bin[i] = ASC_TO_RAW_CHAR(s[j]) << 4;
        j++;
        bin[i] |= ASC_TO_RAW_CHAR(s[j]);
        j++;
        i++;
    }

    res = cry_mpi_load_bin(a, bin, i);
    free(bin);
    return res;
}

#define RAW_TO_ASC_CHAR(c) \
        (((c) < 10) ? ('0' + (c)) : ('a' + ((c) - 10)))

static int store_str_hex(const cry_mpi *a, char *s)
{
    int res;
    size_t i, j;

    i = cry_mpi_count_bytes(a);
    j = i << 1;
    res = cry_mpi_store_bin(a, s, i, 0);
    if (res < 0)
        return res;

    if ((s[0] & 0xF0) == 0)
        j--;
    s[j] = '\0';

    while (i > 0) {
        i--;
        j--;
        s[j] = RAW_TO_ASC_CHAR(s[i] & 0x0F);
        if (j == 0)
            break;
        j--;
        s[j] = RAW_TO_ASC_CHAR((s[i] >> 4) & 0x0F);
    }
    return 0;
}


int cry_mpi_load_str(cry_mpi *a, unsigned int radix, const char *s)
{
    int ret = 0, sign;
    cry_mpi_digit d;
    cry_mpi tmp, base;

    /* max base 36 ('z') */
    if (radix > 36)
        return -1;

    cry_mpi_zero(a);

    /* Sign */
    if (*s == '-') {
        sign = 1;
        s++;
    } else {
        sign = 0;
        if (*s == '+')
            s++;
    }

    if (radix == 16) {
        ret = load_str_hex(a, s);
        a->sign = cry_mpi_is_zero(a) ? 0 : sign;
        return ret;
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

    while ((d = (cry_mpi_digit)*s) != '\0') {
        d = ASC_TO_RAW_CHAR(d);
        if (radix <= d)
            break;
        tmp.data[0] = d;
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
    a->sign = cry_mpi_is_zero(a) ? 0 : sign;
    cry_mpi_clear(&base);
    cry_mpi_clear(&tmp);
    return ret;
}


int cry_mpi_store_str(const cry_mpi *a, unsigned int radix, char *s)
{
    int ret;
    cry_mpi tmp, rad, rem;
    unsigned int i = 0, j, d;

    if (radix < 2 || radix > 16)
        return -1;

    if (cry_mpi_is_zero(a) != 0) {
        s[i++] = '0';
        s[i] = '\0';
        return 0;
    }

    if (radix == 16) {
        if (a->sign)
            *s++ = '-';
        return store_str_hex(a, s);
    }

    if ((ret = cry_mpi_init_list(&tmp, &rad, &rem, (cry_mpi *) NULL)) != 0)
        return ret;

    if ((ret = cry_mpi_copy(&tmp, a)) != 0)
        goto e;
    cry_mpi_set_int(&rad, radix);

    while (!cry_mpi_is_zero(&tmp)) {
        if ((ret = cry_mpi_div_abs(&tmp, &rem, &tmp, &rad)) != 0)
            goto e;
        d = rem.used ? rem.data[0] : 0;
        s[i++] = RAW_TO_ASC_CHAR(d);
    }
    if (a->sign)
        s[i++] = '-';
    s[i] = '\0';

    /* string invert */
    i--;
    j = 0;
    while (j < i) {
        s[j] ^= s[i];
        s[i] ^= s[j];
        s[j] ^= s[i];
        j++;
        i--;
    }

e:  cry_mpi_clear_list(&tmp, &rad, &rem, (cry_mpi *) NULL);
    return ret;
}

/*
 * Initialize a big number from a null terminated ASCII string.
 */
int cry_mpi_init_str(cry_mpi *a, unsigned int radix, const char *s)
{
    int res;

    res = cry_mpi_init(a);
    if (res == 0) {
        res = cry_mpi_load_str(a, radix, s);
        if (res != 0)
            cry_mpi_clear(a);
    }
    return res;
}
