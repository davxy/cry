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
#include <cry/ecdh.h>

void ecdh_test(void)
{
    cry_ecdh_ctx dh1, dh2;
    cry_ecp q1, q2;

    ASSERT_OK(cry_ecp_init(&q1));
    ASSERT_OK(cry_ecp_init(&q2));

    /* Alice */

    ASSERT_OK(cry_ecdh_init(&dh1));
    ASSERT_OK(cry_ec_set_nist_p256(&dh1.ec));
    ASSERT_OK(cry_mpi_rand(&dh1.d, 256));
    ASSERT_OK(cry_ecdh_agree(&dh1)); /* gen pub key */
    ASSERT_OK(cry_ecp_copy(&q1, &dh1.q));

    /* Bob */

    ASSERT_OK(cry_ecdh_init(&dh2));
    ASSERT_OK(cry_ec_set_nist_p256(&dh2.ec));
    ASSERT_OK(cry_mpi_rand(&dh2.d, 256));
    ASSERT_OK(cry_ecdh_agree(&dh2)); /* gen pub key */
    ASSERT_OK(cry_ecp_copy(&q2, &dh2.q));
    
    /* Public keys exchange takes place */
    ASSERT_OK(cry_ecp_copy(&dh1.q, &q2));
    ASSERT_OK(cry_ecp_copy(&dh2.q, &q1));

    /* Final step */
    ASSERT_OK(cry_ecdh_final(&dh1));
    ASSERT_OK(cry_ecdh_final(&dh2));

    /* Final checks */

    ASSERT_EQ(cry_mpi_cmp(&dh1.z.x, &dh2.z.x), 0);
    ASSERT_EQ(cry_mpi_cmp(&dh1.z.y, &dh2.z.y), 0);

    cry_ecdh_clear(&dh1);
    cry_ecdh_clear(&dh2);
    cry_ecp_clear(&q1);
    cry_ecp_clear(&q2);
}

