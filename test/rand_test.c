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

#include "test.h"
#include <cry/prng.h>
#include <time.h>

/* TODO: insert a real randomness test */

void rand_test(void)
{
    time_t t[4];

    t[0] = t[1] = t[2] = t[3] = time(NULL);
    ASSERT_OK(cry_prng_init((unsigned char *)t, sizeof(t)));
    ASSERT_OK(cry_prng_rand(buf, 64));
    PRINT_HEX("rand", buf, 64);
}

