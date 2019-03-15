/**
 * @file    prng.h
 * @brief   Pseudo Random Number Generator
 */

#ifndef CRY_PRNG_H_
#define CRY_PRNG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>

/**
 * Strong pseudo random number generator initialization.
 *
 * Because very often people forget to perform initialization, other prng
 * functions will automatically check to see if this routine has been called
 * and call it at that point if not.
 *
 * @param   seed        Seed data.
 * @param   seed_siz    Seed data size.
 * @return  0 on success, -1 on failure.
 */
int cry_prng_aes_init(const unsigned char *seed, size_t seed_siz);

/**
 * Strong pseudo random number generator production.
 *
 * @param buf   Destination buffer
 * @param siz   Destination buffer size.
 * @return      0 on success, -1 on failure.
 */
int cry_prng_aes_rand(unsigned char *buf, size_t siz);

/**
 * Weak pseudo random number generator initialization.
 *
 * Because very often people forget to perform initialization, other prng
 * functions will automatically check to see if this routine has been called
 * and call it at that point if not.
 *
 * @param   seed        Seed data.
 * @param   seed_siz    Seed data size.
 * @return  0 on success, -1 on failure.
 */
int cry_prng_weak_init(const unsigned char *seed, size_t seed_siz);

/**
 * Weak pseudo random number generator production.
 *
 * @param buf   Destination buffer
 * @param siz   Destination buffer size.
 * @return      0 on success, -1 on failure.
 */
int cry_prng_weak_rand(unsigned char *buf, size_t siz);

#ifdef __cplusplus
}
#endif

#endif /* CRY_PRNG_H_ */
