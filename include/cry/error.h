/**
 * @file    error.h
 * @brief   Overarching error types that may be returned in place of the expected result.
 */

#ifndef CRY_ERROR_H_
#define CRY_ERROR_H_

/**
 * Error triggered by bad or unexpected input data.
 */
#define CRY_ERROR_BAD_DATA         -1
/**
 * Out of memory error.
 * This may be triggered by all algorithms and structures allocating dynamic data.
 * (Currently MPI related).
 */
#define CRY_ERROR_OUT_OF_MEMORY    -2
/**
 * Requested feature is not implemented yet.
 */
#define CRY_ERROR_NOT_IMPLEMENTED  -3
/**
 * Error returned for more borderline situations.
 * For example if the max number of iterations has been reached during primes or key generation.
 */
#define CRY_ERROR_OTHER            -42

#define CRY_IS_ERROR(result)        ((int)(result) < 0)
#define CRY_IS_OK(result)           !CRY_IS_ERROR(result)

#endif /* CRY_ERROR_H_ */
