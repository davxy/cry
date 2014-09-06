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

#include <cry/cmac.h>
#include <string.h>
#include <stdio.h>

#define KEY "0123456789abcdef"
#define MSG "HelloWorld"

int main(int argc, char **argv)
{
    unsigned char md[16];
    int i;

    cry_cmac_digest(md, MSG, strlen(MSG), KEY, sizeof(KEY));

    printf("CMAC: ");
    for (i = 0; i < sizeof(md); i++)
        printf("%x", md[i]);
    printf("\n");

    return 0;
}
