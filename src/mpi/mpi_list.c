#include "mpi_pvt.h"
#include <stdarg.h>

int cry_mpi_init_list(cry_mpi *mp, ...)
{
    int res = 0, n = 0;
    cry_mpi *curr = mp;
    va_list args;

    va_start(args, mp);
    while (curr != NULL) {
        if (cry_mpi_init(curr) == 0) {
            n++;
            curr = va_arg(args, cry_mpi *);
        } else {
            /* on error undo all the initialized variables */
            va_end(args); /* end the current list */
            curr = mp;
            va_start(args, mp);
            while (n--) {
                cry_mpi_clear(curr);
                curr = va_arg(args, cry_mpi *);
            }
            res = -1;
            break;
        }
    }
    va_end(args);
    return res;
}

void cry_mpi_clear_list(cry_mpi *mp, ...)
{
    cry_mpi *curr = mp;
    va_list args;

    va_start(args, mp);
    while (curr != NULL) {
        cry_mpi_clear(curr);
        curr = va_arg(args, cry_mpi *);
    }
    va_end(args);
}
