/**
 * @file    cry_prng.h
 * @brief   Pseudo Random Number Generator
 */

#ifndef _CRY_PRNG_H_
#define _CRY_PRNG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>

/**
 * Pseudo random number generator initialization.
 *
 * Because very often people forget to perform initialization, other prng
 * functions will automatically check to see if this routine has been called
 * and call it at that point if not.
 *
 * @param   seed        Seed data.
 * @param   seed_siz    Seed data size.
 * @return  0 on success, -1 on failure.
 */
int cry_prng_init(const unsigned char *seed, size_t seed_siz);

/**
 * Produce a pseudo random sequence.
 *
 * @param buf   Destination buffer
 * @param siz   Destination buffer size.
 * @return      0 on success, -1 on failure.
 */
int cry_prng_rand(unsigned char *buf, size_t siz);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_PRNG_H_ */
