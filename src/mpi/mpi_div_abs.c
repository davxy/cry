/*
 * Copyright (c) 2013-2015, Davide Galassi. All rights reserved.
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

#include "mpi_pvt.h"

int cry_mpi_div_abs(cry_mpi *rq, cry_mpi *rr, const cry_mpi *a,
                    const cry_mpi *b)
{
    cry_mpi ta, tb, tq, q;
    int res, n;

    /* is divisor zero ? */
    if (cry_mpi_is_zero(b))
        return -1;

    /* if a < b then q=0, r=a */
    if (cry_mpi_cmp_abs(a, b) < 0) {
        if (rr)
            res = cry_mpi_copy(rr, a);
        else
            res = 0;
        if (rq)
            cry_mpi_zero(rq);
        return res;
    }

    if ((res = cry_mpi_init_list(&ta, &tb, &tq, &q, 0)) != 0)
        return res;

    cry_mpi_set_int(&tq, 1);
    n = cry_mpi_count_bits(a) - cry_mpi_count_bits(b);
    if (((res = cry_mpi_abs(&ta, a)) != 0) ||
        ((res = cry_mpi_abs(&tb, b)) != 0) ||
        ((res = cry_mpi_shl(&tb, &tb, n)) != 0) ||
        ((res = cry_mpi_shl(&tq, &tq, n)) != 0))
        goto cleanup;

#if 0
    int k = 0;
    for ( ; n >= 0; n -= k) {
        if (cry_mpi_cmp(&tb, &ta) <= 0) {
            if (k) {
                if ((res = cry_mpi_shr(&tq, &tq, k)) != 0)
                    goto cleanup;
                k = 0;
            }

            if (((res = cry_mpi_sub(&ta, &ta, &tb)) != 0) ||
                ((res = cry_mpi_add(&q, &q, &tq)) != 0))
                goto cleanup;
        }
        k = MAX(cry_mpi_count_bits(&tb) - cry_mpi_count_bits(&ta), 1);
        if ((res = cry_mpi_shr(&tb, &tb, k)) != 0)
            goto cleanup;
    }
#else
    while (n-- >= 0) {
        if (cry_mpi_cmp(&tb, &ta) <= 0) {
            if (((res = cry_mpi_sub(&ta, &ta, &tb)) != 0) ||
                ((res = cry_mpi_add(&q, &q, &tq)) != 0))
                goto cleanup;
        }
        if (((res = cry_mpi_shr(&tb, &tb, 1)) != 0) ||
            ((res = cry_mpi_shr(&tq, &tq, 1)) != 0))
            goto cleanup;
    }
#endif

    /* now q == quotient and ta == remainder */
    if (rq) {
        cry_mpi_swap(rq, &q);
        rq->sign = 0;
    }
    if (rr) {
        cry_mpi_swap(rr, &ta);
        rr->sign = 0;
    }
cleanup:
    cry_mpi_clear_list(&ta, &tb, &tq, &q, 0);
    return res;
}

