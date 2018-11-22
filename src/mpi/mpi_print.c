#include <cry/mpi.h>
#include <stdio.h>
#include <stdlib.h>

void cry_mpi_print(const cry_mpi *a, unsigned int radix)
{
    char *s;
    size_t len;
    int ret;

    len = 2*cry_mpi_count_bytes(a) + 2; /* space for NULL and sign */
    if ((s = malloc(len)) == NULL) {
        fprintf(stderr, "cry_mpi_print: out of memory error\n");
        return;
    }

    if ((ret = cry_mpi_store_str(a, radix, s)) == 0)
        printf("%s\n", s);
    else
        fprintf(stderr, "cry_mpi_print unexpected error (%d)\n", ret);
    free(s);
}
