/**
 * @file    cry_base64.h
 * @brief   Base-64 encoder/decoder.
 */

#ifndef _CRY_BASE64_H_
#define _CRY_BASE64_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>

size_t cry_base64_encode(const char *in, size_t len, char *out);

size_t cry_base64_decode(const char *in, size_t len, char *out);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_BASE64_H_ */
