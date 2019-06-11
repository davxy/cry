/**
 * @file    crc.h
 * @brief   Cyclic Redundancy Check
 *
 * Wikipedia reference:
 * Polynomial_representations_of_cyclic_redundancy_checks
 */

#ifndef CRY_CRC_H_
#define CRY_CRC_H_

#include <stddef.h>
#include <stdint.h>

/** CRC flags @{ */
/** Complement result during finalization */
#define CRY_CRC_FLAG_COMPLEMENT     (1 << 0)
/** Swap result bytes during finalization */
#define CRY_CRC_FLAG_SWAP           (1 << 1)
/** @} */

/*
 * CRC-16 context structure
 */
struct cry_crc16_ctx {
    const uint16_t *tab;
    uint16_t crc;
    unsigned char flags;
};

typedef struct cry_crc16_ctx cry_crc16_ctx;

/*
 * CRC-32 context structure
 */
struct cry_crc32_ctx {
    const uint32_t *tab;
    uint32_t crc;
    unsigned char flags;
};

typedef struct cry_crc32_ctx cry_crc32_ctx;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize a generic CRC-16 algoritm context.
 *
 * @param ctx   CRC-16 context.
 * @param start algorithm start value.
 * @param tab   precomputed lookup table.
 * @param flags algorithm behaviour flags.
 */
void cry_crc16_init(cry_crc16_ctx *ctx, uint16_t start,
                    const uint16_t *tab, unsigned char flags);

/**
 * CRC update
 *
 * @param ctx   CRC-16 context.
 * @param in    Buffer pointer.
 * @param n     Number of octets in the buffer.
 */
void cry_crc16_update(cry_crc16_ctx *ctx, const unsigned char *in, size_t n);

/**
 * CRC finalize
 *
 * @param ctx   CRC-16 context.
 * @return      CRC result.
 */
uint16_t cry_crc16_final(cry_crc16_ctx *ctx);

/**
 * CRC finalize with a byte array output.
 *
 * @param ctx   CRC-16 context.
 * @param out   CRC as a BE byte array.
 */
void cry_crc16_digest(cry_crc16_ctx *ctx, unsigned char *out);

/**
 * Initialize as a CRC-16-CCITT algorithm.
 *
 * Used by: x.25, V.41, HDLC, XMODEM, Bluetooth, SD and many others.
 * Polynomial: x^16 + x^12 + x^5 + 1.
 *
 * @param ctx   CRC-16 context.
 */
void cry_crc16_ccitt_init(cry_crc16_ctx *ctx);

/**
 * CRC-16-CCITT all in one function.
 *
 * @param in    Buffer pointer.
 * @param n     Number of octets in the buffer.
 * @return      CRC value.
 */
uint16_t cry_crc16_ccitt(const unsigned char *in, size_t n);

/**
 * Initialize as a CRC-16-IBM algorithm
 *
 * Also known as CRC-16-ANSI.
 * Used by: Bisync, Modbus, USB, ANSI X.28 and many others.
 * Polynomial: x^16 + x^15 + x^2 + 1.
 *
 * @param ctx   CRC-16 context.
 */
void cry_crc16_ibm_init(cry_crc16_ctx *ctx);

/**
 * CRC-16-IBM all in one function.
 *
 *
 * @param in    Buffer pointer.
 * @param n     Number of octets in the buffer.
 * @return      CRC value.
 */
uint16_t cry_crc16_ibm(const unsigned char *in, size_t n);

/**
 * Initialize as a CRC-16-DNP algorithm
 *
 * Standardized by: CEI EN 60870-5-1
 * Used by: CTR protocol.
 * Polynomial: x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^2 + 1.
 *
 * @param ctx   CRC-16 context.
 */
void cry_crc16_dnp_init(cry_crc16_ctx *ctx);

/**
 * CRC-16-DNP all in one function.
 *
 *
 * @param in    Buffer pointer.
 * @param n     Number of octets in the buffer.
 * @return      CRC value.
 */
uint16_t cry_crc16_dnp(const unsigned char *in, size_t n);

/**
 * Initialize a generic CRC-32 algoritm context.
 *
 * @param ctx   CRC-32 context.
 * @param start algorithm start value.
 * @param tab   precomputed lookup table.
 * @param flags algorithm behaviour flags.
 */
void cry_crc32_init(cry_crc32_ctx *ctx, uint32_t start,
                    const uint32_t *tab, unsigned char flags);

/**
 * CRC update
 *
 * @param ctx   CRC-32 context.
 * @param in    Buffer pointer.
 * @param n     Number of octets in the buffer.
 */
void cry_crc32_update(cry_crc32_ctx *ctx, const unsigned char *in, size_t n);

/**
 * CRC finalize
 *
 * @param ctx   CRC-32 context.
 * @return      CRC result.
 */
uint32_t cry_crc32_final(cry_crc32_ctx *ctx);

/**
 * CRC finalize with a byte array output.
 *
 * @param ctx   CRC-32 context.
 * @param out   CRC as a BE byte array.
 */
void cry_crc32_digest(cry_crc32_ctx *ctx, unsigned char *out);

/**
 * Initialize as a CRC-32-ETH algorithm.
 *
 * Used by: Ethernet, ISO 3309, ANSI X3.66, FIPS PUB 71, ITU-T V.42, SATA,
 *          MPEG-2, Gzip, PKZIP, POSIX cksum, PNG, ZMODEM.
 * Polynomial:
 *  X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
 *
 * @param ctx   CRC-32 context.
 */
void cry_crc32_eth_init(cry_crc32_ctx *ctx);

/**
 * CRC-32-ETH all in one function.
 *
 * @param in    Buffer pointer.
 * @param n     Number of octets in the buffer.
 * @return      CRC value.
 */
uint32_t cry_crc32_eth(const unsigned char *in, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* CRY_CRC_H_ */
