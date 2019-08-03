/**
 * @file    cmac.h
 * @brief   CMAC algorithm
 *
 * Also known as OMAC1 (One-key mac 1).
 */

#ifndef CRY_CMAC_H_
#define CRY_CMAC_H_

#include <cry/ciph.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CRY_CMAC_BLOCK_SIZE 16

struct cry_cmac_ctx {
    void *ciph_ctx;                          /**< Block cipher context */
    const struct cry_ciph_itf *ciph_itf;     /**< Block cipher interface */
    size_t blklen;                           /**< Pending block size */
    unsigned char blk[CRY_CMAC_BLOCK_SIZE];  /**< Pending block */
    unsigned char mac[CRY_CMAC_BLOCK_SIZE];  /**< Current MAC */
};

typedef struct cry_cmac_ctx cry_cmac_ctx;

void cry_cmac_init(cry_cmac_ctx *ctx, void *ciph_ctx,
                   const cry_ciph_itf *ciph_itf,
                   unsigned char *key, size_t keylen);

void cry_cmac_update(cry_cmac_ctx *ctx, const unsigned char *data, size_t len);

void cry_cmac_digest(cry_cmac_ctx *ctx, unsigned char *mac);

#ifdef __cplusplus
}
#endif

#endif /* CRY_CMAC_H_ */
