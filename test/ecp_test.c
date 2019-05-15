#include "test.h"
#include <cry/ecp.h>

/*
 * y^2 = x^3 + 2x + 2 (mod 17)
 *
 *  Generator point = (5, 1)
 *  Generator order = 19
 */
static void load_simple_curve(cry_ecp_grp *ec)
{
    cry_mpi_init_int(&ec->a, 2);
    cry_mpi_init_int(&ec->b, 2);
    cry_mpi_init_int(&ec->p, 17);
    cry_mpi_init_int(&ec->n, 19);
    cry_mpi_init_int(&ec->g.x, 5);
    cry_mpi_init_int(&ec->g.y, 1);
    cry_mpi_init_int(&ec->g.z, 1);
}

/* Check that a point is on the curve */
static void point_check(const cry_ecp *p, const cry_ecp_grp *grp)
{
    /* Check thay y^2 = x^3 + ax + b (mod p) */
    cry_mpi v, t;

    if (cry_ecp_is_zero(p))
        return;
    cry_mpi_init_list(&v, &t, NULL);
    cry_mpi_sqr(&t, &p->x); /* x^2 */
    cry_mpi_mul(&t, &t, &p->x);         /* t = x^3 */
    cry_mpi_mul(&v, &grp->a, &p->x);    /* v = ax */
    cry_mpi_add(&v, &v, &grp->b);       /* v = ax + b */
    cry_mpi_add(&v, &v, &t);            /* v = x^3 + ax + b */
    cry_mpi_sqr(&t, &p->y);             /* t = y^2 */
    cry_mpi_sub(&v, &v, &t);
    cry_mpi_mod(&v, &v, &grp->p);
    ASSERT(cry_mpi_is_zero(&v));
    cry_mpi_clear_list(&v, &t, NULL);
}

static void add_test(void)
{
    cry_ecp_grp ec;
    cry_ecp p;
    int i = 0;

    load_simple_curve(&ec);
    cry_ecp_init(&p);
    do {
        //printf("n = %d\n", i);
        //cry_mpi_print(&p.x, 10);
        //cry_mpi_print(&p.y, 10);
        //cry_mpi_print(&p.z, 10);
        point_check(&p, &ec);
        //printf("--------------------\n");
        cry_ecp_add(&p, &p, &ec.g, &ec);
        i++;
    } while (!cry_ecp_is_zero(&p));
}

static void mul_test(void)
{
    cry_ecp_grp ec;
    cry_ecp p;
    cry_mpi v;

    load_simple_curve(&ec);
    cry_ecp_init_int(&p, 9, 16);    /* p = 5g = (9, 16) */
    cry_mpi_init_int(&v, 2);
    point_check(&p, &ec);
    cry_ecp_mul(&p, &p, &v, &ec);   /* 2p = 10g = (7,11) */
    point_check(&p, &ec);
}

static void secp192r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP192R1);

    ASSERT(res == 0);
}

static void secp224r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP224R1);

    ASSERT(res == 0);
}

static void secp256r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP256R1);

    ASSERT(res == 0);
}

static void secp384r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP384R1);

    ASSERT(res == 0);
}

static void secp521r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP521R1);

    ASSERT(res == 0);
}

static void secp192k1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP192K1);

    ASSERT(res == 0);
}

static void secp224k1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP224K1);

    ASSERT(res == 0);
}

static void secp256k1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_SECP256K1);

    ASSERT(res == 0);
}

static void bp256r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_BP256R1);

    ASSERT(res == 0);
}

static void bp384r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_BP384R1);

    ASSERT(res == 0);
}

static void bp512r1_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, CRY_ECP_GRP_BP512R1);

    ASSERT(res == 0);
}

static void bad_curve_load_test(void)
{
    int res;
    cry_ecp_grp grp;

    res = cry_ecp_grp_load(&grp, 0xFFFF);

    ASSERT(res != 0);
}

static void curve_params_load(void)
{
    RUN(secp192r1_load_test);
    RUN(secp224r1_load_test);
    RUN(secp256r1_load_test);
    RUN(secp384r1_load_test);
    RUN(secp521r1_load_test);
    RUN(secp192k1_load_test);
    RUN(secp224k1_load_test);
    RUN(secp256k1_load_test);
    RUN(bp256r1_load_test);
    RUN(bp384r1_load_test);
    RUN(bp512r1_load_test);
    RUN(bad_curve_load_test);
}

void ecp_test(void)
{
    printf("* ECP load curves parameters\n");
    curve_params_load();
    add_test();
    mul_test();
}
