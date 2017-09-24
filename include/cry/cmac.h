/*
 * Copyright (c) 2013-2017, Davide Galassi. All rights reserved.
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
 * @file    cry_cmac.h
 * @brief   CMAC algorithm
 *
 * Also known as OMAC1 (One-key mac 1).
 */

#ifndef _CRY_CMAC_H_
#define _CRY_CMAC_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Cipher based digest (CMAC).
 *
 * @param mac       Digest output (16 octets).
 * @param input     Input data.
 * @param size      Size of input data.
 * @param key       Key data.
 * @param keysize   Size if key.
 */
void cry_cmac_digest(unsigned char *mac, const unsigned char *input,
                     size_t size, const unsigned char *key, size_t keysize);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_CMAC_H_ */

