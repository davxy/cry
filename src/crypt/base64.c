#include <cry/base64.h>

static const char *base64 =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t cry_base64_encode(char *out, const char *in, size_t len)
{
    size_t i, outlen = 0;

    do {
        outlen += 4;

        i = (in[0] & 0xFC) >> 2;
        *out++ = base64[i];
        if (len == 1) {
            i = (in[0] & 0x03) << 4;
            *out++ = base64[i];
            *out++ = '=';
            *out++ = '=';
            break;
        }

        i = (in[0] & 0x03) << 4 | (in[1] & 0xF0) >> 4;
        *out++ = base64[i];
        if (len == 2) {
            i = (in[1] & 0x0F) << 2;
            *out++ = base64[i];
            *out++ = '=';
            break;
        }

        i = (in[1] & 0x0F) << 2 | (in[2] & 0xC0) >> 6;
        *out++ = base64[i];
        i = (in[2] & 0x3F);
        *out++ = base64[i];

        in += 3;
        len -= 3;
    } while(len != 0);
    return outlen;
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

size_t cry_base64_decode(char *out, const char *in, size_t len)
{
    size_t i, outlen = 0;

    if (len & 0x03)
        return -1; /* should be a multiple of 4 */

    do {
        for (i = 0; i < 4; i++) {
            /* check for illegal base64 characters */
            if (in[i] > 127 || unbase64[(int)in[i]] == 255)
                return -1;
        }

        *out++ = unbase64[(int)in[0]] << 2 |
                    (unbase64[(int)in[1]] & 0x30) >> 4;
        outlen++;

        if (in[2] != '=') {
            *out++ = (unbase64[(int)in[1]] & 0x0F) << 4 |
                        (unbase64[(int)in[2]] & 0x3C) >> 2;
            outlen++;
        }

        if (in[3] != '=') {
            *out++ = (unbase64[(int)in[2]] & 0x03) << 6 |
                        unbase64[(int)in[3]];
            outlen++;
        }

        in += 4;
        len -= 4;
    } while(len != 0);
    return outlen;
}
