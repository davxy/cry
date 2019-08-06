#include "test.h"
#include <cry/elgamal.h>


cry_elgamal_ctx g_elg;


static void setup(void)
{
    cry_elgamal_init(&g_elg);
}

static void teardown(void)
{
    cry_elgamal_clear(&g_elg);
}


static void sign_verify(void)
{
}


#define MYRUN(name, test) \
        run(name, test, setup, teardown)

void elgamal_test(void)
{
    TRACE("* ElGamal Test\n");
    MYRUN("Sign-verify", sign_verify);
}
