#include "mpi_pvt.h"
#include <stdlib.h>
#include <stdio.h>


/* Digits allocation quantum */
#define MPI_PREC    8

int cry_mpi_grow(cry_mpi *a, size_t digs)
{
    cry_mpi_digit *tmp;

    if (digs <= a->alloc)
        return 0;

    /* ensure there are always at least MPI_PREC digits extra on top */
    digs += (MPI_PREC * 2) - (digs % MPI_PREC);
    /* reallocate the array */
    tmp = (cry_mpi_digit *) realloc(a->data, sizeof(cry_mpi_digit) * digs);
    if (tmp == NULL)
        return -1;
    a->data = tmp;

    /* zero excess digits */
    a->alloc = digs;

    return 0;
}

int cry_mpi_init(cry_mpi *a)
{
    /* allocate required memory and clear it */
    a->data = (cry_mpi_digit *) malloc(sizeof(cry_mpi_digit) * MPI_PREC);
    if (a->data == NULL)
        return -1;

    /* finalize the initialization */
    a->used = 0;
    a->alloc = MPI_PREC;
    a->sign = 0;
    return 0;
}

/*
 * Init an cry_mpi for a given size
 */
int cry_mpi_init_size(cry_mpi *a, size_t size)
{
    a->data = malloc(sizeof(cry_mpi_digit) * size);
    if (a->data == NULL)
        return -1;

    a->used = 0;
    a->alloc = size;
    a->sign = 0;
    return 0;
}

/*
 * Initialize a big number and assign it an integer value.
 */
int cry_mpi_init_int(cry_mpi *a, long val)
{
    int res;

    if ((res = cry_mpi_init(a)) == 0) {
        if ((res = cry_mpi_set_int(a, val)) < 0)
            cry_mpi_clear(a);
    }
    return res;
}

int cry_mpi_set_int(cry_mpi *a, long val)
{
    int res;
    size_t used = CRY_MPI_BYTES_TO_DIGS(sizeof(val));
    cry_mpi_dword dd;

    if ((res = cry_mpi_grow(a, used)) < 0)
        return res;

    if (val < 0) {
        a->sign = 1;
        dd = -val;
    } else {
        a->sign = 0;
        dd = val;
    }

    a->used = 0;
    while (dd != 0 && a->used < used) {
        a->data[a->used++] = (cry_mpi_digit) dd;
        dd >>= CRY_MPI_DIGIT_BITS;
    }
    return res;
}

int cry_mpi_get_int(cry_mpi *a, long *val)
{
    size_t used;
    size_t i;
    long rval = 0;

    used = cry_mpi_count_bytes(a);
    if (used > sizeof(long))
        return -1;

    i = a->used;
    while (i-- > 0) {
        rval <<= sizeof(cry_mpi_digit);
        rval |= a->data[i];
    }
    *val = (a->sign == 0) ? rval : -rval;
    return 0;
}

void cry_mpi_clear(cry_mpi *a)
{
    if (a->data != NULL)
        free(a->data);
    /* reset the members to make debugging easier */
    a->data = NULL;
    a->alloc = 0;
    a->used = 0;
    a->sign = 0;
}

int cry_mpi_copy(cry_mpi *d, const cry_mpi *s)
{
    int res;
    unsigned int i;

    /* if dst == src do nothing */
    if (d == s)
        return 0;

    /* grow dest */
    if (d->alloc < s->used) {
        res = cry_mpi_grow(d, s->used);
        if (res != 0)
            return res;
    }

    /* zero dest and copy the parameters over */
    for (i = 0; i < s->used; i++)
        d->data[i] = s->data[i];
    d->used = s->used;
    d->sign = s->sign;
    return 0;
}

int cry_mpi_init_copy(cry_mpi *d, const cry_mpi *s)
{
    int res;

    if ((res = cry_mpi_init(d)) == 0) {
        if ((res = cry_mpi_copy(d, s)) != 0)
            cry_mpi_clear(d);
    }
    return res;
}

/*
 * Quick way to get number of bits in a word
 */
static size_t word_bits(unsigned long l)
{
    static const unsigned char bits[256] = {
        0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    };

#if __WORDSIZE == 64
    if (l & 0xffffffff00000000L) {
        if (l & 0xffff000000000000L) {
            if (l & 0xff00000000000000L)
                return (bits[(int)(l >> 56)] + 56);
            else
                return (bits[(int)(l >> 48)] + 48);
        } else {
            if (l & 0x0000ff0000000000L)
                return (bits[(int)(l >> 40)] + 40);
            else
                return (bits[(int)(l >> 32)] + 32);
        }
    } else {
#endif
        if (l & 0xffff0000L) {
            if (l & 0xff000000L)
                return (bits[(int)(l >> 24L)] + 24);
            else
                return (bits[(int)(l >> 16L)] + 16);
        } else {
            if (l & 0xff00L)
                return (bits[(int)(l >> 8)] + 8);
            else
                return (bits[(int)(l)]);
        }
#if __WORDSIZE == 64
    }
#endif
}

/*
 * Big number size in bits
 */
size_t cry_mpi_count_bits(const cry_mpi *a)
{
    size_t n = 0;

    if (a->used != 0)
        n = ((a->used-1)*CRY_MPI_DIGIT_BITS) + word_bits(a->data[a->used-1]);
    return n;
}
