#include <cry/ecdh.h>
#include "mpi/mpi_pvt.h"

#define CHK0(exp) CRY_CHK(res = (exp), e0)
#define CHK1(exp) CRY_CHK(res = (exp), e1)

int cry_ecdh_init(cry_ecdh_ctx *ctx, int grp_id)
{
    int res;

    if ((res = cry_mpi_init(&ctx->d)) < 0)
        return res;
    CHK0(cry_ecp_init_list(&ctx->q, &ctx->z, NULL));
    if (grp_id != -1) {
        CHK1(cry_ecp_grp_load(&ctx->grp, grp_id));
    } else {
        CHK1(cry_ecp_grp_init(&ctx->grp));
    }
    return 0;

e1: cry_ecp_clear_list(&ctx->q, &ctx->z, NULL);
e0: cry_mpi_clear(&ctx->d);
    return res;
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
