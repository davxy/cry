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

#include <cry.h>
#include <stdio.h>

int main(void)
{
    printf("CRY version: %d.%d.%d (%d)\n",
            CRY_MAJOR, CRY_MINOR, CRY_PATCH, cry_version());
    printf("CRY version (build-time): %d\n", cry_version());
    if (cry_version() != CRY_VERSION)
        printf("Misaligned build/headers version\n");

    return 0;
}
