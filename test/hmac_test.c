#include "test.h"
#include <cry/hmac.h>
#include <cry/sha256.h>

struct hmac_param {
    size_t keylen;
    size_t msglen;
    size_t maclen;
    unsigned char key[256];
    unsigned char msg[256];
    unsigned char mac[64];
};

static void param_init(struct hmac_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->keylen = raw_init(par->key, sizeof(par->key), argv[0]);
    par->msglen = raw_init(par->msg, sizeof(par->msg), argv[1]);
    par->maclen = raw_init(par->mac, sizeof(par->mac), argv[2]);
}

static void sha256_clear(void *ctx)
{
    /* Just to increase lib coverage */
}

static const struct cry_hash_itf sha256_itf = {
    (cry_hash_init_f)cry_sha256_init,
    (cry_hash_clear_f)sha256_clear,
    (cry_hash_update_f)cry_sha256_update,
    (cry_hash_digest_f)cry_sha256_digest,
};

static void hmac_digest(int argc, char *argv[])
{
    cry_sha256_ctx sha256;
    cry_hmac_ctx hmac;
    struct hmac_param par;
    unsigned char mac[64];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_hmac_init(&hmac, &sha256, &sha256_itf, 32, par.key, par.keylen);
    cry_hmac_update(&hmac, par.msg, par.msglen);
    cry_hmac_digest(&hmac, mac);

    ASSERT(memcmp(mac, par.mac, par.maclen) == 0);

    cry_hmac_clear(&hmac);
}


static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;
    if (strcmp(test, "hmac_digest") == 0)
        hmac_digest(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}

void hmac_test(void)
{
    TRACE("* HMAC NIST CAVS 11.0\n");
    func_test("hmac_test.data", dispatch);
    TRACE("\n");
}
