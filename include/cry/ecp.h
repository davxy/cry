/**
 * @file    ecp.h
 * @brief   Elliptic curve point arithmetic
 */

#ifndef _CRY_ECP_H_
#define _CRY_ECP_H_

#include "cry/mpi.h"

/** Elliptic curve point structure */
struct cry_ecp {
    cry_mpi x;
    cry_mpi y;
};

typedef struct cry_ecp cry_ecp;

#define cry_ecp_init(p) \
    cry_mpi_init_list(&(p)->x, &(p)->y, NULL)

#define cry_ecp_clear(p) \
    cry_mpi_clear_list(&(p)->x, &(p)->y, NULL)

#define cry_ecp_swap(a, b) do {     \
    cry_mpi_swap(&(a)->x, &(b)->x); \
    cry_mpi_swap(&(a)->y, &(b)->y); \
    } while (0)

#ifdef __cplusplus
extern "C"{
#endif

int cry_ecp_copy(cry_ecp *pr, const cry_ecp *p1);

int cry_ecp_add(cry_ecp *pr, const cry_ecp *p1, const cry_ecp *p2,
                const cry_mpi *p);

int cry_ecp_dbl(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *a,
                const cry_mpi *p);

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_mpi *a, const cry_mpi *p);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_ECP_H_ */
