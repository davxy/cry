#include "cry/ecp.h"

#define CHK(exp) do { if ((res = (exp)) != 0) goto e; } while (0)

int cry_ecp_add(cry_ecp *pr, const cry_ecp *p1, const cry_ecp *p2,
                const cry_mpi *p)
{
    int res;
    cry_mpi num, den, lam;
    cry_ecp r;

    if ((res = cry_mpi_init_list(&num, &den, &lam, &r.x, &r.y, NULL)) != 0)
        return res;

    CHK(cry_mpi_sub(&num, &p2->y, &p1->y)); /* num = y2 - y1 */
    CHK(cry_mpi_sub(&den, &p2->x, &p1->x)); /* den = x2 - x1 */
    CHK(cry_mpi_inv(&den, &den, p));        /* den^(-1) (mod p) */
    CHK(cry_mpi_mul(&lam, &num, &den));     /* lam = num / den */

    CHK(cry_mpi_sqr(&r.x, &lam));           /* x =  lam^2 */
    CHK(cry_mpi_sub(&r.x, &r.x, &p1->x));   /* x =  lam^2 - x1 */
    CHK(cry_mpi_sub(&r.x, &r.x, &p2->x));   /* x =  lam^2 - x1 - x2 */
    CHK(cry_mpi_mod(&r.x, &r.x, p));        /* x = (lam^2 - x1 - x2) % p */

    CHK(cry_mpi_sub(&r.y, &p1->x, &r.x));   /* y =   x1 - x */
    CHK(cry_mpi_mul(&r.y, &r.y, &lam));     /* y =  (x1 - x) * lam */
    CHK(cry_mpi_sub(&r.y, &r.y, &p1->y));   /* y =  (x1 - x) * lam - y1 */
    CHK(cry_mpi_mod(&r.y, &r.y, p));        /* y = ((x1 - x) * lam - y1) % p */

    cry_ecp_swap(pr, &r);
e:  cry_mpi_clear_list(&num, &den, &lam, &r.x, &r.y, NULL);
    return res;
}
