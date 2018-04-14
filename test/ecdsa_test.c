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
#include <cry/ecdsa.h>

/* key */
static const unsigned char d_dat[] = {
    0xDC, 0x51, 0xD3, 0x86, 0x6A, 0x15, 0xBA, 0xCD,
    0xE3, 0x3D, 0x96, 0xF9, 0x92, 0xFC, 0xA9, 0x9D,
    0xA7, 0xE6, 0xEF, 0x09, 0x34, 0xE7, 0x09, 0x75,
    0x59, 0xC2, 0x7F, 0x16, 0x14, 0xC8, 0x8A, 0x7F
};

static const unsigned char *msg = "Hello World";

void ecdsa_test(void)
{
    cry_ecdsa_ctx ecdsa;
    cry_ecdsa_signature sign;

    ASSERT_OK(cry_mpi_init_list(&sign.r, &sign.s, NULL));

    /* Load a well known curve params */
    ASSERT_OK(cry_ec_init_nist_p256(&ecdsa.ec));

    /* Generate new public key from private key w and point G */
//    cry_mpi_load_bin(&ecdsa.d, d_dat, sizeof(d_dat));
    ASSERT_OK(cry_ecp_init(&ecdsa.q));
    ASSERT_OK(cry_mpi_init(&ecdsa.d));
    ASSERT_OK(cry_ecdsa_keygen(&ecdsa.ec, &ecdsa.d, &ecdsa.q));

    ASSERT_OK(cry_ecdsa_sign(&ecdsa, &sign, msg, strlen(msg)));
    PRINT_MPI("r", &sign.r, 16);
    PRINT_MPI("s", &sign.s, 16);

    ASSERT_OK(cry_ecdsa_verify(&ecdsa, &sign, msg, strlen(msg)));

    cry_mpi_clear_list(&sign.r, &sign.s, NULL);
    /* TODO: release ecdsa context data */
}

