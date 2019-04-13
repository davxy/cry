#include <cry/des.h>

#define EXPANSION_BLOCK_SIZE    6
#define PC1_KEY_SIZE            7
#define SUBKEY_SIZE             6

/* Initial permutation table */
static const unsigned char ip_tab[] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

/* Final permutation table */
static const unsigned char fp_tab[] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

static const unsigned char pc1_tab[] = {
    57, 49, 41, 33, 25, 17,  9,  1,
    58, 50, 42, 34, 26, 18, 10,  2,
    59, 51, 43, 35, 27, 19, 11,  3,
    60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15,  7, 62, 54, 46, 38,
    30, 22, 14,  6, 61, 53, 45, 37,
    29, 21, 13,  5, 28, 20, 12,  4
};

static const unsigned char pc2_tab[] = {
    14, 17, 11, 24,  1,  5,  3, 28,
    15,  6, 21, 10, 23, 19, 12,  4,
    26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

/* Block expansion permutation table */
static const unsigned char exp_tab[] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static const unsigned char p_tab[] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

/* S-Box table.
 * In textbooks, the presented S-Box table is usually constructed by
 * interpreting the six bits input as
 *  [ R1 | C3 | C2 | C1 | C0 | R0 ]
 * Thus, for example, the output for the bit string 101100 is found
 * in row 2 and column 6.
 * The table below was obviously constructed as a more software
 * friendly array. Thus the same element could be found at offset 44.
 *
 */
static const unsigned char sbox[8][64] = {
    {
        14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1,
         3, 10, 10,  6,  6, 12, 12, 11,  5,  9,  9,  5,  0,  3,  7,  8,
         4, 15,  1, 12, 14,  8,  8,  2, 13,  4,  6,  9,  2,  1, 11,  7,
        15,  5, 12, 11,  9,  3,  7, 14,  3, 10, 10,  0,  5,  6,  0, 13
    },
    {
        15,  3,  1, 13,  8,  4, 14,  7,  6, 15, 11,  2,  3,  8,  4, 14,
         9, 12,  7,  0,  2,  1, 13, 10, 12,  6,  0,  9,  5, 11, 10,  5,
         0, 13, 14,  8,  7, 10, 11,  1, 10,  3,  4, 15, 13,  4,  1,  2,
         5, 11,  8,  6, 12,  7,  6, 12,  9,  0,  3,  5,  2, 14, 15,  9
    },
    {
        10, 13,  0,  7,  9,  0, 14,  9,  6,  3,  3,  4, 15,  6,  5, 10,
         1,  2, 13,  8, 12,  5,  7, 14, 11, 12,  4, 11,  2, 15,  8,  1,
        13,  1,  6, 10,  4, 13,  9,  0,  8,  6, 15,  9,  3,  8,  0,  7,
        11,  4,  1, 15,  2, 14, 12,  3,  5, 11, 10,  5, 14,  2,  7, 12
    },
    {
         7, 13, 13,  8, 14, 11,  3,  5,  0,  6,  6, 15,  9,  0, 10,  3,
         1,  4,  2,  7,  8,  2,  5, 12, 11,  1, 12, 10,  4, 14, 15,  9,
        10,  3,  6, 15,  9,  0,  0,  6, 12, 10, 11,  1,  7, 13, 13,  8,
        15,  9,  1,  4,  3,  5, 14, 11,  5, 12,  2,  7,  8,  2,  4, 14
    },
    {
         2, 14, 12, 11,  4,  2,  1, 12,  7,  4, 10,  7, 11, 13,  6,  1,
         8,  5,  5,  0,  3, 15, 15, 10, 13,  3,  0,  9, 14,  8,  9,  6,
         4, 11,  2,  8,  1, 12, 11,  7, 10,  1, 13, 14,  7,  2,  8, 13,
        15,  6,  9, 15, 12,  0,  5,  9,  6, 10,  3,  4,  0,  5, 14,  3
    },
    {
        12, 10,  1, 15, 10,  4, 15,  2,  9,  7,  2, 12,  6,  9,  8,  5,
         0,  6, 13,  1,  3, 13,  4, 14, 14,  0,  7, 11,  5,  3, 11,  8,
         9,  4, 14,  3, 15,  2,  5, 12,  2,  9,  8,  5, 12, 15,  3, 10,
         7, 11,  0, 14,  4,  1, 10,  7,  1,  6, 13,  0, 11,  8,  6, 13
    },
    {
         4, 13, 11,  0,  2, 11, 14,  7, 15,  4,  0,  9,  8,  1, 13, 10,
         3, 14, 12,  3,  9,  5,  7, 12,  5,  2, 10, 15,  6,  8,  1,  6,
         1,  6,  4, 11, 11, 13, 13,  8, 12,  1,  3,  4,  7, 10, 14,  7,
        10,  9, 15,  5,  6,  0,  8, 15,  0, 14,  5,  2,  9,  3,  2, 12
    },
    {
        13,  1,  2, 15,  8, 13,  4,  8,  6, 10, 15,  3, 11,  7,  1,  4,
        10, 12,  9,  5,  3,  6, 14, 11,  5,  0,  0, 14, 12,  9,  7,  2,
         7,  2, 11,  1,  4, 14,  1,  7,  9,  4, 12, 10, 14,  8,  2, 13,
         0, 15,  6, 12, 10,  9, 13,  0, 15,  3,  3,  5,  5,  6,  8, 11
    }
};


#define GET_BIT(buf, bit) \
    (buf[(bit) / 8] & (0x80 >> ((bit) % 8)))

#define SET_BIT(buf, bit) \
    (buf[(bit) / 8] |= (0x80 >> ((bit) % 8)))

/*
 * Implement the initial and final permutation functions. 'permute_tab'
 * and 'dst' must have exactly len and len * 8 number of entries,
 * respectively, but src can be shorter (expansion function depends on this).
 * NOTE: this assumes that the permutation tables are defined as one-based
 * rather than 0-based arrays, since they're given that way in the
 * specification.
 */
static void permute(unsigned char *dst, const unsigned char *src,
                    const unsigned char *tab, size_t len)
{
    unsigned int i;

    memset(dst, 0, len);
    for (i = 0; i < len * 8; i++) {
        if (GET_BIT(src, tab[i] - 1))
            SET_BIT(dst, i);
    }
}

/*
 * Perform the left rotation operation on the key.
 * This is made fairly complex by the fact that the key is split into
 * two 28-bit halves, each of which has to be rotated independentlyi
 * (so the second rotation operation starts in the middle of byte 3).
 */
static void rol(unsigned char *buf)
{
    int carry_left, carry_right;

    carry_left = (buf[0] & 0x80) >> 3;

    buf[0] = (buf[0] << 1) | ((buf[1] & 0x80) >> 7);
    buf[1] = (buf[1] << 1) | ((buf[2] & 0x80) >> 7);
    buf[2] = (buf[2] << 1) | ((buf[3] & 0x80) >> 7);

    /* special handling for byte 3 */
    carry_right = (buf[3] & 0x08) >> 3;
    buf[3] = (((buf[3] << 1) |
              ((buf[4] & 0x80) >> 7)) & ~0x10) | carry_left;

    buf[4] = (buf[4] << 1) | ((buf[5] & 0x80) >> 7);
    buf[5] = (buf[5] << 1) | ((buf[6] & 0x80) >> 7);
    buf[6] = (buf[6] << 1) | carry_right;
}

static void ror(unsigned char *buf)
{
    int carry_left, carry_right;

    carry_right = (buf[6] & 0x01) << 3;

    buf[6] = (buf[6] >> 1) | ((buf[5] & 0x01) << 7);
    buf[5] = (buf[5] >> 1) | ((buf[4] & 0x01) << 7);
    buf[4] = (buf[4] >> 1) | ((buf[3] & 0x01) << 7);

    carry_left = (buf[3] & 0x10) << 3;
    buf[3] = (((buf[3] >> 1) |
              ((buf[2] & 0x01) << 7)) & ~0x08) | carry_right;

    buf[2] = (buf[2] >> 1) | ((buf[1] & 0x01) << 7);
    buf[1] = (buf[1] >> 1) | ((buf[0] & 0x01) << 7);
    buf[0] = (buf[0] >> 1) | carry_left;
}

static void memxor(unsigned char *dst, unsigned char *src,
                   size_t len)
{
    while (len--)
        *dst++ ^= *src++;
}

static void des_block_operate(unsigned char *dst, const unsigned char *src,
                              const unsigned char *key, int encrypt)
{
    unsigned char ip_block[CRY_DES_BLOCK_SIZE];
    unsigned char expansion_block[EXPANSION_BLOCK_SIZE];
    unsigned char substitution_block[CRY_DES_BLOCK_SIZE / 2];
    unsigned char pbox_target[CRY_DES_BLOCK_SIZE / 2];
    unsigned char recomb_box[CRY_DES_BLOCK_SIZE / 2];
    unsigned char pc1key[PC1_KEY_SIZE];
    unsigned char subkey[SUBKEY_SIZE];
    int round;

    /* Initial permutation */
    permute(ip_block, src, ip_tab, CRY_DES_BLOCK_SIZE);

    /* Key schedule computation */
    permute(pc1key, key, pc1_tab, PC1_KEY_SIZE);
    for (round = 0; round < 16; round++) {
        /*
         * "Feistel function" on the first half of the block in 'ip_block'
         * "Expansion". This permutation only looks at the first
         * four bytes (32 bits of ip_block); 16 of these are repeated
         * in "expansion_tab".
         */
        permute(expansion_block, ip_block + 4, exp_tab, 6);

        /* "Key mixing" */
        /* Rotate both halves of the initial key */
        if (encrypt) {
            rol(pc1key);
            if (!(round <= 1 || round == 8 || round == 15))
                /* Rotate twice except in rounds 1, 2, 9 and 16 */
                rol(pc1key);
        }

        permute(subkey, pc1key, pc2_tab, SUBKEY_SIZE);

        if (!encrypt) {
            ror(pc1key);
            if (!(round >= 14 || round == 7 || round == 0))
                /* Rotate twice except in rounds 1, 2, 9 & 16 */
                ror(pc1key);
        }

        memxor(expansion_block, subkey, 6);

        /*
         * Substitution;
         * "copy" from updated expansion block to ciphertext block
         */
        memset(substitution_block, 0, CRY_DES_BLOCK_SIZE / 2);
        substitution_block[0] =
            sbox[0][(expansion_block[0] & 0xFC) >> 2] << 4;
        substitution_block[0] |=
            sbox[1][(expansion_block[0] & 0x03) << 4 |
            (expansion_block[1] & 0xF0) >> 4];
        substitution_block[1] =
            sbox[2][(expansion_block[1] & 0x0F) << 2 |
            (expansion_block[2] & 0xC0) >> 6] << 4;
        substitution_block[1] |=
            sbox[3][(expansion_block[2] & 0x3F)];
        substitution_block[2] =
            sbox[4][(expansion_block[3] & 0xFC) >> 2] << 4;
        substitution_block[2] |=
            sbox[5][(expansion_block[3] & 0x03) << 4 |
            (expansion_block[4] & 0xF0) >> 4];
        substitution_block[3] =
            sbox[6][(expansion_block[4] & 0x0F) << 2 |
            (expansion_block[5] & 0xC0) >> 6] << 4;
        substitution_block[3] |=
            sbox[7][(expansion_block[5] & 0x3F)];

        /* Permutation */
        permute(pbox_target, substitution_block, p_tab, CRY_DES_BLOCK_SIZE / 2);

        /* Recombination. XOR the pbox with left half and then switch sides. */
        memcpy(recomb_box, ip_block, CRY_DES_BLOCK_SIZE / 2);
        memcpy(ip_block, ip_block + 4, CRY_DES_BLOCK_SIZE / 2);
        memxor(recomb_box, pbox_target, CRY_DES_BLOCK_SIZE / 2);
        memcpy(ip_block + 4, recomb_box, CRY_DES_BLOCK_SIZE / 2);
    }

    /* Swap one last time */
    memcpy(recomb_box, ip_block, CRY_DES_BLOCK_SIZE / 2);
    memcpy(ip_block, ip_block + 4, CRY_DES_BLOCK_SIZE / 2);
    memcpy(ip_block + 4, recomb_box, CRY_DES_BLOCK_SIZE / 2);

    /* Final permutation (undo initial permutation) */
    permute(dst, ip_block, fp_tab, CRY_DES_BLOCK_SIZE);
}

void cry_des_encrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size)
{
    while (size) {
        des_block_operate(dst, src, ctx->key, 1);
        if (ctx->keylen == 24) {
            des_block_operate(dst, dst, ctx->key + CRY_DES_BLOCK_SIZE, 0);
            des_block_operate(dst, dst, ctx->key + 2 * CRY_DES_BLOCK_SIZE, 1);
        }
        src  += CRY_DES_BLOCK_SIZE;
        dst  += CRY_DES_BLOCK_SIZE;
        size -= CRY_DES_BLOCK_SIZE;
    }
}

void cry_des_decrypt(cry_des_ctx *ctx, unsigned char *dst,
                     const unsigned char *src, size_t size)
{
    while (size) {
        if (ctx->keylen == 24) {
            des_block_operate(dst, src, ctx->key + 2 * CRY_DES_BLOCK_SIZE, 0);
            des_block_operate(dst, dst, ctx->key + CRY_DES_BLOCK_SIZE, 1);
            des_block_operate(dst, dst, ctx->key, 0);
        }
        else {
            des_block_operate(dst, src, ctx->key, 0);
        }
        src  += CRY_DES_BLOCK_SIZE;
        dst  += CRY_DES_BLOCK_SIZE;
        size -= CRY_DES_BLOCK_SIZE;
    }
}

void cry_des_key_set(cry_des_ctx *ctx, unsigned char *key,
                     size_t size)
{
    memset(ctx, 0, sizeof(*ctx));
    if (size > CRY_DES_BLOCK_SIZE*3)
        size = CRY_DES_BLOCK_SIZE*3;
    ctx->keylen = size;
    memcpy(ctx->key, key, size);
}
