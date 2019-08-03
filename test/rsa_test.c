#include "test.h"
#include <cry/rsa.h>
#include <cry/prng.h>

/* Just for... coverage :-) */
#define KEYGEN_BITS 256

static void keygen(void)
{
    cry_rsa_ctx rsa;

    /* Seed the PRNG to make results predictable */
    cry_prng_aes_init(NULL, 0);

    ASSERT_OK(cry_rsa_keygen(&rsa, KEYGEN_BITS));

    cry_mpi_clear_list(&rsa.n, &rsa.e, &rsa.d, NULL);
}

static const unsigned char modulus[] = {
    0xC4, 0xF8, 0xE9, 0xE1, 0x5D, 0xCA, 0xDF, 0x2B,
    0x96, 0xC7, 0x63, 0xD9, 0x81, 0x00, 0x6A, 0x64,
    0x4F, 0xFB, 0x44, 0x15, 0x03, 0x0A, 0x16, 0xED,
    0x12, 0x83, 0x88, 0x33, 0x40, 0xF2, 0xAA, 0x0E,
    0x2B, 0xE2, 0xBE, 0x8F, 0xA6, 0x01, 0x50, 0xB9,
    0x04, 0x69, 0x65, 0x83, 0x7C, 0x3E, 0x7D, 0x15,
    0x1B, 0x7D, 0xE2, 0x37, 0xEB, 0xB9, 0x57, 0xC2,
    0x06, 0x63, 0x89, 0x82, 0x50, 0x70, 0x3B, 0x3F
};

static const unsigned char private[] = {
    0x8a, 0x7e, 0x79, 0xf3, 0xfb, 0xfe, 0xa8, 0xeb,
    0xfd, 0x18, 0x35, 0x1c, 0xb9, 0x97, 0x91, 0x36,
    0xf7, 0x05, 0xb4, 0xd9, 0x11, 0x4a, 0x06, 0xd4,
    0xaa, 0x2f, 0xd1, 0x94, 0x38, 0x16, 0x67, 0x7a,
    0x53, 0x74, 0x66, 0x18, 0x46, 0xa3, 0x0c, 0x45,
    0xb3, 0x0a, 0x02, 0x4b, 0x4d, 0x22, 0xb1, 0x5a,
    0xb3, 0x23, 0x62, 0x2b, 0x2d, 0xe4, 0x7b, 0xa2,
    0x91, 0x15, 0xf0, 0x6e, 0xe4, 0x2c, 0x41
};

static const unsigned char public[] = {
    0x01, 0x00, 0x01
};

static const unsigned char plain_text[] = {
    0x09
};

/* Signature for RSA PKCS #1 v15 */
static const unsigned char sign[] = {
    0x26, 0x78, 0xbe, 0x19, 0x97, 0x95, 0x2e, 0x0a,
    0xed, 0xb2, 0xc6, 0xe6, 0xdb, 0xbd, 0x07, 0x7d,
    0x92, 0x1f, 0xca, 0xc0, 0x74, 0x00, 0xe5, 0x6a,
    0x66, 0x8d, 0x95, 0x92, 0xf2, 0xad, 0xc8, 0x3b,
    0x06, 0xda, 0x29, 0x78, 0x1f, 0x48, 0x87, 0xe2,
    0x08, 0x40, 0x03, 0x29, 0x32, 0x78, 0xa9, 0xc6,
    0xee, 0xf9, 0x22, 0x28, 0xce, 0x9e, 0x37, 0x97,
    0x74, 0x46, 0x88, 0x96, 0xf6, 0x24, 0x02, 0xa0
};

#define PLAIN_LEN sizeof(plain_text)
#define SIGN_LEN sizeof(sign)
#define CIPHER_LEN SIGN_LEN

static void encrypt_decrypt(void)
{
    cry_rsa_ctx rsa;
    size_t outlen;
    unsigned char *cipher_buf;
    unsigned char *plain_buf;

    cry_rsa_init(&rsa, CRY_RSA_PADDING_PKCS_V15);
    cry_mpi_init_bin(&rsa.n, modulus, sizeof(modulus));
    cry_mpi_init_bin(&rsa.e, public, sizeof(public));
    cry_mpi_init_bin(&rsa.d, private, sizeof(private));

    ASSERT_OK(cry_rsa_encrypt(&rsa, &cipher_buf, &outlen,
                              plain_text, PLAIN_LEN));
    if (cipher_buf) {
        ASSERT_EQ(outlen, CIPHER_LEN);

        ASSERT_OK(cry_rsa_decrypt(&rsa, &plain_buf, &outlen,
                                  cipher_buf, outlen));
        if (plain_buf) {
            ASSERT_EQ(outlen, PLAIN_LEN);
            ASSERT_EQ_BUF(plain_buf, plain_text, outlen);
            free(plain_buf);
        }
        free(cipher_buf);
    }
    cry_mpi_clear_list(&rsa.n, &rsa.e, &rsa.d, NULL);
}


static void sign_verify(void)
{
    cry_rsa_ctx rsa;
    size_t siglen;
    unsigned char *sig_buf;

    cry_rsa_init(&rsa, CRY_RSA_PADDING_PKCS_V15);
    cry_mpi_init_bin(&rsa.n, modulus, sizeof(modulus));
    cry_mpi_init_bin(&rsa.e, public, sizeof(public));
    cry_mpi_init_bin(&rsa.d, private, sizeof(private));

    ASSERT_OK(cry_rsa_sign(&rsa, &sig_buf, &siglen,
                           plain_text, PLAIN_LEN));
    if (sig_buf) {
        ASSERT_EQ(siglen, CIPHER_LEN);
        ASSERT_EQ_BUF(sig_buf, sign, siglen);

        ASSERT_OK(cry_rsa_verify(&rsa, sig_buf, siglen,
                                 plain_text, PLAIN_LEN));
        free(sig_buf);
    }
    cry_mpi_clear_list(&rsa.n, &rsa.e, &rsa.d, NULL);
}

struct rsa_param {
    size_t mlen;
    size_t elen;
    size_t clrlen;
    size_t ciplen;
    unsigned char *mraw;
    unsigned char *eraw;
    unsigned char *clrraw;
    unsigned char *cipraw;
};

/*
 * Params:
 * p0 : M
 * p1 : E
 * p2 : cleartext
 * p3 : ciphertext
 */
static void rsa_param_init(struct rsa_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));

    ASSERT(argc == 4);

    par->mlen = strlen(argv[0]) >> 1;
    par->elen = strlen(argv[1]) >> 1;
    par->clrlen = strlen(argv[2]) >> 1;
    par->ciplen = strlen(argv[3]) >> 1;
    par->mraw = malloc(par->mlen + par->elen + par->clrlen + par->ciplen);
    par->eraw = par->mraw + par->mlen;
    par->clrraw = par->eraw + par->elen;
    par->cipraw = par->clrraw + par->clrlen;

    raw_init(par->mraw, par->mlen, argv[0]);
    raw_init(par->eraw, par->elen, argv[1]);
    raw_init(par->clrraw, par->clrlen, argv[2]);
    raw_init(par->cipraw, par->ciplen, argv[3]);
}

static void rsa_pkcs1_encrypt(int argc, char *argv[])
{
    struct rsa_param par;
    cry_rsa_ctx rsa;
    size_t outlen;
    unsigned char *cipher_buf;

    cry_prng_aes_init(NULL, 0);
    rsa_param_init(&par, argc, argv);

    cry_rsa_init(&rsa, CRY_RSA_PADDING_PKCS_V15);
    cry_mpi_init_bin(&rsa.n, par.mraw, par.mlen);
    cry_mpi_init_bin(&rsa.e, par.eraw, par.elen);

    ASSERT_OK(cry_rsa_encrypt(&rsa, &cipher_buf, &outlen,
                              par.clrraw, par.clrlen));

    free(par.mraw);
    free(cipher_buf);
    cry_mpi_clear_list(&rsa.n, &rsa.e, NULL);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;
    if (strcmp(test, "rsa_pkcs1_encrypt") == 0)
        rsa_pkcs1_encrypt(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void rsa_test(void)
{
    TRACE("* RSA Test\n");
    run("Keygen 512", keygen, NULL, NULL);
    run("Encrypt-Decrypt", encrypt_decrypt, NULL, NULL);
    run("Sign-Verify", sign_verify, NULL, NULL);
    func_test("rsa_test.data", dispatch);
    TRACE("\n");
}
