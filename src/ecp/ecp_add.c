#include <cry/ecp.h>

#define CHK(exp) do { if ((res = (exp)) != 0) goto e; } while (0)

int cry_ecp_add(cry_ecp *pr, const cry_ecp *p1, const cry_ecp *p2,
                const cry_ecp_grp *grp)
{
    int res;
    cry_mpi num, den, lam;
    cry_ecp r;

    /* Check if one of the two points is the infinity point */
    if (cry_ecp_is_zero(p1))
        return (pr != p2) ? cry_ecp_copy(pr, p2) : 0;
    if (cry_ecp_is_zero(p2))
        return (pr != p1) ? cry_ecp_copy(pr, p1) : 0;

    if ((res = cry_mpi_init_list(&num, &den, &lam,
            &r.x, &r.y, &r.z, (cry_mpi *) NULL)) != 0) {
        return res;
    }

    CHK(cry_mpi_sub(&num, &p2->y, &p1->y)); /* num = y2 - y1 */
    CHK(cry_mpi_mod(&num, &num, &grp->p));
    CHK(cry_mpi_sub(&den, &p2->x, &p1->x)); /* den = x2 - x1 */
    CHK(cry_mpi_mod(&den, &den, &grp->p));
    if (cry_mpi_is_zero(&den)) {
        if (cry_mpi_is_zero(&num))
            res = cry_ecp_dbl(pr, p1, grp);
        else
            cry_ecp_set_zero(pr);
        /*
         * Shall we check that p1.y+p2.y==0 (mod p) ?
         * Is guaranteed that p1.x==p2.x is sufficient condition to say that
         * p2 is the inverse of p1???
         */
        goto e;
    }
    CHK(cry_mpi_inv(&den, &den, &grp->p));  /* den^(-1) (mod p) */
    CHK(cry_mpi_mul(&lam, &num, &den));     /* lam = num / den */

    CHK(cry_mpi_sqr(&r.x, &lam));           /* x =  lam^2 */
    CHK(cry_mpi_sub(&r.x, &r.x, &p1->x));   /* x =  lam^2 - x1 */
    CHK(cry_mpi_sub(&r.x, &r.x, &p2->x));   /* x =  lam^2 - x1 - x2 */
    CHK(cry_mpi_mod(&r.x, &r.x, &grp->p));  /* x = (lam^2 - x1 - x2) % p */

    CHK(cry_mpi_sub(&r.y, &p1->x, &r.x));   /* y =   x1 - x */
    CHK(cry_mpi_mul(&r.y, &r.y, &lam));     /* y =  (x1 - x) * lam */
    CHK(cry_mpi_sub(&r.y, &r.y, &p1->y));   /* y =  (x1 - x) * lam - y1 */
    CHK(cry_mpi_mod(&r.y, &r.y, &grp->p));  /* y = ((x1 - x) * lam - y1) % p */

    CHK(cry_mpi_set_int(&r.z, 1));

    cry_ecp_swap(pr, &r);
e:  cry_mpi_clear_list(&num, &den, &lam, &r.x, &r.y, &r.z, (cry_mpi *) NULL);
    return res;
}
