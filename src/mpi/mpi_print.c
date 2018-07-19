#include "cry/mpi.h"
#include <stdio.h>
#include <stdlib.h>

void cry_mpi_print(const cry_mpi *a, unsigned int radix)
{
    char *s;
    size_t n = BUFSIZ;
    char fallback[16];
    int ret;

    if ((s = malloc(BUFSIZ)) == NULL) {
        s = fallback;
        n = sizeof(fallback);
    }

    if ((ret = cry_mpi_store_str(a, radix, s, n)) == 0)
        printf("%s\n", s);
    else
        printf("cry_mpi_print unexpected error (%d)\n", ret);

    if (s != fallback)
        free(s);
}

