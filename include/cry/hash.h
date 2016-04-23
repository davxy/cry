/*
 * Copyright (c) 2013-2016, Davide Galassi. All rights reserved.
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

#ifndef _CRY_HASH_H_
#define _CRY_HASH_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <stdint.h>

/**
 * Context initialization.
 *
 * @param ctx   Crypto context
 */
typedef void (* cry_hash_init_f) (void *ctx);

/**
 * Context cleanup.
 * Eventually release resources allocated by the init.
 *
 * @param ctx   Hash context
 */
typedef void (* cry_hash_clean_f) (void *ctx);

/**
 * Digest update
 *
 * @param ctx   Hash context
 * @param data  Input data
 * @param suze  Input size
 */
typedef void (* cry_hash_update_f) (void *ctx, const uint8_t *data,
                                    size_t size);

/**
 * Finalize.
 *
 * @param ctx   Hash context
 * @digest      Preallocated output buffer
 */
typedef void (* cry_hash_digest_f) (void *ctx, uint8_t *digest);

/**
 * Hash algorithm generic interface
 */
struct cry_hash_itf {
    cry_hash_init_f   init;     /**< Initialize (optional) */
    cry_hash_clean_f  clean;    /**< Clean (optional) */
    cry_hash_update_f update;   /**< Update */ 
    cry_hash_digest_f digest;   /**< Digest (finalize) */
};

typedef struct cry_hash_itf cry_hash_itf;

#ifdef __cplusplus
}
#endif

#endif /* _CRY_HASH_H_ */

