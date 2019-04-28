/**
 * @file    ecp.h
 * @brief   Elliptic curve point arithmetic
 */

#ifndef CRY_ECP_H_
#define CRY_ECP_H_

#include <cry/mpi.h>

/**
 * EC point
 */
struct cry_ecp {
    cry_mpi x;
    cry_mpi y;
    cry_mpi z;
};

typedef struct cry_ecp cry_ecp;

/*
 * EC point group equation parameters
 * y^2 = (x^3 + ax + b) % p
 */
struct cry_ecp_grp {
    cry_mpi p;  /**< Modulus */
    cry_mpi a;  /**< x coefficient */
    cry_mpi b;  /**< Constant */
    cry_ecp g;  /**< Generator point */
    cry_mpi n;  /**< Generator order */
};

typedef struct cry_ecp_grp cry_ecp_grp;


#ifdef __cplusplus
extern "C"{
#endif

int cry_ecp_init(cry_ecp *p);

void cry_ecp_clear(cry_ecp *p);

int cry_ecp_copy(cry_ecp *pr, const cry_ecp *p1);

int cry_ecp_add(cry_ecp *pr, const cry_ecp *p1, const cry_ecp *p2,
                const cry_ecp_grp *grp);

int cry_ecp_dbl(cry_ecp *pr, const cry_ecp *p1, const cry_ecp_grp *grp);

int cry_ecp_mul(cry_ecp *pr, const cry_ecp *p1, const cry_mpi *k,
                const cry_ecp_grp *grp);

#ifdef __cplusplus
}
#endif

#define cry_ecp_is_zero(p) \
    cry_mpi_is_zero(&(p)->z)

#define cry_ecp_set_zero(p) do { \
    cry_mpi_set_int(&(p)->x, 1); \
    cry_mpi_set_int(&(p)->y, 1); \
    cry_mpi_set_int(&(p)->z, 0); \
} while (0)

#define cry_ecp_swap(a, b) do {     \
    cry_mpi_swap(&(a)->x, &(b)->x); \
    cry_mpi_swap(&(a)->y, &(b)->y); \
    cry_mpi_swap(&(a)->z, &(b)->z); \
} while (0)

int cry_ecp_grp_init(cry_ecp_grp *ec);

void cry_ecp_grp_clear(cry_ecp_grp *ec);

#endif /* CRY_ECP_H_ */
