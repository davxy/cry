/*
 * Copyright (c) 2013-2017, Davide Galassi. All rights reserved.
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

#include "test.h"
#include <cry/ecp.h>
#include <cry/ec.h>

static cry_ec ec;

#define P1X "2"
#define P1Y "3"
#define P2X "4"
#define P2Y "5"

void ecp_add(void)
{
    cry_ecp p1, p2, pr;

    cry_ecp_init(&p1);
    cry_ecp_init(&p2);
    cry_ecp_init(&pr);

    ASSERT_OK(cry_mpi_init_str(&p1.x, 16, P1X));
    ASSERT_OK(cry_mpi_init_str(&p1.y, 16, P1Y));
    ASSERT_OK(cry_mpi_init_str(&p2.x, 16, P2X));
    ASSERT_OK(cry_mpi_init_str(&p2.y, 16, P2Y));
    ASSERT_OK(cry_ecp_add(&pr, &p1, &p2, &ec.p));

    cry_ecp_clear(&p1);
    cry_ecp_clear(&p2);
    cry_ecp_clear(&pr);
}

void ecp_test(void)
{
    ASSERT_OK(cry_ec_init_nist_p256(&ec));
    RUN(ecp_add);
}

