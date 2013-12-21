/*
 * Copyright (c) 2013, Davide Galassi. All rights reserved.
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

#ifndef _CRY_MEMXOR_H_
#define _CRY_MEMXOR_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
 * XOR two memory regions.
 *
 * The result is stored in the first operand.
 * The memory segments may overlap and can work inplace.
 *
 * @param dst   First source and destination.
 * @param src   Second source.
 * @param size  Number of bytes to XOR.
 */
void cry_memxor(unsigned char *dst, const unsigned char *src,
                unsigned int size);

/**
 * XOR two memory regions.
 *
 * The memory segments may overlap and can work inplace.
 *
 * @param dst   Destination.
 * @param src1  First source.
 * @param src2  Second source.
 * @param size  Number of bytes to XOR.
 */
void cry_memxor3(unsigned char *dst, const unsigned char *src1,
                 const unsigned char *src2, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_MEMXOR_H_ */

