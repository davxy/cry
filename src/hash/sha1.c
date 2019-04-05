#include <cry/sha1.h>
#include <string.h>
#include "../misc.h"


#define R(t) \
( \
    tmp = W[(t - 3) & 0x0F] ^ W[(t - 8) & 0x0F] ^ \
          W[(t - 14) & 0x0F] ^ W[t & 0x0F], \
    (W[t & 0x0F] = CRY_ROTL32(tmp, 1)) \
)

#define P(a,b,c,d,e,x) do { \
    e += CRY_ROTL32(a, 5) + F(b, c, d) + K + x; \
    b  = CRY_ROTL32(b, 30); \
    } while (0)


static void cry_sha1_process(cry_sha1_ctx *ctx, const unsigned char *data)
{
    uint32_t W[16], A, B, C, D, E, tmp;

    CRY_READ32_BE(W[0], data);
    CRY_READ32_BE(W[1], data + 4);
    CRY_READ32_BE(W[2], data + 8);
    CRY_READ32_BE(W[3], data + 12);
    CRY_READ32_BE(W[4], data + 16);
    CRY_READ32_BE(W[5], data + 20);
    CRY_READ32_BE(W[6], data + 24);
    CRY_READ32_BE(W[7], data + 28);
    CRY_READ32_BE(W[8], data + 32);
    CRY_READ32_BE(W[9], data + 36);
    CRY_READ32_BE(W[10], data + 40);
    CRY_READ32_BE(W[11], data + 44);
    CRY_READ32_BE(W[12], data + 48);
    CRY_READ32_BE(W[13], data + 52);
    CRY_READ32_BE(W[14], data + 56);
    CRY_READ32_BE(W[15], data + 60);

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];

#define F(x, y, z) (z ^ (x & (y ^ z)))
#define K 0x5A827999
    P(A, B, C, D, E, W[0]);
    P(E, A, B, C, D, W[1]);
    P(D, E, A, B, C, W[2]);
    P(C, D, E, A, B, W[3]);
    P(B, C, D, E, A, W[4]);
    P(A, B, C, D, E, W[5]);
    P(E, A, B, C, D, W[6]);
    P(D, E, A, B, C, W[7]);
    P(C, D, E, A, B, W[8]);
    P(B, C, D, E, A, W[9]);
    P(A, B, C, D, E, W[10]);
    P(E, A, B, C, D, W[11]);
    P(D, E, A, B, C, W[12]);
    P(C, D, E, A, B, W[13]);
    P(B, C, D, E, A, W[14]);
    P(A, B, C, D, E, W[15]);
    P(E, A, B, C, D, R(16));
    P(D, E, A, B, C, R(17));
    P(C, D, E, A, B, R(18));
    P(B, C, D, E, A, R(19));
#undef K
#undef F

#define F(x, y, z) (x ^ y ^ z)
#define K 0x6ED9EBA1
    P(A, B, C, D, E, R(20));
    P(E, A, B, C, D, R(21));
    P(D, E, A, B, C, R(22));
    P(C, D, E, A, B, R(23));
    P(B, C, D, E, A, R(24));
    P(A, B, C, D, E, R(25));
    P(E, A, B, C, D, R(26));
    P(D, E, A, B, C, R(27));
    P(C, D, E, A, B, R(28));
    P(B, C, D, E, A, R(29));
    P(A, B, C, D, E, R(30));
    P(E, A, B, C, D, R(31));
    P(D, E, A, B, C, R(32));
    P(C, D, E, A, B, R(33));
    P(B, C, D, E, A, R(34));
    P(A, B, C, D, E, R(35));
    P(E, A, B, C, D, R(36));
    P(D, E, A, B, C, R(37));
    P(C, D, E, A, B, R(38));
    P(B, C, D, E, A, R(39));
#undef K
#undef F

#define F(x, y, z) ((x & y) | (z & (x | y)))
#define K 0x8F1BBCDC
    P(A, B, C, D, E, R(40));
    P(E, A, B, C, D, R(41));
    P(D, E, A, B, C, R(42));
    P(C, D, E, A, B, R(43));
    P(B, C, D, E, A, R(44));
    P(A, B, C, D, E, R(45));
    P(E, A, B, C, D, R(46));
    P(D, E, A, B, C, R(47));
    P(C, D, E, A, B, R(48));
    P(B, C, D, E, A, R(49));
    P(A, B, C, D, E, R(50));
    P(E, A, B, C, D, R(51));
    P(D, E, A, B, C, R(52));
    P(C, D, E, A, B, R(53));
    P(B, C, D, E, A, R(54));
    P(A, B, C, D, E, R(55));
    P(E, A, B, C, D, R(56));
    P(D, E, A, B, C, R(57));
    P(C, D, E, A, B, R(58));
    P(B, C, D, E, A, R(59));
#undef K
#undef F

#define F(x, y, z) (x ^ y ^ z)
#define K 0xCA62C1D6
    P(A, B, C, D, E, R(60));
    P(E, A, B, C, D, R(61));
    P(D, E, A, B, C, R(62));
    P(C, D, E, A, B, R(63));
    P(B, C, D, E, A, R(64));
    P(A, B, C, D, E, R(65));
    P(E, A, B, C, D, R(66));
    P(D, E, A, B, C, R(67));
    P(C, D, E, A, B, R(68));
    P(B, C, D, E, A, R(69));
    P(A, B, C, D, E, R(70));
    P(E, A, B, C, D, R(71));
    P(D, E, A, B, C, R(72));
    P(C, D, E, A, B, R(73));
    P(B, C, D, E, A, R(74));
    P(A, B, C, D, E, R(75));
    P(E, A, B, C, D, R(76));
    P(D, E, A, B, C, R(77));
    P(C, D, E, A, B, R(78));
    P(B, C, D, E, A, R(79));
#undef K
#undef F

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
    ctx->state[4] += E;
}


void cry_sha1_init(cry_sha1_ctx *ctx)
{
    memset(ctx, 0, sizeof(*ctx));
    /* Load magic initialization constants. */
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
}


void cry_sha1_clear(cry_sha1_ctx *ctx)
{
    cry_memset(ctx, 0, sizeof(*ctx));
}


void cry_sha1_update(cry_sha1_ctx *ctx, const unsigned char *data, size_t len)
{
    size_t fill;
    uint32_t left;

    if(len == 0)
        return;

    left = ctx->len[0] & 0x3F;
    fill = 64 - left;

    ctx->len[0] += (uint32_t) len;
    if(ctx->len[0] < (uint32_t) len)
        ctx->len[1]++;

    if(left && len >= fill) {
        memcpy(ctx->data + left, data, fill);
        cry_sha1_process(ctx, ctx->data);
        data += fill;
        len  -= fill;
        left = 0;
    }

    while(len >= 64) {
        cry_sha1_process(ctx, data);
        data += 64;
        len  -= 64;
    }

    if(len > 0)
        memcpy(ctx->data + left, data, len);
}


void cry_sha1_digest(cry_sha1_ctx *ctx, unsigned char *out)
{
    uint32_t last;
    uint32_t hi, lo;
    size_t padlen;
    unsigned char msglen[8];
    unsigned char pad[64] = { 0 };

    /* Length in bits */
    hi = (ctx->len[0] >> 29) | (ctx->len[1] <<  3);
    lo = (ctx->len[0] <<  3);

    CRY_WRITE32_BE(hi, msglen);
    CRY_WRITE32_BE(lo, msglen + 4);

    last = ctx->len[0] & 0x3F;
    padlen = (last < 56) ? (56 - last) : (120 - last);

    pad[0] = 0x80;
    cry_sha1_update(ctx, pad, padlen);
    cry_sha1_update(ctx, msglen, 8);

    CRY_WRITE32_BE(ctx->state[0], out);
    CRY_WRITE32_BE(ctx->state[1], out + 4);
    CRY_WRITE32_BE(ctx->state[2], out + 8);
    CRY_WRITE32_BE(ctx->state[3], out + 12);
    CRY_WRITE32_BE(ctx->state[4], out + 16);
}


void cry_sha1(unsigned char *out, const unsigned char *data, size_t len)
{
    cry_sha1_ctx ctx;

    cry_sha1_init(&ctx);
    cry_sha1_update(&ctx, data, len);
    cry_sha1_digest(&ctx, out);
    cry_sha1_clear(&ctx);
}
