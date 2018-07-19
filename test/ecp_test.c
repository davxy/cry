#include "test.h"
#include <cry/ecp.h>
#include <cry/ec.h>

static cry_ec ec;

#define P1X "2"
#define P1Y "3"
#define P2X "4"
#define P2Y "5"

void ecp_add(void)
{
    cry_ecp p1, p2, pr;

    cry_ecp_init(&p1);
    cry_ecp_init(&p2);
    cry_ecp_init(&pr);

    ASSERT_OK(cry_mpi_init_str(&p1.x, 16, P1X));
    ASSERT_OK(cry_mpi_init_str(&p1.y, 16, P1Y));
    ASSERT_OK(cry_mpi_init_str(&p2.x, 16, P2X));
    ASSERT_OK(cry_mpi_init_str(&p2.y, 16, P2Y));
    ASSERT_OK(cry_ecp_add(&pr, &p1, &p2, &ec.p));

    cry_ecp_clear(&p1);
    cry_ecp_clear(&p2);
    cry_ecp_clear(&pr);
}

void ecp_test(void)
{
    ASSERT_OK(cry_ec_init_nist_p256(&ec));
    RUN(ecp_add);
}

