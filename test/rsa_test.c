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

#include "test.h"
#include <cry/rsa.h>

#if 0

const unsigned char modulus[] = {
    0xC4, 0xF8, 0xE9, 0xE1, 0x5D, 0xCA, 0xDF, 0x2B,
    0x96, 0xC7, 0x63, 0xD9, 0x81, 0x00, 0x6A, 0x64,
    0x4F, 0xFB, 0x44, 0x15, 0x03, 0x0A, 0x16, 0xED,
    0x12, 0x83, 0x88, 0x33, 0x40, 0xF2, 0xAA, 0x0E,
    0x2B, 0xE2, 0xBE, 0x8F, 0xA6, 0x01, 0x50, 0xB9,
    0x04, 0x69, 0x65, 0x83, 0x7C, 0x3E, 0x7D, 0x15,
    0x1B, 0x7D, 0xE2, 0x37, 0xEB, 0xB9, 0x57, 0xC2,
    0x06, 0x63, 0x89, 0x82, 0x50, 0x70, 0x3B, 0x3F
};

const unsigned char private[] = {
    0x8a, 0x7e, 0x79, 0xf3, 0xfb, 0xfe, 0xa8, 0xeb,
    0xfd, 0x18, 0x35, 0x1c, 0xb9, 0x97, 0x91, 0x36,
    0xf7, 0x05, 0xb4, 0xd9, 0x11, 0x4a, 0x06, 0xd4,
    0xaa, 0x2f, 0xd1, 0x94, 0x38, 0x16, 0x67, 0x7a,
    0x53, 0x74, 0x66, 0x18, 0x46, 0xa3, 0x0c, 0x45,
    0xb3, 0x0a, 0x02, 0x4b, 0x4d, 0x22, 0xb1, 0x5a,
    0xb3, 0x23, 0x62, 0x2b, 0x2d, 0xe4, 0x7b, 0xa2,
    0x91, 0x15, 0xf0, 0x6e, 0xe4, 0x2c, 0x41
};

const unsigned char public[] = {
    0x01, 0x00, 0x01
};

#else

const unsigned char modulus[] = {
    143
};

const unsigned char private[] = {
    103
};

const unsigned char public[] = {
    7
};

#endif

#define MSG     "\x09"
#define CIPHTXT "\x30"
#define LEN     1

void rsa_test(void)
{
    cry_rsa_ctx rsa;

    WARN("improve performance\n");

    cry_mpi_init_bin(&rsa.m, modulus, sizeof(modulus));
    cry_mpi_init_bin(&rsa.e, public, sizeof(public));
    cry_mpi_init_bin(&rsa.d, private, sizeof(private));

    TRACE("m-bits: %d\n", cry_mpi_count_bits(&rsa.m));
    TRACE("e-bits: %d\n", cry_mpi_count_bits(&rsa.e));
    TRACE("d-bits: %d\n", cry_mpi_count_bits(&rsa.d));
    PRINT_MPI("m", &rsa.m, 10);
    PRINT_MPI("e", &rsa.e, 10);
    PRINT_MPI("d", &rsa.d, 10);

    cry_rsa_encrypt(&rsa, buf, MSG, LEN);
    PRINT_HEX("ciphertext", buf, LEN);
    ASSERT_EQ_BUF(buf, CIPHTXT, LEN);

    cry_rsa_decrypt(&rsa, buf, buf, LEN);
    PRINT_HEX("plaintext ", buf, LEN);
    ASSERT_EQ_BUF(buf, MSG, LEN);

    cry_mpi_clear_list(&rsa.m, &rsa.e, &rsa.d, 0);
}

