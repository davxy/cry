#include <cry/ecdh.h>

int cry_ecdh_init(cry_ecdh_ctx *ctx)
{
    cry_ecp_grp_init(&ctx->grp);
    cry_mpi_init(&ctx->d);
    cry_ecp_init(&ctx->q);
    cry_ecp_init(&ctx->z);
    return 0;
}

void cry_ecdh_clear(cry_ecdh_ctx *ctx)
{
    cry_ecp_grp_clear(&ctx->grp);
    cry_mpi_clear(&ctx->d);
    cry_ecp_clear(&ctx->q);
    cry_ecp_clear(&ctx->z);
}

int cry_ecdh_agree(cry_ecdh_ctx *ctx)
{
    return cry_ecp_mul(&ctx->q, &ctx->grp.g, &ctx->d, &ctx->grp);
}

int cry_ecdh_final(cry_ecdh_ctx *ctx)
{
    return cry_ecp_mul(&ctx->z, &ctx->q, &ctx->d, &ctx->grp);
}
