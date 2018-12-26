#include <cry/dh.h>

int cry_dh_init(cry_dh_ctx *dh)
{
    cry_mpi_init(&dh->p);
    cry_mpi_init(&dh->g);
    cry_mpi_init(&dh->e);
    cry_mpi_init(&dh->Y);
    cry_mpi_init(&dh->Z);
    return 0;
}

void cry_dh_clear(cry_dh_ctx *dh)
{
    cry_mpi_clear(&dh->p);
    cry_mpi_clear(&dh->g);
    cry_mpi_clear(&dh->e);
    cry_mpi_clear(&dh->Y);
    cry_mpi_clear(&dh->Z);
}

int cry_dh_agree(cry_dh_ctx *dh)
{
    return cry_mpi_mod_exp(&dh->Y, &dh->g, &dh->e, &dh->p);
}

int cry_dh_finalize(cry_dh_ctx *dh)
{
    return cry_mpi_mod_exp(&dh->Z, &dh->Y, &dh->e, &dh->p);
}

int cry_dh_get_tok(cry_dh_ctx *dh, unsigned char *out, size_t out_len)
{
    return cry_mpi_store_bin(&dh->Y, out, out_len, 0);
}

int cry_dh_set_tok(cry_dh_ctx *dh, unsigned char *in, size_t in_len)
{
    return cry_mpi_load_bin(&dh->Y, in, in_len);
}

int cry_dh_get_key(cry_dh_ctx *dh, unsigned char *out, size_t out_len)
{
    return cry_mpi_store_bin(&dh->Z, out, out_len, 0);
}
