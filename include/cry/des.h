/**
 * @file    des.h
 * @brief   DES block cipher.
 */

#ifndef CRY_DES_H_
#define CRY_DES_H_

#include <string.h>

/** DES algorithm key size in octets (64 bit) */
#define CRY_DES_BLOCK_SIZE  8


/** DES context */
struct cry_des_ctx {
    unsigned int  keylen; /* 8 for normal DES, 24 for triple DES */
    unsigned char key[3*CRY_DES_BLOCK_SIZE];
};

typedef struct cry_des_ctx cry_des_ctx;


void cry_des_key_set(cry_des_ctx *ctx, unsigned char *key,
                     unsigned int size);

void cry_des_encrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

void cry_des_decrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, unsigned int size);

#endif /* CRY_DES_H_ */
