/*
 * Copyright (c) 2013-2015, Davide Galassi. All rights reserved.
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
 * @file    cry_rand.h
 * @brief   Pseudo Random Number Generator
 */

#ifndef _CRY_RAND_H_
#define _CRY_RAND_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Random number generator initialization.
 * Because very often people forget to perform initialization, other rand
 * functions will automatically check to see if this routine has been called
 * and call it at that point if not.
 *
 * @return      0 on success, -1 on failure.
 */
int cry_rand_init(void);

/**
 * Produce a cryptographically strong random sequence.
 *
 * @param buf   Destination buffer
 * @param siz   Destination buffer size.
 * @return      0 on success, -1 on failure.
 */
int cry_rand(unsigned char *buf, unsigned int siz);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_RAND_H_ */

