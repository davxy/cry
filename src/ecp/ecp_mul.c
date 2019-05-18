#include "misc.h"
#include <cry/config.h>
#include <cry/ecp.h>
#include <stdlib.h>      /* malloc() */

#define CHK(exp) CRY_CHK(res = (exp), e)

#if defined(CRY_ECP_MUL_SLIDING_WIN)

#define WINPTS     8

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_ecp_grp *grp)
{
    int res, i, j, w, paf = 1;
    struct cry_ecp r, *win = NULL;

    if (cry_ecp_is_zero(p1))
        return (pr != p1) ? cry_ecp_copy(pr, p1) : 0;

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
    CHK(cry_ecp_copy(&win[0], p1));                 /* 1P (tmp) */
    CHK(cry_ecp_dbl(&win[0], &win[0], grp));        /* 2P (tmp) */
    CHK(cry_ecp_dbl(&win[0], &win[0], grp));        /* 4P (tmp) */
    CHK(cry_ecp_dbl(&win[0], &win[0], grp));        /* 8P */
    CHK(cry_ecp_add(&win[1], &win[0], p1, grp));    /* 9P */
    CHK(cry_ecp_add(&win[2], &win[1], p1, grp));    /* 10P */
    CHK(cry_ecp_add(&win[3], &win[2], p1, grp));    /* 11P */
    CHK(cry_ecp_add(&win[4], &win[3], p1, grp));    /* 12P */
    CHK(cry_ecp_add(&win[5], &win[4], p1, grp));    /* 13P */
    CHK(cry_ecp_add(&win[6], &win[5], p1, grp));    /* 14P */
    CHK(cry_ecp_add(&win[7], &win[6], p1, grp));    /* 15P */
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
                CHK(cry_ecp_dbl(&r, &r, grp));

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
                    if (!paf)
                        CHK(cry_ecp_dbl(&r, &r, grp));
                    w <<= 1;
                    if (k->data[i] & ((cry_mpi_digit)1 << j))
                        w |= 1;
                }
                w &= 0x7; /* -8 */
                // if (w >= 8)
                //    w -= 8;
                if (!paf) {
                    CHK(cry_ecp_add(&r, &r, &win[w], grp));
                } else {
                    paf = 0; /* First addition */
                    CHK(cry_ecp_copy(&r, &win[w]));
                }
            } else {
                /* special case */
                if (!paf) {
                    CHK(cry_ecp_add(&r, &r, p1, grp));
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
                const cry_ecp_grp *grp)
{
    int res, i, j, w, paf = 1;
    struct cry_ecp r, *win = NULL;
    cry_mpi_digit msk;
    cry_mpi *a = &grp->a;
    cry_mpi *p = &grp->p;

    if (cry_ecp_is_zero(p1))
        return (pr != p1) ? cry_ecp_copy(pr, p1) : 0;

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

#if 1

#include "../mpi/mpi_pvt.h"

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_ecp_grp *grp)
{
    int res;
    cry_ecp r;
    size_t bits;

    if (cry_ecp_is_zero(p1))
        return (pr != p1) ? cry_ecp_copy(pr, p1) : 0;

    if ((res = cry_mpi_init_list(&r.x, &r.y, &r.z, (cry_mpi *) NULL)) != 0)
        return res;

    cry_ecp_set_zero(&r);
    bits = cry_mpi_count_bits(k);
    while (bits-- > 0) {
        CHK(cry_ecp_dbl(&r, &r, grp));
        if (cry_mpi_is_bit_set(k, bits) != 0)
            CHK(cry_ecp_add(&r, &r, p1, grp));
    }

    cry_ecp_swap(pr, &r);
e:  cry_mpi_clear_list(&r.x, &r.y, &r.z, (cry_mpi *) NULL);
    return res;
}
#else
int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_ecp_grp *grp)
{
    int i, j, res;
    struct cry_ecp dp, r;

    if (cry_ecp_is_zero(p1))
        return (pr != p1) ? cry_ecp_copy(pr, p1) : 0;

    if ((res = cry_mpi_init_list(&dp.x, &dp.y, &dp.z, &r.x, &r.y, &r.z,
                                (cry_mpi *) NULL)) != 0)
        return res;

    cry_ecp_set_zero(&r);
    /* Old implementation */
    CHK(cry_ecp_copy(&dp, p1));
    for (i = 0; i < k->used; i++) {
        for (j = 0; j < CRY_MPI_DIGIT_BITS; j++) {
            if (k->data[i] & (1UL << j))
                CHK(cry_ecp_add(&r, &r, &dp, grp));
            CHK(cry_ecp_dbl(&dp, &dp, grp));
        }
    }
    cry_ecp_swap(pr, &r);

e:  cry_mpi_clear_list(&dp.x, &dp.y, &dp.z, &r.x, &r.y, &r.z,
                       (cry_mpi *) NULL);
    return res;
}
#endif

#endif /* CRY_ECP_MUL_WIN */
