/**
 * @file    base64.h
 * @brief   Base-64 encoder/decoder.
 */

#ifndef CRY_BASE64_H_
#define CRY_BASE64_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int cry_base64_encode(char *out, const char *in, size_t len);

int cry_base64_decode(char *out, const char *in, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* CRY_BASE64_H_ */
