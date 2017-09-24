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
 * @file    cry_prng.h
 * @brief   Pseudo Random Number Generator
 */

#ifndef _CRY_PRNG_H_
#define _CRY_PRNG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>

/**
 * Pseudo random number generator initialization.
 *
 * Because very often people forget to perform initialization, other prng
 * functions will automatically check to see if this routine has been called
 * and call it at that point if not.
 *
 * @param   seed        Seed data.
 * @param   seed_siz    Seed data size.
 * @return  0 on success, -1 on failure.
 */
int cry_prng_init(const unsigned char *seed, size_t seed_siz);

/**
 * Produce a pseudo random sequence.
 *
 * @param buf   Destination buffer
 * @param siz   Destination buffer size.
 * @return      0 on success, -1 on failure.
 */
int cry_prng_rand(unsigned char *buf, size_t siz);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_PRNG_H_ */

