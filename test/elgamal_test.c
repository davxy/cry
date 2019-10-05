#include "test.h"
#include <cry/elgamal.h>


cry_elgamal_ctx g_elg;

#define STR_MAX    128

struct elg_param {
    unsigned int plen;
    unsigned int glen;
    unsigned char p[512];
    unsigned char g[512];
};

static void param_init(struct elg_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->plen = raw_init(par->p, sizeof(par->p), argv[0]);
    par->glen = raw_init(par->g, sizeof(par->g), argv[1]);
}


static void sign_verify(int argc, char *argv[])
{
    int res;
    struct elg_param par;
    cry_elgamal_sig sig;
    cry_mpi min, max;
    size_t msg_len;
    unsigned char msg_raw[512];

    ASSERT(argc == 2);

    param_init(&par, argc, argv);

    ASSERT_OK(cry_mpi_init_list(&min, &max, &sig.r, &sig.s, NULL));

    ASSERT_OK(cry_mpi_load_bin(&g_elg.p, par.p, par.plen));
    ASSERT_OK(cry_mpi_load_bin(&g_elg.g, par.g, par.glen));

    /* Random stuff */
    ASSERT_OK(cry_mpi_set_int(&min, 2));
    ASSERT_OK(cry_mpi_sub(&max, &g_elg.p, &min));
    do {
        ASSERT_OK(cry_mpi_rand_range(&g_elg.d, &max));
    } while (cry_mpi_cmp(&min, &g_elg.d) > 0);

    res = cry_mpi_set_int(&min, 1);
    res = cry_mpi_sub(&max, &g_elg.p, &min);
    res = cry_mpi_rand_range(&min, &max);
    msg_len = cry_mpi_count_bytes(&min);
    res = cry_mpi_store_bin(&min, msg_raw, msg_len, 0);

    /* Sign */
    res = cry_elgamal_sign(&g_elg, &sig, msg_raw, msg_len);

    /* Set the public key of the signer */
    res = cry_mpi_mod_exp(&g_elg.y, &g_elg.g, &g_elg.d, &g_elg.p);
    /* Verify */
    res = cry_elgamal_verify(&g_elg, &sig, msg_raw, msg_len);

    cry_mpi_clear_list(&min, &max, &sig.r, &sig.s, NULL);

    ASSERT(res == 0);
}

static void setup(void)
{
    cry_elgamal_init(&g_elg);
}

static void teardown(void)
{
    cry_elgamal_clear(&g_elg);
}


static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;

    setup();
    if (strcmp(test, "sign_verify") == 0)
        sign_verify(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
    teardown();
}

void elgamal_test(void)
{
    TRACE("* ElGamal Digital Signature\n");
    func_test("elgamal_test.data", dispatch);
    TRACE("\n");
}
