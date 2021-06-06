/**
 * @file    hash.h
 * @brief   Generic hash algorithm interface
 */

#ifndef CRY_HASH_H_
#define CRY_HASH_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Context initialization.
 *
 * @param ctx   Hash context
 */
typedef void (*cry_hash_init_f)(void *ctx);

/**
 * Context cleanup.
 *
 * @param ctx   Hash context
 */
typedef void (*cry_hash_clear_f)(void *ctx);

/**
 * Digest update
 *
 * @param ctx   Hash context
 * @param data  Input data
 * @param suze  Input size
 */
typedef void (*cry_hash_update_f)(void *ctx, const unsigned char *data,
                                  size_t size);

/**
 * Finalize.
 *
 * @param ctx    Hash context
 * @param digest Preallocated output buffer
 */
typedef void (*cry_hash_digest_f)(void *ctx, unsigned char *digest);

#ifdef __cplusplus
}
#endif

/**
 * Hash algorithm generic interface
 */
struct cry_hash_itf {
    cry_hash_init_f init;       /**< Initialize (optional) */
    cry_hash_clear_f clear;     /**< Clear (optional) */
    cry_hash_update_f update;   /**< Update */
    cry_hash_digest_f digest;   /**< Digest (finalize) */
};

typedef struct cry_hash_itf cry_hash_itf;

#endif /* CRY_HASH_H_ */
