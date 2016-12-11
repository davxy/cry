/*
 * Copyright (c) 2013-2016, Davide Galassi. All rights reserved.
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

/**
 * @file    ec.h
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
    cry_mpi_init_list(&(p)->x, &(p)->y, 0)

#define cry_ecp_clear(p) \
    cry_mpi_clear_list(&(p)->x, &(p)->y, 0)

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

#ifdef __cplusplus
}
#endif

#endif /* _CRY_ECP_H_ */

