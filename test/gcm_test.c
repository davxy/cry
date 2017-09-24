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
#include <cry/gcm.h>
#include <cry/aes.h>

static const struct cry_ciph_itf aes_itf = {
    .init = NULL,
    .clean = NULL,
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
    .decrypt = (cry_ciph_decrypt_f) cry_aes_decrypt
};

void cry_aes_128_gcm_encrypt(unsigned char *dst,
                             const unsigned char *src,
                             const unsigned int src_size,
                             const unsigned char *key,
                             const unsigned char *iv,
                             unsigned char *mac, 
                             const unsigned char *aad,
                             unsigned int aad_size)
{
    struct cry_aes_ctx aes;
    struct cry_gcm_ctx gcm;

    gcm.ciph_itf = &aes_itf;
    gcm.ciph_ctx = &aes;
    cry_gcm_key_set(&gcm, key, 16);
    cry_gcm_iv_set(&gcm, iv, 16);
    cry_gcm_encrypt(&gcm, dst, src, src_size);
    if (aad != NULL)
        cry_gcm_update(&gcm, aad, aad_size);
    if (mac != NULL)
        cry_gcm_digest(&gcm, mac, 16);
}

void cry_aes_128_gcm_decrypt(unsigned char *dst,
                             const unsigned char *src,
                             const unsigned int src_size,
                             const unsigned char *key,
                             const unsigned char *iv,
                             unsigned char *mac, 
                             const unsigned char *aad,
                             unsigned int aad_size)
{
    struct cry_aes_ctx aes;
    struct cry_gcm_ctx gcm;

    gcm.ciph_itf = &aes_itf;
    gcm.ciph_ctx = &aes;
    cry_gcm_key_set(&gcm, key, 16);
    cry_gcm_iv_set(&gcm, iv, 16);
    cry_gcm_decrypt(&gcm, dst, src, src_size);
    if (aad != NULL)
        cry_gcm_update(&gcm, aad, aad_size);
    if (mac != NULL)
        cry_gcm_digest(&gcm, mac, 16);
}



void gcm_test(void)
{
    char *msg = "This file is part of CRY software.";
    char key[] = {  0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9,10,11,12,13,14,15 };
    char iv[] =  {  0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9,10,11,12,13,14,15 };
    char *aad = "Hello";
    int msglen = strlen(msg);
    int aadlen = strlen(aad);

    memcpy(buf, msg, msglen);

    cry_aes_128_gcm_encrypt(buf, msg, msglen, key, iv,
                            buf+msglen, aad, aadlen);
    PRINT_HEX("ciphertext", buf, msglen);

    cry_aes_128_gcm_decrypt(buf, buf, msglen, key, iv,
                            buf+msglen+16, aad, aadlen);
    PRINT_ASC("plaintext ", buf, msglen);

    /* GMAC check */
    PRINT_HEX("gmac", buf+msglen, 16);
    ASSERT_EQ_BUF(buf+msglen, buf+msglen+16, 16);
}

