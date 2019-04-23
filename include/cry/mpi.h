/**
 * @file    mpi.h
 * @brief   Multiple precision integers.
 */

#ifndef CRY_MPI_H_
#define CRY_MPI_H_

#include <stddef.h>
#include <limits.h>

/* Digit max value */
#ifndef CRY_MPI_DIGIT_MAX
# ifndef CRY_HAS_NO_CONFIG_H
# include <cry/config.h>
# endif
# ifndef CRY_MPI_DEBUG_CONF
# define CRY_MPI_DIGIT_MAX ULONG_MAX
# else
# define CRY_MPI_DIGIT_MAX UCHAR_MAX
# endif
#endif

/** Definition of the digit type macro */
#if CRY_MPI_DIGIT_MAX == ULONG_MAX
#define CRY_MPI_DIGIT_TYPE unsigned long
#elif CRY_MPI_DIGIT_MAX == USHRT_MAX
#define CRY_MPI_DIGIT_TYPE unsigned short
#elif CRY_MPI_DIGIT_MAX == UINT_MAX
#define CRY_MPI_DIGIT_TYPE unsigned int
#elif CRY_MPI_DIGIT_MAX == UCHAR_MAX
#define CRY_MPI_DIGIT_TYPE unsigned char
#else
#error "Unsupported MPI digit max value"
#endif

/** Digit type definition */
typedef CRY_MPI_DIGIT_TYPE cry_mpi_digit;

/** Number of bytes in one digit */
#define CRY_MPI_DIGIT_BYTES sizeof(cry_mpi_digit)

/** Number of bits in one digit */
#define CRY_MPI_DIGIT_BITS  (CRY_MPI_DIGIT_BYTES << 3)

struct cry_mpi {
    int           sign;
    size_t        used;
    size_t        alloc;
    cry_mpi_digit *data;
};

typedef struct cry_mpi cry_mpi;

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Construction
 */

int cry_mpi_init(cry_mpi *a);

void cry_mpi_clear(cry_mpi *a);

int cry_mpi_copy(cry_mpi *d, const cry_mpi *s);

int cry_mpi_init_copy(cry_mpi *d, const cry_mpi *s);

int cry_mpi_init_size(cry_mpi *a, size_t size);

int cry_mpi_init_int(cry_mpi *a, long val);

int cry_mpi_set_int(cry_mpi *a, long val);

int cry_mpi_get_int(cry_mpi *a, long *val);

int cry_mpi_init_bin(cry_mpi *a, const void *b, size_t size);

int cry_mpi_load_bin(cry_mpi *a, const void *b, size_t size);

int cry_mpi_store_bin(const cry_mpi *a, void *b, size_t size, int pad);

int cry_mpi_init_str(cry_mpi *a, unsigned int radix, const char *s);

int cry_mpi_load_str(cry_mpi *a, unsigned int radix, const char *s);

int cry_mpi_store_str(const cry_mpi *a, unsigned int radix, char *s);

int cry_mpi_init_list(cry_mpi *a, ... /* (cry_mpi *)NULL */);

void cry_mpi_clear_list(cry_mpi *a, ... /* (cry_mpi *)NULL */);

/*
 *  Arithmetic
 */

int cry_mpi_abs(cry_mpi *d, const cry_mpi *s);

int cry_mpi_add_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_sub_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_mul_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_div_abs(cry_mpi *q, cry_mpi *r, const cry_mpi *a,
                    const cry_mpi *b);

int cry_mpi_add(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_sub(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_mul(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_div(cry_mpi *q, cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

#define cry_mpi_mod(r, a, b) cry_mpi_div(NULL, r, a, b)

int cry_mpi_sqr(cry_mpi *r, const cry_mpi *a);

int cry_mpi_sqrt(cry_mpi *r, const cry_mpi *a);

int cry_mpi_shl(cry_mpi *c, const cry_mpi *a, int n);

int cry_mpi_shr(cry_mpi *c, const cry_mpi *a, int n);

/*
 * More advanced arithmetic algorithms
 */

int cry_mpi_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e);

int cry_mpi_mod_exp(cry_mpi *r, const cry_mpi *b, const cry_mpi *e,
                    const cry_mpi *m);

int cry_mpi_gcd(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_lcm(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_inv(cry_mpi *r, const cry_mpi *a, const cry_mpi *m);

int cry_mpi_mul_baseline(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_mul_comba(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_mul_karatsuba(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

int cry_mpi_mul_toom3(cry_mpi *r, const cry_mpi *a, const cry_mpi *b);

/*
 * Utilities
 */

int cry_mpi_cmp_abs(const cry_mpi *a, const cry_mpi *b);

int cry_mpi_cmp(const cry_mpi *a, const cry_mpi *b);

size_t cry_mpi_count_bits(const cry_mpi *a);

#define cry_mpi_count_bytes(a) \
    ((cry_mpi_count_bits(a) + 7) / 8)

#define cry_mpi_swap(a, b) do { \
    cry_mpi __t = *(a);         \
    *(a) = *(b);                \
    *(b) = __t;                 \
    } while (0)

#define cry_mpi_is_zero(a) \
    ((a)->used == 0)

#define cry_mpi_is_odd(a) \
    (!cry_mpi_is_zero(a) && ((a)->data[0] & 1))

#define cry_mpi_is_even(a) \
    (!cry_mpi_is_odd(a))

#define cry_mpi_is_pos(a) \
    ((a)->sign == 0)

#define cry_mpi_is_neg(a) \
    (!cry_mpi_is_pos(a))

void cry_mpi_print(const cry_mpi *a, unsigned int radix);

#define cry_mpi_zero(a) do { \
    (a)->sign = 0;           \
    (a)->used = 0;           \
    } while (0)

int cry_mpi_rand(cry_mpi *a, unsigned int bits);

int cry_mpi_rand_range(cry_mpi *a, const cry_mpi *max);

int cry_mpi_prime(cry_mpi *a, unsigned int bits, unsigned int *iter);

int cry_mpi_is_prime(const cry_mpi *a);

int cry_mpi_mul_dig(cry_mpi *r, const cry_mpi *a, cry_mpi_digit b);

#ifdef __cplusplus
}
#endif

#endif /* CRY_MPI_H_ */
