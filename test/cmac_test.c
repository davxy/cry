#include "test.h"
#include <cry/cmac.h>
#include <cry/aes.h>


struct cmac_param {
    size_t keylen;
    size_t msglen;
    size_t maclen;
    unsigned char key[256];
    unsigned char msg[256];
    unsigned char mac[64];
};

static void param_init(struct cmac_param *par, int argc, char *argv[])
{
    memset(par, 0, sizeof(*par));
    par->keylen = raw_init(par->key, sizeof(par->key), argv[0]);
    par->msglen = raw_init(par->msg, sizeof(par->msg), argv[1]);
    par->maclen = raw_init(par->mac, sizeof(par->mac), argv[2]);
}

static const struct cry_ciph_itf g_aes_itf = {
    NULL, /* init */
    NULL, /* clean */
    (cry_ciph_key_set_f)cry_aes_key_set,
    NULL, /* ivset */
    (cry_ciph_encrypt_f)cry_aes_encrypt,
    NULL /* decrypt */
};

static void cmac_aes_digest(int argc, char *argv[])
{
    struct cmac_param par;
    cry_cmac_ctx cmac;
    cry_aes_ctx aes;
    unsigned char mac[16];

    ASSERT(argc == 3);
    param_init(&par, argc, argv);

    cry_cmac_init(&cmac, &aes, &g_aes_itf, par.key, par.keylen);
    cry_cmac_update(&cmac, par.msg, par.msglen);
    cry_cmac_digest(&cmac, mac);

    ASSERT(memcmp(mac, par.mac, par.maclen) == 0);
}

static void dispatch(int argc, char *argv[])
{
    char *test = *argv;

    argv++;
    argc--;
    if (strcmp(test, "cmac_aes_digest") == 0)
        cmac_aes_digest(argc, argv);
    else
        TRACE("Test '%s' not defined\n", test);
}


#define KEY  "0123456789abcdef"
#define MSG  "LoremipsumdolorsitametconsecteturadipiscingelitV"
#define LEN  (sizeof(MSG)-1)
#define MAC  "\x70\xa7\x85\xad\x19\x62\xa7\x59\x7d\x1f\x0f\x0f\x2c\xfa\xd8\x57"
#define MAC2 "\x3a\x20\x0f\x07\x78\xf8\x57\x42\xd8\x64\x7d\x46\x73\xd5\xb6\xc7"

struct slice {
    const unsigned char *ptr;
    int len;
};

static void cmac_aes_sliced(struct slice *slices, int nslices,
                            unsigned char *mac2)
{
    cry_cmac_ctx cmac;
    cry_aes_ctx aes;
    unsigned char mac[16];
    int i;

    cry_cmac_init(&cmac, &aes, &g_aes_itf, (unsigned char *)KEY, 16);
    for (i = 0; i < nslices; i++)
        cry_cmac_update(&cmac, slices[i].ptr, slices[i].len);
    cry_cmac_digest(&cmac, mac);

    ASSERT_EQ_BUF(mac, mac2, 16);
}

/*
 * [ Lor emipsumdolors ] [ itametconsectetu ] [ radipiscingelitV ]
 */
static void cmac_aes_first_sliced1(void)
{
    struct slice slices[] = {
        { (unsigned char *)MSG, 3 },
        { (unsigned char *)MSG + 3, LEN - 3 }
    };

    cmac_aes_sliced(slices, 2, (unsigned char *)MAC);
}

/*
 * [ Lor emipsumdol ors ] [ itametconsectetu ] [ radipiscingelitV ]
 */
static void cmac_aes_first_sliced2(void)
{
    struct slice slices[] = {
        { (unsigned char *)MSG, 3 },
        { (unsigned char *)MSG + 3, 10 },
        { (unsigned char *)MSG + 13, LEN - 13 },
    };

    cmac_aes_sliced(slices, 3, (unsigned char *)MAC);
}

/*
 * [ Loremipsumdolors ] [ ita metconsectetu ] [ radipiscingelitV ]
 */
static void cmac_aes_mid_sliced(void)
{
    struct slice slices[] = {
        { (unsigned char *)MSG, 19 },
        { (unsigned char *)MSG + 19, LEN - 19 }
    };

    cmac_aes_sliced(slices, 2, (unsigned char *)MAC);
}

/*
 * [ Loremipsumdolors ] [ itametconsectetu ] [ rad ipiscingelitV ]
 */
static void cmac_aes_last_sliced(void)
{
    struct slice slices[] = {
        { (unsigned char *)MSG, 35 },
        { (unsigned char *)MSG + 35, LEN - 35 }
    };

    cmac_aes_sliced(slices, 2, (unsigned char *)MAC);
}

/*
 * [ Loremipsumdolors ] [ ita metconsectetu ] [ rad ]
 */
static void cmac_aes_last_incomplete(void)
{
    struct slice slices[] = {
        { (unsigned char *)MSG, 19 },
        { (unsigned char *)MSG + 19, LEN - 19 - 13 }
    };

    cmac_aes_sliced(slices, 2, (unsigned char *)MAC2);
}


void cmac_test(void)
{
    TRACE("* CMAC Tests\n");
    run("CMAC-AES First Sliced #1", cmac_aes_first_sliced1, NULL, NULL);
    run("CMAC-AES First Sliced #2", cmac_aes_first_sliced2, NULL, NULL);
    run("CMAC-AES Mid Sliced #1", cmac_aes_mid_sliced, NULL, NULL);
    run("CMAC-AES Last Sliced #1", cmac_aes_last_sliced, NULL, NULL);
    run("CMAC-AES Last Incomplete", cmac_aes_last_incomplete, NULL, NULL);

    func_test("cmac_test.data", dispatch);
    TRACE("\n");
}
