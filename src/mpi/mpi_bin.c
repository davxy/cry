#include "mpi_pvt.h"
#include <string.h>

/*
 * Initialize a big number from big endian binary data.
 */
int cry_mpi_load_bin(cry_mpi *x, const void *b, unsigned int size)
{
    unsigned int i, m, res;
    cry_mpi_digit l;
    unsigned char *s = (unsigned char *)b;

    /* skip leading zeros */
    for ( ; size > 0 && *s == 0; s++, size--);
    if (size == 0)
        return 0;

    i = ((size - 1) / CRY_MPI_DIGIT_BYTES) + 1; /* Number of required octets */
    m =  (size - 1) % CRY_MPI_DIGIT_BYTES;      /* Remainder */

    if (x->alloc < i) {
        if ((res = cry_mpi_grow(x, i)) != 0)
            return res;
    }
    x->used = i;

    l = 0;
    while (size--) {
        l = (l << 8L) | *s++;
        if (m-- == 0) {
            x->data[--i] = l;
            l = 0;
            m = CRY_MPI_DIGIT_BYTES - 1;
        }
    }
    return 0;
}

/*
 * Write a big number to a memory buffer in big endian byte order.
 */
int cry_mpi_store_bin(const cry_mpi *x, void *buf,
        unsigned int bufsiz, int pad)
{
    int i;
    unsigned long l;
    unsigned char *d = (unsigned char *) buf;

    i = cry_mpi_count_bytes(x);
    if (bufsiz < i)
        return -1;  /* Do not silently truncate */
    if (!pad)
        bufsiz = i;
    /* Add leading zeroes if necessary */
    if (bufsiz > i) {
        memset(d, 0, bufsiz - i);
        d += bufsiz - i;
    }
    while (i--) {
        l = x->data[i / CRY_MPI_DIGIT_BYTES];
        *d++ = (unsigned char)(l >> (8 * (i % CRY_MPI_DIGIT_BYTES))) & 0xff;
    }
    return 0;
}

int cry_mpi_init_bin(cry_mpi *x, const void *b, unsigned int size)
{
    int res;

    res = cry_mpi_init_size(x, CRY_MPI_BYTES_TO_DIGS(size));
    if (res == 0) {
        res = cry_mpi_load_bin(x, b, size);
        if (res != 0)
            cry_mpi_clear(x);
    }
    return res;
}
