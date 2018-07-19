/**
 * @file    cry_sum.h
 * @brief   Common checksum algorithms.
 */

#ifndef _CRY_SUM_H_
#define _CRY_SUM_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Trivial checksum.
 *
 * @param in    Input buffer.
 * @param n     Size of input buffer.
 * @return      Checksum value.
 */
unsigned char cry_cs8(const unsigned char *in, size_t n);

/**
 * Longitudinal redundancy check (LRC).
 *
 * @param in    Input buffer.
 * @param n     Size of input buffer.
 * @return      LRC value.
 */
unsigned char cry_lrc(const unsigned char *in, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_SUM_H_ */
