#ifndef CRY_ASSERT_H_
#define CRY_ASSERT_H_

#include <cry/config.h>

/**
 * User supplied callback function for parameter validation failure.
 *
 * This function will be called unless an alternative treatement
 * is defined through the CRY_ASSERT_FAIL macro.
 *
 * This function can return, and the operation will be aborted, or
 * alternatively, through use of setjmp()/longjmp() can resume
 * execution in the application code.
 *
 * This function is invoked only if CRY_ASSERT_CHECK is defined.
 *
 * @param cond  Assertion that didn't hold.
 * @param file  File where the assertion failed.
 * @param line  Line in the file where the assertion failed.
 */
void cry_assert_fail(const char *cond, const char *file, int line);


#ifdef CRY_ASSERT_CHECK

#ifndef CRY_ASSERT_FAIL
#define CRY_ASSERT_FAIL(cond) \
        cry_assert_fail(#cond, __FILE__, __LINE__)
#endif

#define CRY_ASSERT_RES(cond, res) do { \
    if (!(cond)) { \
        CRY_ASSERT_FAIL(cond); \
        return (res); \
    } \
} while (0)

#else

#define CRY_ASSERT_RES(cond, res) ((void)0)

#endif /* ~CRY_ASSERT_CHECK */

#define CRY_ASSERT(cond)    CRY_ASSERT_RES(cond, (void)0)
#define CRY_ASSERT2(cond)   CRY_ASSERT_RES(cond, -1)

#endif /* CRY_ASSERT_H_ */
