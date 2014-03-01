/*
 * Copyright (c) 2013-2014, Davide Galassi. All rights reserved.
 *
 * This file is part of CRY software.
 *
 * CRY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CRY; if not, see <http://www.gnu/licenses/>.
 */

/**
 * @file    cry_crc.h
 * @brief   Cyclic Redundancy Check
 *
 * Wikipedia reference:
 * Polynomial_representations_of_cyclic_redundancy_checks
 */

#ifndef _CRY_CRC_H_
#define _CRY_CRC_H_

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
    const unsigned short  *tab;
    unsigned short        crc;
    unsigned char         flags;
};

typedef struct cry_crc16_ctx cry_crc16_ctx;

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Initialize a generic CRC-16 algoritm context.
 *
 * @param ctx   CRC-16 context.
 * @param start algorithm start value.
 * @param tab   precomputed lookup table.
 * @param flags algorithm behaviour flags.
 */
void cry_crc16_init(struct cry_crc16_ctx *ctx, unsigned short start,
                    const unsigned short *tab, unsigned char flags);

/**
 * CRC update
 *
 * @param ctx   CRC-16 context.
 * @param ptr   Buffer pointer.
 * @param n     Number of octets in the buffer.
 */
void cry_crc16_update(struct cry_crc16_ctx *ctx,
                      const unsigned char *ptr, unsigned int n);

/**
 * CRC finalize
 *
 * @param ctx   CRC-16 context.
 * @return      CRC result.
 */
unsigned short cry_crc16_final(struct cry_crc16_ctx *ctx);

/**
 * Initialize as a CRC-16-CCITT algorithm.
 *
 * Used by: x.25, V.41, HDLC, XMODEM, Bluetooth, SD and many others.
 * Polynomial: x^16 + x^12 + x^5 + 1.
 *
 * @param ctx   CRC-16 context.
 */
void cry_crc16_ccitt_init(struct cry_crc16_ctx *ctx);

/**
 * CRC-16-CCITT all in one function.
 *
 * @param ptr   Buffer pointer.
 * @param n     Number of octets in the buffer.
 * @return      CRC value.
 */
unsigned short cry_crc16_ccitt(const unsigned char *ptr, unsigned int n);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_CRC_H_ */

