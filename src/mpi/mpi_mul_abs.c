/*
 * Copyright (c) 2013-2018, Davide Galassi. All rights reserved.
 *
 * This file is part of CRY software.
 *
 * CRY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CRY; if not, see <http://www.gnu/licenses/>.
 */

#include "mpi_pvt.h"
#include "misc.h"

#ifdef CRY_MPI_MUL_COMBA

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

static int mul_base(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    int res, ix, iy, iz, tx, ty, pa;
    cry_mpi_digit c0, c1, c2, *tmpx, *tmpy;
    cry_mpi tmp, *dst;
    size_t digs = a->used + b->used;

    c0 = c1 = c2 = 0;
    pa = CRY_MIN(digs, a->used + b->used);

    if (r == a || r == b) {
        if ((res = cry_mpi_init_size(&tmp, pa)) != 0)
            return res;
        dst = &tmp;
    } else {
        cry_mpi_grow(r, pa);
        dst = r;
    }
    cry_mpi_set_used(dst, pa);

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

#else /* !CRY_MPI_MUL_COMBA */

static int mul_base(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    cry_mpi t;
    int res, pa, pb, i, j;
    cry_mpi_digit u, tmpx, *tmpt, *tmpy;
    cry_mpi_dword dw;
    size_t digs = a->used + b->used;

    if ((res = cry_mpi_init_size(&t, digs)) != 0)
        return res;
    cry_mpi_set_used(&t, digs);

    /* compute the digits of the product directly */
    pa = a->used;
    /* iterate through every digit in the first operand */
    for (i = 0; i < pa; i++) {
        /* limit ourselves to making digs digits of output */
        pb = CRY_MIN(b->used, digs - i);
        if (pb < 0)
            break;
        /* copy of the digit to be used within the nested loop */
        tmpx = a->data[i];
        /* an alias for the destination shifted i places */
        tmpt = t.data + i;
        /* an alias for the digits of b */
        tmpy = b->data;

        /* set initial carry to zero */
        u = 0;
        /* compute the columns of the output and propagate the carry */
        for (j = 0; j < pb; j++) {
            /* compute the column as a cry_mpi_dword */
            dw = (cry_mpi_dword)*tmpt +
                 (cry_mpi_dword)tmpx * (cry_mpi_dword)(*tmpy++) +
                 (cry_mpi_dword)u;
            /* the new column is the lower part of the result */
            *tmpt++ = (cry_mpi_digit)(dw & (cry_mpi_digit)-1);
            /* get the carry digit from the result */
            u = (cry_mpi_digit)(dw >> CRY_MPI_DIGIT_BITS);
        }
        /* set the carry if it fits within the required digs */
        if (i + j < digs)
            *tmpt = u;
    }
    cry_mpi_adjust(&t);
    cry_mpi_swap(&t, r);
    cry_mpi_clear(&t);
    return 0;
}

#endif /* CRY_MPI_MUL_COMBA */

#ifdef CRY_MPI_MUL_KARATSUBA

#define KARATSUBA_CUTOFF    64

static int mul_karatsuba(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
    cry_mpi x0, x1, y0, y1, t1, x0y0, x1y1;
    int B, hB, res;

    /* minimum number of digits */
    B = CRY_MIN(a->used, b->used);
    /* divide by two */
    hB = B >> 1;
    B = hB << 1;

    /* init copy all the temporaries */
    if ((res = cry_mpi_init_size(&x0, hB)) < 0)
        goto e0;
    if ((res = cry_mpi_init_size(&x1, a->used - hB)) < 0)
        goto e1;
    if ((res = cry_mpi_init_size(&y0, hB)) < 0)
        goto e2;
    if ((res = cry_mpi_init_size(&y1, b->used - hB)) < 0)
        goto e3;

    if ((res = cry_mpi_init_size(&t1, B)) < 0)
        goto e4;
    if ((res = cry_mpi_init_size(&x0y0, B)) < 0)
        goto e5;
    if ((res = cry_mpi_init_size(&x1y1, B)) < 0)
        goto e6;

    /* Shift the digits */
    x0.used = y0.used = hB;
    x1.used = a->used - hB;
    y1.used = b->used - hB;

#if 1
    {
        register int x;
        register cry_mpi_digit *tmpa, *tmpb, *tmpx, *tmpy;

        /*
         * We copy the digits directly instead of using higher level functions
         * since we also need to shift digits.
         */
        tmpa = a->data;
        tmpb = b->data;

        tmpx = x0.data;
        tmpy = y0.data;

        /* TODO: use memcpy? */
        for (x = 0; x < hB; x++) {
            *tmpx++ = *tmpa++;
            *tmpy++ = *tmpb++;
        }

        tmpx = x1.data;
        for (x = hB; x < a->used; x++)
            *tmpx++ = *tmpa++;

        tmpy = y1.data;
        for (x = hB; x < b->used; x++)
            *tmpy++ = *tmpb++;

    }
#else
    int x = hB * sizeof(cry_mpi_digit);
    memcpy(x0.data, a->data, x);
    memcpy(x1.data, a->data + x, a->used * sizeof(cry_mpi_digit) - x);
    memcpy(y0.data, b->data, x);
    memcpy(y1.data, b->data + x, b->used * sizeof(cry_mpi_digit) - x);
#endif

    /*
     * Only need to clamp the lower words since by definition the upper
     * words x1/y1 must have a known number of digits
     */
    cry_mpi_adjust(&x0);
    cry_mpi_adjust(&y0);

    /*
     * Now calc the products x0y0 and x1y1.
     * After this x0 is no longer required, free temp [x0==t2]
     */
    if ((res = cry_mpi_mul(&x0y0, &x0, &y0)) < 0)
        goto e7;
    if ((res = cry_mpi_mul(&x1y1, &x1, &y1)) < 0)
        goto e7;

    /* Now calc x1+x0 and y1+y0 */
    if ((res = cry_mpi_add(&t1, &x1, &x0)) < 0)        /* t1 = x1 + x0 */
        goto e7;
    if ((res = cry_mpi_add(&x0, &y1, &y0)) < 0)     /* t2 = y1 + y0 */
        goto e7;

    if ((res = cry_mpi_mul(&t1, &x0, &t1)) < 0)      /* t1 = (x1+x0)*(y1+y0) */
        goto e7;

    /* Add x0y0 */
    if ((res = cry_mpi_add(&x0, &x0y0, &x1y1)) < 0)  /* t2 = x0y0 + x1y1 */
        goto e7;
    if ((res = cry_mpi_sub(&t1, &t1, &x0)) < 0)
        goto e7;                    /* t1 = (x1+x0)*(y1+y0) - (x0y0+x1y1) */

    /* Shift by hB */
    if ((res = cry_mpi_shld(&t1, hB)) < 0)
        goto e7;
    if ((res = cry_mpi_shld(&x1y1, B)) < 0)
        goto e7;

    if ((res = cry_mpi_add(&t1, &x0y0, &t1)) < 0)
        goto e7;
    if ((res = cry_mpi_add(r, &t1, &x1y1)) < 0)    /* r = x0y0 + t1 + x1y1 */
        goto e7;

e7: cry_mpi_clear(&x1y1);
e6: cry_mpi_clear(&x0y0);
e5: cry_mpi_clear(&t1);
e4: cry_mpi_clear(&y1);
e3: cry_mpi_clear(&y0);
e2: cry_mpi_clear(&x1);
e1: cry_mpi_clear(&x0);
e0: return res;
}

#endif /* CRY_MPI_MUL_KARATSUBA */

int cry_mpi_mul_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
#if defined(CRY_MPI_MUL_KARATSUBA)
    if (CRY_MIN(a->used, b->used) > KARATSUBA_CUTOFF)
        return mul_karatsuba(r, a, b);
#endif

    return mul_base(r, a, b);
}

