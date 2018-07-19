#include "test.h"
#include <cry/ctr.h>
#include <cry/aes.h>

static const struct cry_ciph_itf aes_itf = {
    .init = NULL,
    .clean = NULL,
    .key_set = (cry_ciph_key_set_f) cry_aes_key_set,
    .encrypt = (cry_ciph_encrypt_f) cry_aes_encrypt,
    .decrypt = (cry_ciph_decrypt_f) cry_aes_decrypt
};

void cry_aes_128_ctr_encrypt(unsigned char *dst,
                             const unsigned char *src,
                             const unsigned int src_size,
                             const unsigned char *key,
                             const unsigned char *iv)
{
    struct cry_aes_ctx aes;
    struct cry_ctr_ctx ctr;

    ctr.ciph_itf = &aes_itf;
    ctr.ciph_ctx = &aes;
    cry_ctr_key_set(&ctr, key, 16);
    cry_ctr_iv_set(&ctr, iv, 16);
    cry_ctr_encrypt(&ctr, dst, src, src_size);
}

void cry_aes_128_ctr_decrypt(unsigned char *dst,
                             const unsigned char *src,
                             const unsigned int src_size,
                             const unsigned char *key,
                             const unsigned char *iv)
{
    struct cry_aes_ctx aes;
    struct cry_ctr_ctx ctr;

    ctr.ciph_itf = &aes_itf;
    ctr.ciph_ctx = &aes;
    cry_ctr_key_set(&ctr, key, 16);
    cry_ctr_iv_set(&ctr, iv, 16);
    cry_ctr_decrypt(&ctr, dst, src, src_size);
}

void ctr_test(void)
{
    char buf[128];
    char *msg = "This file is part of CRY software.";
    char key[] = {  0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9,10,11,12,13,14,15 };
    char iv[] =  {  0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9,10,11,12,13,14,15 };
    int msglen = strlen(msg);

    memcpy(buf, msg, sizeof(msg));
    cry_aes_128_ctr_encrypt(buf, buf, msglen, key, iv);
    PRINT_HEX("ciphertext", buf, msglen);

    cry_aes_128_ctr_decrypt(buf, buf, msglen, key, iv);
    PRINT_ASC("plaintext ", buf, msglen);

    CONTINUE(ASSERT_EQ_BUF(msg, buf, msglen));
    //ASSERT_EQ_BUF(msg, buf, msglen);
}
