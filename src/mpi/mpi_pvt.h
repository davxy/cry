/*
 * Copyright (c) 2013-2015, Davide Galassi. All rights reserved.
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

#ifndef _CRY_MPI_PVT_H_
#define _CRY_MPI_PVT_H_

#include "cry/mpi.h"
#include <string.h> /* memset */

/* Double precision digits */
typedef unsigned short cry_mpi_dword;

/* Number of bytes in one digit */
#define CRY_MPI_DIGIT_BYTES  sizeof(cry_mpi_digit)

/* Number of bits in one digit */
#define CRY_MPI_DIGIT_BITS   (CRY_MPI_DIGIT_BYTES << 3)

/* Bits to digits */
#define CRY_MPI_BITS_TO_DIGS(a) \
        ((a != 0) ? (((a) - 1) / CRY_MPI_DIGIT_BITS + 1) : 0)

/* Octets to digits */
#define CRY_MPI_BYTES_TO_DIGS(a) \
        ((a != 0) ? (((a) - 1) / CRY_MPI_DIGIT_BYTES + 1) : 0)


int cry_mpi_grow(cry_mpi *a, unsigned int size);

/* decrease used while the most significant digit is zero */
#define cry_mpi_adjust(a) do { \
    while ((a)->used > 0 && (a)->data[(a)->used - 1] == 0) \
        ((a)->used)--; \
    } while(0)

#define cry_mpi_set_used(a, n) do { \
    (a)->used = (n); \
    memset((a)->data, 0, (n) * CRY_MPI_DIGIT_BYTES); \
    } while(0)

#endif /* _CRY_MPI_PVT_H_ */

