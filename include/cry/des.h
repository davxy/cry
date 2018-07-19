/**
 * @file    des.h
 * @brief   DES block cipher.
 */

#ifndef _CRY_DES_H_
#define _CRY_DES_H_

#include <string.h>

/** DES algorithm key size in octets (64 bit) */
#define CRY_DES_BLOCK_SIZE  8


/** DES context */
struct cry_des_ctx {
    unsigned int  keylen; /* 8 for normal DES, 24 for triple DES */
    unsigned char key[3*CRY_DES_BLOCK_SIZE];
};

typedef struct cry_des_ctx cry_des_ctx;


#define cry_des_key_set(ctx, _key, _keylen) do { \
    (ctx)->keylen = _keylen; \
    memcpy((ctx)->key, _key, _keylen); \
    } while(0)

void cry_des_encrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

void cry_des_decrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

#endif /* _CRY_DES_H_ */
