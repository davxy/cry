/**
 * @file    hash.h
 * @brief   Generic hash algorithm interface
 */

#ifndef CRY_HASH_H_
#define CRY_HASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

/**
 * Context initialization.
 *
 * @param ctx   Crypto context
 */
typedef void (* cry_hash_init_f)(void *ctx);

/**
 * Context cleanup.
 * Eventually release resources allocated by the init.
 *
 * @param ctx   Hash context
 */
typedef void (* cry_hash_clean_f)(void *ctx);

/**
 * Digest update
 *
 * @param ctx   Hash context
 * @param data  Input data
 * @param suze  Input size
 */
typedef void (* cry_hash_update_f)(void *ctx, const uint8_t *data,
                                   size_t size);

/**
 * Finalize.
 *
 * @param ctx    Hash context
 * @param digest Preallocated output buffer
 */
typedef void (* cry_hash_digest_f)(void *ctx, uint8_t *digest);

/**
 * Hash algorithm generic interface
 */
struct cry_hash_itf {
    cry_hash_init_f init;       /**< Initialize (optional) */
    cry_hash_clean_f clean;     /**< Clean (optional) */
    cry_hash_update_f update;   /**< Update */
    cry_hash_digest_f digest;   /**< Digest (finalize) */
};

typedef struct cry_hash_itf cry_hash_itf;

#ifdef __cplusplus
}
#endif

#endif /* CRY_HASH_H_ */
