#ifndef CRY_HILL_H_
#define CRY_HILL_H_

#include <stddef.h>

#define CRY_HILL_KEYDIM_MAX 10
#define CRY_HILL_KEYLEN_MAX (CRY_HILL_KEYDIM_MAX*CRY_HILL_KEYDIM_MAX)

#define CRY_HILL_KEYGEN_TRIALS 100

struct cry_hill_ctx {
    unsigned char key[CRY_HILL_KEYLEN_MAX];
    unsigned char ikey[CRY_HILL_KEYLEN_MAX];
    size_t n;
};

typedef struct cry_hill_ctx cry_hill_ctx;

int cry_hill_init(struct cry_hill_ctx *ctx, const unsigned char *key,
                  const unsigned char *ikey, size_t keylen);

void cry_hill_encrypt(struct cry_hill_ctx *ctx, unsigned char *out,
                      const unsigned char *in, size_t len);

void cry_hill_decrypt(struct cry_hill_ctx *ctx, unsigned char *out,
                      const unsigned char *in, size_t len);

#endif /* CRY_HILL_H_ */
