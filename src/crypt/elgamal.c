#include <cry/elgamal.h>


int cry_elgamal_init(cry_elgamal_ctx *ctx)
{
    return cry_mpi_init_list(&ctx->g, &ctx->p, &ctx->x, &ctx->y,
                             (cry_mpi *)NULL);
}

void cry_elgamal_clear(cry_elgamal_ctx *ctx)
{
    cry_mpi_clear_list(&ctx->g, &ctx->p, &ctx->x, &ctx->y, (cry_mpi *)NULL);
}

int cry_elgamal_sign(cry_dsa_ctx *ctx, cry_elgamal_sig *sign,
                     const unsigned char *in, size_t len)
{

}

int cry_elgamal_verify(cry_dsa_ctx *ctx, const cry_dsa_signature *sign,
                       const unsigned char *in, size_t len)
{

}
