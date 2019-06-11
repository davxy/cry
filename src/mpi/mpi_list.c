#include "mpi_pvt.h"
#include <stdarg.h>

int cry_mpi_init_list(cry_mpi *a, ...)
{
    int res;
    va_list args;

    va_start(args, a);
    res = cry_list_init((cry_list_elem_init_f)cry_mpi_init,
                        (cry_list_elem_clear_f)cry_mpi_clear, a, args);
    va_end(args);
    return res;
}

void cry_mpi_clear_list(cry_mpi *a, ...)
{
    va_list args;

    va_start(args, a);
    cry_list_clear((cry_list_elem_clear_f)cry_mpi_clear, a, args);
    va_end(args);
}
