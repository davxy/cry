/**
 * @file    cmac.h
 * @brief   CMAC algorithm
 *
 * Also known as OMAC1 (One-key mac 1).
 */

#ifndef _CRY_CMAC_H_
#define _CRY_CMAC_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Cipher based digest (CMAC).
 *
 * @param mac       Digest output (16 octets).
 * @param input     Input data.
 * @param size      Size of input data.
 * @param key       Key data.
 * @param keysize   Size if key.
 */
void cry_cmac_digest(unsigned char *mac, const unsigned char *input,
                     size_t size, const unsigned char *key, size_t keysize);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_CMAC_H_ */
