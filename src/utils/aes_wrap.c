/*
 * The key wrap algorithm includes a strong integrity check on the key
 * data.  If unwrapping produces the expected check value in A[0], then
 * the chance that the key data is corrupt is 2^-64.  If unwrapping
 * produces an unexpected value, then the algorithm implementation MUST
 * return an error, and it MUST NOT return any key data.
 *
 * Implementations must protect the KEK from disclosure.  Compromise of
 * the KEK may result in the disclosure of all key data protected with
 * that KEK.
 */

#include <cry/utils.h>
#include <cry/aes.h>
#include <cry/error.h>
#include <string.h>


#define INIT_VAL    0xA6

int cry_aes_wrap(unsigned char *out, const unsigned char *in, size_t len,
                 const unsigned char *key, size_t keylen)
{
    unsigned char *a, *r, b[16];
    size_t i, j, blks;
    cry_aes_ctx aes;

    a = out;
    r = out + 8;

    /* Initialize variables */
    memset(a, INIT_VAL, 8);
    memcpy(r, in, len);
    cry_aes_key_set(&aes, key, keylen);

    /* Compute intermediate values */
    blks = len >> 3; /* Number of 64 bit (8 bytes) blocks */
    for (i = 0; i <= 5; i++) {
        r = out + 8;
        for (j = 1; j <= blks; j++) {
            memcpy(b, a, 8);
            memcpy(b + 8, r, 8);
            cry_aes_encrypt(&aes, b, b, 16);
            memcpy(a, b, 8);
            a[7] ^= blks * i + j;
            memcpy(r, b + 8, 8);
            r += 8;
        }
    }
    return 0;
}

int cry_aes_unwrap(unsigned char *out, const unsigned char *in, size_t len,
                   const unsigned char *key, size_t keylen)
{
    unsigned char *r, a[8], b[16];
    cry_aes_ctx aes;
    size_t i, j, blks;
    int res = 0;

    /* Initialize variables */
    memcpy(a, in, 8);
    r = out;
    memcpy(r, in + 8, len);
    cry_aes_key_set(&aes, key, keylen);

    /* Compute intermediate values */
    blks = len >> 3; /* Number of 64 bit (8 bytes) blocks */
    i = 6;
    while (i-- > 0) {
        r = out + (blks - 1) * 8;
        for (j = blks; j >= 1; j--) {
            memcpy(b, a, 8);
            b[7] ^= blks * i + j;
            memcpy(b + 8, r, 8);
            cry_aes_decrypt(&aes, b, b, 16);
            memcpy(a, b, 8);
            memcpy(r, b + 8, 8);
            r -= 8;
        }
    }
    /* Unwrap check */
    for (i = 0; i < 8; i++) {
        if (b[i] != INIT_VAL) {
            memset(out, 0, len);
            res = CRY_ERROR_BAD_DATA;
            break;
        }
    }
    return res;
}
