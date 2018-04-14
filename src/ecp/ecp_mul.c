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

#include "cry/ecp.h"
#include "mpi/mpi_pvt.h" /* CRY_MPI_DIGIT_BITS */
#include <stdlib.h>      /* malloc() */


#define CHK(exp) do { if ((res = (exp)) != 0) goto e; } while (0)


#if defined(CRY_ECP_MUL_SLIDING_WIN)

#define WINPTS     8

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_mpi *a, const cry_mpi *p)
{
    int res, i, j, w, paf = 1;
    struct cry_ecp r, *win = NULL;

    if ((res = cry_ecp_init(&r)) != 0)
        return res;

    /*
     * BEGIN window generation
     */
    win = malloc(sizeof(cry_ecp) * WINPTS);
    if (win == NULL)
        goto e0;
    for (i = 0; i < WINPTS; i++) {
        if ((res = cry_ecp_init(&win[i])) != 0) {
            while (i-- > 0) /* rollback */
                cry_ecp_clear(&win[i]);
            goto e1;
        }
    }
    CHK(cry_ecp_copy(&win[0], p1));             /* 1P (tmp) */
    CHK(cry_ecp_dbl(&win[0], &win[0], a, p));   /* 2P (tmp) */
    CHK(cry_ecp_dbl(&win[0], &win[0], a, p));   /* 4P (tmp) */ 
    CHK(cry_ecp_dbl(&win[0], &win[0], a, p));   /* 8P */
    CHK(cry_ecp_add(&win[1], &win[0], p1, p));  /* 9P */
    CHK(cry_ecp_add(&win[2], &win[1], p1, p));  /* 10P */ 
    CHK(cry_ecp_add(&win[3], &win[2], p1, p));  /* 11P */
    CHK(cry_ecp_add(&win[4], &win[3], p1, p));  /* 12P */
    CHK(cry_ecp_add(&win[5], &win[4], p1, p));  /* 13P */
    CHK(cry_ecp_add(&win[6], &win[5], p1, p));  /* 14P */
    CHK(cry_ecp_add(&win[7], &win[6], p1, p));  /* 15P */
    /*
     * END window generation
     */

    i = k->used;
    while (i > 0) {
        i--;
        j = CRY_MPI_DIGIT_BITS;
        while (j > 0) {
            j--;
            if (!paf)
                CHK(cry_ecp_dbl(&r, &r, a, p));

            if (!(k->data[i] & ((cry_mpi_digit)1 << j)))
                continue;

            if (i != 0 || j >= 3) { /* TODO: maybe j >= 3 is sufficient */
                w = 1;
                while (w < 8) {
                    if (j == 0) {
                        i--;
                        j = CRY_MPI_DIGIT_BITS;
                    }
                    j--;
                    if (!paf) {
                        CHK(cry_ecp_dbl(&r, &r, a, p));
                    }
                    w <<= 1;
                    if (k->data[i] & ((cry_mpi_digit)1 << j))
                        w |= 1;
                }
                w &= 0x7; /* -8 */
                // if (w >= 8)
                //    w -= 8;
                if (!paf) {
                    CHK(cry_ecp_add(&r, &r, &win[w], p));
                } else {
                    paf = 0; /* First addition */
                    CHK(cry_ecp_copy(&r, &win[w]));
                }
            } else {
                /* special case */
                if (!paf) {
                    CHK(cry_ecp_add(&r, &r, p1, p));
                } else {
                    paf = 0; /* First addition */
                    CHK(cry_ecp_copy(&r, p1));
                }
            }
        }
    }
    /* Success */
    cry_ecp_swap(pr, &r);

e:  for (i = 0; i < WINPTS; i++)
        cry_ecp_clear(&win[i]);
e1: free(win);
e0: cry_ecp_clear(&r);
    return res;
}

#elif defined(CRY_ECP_MUL_WIN)

#define WINSIZ      4
#define WINPTS      (1 << WINSIZ)
#define WINMSK      (WINPTS-1)

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_mpi *a, const cry_mpi *p)
{
    int res, i, j, w, paf = 1;
    struct cry_ecp r, *win = NULL;
    cry_mpi_digit msk;

    if ((res = cry_ecp_init(&r)) != 0)
        return res;

    /*
     * BEGIN window generation
     */
    win = malloc(sizeof(cry_ecp) * WINPTS);
    if (win == NULL)
        goto e0;
    for (i = 0; i < WINPTS; i++) {
        if ((res = cry_ecp_init(&win[i])) != 0) {  // 0P
            while (i-- > 0) /* rollback */
                cry_ecp_clear(&win[i]);
            goto e1;
        }
    }
    CHK(cry_ecp_copy(&win[1], p1));                     /* 1P */
    CHK(cry_ecp_dbl(&win[2],  &win[1], a, p));          /* 2P */
    CHK(cry_ecp_add(&win[3],  &win[2], &win[1], p));    /* 3P */
    CHK(cry_ecp_dbl(&win[4],  &win[2], a, p));          /* 4P */
    CHK(cry_ecp_add(&win[5],  &win[4], &win[1], p));    /* 5P */
    CHK(cry_ecp_add(&win[6],  &win[5], &win[1], p));    /* 6P */
    CHK(cry_ecp_add(&win[7],  &win[6], &win[1], p));    /* 7P */
    CHK(cry_ecp_dbl(&win[8],  &win[4], a, p));          /* 8P */
    CHK(cry_ecp_add(&win[9],  &win[8], &win[1], p));    /* 9P */
    CHK(cry_ecp_add(&win[10], &win[9], &win[1], p));    /* 10P */
    CHK(cry_ecp_add(&win[11], &win[10], &win[1], p));   /* 11P */
    CHK(cry_ecp_add(&win[12], &win[11], &win[1], p));   /* 12P */
    CHK(cry_ecp_add(&win[13], &win[12], &win[1], p));   /* 13P */
    CHK(cry_ecp_add(&win[14], &win[13], &win[1], p));   /* 14P */
    CHK(cry_ecp_add(&win[15], &win[14], &win[1], p));   /* 15P */
    /*
     * END window generation
     */

    i = k->used;
    while (i > 0) {
        i--;
        w = (CRY_MPI_DIGIT_BITS - WINSIZ);
        msk = ((cry_mpi_digit)WINMSK) << w;
        j = CRY_MPI_DIGIT_BITS;
        while (j > 0) {
            if (!paf) {
                for (w = 0; w < WINSIZ; w++)
                    CHK(cry_ecp_dbl(&r, &r, a, p));
            }

            j -= WINSIZ;
            w = (k->data[i] & msk) >> j;
            if (w > 0) {
                if (!paf) {
                    CHK(cry_ecp_add(&r, &r, &win[w], p));
                } else {
                    paf = 0; /* First addition */
                    CHK(cry_ecp_copy(&r, &win[w]));
                }
            }
            msk >>= WINSIZ;
        }
    }
    /* Success */
    cry_ecp_swap(pr, &r);

e:  for (i = 0; i < WINPTS; i++)
        cry_ecp_clear(&win[i]);
e1: free(win);
e0: cry_ecp_clear(&r);
    return res;
}

#else /* !CRY_ECP_MUL_WIN */

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_mpi *a, const cry_mpi *p)
{
    int i, j, res, paf = 1;
    struct cry_ecp dp, r;

    if ((res = cry_mpi_init_list(&dp.x, &dp.y, &r.x, &r.y, NULL)) != 0)
        return res;

    if ((res = cry_mpi_copy(&dp.x, &p1->x)) != 0)
        goto e;
    if ((res = cry_mpi_copy(&dp.y, &p1->y)) != 0)
        goto e;

    for (i = 0; i < k->used; i++) {
        for (j = 0; j < CRY_MPI_DIGIT_BITS; j++) {
            if (k->data[i] & (1UL << j)) {
                if (!paf) {
                    if ((res = cry_ecp_add(&r, &r, &dp, p)) != 0)
                        goto e;
                } else {
                    paf = 0; /* First addition */
                    if ((res = cry_mpi_copy(&r.x, &dp.x)) != 0)
                        goto e;
                    if ((res = cry_mpi_copy(&r.y, &dp.y)) != 0)
                        goto e;
                }
            }
            if ((res = cry_ecp_dbl(&dp, &dp, a, p)) != 0)
                goto e;
        }
    }

    cry_ecp_swap(pr, &r);
e:  cry_mpi_clear_list(&dp.x, &dp.y, &r.x, &r.y, NULL);
    return res;
}

#endif /* CRY_ECP_MUL_WIN */

