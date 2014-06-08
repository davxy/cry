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
 * @file    cry_sum.h
 * @brief   Common checksum algorithms.
 */

#ifndef _CRY_SUM_H_
#define _CRY_SUM_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Trivial checksum.
 *
 * @param in    Input buffer.
 * @param n     Size of input buffer.
 * @return      Checksum value.
 */
unsigned char cry_cs(const unsigned char *in, size_t n);

/**
 * Longitudinal redundancy check (LRC).
 *
 * @param in    Input buffer.
 * @param n     Size of input buffer.
 * @return      LRC value.
 */
unsigned char cry_lrc(const unsigned char *in, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_SUM_H_ */

