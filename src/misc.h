/*
 * A bounch of helper macros and functions meant for internal use.
 */

#ifndef CRY_MISC_H_
#define CRY_MISC_H_

#include <stdint.h>

/** Statically get array number of elements */
#define CRY_ARRAY_LEN(ar) (sizeof(ar)/sizeof((ar)[0]))

/** Macro used to compute the minimum of two integral values. */
#define CRY_MIN(a, b)     (((a) < (b)) ? (a) : (b))
/** Macro used to compute the maximum of two integral values. */
#define CRY_MAX(a, b)     (((a) > (b)) ? (a) : (b))

/**
 * Increments a big endian value of a give size.
 * Used to directly increment a value within a buffer.
 */
#define CRY_INCREMENT_BE(val_ptr, val_size) do { \
    int i = (val_size) - 1; \
    if (++(val_ptr)[i] == 0) \
        while (++(val_ptr)[--i] == 0 && i > 0); \
    } while (0)


/** Architecture independent little endian 16 bit value write. */
#define CRY_WRITE16_LE(val, dst) do { \
    ((uint8_t *)(dst))[1] = (uint8_t)(((val) >> 8) & 0xff); \
    ((uint8_t *)(dst))[0] = (uint8_t) ((val) & 0xff); \
    } while(0)

/** Architecture independent big endian 16 bit value write. */
#define CRY_WRITE16_BE(val, dst) do { \
    ((uint8_t *)(dst))[0] = (uint8_t)(((val) >> 8) & 0xff); \
    ((uint8_t *)(dst))[1] = (uint8_t) ((val) & 0xff); \
    } while(0)

/** Architecture independent little endian 16 bit value read. */
#define CRY_READ16_LE(val, src) \
    ((val) = ((((uint16_t) (src)[1]) << 8U) \
            |  ((uint16_t) (src)[0])))

/** Architecture independent big endian 16 bit value read. */
#define CRY_READ16_BE(val, src) \
    ((val) = ((((uint16_t) (src)[0]) << 8U) \
           |   ((uint16_t) (src)[1])))

/** Architecture independent little endian 32 bit value write. */
#define CRY_WRITE32_LE(val, dst) do { \
    ((uint8_t *)(dst))[3] = (uint8_t)(((val) >> 24) & 0xff); \
    ((uint8_t *)(dst))[2] = (uint8_t)(((val) >> 16) & 0xff); \
    ((uint8_t *)(dst))[1] = (uint8_t)(((val) >> 8) & 0xff); \
    ((uint8_t *)(dst))[0] = (uint8_t) ((val) & 0xff); \
} while(0)

/** Architecture independent big endian 32 bit value write. */
#define CRY_WRITE32_BE(val, dst) do { \
    ((uint8_t *)(dst))[0] = (uint8_t)(((val) >> 24U) & 0xffU); \
    ((uint8_t *)(dst))[1] = (uint8_t)(((val) >> 16U) & 0xffU); \
    ((uint8_t *)(dst))[2] = (uint8_t)(((val) >> 8U) & 0xffU); \
    ((uint8_t *)(dst))[3] = (uint8_t) ((val) & 0xff); \
} while(0)

/** Architecture independent little endian 32 bit value read. */
#define CRY_READ32_LE(val, src) \
    ((val) = ((((uint32_t) (src)[3]) << 24) \
            | (((uint32_t) (src)[2]) << 16) \
            | (((uint32_t) (src)[1]) << 8) \
            |  ((uint32_t) (src)[0])))

/** Architecture independent big endian 32 bit value read. */
#define CRY_READ32_BE(val, src) \
    ((val) = ((((uint32_t) (src)[0]) << 24U) \
            | (((uint32_t) (src)[1]) << 16U) \
            | (((uint32_t) (src)[2]) << 8U)  \
            |  ((uint32_t) (src)[3])))

/** In-place swap macro */
#define CRY_SWAP(v1, v2) do { \
    (v1) ^= (v2); \
    (v2) ^= (v1); \
    (v1) ^= (v2); \
    } while(0)

/** Rotate the bits left */
#define CRY_ROTL(val, size, bits) \
    ((((val))<<(bits)) | (((val))>>(size-(bits))))

/** Rotate the bits of a unsigned 32 right. */
#define CRY_ROTL32(val, bits) \
    CRY_ROTL(val, 32, bits)

/**
 * Modular inverse using Euclid algorithm.
 *
 * @param val   Value.
 * @param mod   Module.
 * @return      On success returns the positive modular inverse.
 *              If the value is not invertible returns 0.
 */
unsigned long cry_long_inv(unsigned long val, unsigned long mod);

/**
 * Secure memset.
 *
 * Implementation that should never be optimized out by the compiler.
 *
 * @param dst   Destination buffer pointer.
 * @param val   Constant filling value.
 * @param len   Destination buffer size.
 */
#define cry_memset(dst, val, len) do { \
    volatile unsigned char *__dst = (unsigned char*)dst; \
    size_t __len = len; \
    while(__len--) \
        *__dst++ = (val); \
    } while (0)

#endif /* CRY_MISC_H_ */
