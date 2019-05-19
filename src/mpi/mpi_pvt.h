#ifndef CRY_MPI_PVT_H_
#define CRY_MPI_PVT_H_

#include <cry/mpi.h>
#include <string.h> /* memset */
#include <stdint.h>
#include "../misc.h"

/* Double precision digits */

#if CRY_MPI_DIGIT_BYTES == 1
typedef uint16_t cry_mpi_dword;
#elif CRY_MPI_DIGIT_BYTES == 2
typedef uint32_t cry_mpi_dword;
#elif CRY_MPI_DIGIT_BYTES == 4
typedef uint64_t cry_mpi_dword;
#elif CRY_MPI_DIGIT_BYTES == 8
# if defined(_WIN32) || defined(__GNUC__)
typedef unsigned __int128 cry_mpi_dword;
# else
typedef uint128_t cry_mpi_dword;
# endif
#else
# error "Invalid DIGIT_MAX value"
#endif

/* Digit max value */
#ifndef CRY_MPI_QUANTUM
# include <cry/config.h>
# ifndef CRY_MPI_DEBUG_CONF
# define CRY_MPI_QUANTUM 8
# else
# define CRY_MPI_QUANTUM 1
# endif
#endif

/* Bits to digits */
#define CRY_MPI_BITS_TO_DIGS(a) \
        ((a != 0) ? (((a) - 1) / CRY_MPI_DIGIT_BITS + 1) : 0)

/* Octets to digits */
#define CRY_MPI_BYTES_TO_DIGS(a) \
        ((a != 0) ? (((a) - 1) / CRY_MPI_DIGIT_BYTES + 1) : 0)


int cry_mpi_grow(cry_mpi *a, size_t size);

int cry_mpi_shrd(cry_mpi *a, size_t n);

int cry_mpi_shld(cry_mpi *a, size_t n);

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

#define cry_mpi_is_bit_set(n, b) \
    (((n)->data[b / CRY_MPI_DIGIT_BITS] & \
      ((cry_mpi_digit)1 << ((b) % CRY_MPI_DIGIT_BITS))) != 0)

#endif /* CRY_MPI_PVT_H_ */
