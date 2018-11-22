#include <cry/ecp.h>

int cry_ecp_copy(cry_ecp *d, const cry_ecp *s)
{
    int res;

    if ((res = cry_mpi_copy(&d->x, &s->x)) == 0)
        res = cry_mpi_copy(&d->y, &s->y);
    return res;
}
