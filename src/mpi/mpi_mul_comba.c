#include "mpi_pvt.h"
#include "misc.h"


#if defined(CRY_ARCH_X86)

#define MULADD(i, j) asm(                           \
     "movl  %6, %%eax    \n\t"                      \
     "mull  %7           \n\t"                      \
     "addl  %%eax, %0    \n\t"                      \
     "adcl  %%edx, %1    \n\t"                      \
     "adcl  $0, %2       \n\t"                      \
     : "=r"(c0), "=r"(c1), "=r"(c2)                 \
     : "0"(c0), "1"(c1), "2"(c2), "m"(i), "m"(j)    \
     : "%eax","%edx","cc")

#elif defined(CRY_ARCH_X86_64)

#define MULADD(i, j) asm(                           \
     "movq  %6, %%rax    \n\t"                      \
     "mulq  %7           \n\t"                      \
     "addq  %%rax, %0    \n\t"                      \
     "adcq  %%rdx, %1    \n\t"                      \
     "adcq  $0, %2       \n\t"                      \
     : "=r"(c0), "=r"(c1), "=r"(c2)                 \
     : "0"(c0), "1"(c1), "2"(c2), "g"(i), "g"(j)    \
     : "%rax", "%rdx", "cc")

#elif defined(CRY_ARCH_ARM)

#define MULADD(i, j) asm(                           \
    "umull  r0, r1, %6, %7  \n\t"                   \
    "adds   %0, %0, r0      \n\t"                   \
    "adcs   %1, %1, r1      \n\t"                   \
    "adc    %2, %2, #0      \n\t"                   \
    : "=r"(c0), "=r"(c1), "=r"(c2)                  \
    : "0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j)     \
    : "r0", "r1", "cc")

#else /* ISO C code */

#define MULADD(i, j) do {                           \
   cry_mpi_dword t;                                 \
   t = (cry_mpi_dword)c0 + ((cry_mpi_dword)(i)) * ((cry_mpi_dword)(j)); \
   c0 = t;                                          \
   t = (cry_mpi_dword)c1 + (t >> CRY_MPI_DIGIT_BITS); \
   c1 = t;                                          \
   c2 += t >> CRY_MPI_DIGIT_BITS;                   \
   } while (0);

#endif

int cry_mpi_mul_comba(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int res, ix, iy, iz, tx, ty, pa;
    cry_mpi_digit c0, c1, c2, *tmpx, *tmpy;
    cry_mpi tmp, *dst;

    if (cry_mpi_is_zero(a) || cry_mpi_is_zero(b)) {
        cry_mpi_zero(r);
        return 0;
    }

    pa = a->used + b->used;
    if (r == a || r == b) {
        if ((res = cry_mpi_init_size(&tmp, pa)) != 0)
            return res;
        dst = &tmp;
    } else {
        cry_mpi_grow(r, pa);
        dst = r;
    }
    cry_mpi_set_used(dst, pa);

    c1 = c2 = 0;
    for (ix = 0; ix < pa; ix++) {

        /* get offsets into the two bignums */
        ty = CRY_MIN(ix, b->used - 1);
        tx = ix - ty;

        /* setup temporary aliases */
        tmpx = a->data + tx;
        tmpy = b->data + ty;

        /*
         * this is the number of times the loop will iterate, essentially its
         * while (tx++ < a->used && ty-- >= 0
         */
        iy = CRY_MIN(a->used - tx, ty + 1);

        /* shift accumulator right */
        c0 = c1;
        c1 = c2;
        c2 = 0;

        /* execute loop */
        for (iz = 0; iz < iy; iz++) {
            MULADD(*tmpx, *tmpy);
            tmpx++;
            tmpy--;
        }

        /* store term */
        dst->data[ix] = c0;
    }

    if (r != dst) {
        cry_mpi_swap(r, dst);
        cry_mpi_clear(dst);
    }
    cry_mpi_adjust(r);
    return 0;
}
