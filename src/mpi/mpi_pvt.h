#ifndef CRY_MPI_PVT_H_
#define CRY_MPI_PVT_H_

#include <cry/mpi.h>
#include <string.h> /* memset */
#include <limits.h>
#include <stdint.h>
#include "../misc.h"

/*
 * Double precision digits
 */

#if ULONG_MAX == 18446744073709551615UL
# if defined(_WIN32) || defined(__GNUC__)
typedef unsigned __int128 cry_mpi_dword;
# else
typedef uint128_t cry_mpi_dword;
# endif
#elif ULONG_MAX == 4294967295UL
typedef uint64_t cry_mpi_dword;
#else
# error "Invalid ULONG_MAX value"
#endif

/* Number of bytes in one digit */
#define CRY_MPI_DIGIT_BYTES  sizeof(cry_mpi_digit)

/* Number of bits in one digit */
#define CRY_MPI_DIGIT_BITS   (CRY_MPI_DIGIT_BYTES << 3)

/* Bits to digits */
#define CRY_MPI_BITS_TO_DIGS(a) \
        ((a != 0) ? (((a) - 1) / CRY_MPI_DIGIT_BITS + 1) : 0)

/* Octets to digits */
#define CRY_MPI_BYTES_TO_DIGS(a) \
        ((a != 0) ? (((a) - 1) / CRY_MPI_DIGIT_BYTES + 1) : 0)


int cry_mpi_grow(cry_mpi *a, size_t size);

int cry_mpi_shrd(cry_mpi *a, int n);

int cry_mpi_shld(cry_mpi *a, int n);

/* decrease used while the most significant digit is zero */
#define cry_mpi_adjust(a) do { \
    while ((a)->used > 0 && (a)->data[(a)->used - 1] == 0) \
        ((a)->used)--; \
    } while (0)

#define cry_mpi_set_used(a, n) do { \
    (a)->used = (n); \
    memset((a)->data, 0, (n) * CRY_MPI_DIGIT_BYTES); \
    } while (0)

#define cry_mpi_set_bit(a, bit) \
    ((a)->data[(bit) / CRY_MPI_DIGIT_BITS] |= \
         1U << ((bit) % CRY_MPI_DIGIT_BITS))

#endif /* CRY_MPI_PVT_H_ */
