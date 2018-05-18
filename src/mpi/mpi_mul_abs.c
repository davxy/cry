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

#define KARATSUBA_CUTOFF 64

int cry_mpi_mul_abs(cry_mpi *r, const cry_mpi *a, const cry_mpi *b)
{
#ifdef CRY_MPI_MUL_KARATSUBA
    if (CRY_MIN(a->used, b->used) > KARATSUBA_CUTOFF)
        return cry_mpi_mul_karatsuba(r, a, b);
#endif

#ifdef CRY_MPI_MUL_COMBA
    return cry_mpi_mul_comba(r, a, b);
#else
    return cry_mpi_mul_baseline(r, a, b);
#endif
}
