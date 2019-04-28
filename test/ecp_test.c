#include "test.h"
#include <cry/ecp.h>

/* y^2 = x^3 + 2x + 2 (mod 17)
 * Generator point = (5, 1)
 * Generator order = 19
 */
static void load_curve(cry_ecp_grp *ec)
{
    cry_mpi_init_int(&ec->a, 2);
    cry_mpi_init_int(&ec->b, 2);
    cry_mpi_init_int(&ec->p, 17);
    cry_mpi_init_int(&ec->n, 19);
    cry_mpi_init_int(&ec->g.x, 5);
    cry_mpi_init_int(&ec->g.y, 1);
    cry_mpi_init_int(&ec->g.z, 1);
}

void ecp_test(void)
{
#if 0
    cry_ecp_grp ec;
    cry_ecp p;

    load_curve(&ec);
    cry_ecp_init(&p);
    cry_ecp_dbl(&p, &ec.g, &ec);
    cry_mpi_print(&ec.g.x, 10);
    cry_mpi_print(&ec.g.y, 10);
    int i = 0;
    while (cry_mpi_cmp(&p.x, &ec.g.x) != 0 ||
           cry_mpi_cmp(&p.y, &ec.g.y) != 0) {
        printf("-----\n");
        cry_mpi_print(&p.x, 10);
        cry_mpi_print(&p.y, 10);
        if (++i == 19)
            printf("*\n");
        cry_ecp_add(&p, &p, &ec.g, &ec);
    }
#endif
}
