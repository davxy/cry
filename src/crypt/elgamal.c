#include <cry/elgamal.h>

#define UNUSED(x) (void)(x)

int cry_elgamal_init(cry_elgamal_ctx *ctx)
{
    return cry_mpi_init_list(&ctx->g, &ctx->p, &ctx->x, &ctx->y,
                             (cry_mpi *)NULL);
}

void cry_elgamal_clear(cry_elgamal_ctx *ctx)
{
    cry_mpi_clear_list(&ctx->g, &ctx->p, &ctx->x, &ctx->y, (cry_mpi *)NULL);
}

int cry_elgamal_sign(cry_elgamal_ctx *ctx, cry_elgamal_sig *sign,
                     const unsigned char *in, size_t len)
{
    UNUSED(ctx);
    UNUSED(sign);
    UNUSED(in);
    UNUSED(len);
    return -1;
}

int cry_elgamal_verify(cry_elgamal_ctx *ctx, const cry_elgamal_sig *sign,
                       const unsigned char *in, size_t len)
{
    UNUSED(ctx);
    UNUSED(sign);
    UNUSED(in);
    UNUSED(len);
    return -1;
}
