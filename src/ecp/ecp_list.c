#include <cry/ecp.h>
#include <stdarg.h>
#include "misc.h"


int cry_ecp_init_list(cry_ecp *p, ...)
{
    int res;
    va_list args;

    va_start(args, p);
    res = cry_list_init((cry_list_elem_init_f)cry_ecp_init,
                        (cry_list_elem_clear_f)cry_ecp_clear, p, args);
    va_end(args);
    return res;
}

void cry_ecp_clear_list(cry_ecp *p, ...)
{
    va_list args;

    va_start(args, p);
    cry_list_clear((cry_list_elem_clear_f)cry_ecp_clear, p, args);
    va_end(args);
}
