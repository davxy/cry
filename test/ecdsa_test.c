#include "test.h"
#include <cry/ecdsa.h>

/* key */
static const unsigned char d_dat[] = {
    0xDC, 0x51, 0xD3, 0x86, 0x6A, 0x15, 0xBA, 0xCD,
    0xE3, 0x3D, 0x96, 0xF9, 0x92, 0xFC, 0xA9, 0x9D,
    0xA7, 0xE6, 0xEF, 0x09, 0x34, 0xE7, 0x09, 0x75,
    0x59, 0xC2, 0x7F, 0x16, 0x14, 0xC8, 0x8A, 0x7F
};

static const unsigned char *msg = "Hello World";

void ecdsa_test(void)
{
    cry_ecdsa_ctx ecdsa;
    cry_ecdsa_signature sign;

    ASSERT_OK(cry_mpi_init_list(&sign.r, &sign.s, NULL));

    /* Load a well known curve params */
    ASSERT_OK(cry_ec_init_nist_p256(&ecdsa.ec));

    /* Generate new public key from private key w and point G */
//    cry_mpi_load_bin(&ecdsa.d, d_dat, sizeof(d_dat));
    ASSERT_OK(cry_ecp_init(&ecdsa.q));
    ASSERT_OK(cry_mpi_init(&ecdsa.d));
    ASSERT_OK(cry_ecdsa_keygen(&ecdsa.ec, &ecdsa.d, &ecdsa.q));

    ASSERT_OK(cry_ecdsa_sign(&ecdsa, &sign, msg, strlen(msg)));
    PRINT_MPI("r", &sign.r, 16);
    PRINT_MPI("s", &sign.s, 16);

    ASSERT_OK(cry_ecdsa_verify(&ecdsa, &sign, msg, strlen(msg)));

    cry_mpi_clear_list(&sign.r, &sign.s, NULL);
    /* TODO: release ecdsa context data */
}

