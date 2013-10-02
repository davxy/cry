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

#include "cry_base64.h"

static const char *base64 =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int cry_base64_encode(const char *input, int len, char *output)
{
    /*
     * Looks like the output length is : (len/4 + 1) * 4
     */
    int i;
    do {
        i = (input[0] & 0xFC) >> 2;
        *output++ = base64[i];
        if (len == 1) {
            i = (input[0] & 0x03) << 4;
            *output++ = base64[i];
            *output++ = '=';
            *output++ = '=';
            break;
        }

        i = (input[0] & 0x03) << 4 | (input[1] & 0xF0) >> 4;
        *output++ = base64[i];
        if (len == 2) {
            i = (input[1] & 0x0F) << 2;
            *output++ = base64[i];
            *output++ = '=';
            break;
        }

        i = (input[1] & 0x0F) << 2 | (input[2] & 0xC0) >> 6;
        *output++ = base64[i];
        i = (input[2] & 0x3F);
        *output++ = base64[i];

        input += 3;
        len -= 3;
    } while(len != 0);
    *output = '\0';
    return 0;
}

static const unsigned char unbase64[] = {
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255,  62, 255, 255, 255,  63,
     52,  53,  54,  55,  56,  57,  58,  59,
     60,  61, 255, 255, 255,   0, 255, 255,
    255,   0,   1,   2,   3,   4,   5,   6,
      7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,
     23,  24,  25, 255, 255, 255, 255, 255,
    255,  26,  27,  28,  29,  30,  31,  32,
     33,  34,  35,  36,  37,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 255, 255, 255, 255, 255,
};

int cry_base64_decode(const char *input, int len, char *output)
{
    int outlen = 0, i;

    if (len & 0x03)
        return -1; /* should be a multiple of 4 */

    do {
        for (i = 0; i < 4; i++) {
            /* check for illegal base64 characters */
            if (input[i] > 127 || unbase64[(int)input[i]] == 255)
                return -1;
        }

        *output++ = unbase64[(int)input[0]] << 2 |
                    (unbase64[(int)input[1]] & 0x30) >> 4;
        outlen++;

        if (input[2] != '=') {
            *output++ = (unbase64[(int)input[1]] & 0x0F) << 4 |
                        (unbase64[(int)input[2]] & 0x3C) >> 2;
            outlen++;
        }

        if (input[3] != '=') {
            *output++ = (unbase64[(int)input[2]] & 0x03) << 6 |
                        unbase64[(int)input[3]];
            outlen++;
        }

        input += 4;
        len -= 4;
    } while(len != 0);
    return outlen;
}

