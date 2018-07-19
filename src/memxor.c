#include "memxor.h"
#include <stdlib.h>
#include <string.h>

void cry_memxor3_worst(unsigned char *dst, const unsigned char *src1,
                       const unsigned char *src2, unsigned int size)
{
    unsigned int i;
    unsigned char *tmp;
    
    tmp = malloc(size);
    if (tmp != NULL) {
        for (i = 0; i < size; i++)
            tmp[i] = src1[i] ^ src2[i];
        memcpy(dst, tmp, size);
        free(tmp);
    } else {
        /* 
         * TODO: this could be a security issue FIXME
         */
    }
}

/* We assume src1 <= src2
 *
 * CASE 1 : Start from the beginning
 *
 *   1.a) dst is less than or equal both.
 *      src1 :        [----------]
 *      src2 :            [----------]
 *      dst  :   [----------]
 *
 *   1.b) src1 and src2 are disjointed AND dst is less than or equal src2
 *      src1 :   [----------]
 *      src2 :                     [----------]
 *      dst  :                 [----------]
 *
 * CASE 2: Start from the end
 *
 *   2.a) dst is greater than or equal both
 *      src1 :   [----------]
 *      src2 :       [----------]
 *      dst  :            [----------]
 *
 *   2.b) src1 and src2 are disjointed AND dst+n is smaller than or equal src2
 *      src1 :   [----------]
 *      src2 :                     [----------]
 *      dst  :      [----------]
 *
 *  CASE 3: src1 and src2 are disjointed but dst contains the end of src1 and
 *          the start of src2.
 *          Start from k and finally rotate the result buffer at the end.
 *
 *      src1 :   [----------]
 *      src2 :                   [----------]
 *      dst  :           [---k------]
 *
 *  CASE 4: worst case, use malloc for temporary allocation.
 *          Inplace implementation is still possible but is not implemented.
 */
void cry_memxor3(unsigned char *dst, const unsigned char *src1,
                 const unsigned char *src2, unsigned int n)
{
    unsigned int i;

    if (src2 < src1) {
        const unsigned char *tmp = src1;

        src1 = src2;
        src2 = tmp;
    }

    if ((dst <= src1) || ((src1+n <= dst) && (dst <= src2))) {
        /* Case 1 */
        for (i = 0; i < n; i++) 
            dst[i] = src1[i] ^ src2[i];
    } else if ((src2 <= dst) || ((src1 <= dst) && (dst+n <= src2))) {
        /* Case 2 */
        while(n != 0) {
            n--;
            dst[n] = src1[n] ^ src2[n];
        }
    }
#if 0   /* TODO: EXAMINATE better before production */
    /* Case 3 */
    else if (disjoint == TRUE) {
    }
#endif
    /* Case 4 */
    else {
        cry_memxor3_worst(dst, src1, src2, n);
    }
}

void cry_memxor(unsigned char *dst, const unsigned char *src,
                unsigned int n)
{
    if (dst <= src) {
        while (n--)
            *dst++ ^= *src++;
    } else {
        while (n--)
            dst[n] ^= src[n];
    }
}
