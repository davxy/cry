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
 * @file    cry_cmac.h
 * @brief   CBC-MAC algorithm
 */

#ifndef _CRY_CMAC_H_
#define _CRY_CMAC_H_

#include "cry_aes.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Cipher based digest (CMAC).
 *
 * @param ctx       AES context.
 * @param digest    Digest output.
 * @param input     Input data.
 * @param size      Size of input data.
 */
void cry_cmac_digest(struct cry_aes_ctx *ctx, unsigned char *digest,
                     const unsigned char *input, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_CMAC_H_ */

