/**
 * @file    prng.h
 * @brief   Pseudo Random Number Generators
 */

#ifndef CRY_PRNG_H_
#define CRY_PRNG_H_

#include <stddef.h>

/*
 * Defaults to the cryptographically secure PRNG.
 */

#define cry_prng_init cry_prng_aes_init
#define cry_prng_rand cry_prng_aes_rand

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Strong pseudo random number generator initialization.
 *
 * Because very often people forget to perform initialization, other prng
 * functions will automatically check to see if this routine has been called
 * and call it at that point if not.
 *
 * @param   seed        Seed data.
 * @param   seed_siz    Seed data size.
 * @return              Operation result.
 */
int cry_prng_aes_init(const unsigned char *seed, size_t seed_siz);

/**
 * Strong pseudo random number generator production.
 *
 * @param buf   Destination buffer
 * @param siz   Destination buffer size.
 * @return      Operation result.
 */
int cry_prng_aes_rand(unsigned char *buf, size_t siz);

/**
 * LFSR-113 pseudo random number generator initialization.
 *
 * Because very often people forget to perform initialization, other prng
 * functions will automatically check to see if this routine has been called
 * and call it at that point if not.
 *
 * @param   seed        Seed data.
 * @param   seed_siz    Seed data size.
 * @return              Operation result.
 */
int cry_prng_lfsr113_init(const unsigned char *seed, size_t seed_siz);

/**
 * LFSR-113 pseudo random number generator production.
 *
 * @param buf   Destination buffer
 * @param siz   Destination buffer size.
 * @return      Operation result.
 */
int cry_prng_lfsr113_rand(unsigned char *buf, size_t siz);

#ifdef __cplusplus
}
#endif


#endif /* CRY_PRNG_H_ */
