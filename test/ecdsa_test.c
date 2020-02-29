#include "test.h"
#include <cry/ecdsa.h>

#if 0
/* key */
static const unsigned char d_dat[] = {
    0xDC, 0x51, 0xD3, 0x86, 0x6A, 0x15, 0xBA, 0xCD,
    0xE3, 0x3D, 0x96, 0xF9, 0x92, 0xFC, 0xA9, 0x9D,
    0xA7, 0xE6, 0xEF, 0x09, 0x34, 0xE7, 0x09, 0x75,
    0x59, 0xC2, 0x7F, 0x16, 0x14, 0xC8, 0x8A, 0x7F
};

static const unsigned char sha[] = {
    0x97, 0x7f, 0x2b, 0x13, 0xa2, 0xc3, 0x46, 0x38,
    0x23, 0x35, 0x95, 0x6e, 0xa6, 0xc7, 0x5a, 0xf1,
    0xd0, 0x13, 0xad, 0xb4, 0x07, 0xa2, 0xa3, 0x30,
    0x20, 0xa8, 0x5f, 0x18, 0x78, 0x9f, 0xa1, 0x43,
};

void ecdsa_test(void)
{
    cry_ecdsa_ctx ecdsa;
    cry_ecdsa_signature sig;

    cry_mpi_init_list(&sig.r, &sig.s, NULL);

    /* Load a well known curve params */
    ASSERT_OK(cry_ecp_grp_load(&ecdsa.ec, CRY_ECP_GRP_SECP256R1));
    PRINT_MPI("p", &ecdsa.ec.p, 16);
    PRINT_MPI("a", &ecdsa.ec.a, 16);
    PRINT_MPI("b", &ecdsa.ec.b, 16);
    PRINT_MPI("g.x", &ecdsa.ec.g.x, 16);
    PRINT_MPI("g.y", &ecdsa.ec.g.y, 16);
    PRINT_MPI("n", &ecdsa.ec.n, 16);

    /* Generate new public key from private key w and point G */
//    cry_mpi_load_bin(&ecdsa.d, d_dat, sizeof(d_dat));
    ASSERT_OK(cry_ecp_init(&ecdsa.q));
    ASSERT_OK(cry_mpi_init(&ecdsa.d));
#if 0
    ASSERT_OK(cry_ecdsa_keygen(&ecdsa.ec, &ecdsa.d, &ecdsa.q));
#else
    cry_mpi_load_bin(&ecdsa.d, d_dat, sizeof(d_dat));
    cry_ecp_mul(&ecdsa.q, &ecdsa.ec.g, &ecdsa.d, &ecdsa.ec);
#endif
    PRINT_MPI("d", &ecdsa.d, 16);
    PRINT_MPI("q.x", &ecdsa.q.x, 16);
    PRINT_MPI("q.y", &ecdsa.q.y, 16);

    ASSERT_OK(cry_ecdsa_sign(&ecdsa, &sig, sha, 32));
    PRINT_MPI("r", &sig.r, 16);
    PRINT_MPI("s", &sig.s, 16);

    ASSERT_OK(cry_ecdsa_verify(&ecdsa, &sig, sha, 32));

    cry_mpi_clear_list(&sig.r, &sig.s, NULL);
    /* TODO: release ecdsa context data */
}
#else
void ecdsa_test(void)
{
    TRACE("* ECDSA (TODO)\n");
    TRACE("\n");
}
#endif
