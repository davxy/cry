/*
 * Copyright (c) 2013-2014, Davide Galassi. All rights reserved.
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

int cry_mpi_grow(cry_mpi *a, unsigned int size);

/* decrease used while the most significant digit is zero */
#define cry_mpi_adjust(a) do { \
    while ((a)->used > 0 && (a)->data[(a)->used - 1] == 0) \
        ((a)->used)--; \
    } while(0)

#endif /* _CRY_MPI_PVT_H_ */

