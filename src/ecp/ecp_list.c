#include <cry/ecp.h>
#include <stdarg.h>

int cry_ecp_init_list(cry_ecp *mp, ...)
{
    int res = 0, n = 0;
    cry_ecp *curr = mp;
    va_list args;

    va_start(args, mp);
    while (curr != NULL) {
        if (cry_ecp_init(curr) == 0) {
            n++;
            curr = va_arg(args, cry_ecp *);
        } else {
            /* on error undo all the initialized variables */
            va_end(args); /* end the current list */
            curr = mp;
            va_start(args, mp);
            while (n--) {
                cry_ecp_clear(curr);
                curr = va_arg(args, cry_ecp *);
            }
            res = -1;
            break;
        }
    }
    va_end(args);
    return res;
}

void cry_ecp_clear_list(cry_ecp *mp, ...)
{
    cry_ecp *curr = mp;
    va_list args;

    va_start(args, mp);
    while (curr != NULL) {
        cry_ecp_clear(curr);
        curr = va_arg(args, cry_ecp *);
    }
    va_end(args);
}
