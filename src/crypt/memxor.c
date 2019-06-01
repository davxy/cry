#include "memxor.h"
#include <string.h>
#include <stdlib.h>


/* In the four cases below, we assume src1 <= src2.
 * The input pointers are eventually swapped internally.
 *
 * CASE 1 : Start from the beginning
 *
 *   1.a) dst is less than or equal both.
 *
 *      src1 :        [----------]
 *      src2 :            [----------]
 *      dst  :   [----------]
 *
 *   1.b) src1 and src2 are disjointed AND dst is less than or equal src2
 *
 *      src1 :   [----------]
 *      src2 :                     [----------]
 *      dst  :                 [----------]
 *
 * CASE 2: Start from the end
 *
 *   2.a) dst is greater than or equal both
 *
 *      src1 :   [----------]
 *      src2 :       [----------]
 *      dst  :            [----------]
 *
 *   2.b) src1 and src2 are disjointed AND dst+n is smaller than or equal src2
 *
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
 *  CASE 4: worst case, inplace implementation is too much involved.
 *          Use malloc for temporary allocation.
 *
 *      src1 :   [----------]
 *      src2 :         [----------]
 *      dst  :      [----------]
 */
void cry_memxor2(unsigned char *dst, const unsigned char *src1,
                 const unsigned char *src2, size_t size)
{
    size_t i, j;
    unsigned char *tmp, t;
    size_t n = size;

    if (src2 < src1) {
        tmp = (unsigned char *)src1;
        src1 = src2;
        src2 = tmp;
    }

    if (dst <= src1 || (src1 + n <= dst && dst <= src2)) {
        /* Case 1 */
        for (i = 0; i < n; i++)
            dst[i] = src1[i] ^ src2[i];
    } else if (src2 <= dst || (src1 <= dst && dst + n <= src2)) {
        /* Case 2 */
        while (n != 0) {
            n--;
            dst[n] = src1[n] ^ src2[n];
        }
    } else if (src1 + n < src2){
        /* Case 3 */
        j = (size_t) ((src1 + n) - dst);
        for (i = 0; i < n; i++) {
            dst[j++] = src1[i] ^ src2[i];
            if (j == n)
                j = 0;
        }
        /* Now shift the dst buffer */
        j = (size_t) ((src1 + n) - dst);
        for (i = 0; i < n-1; i++) {
            t = dst[j];
            dst[j] = dst[i];
            dst[i] = t;
            if (j < n-1)
                j++;
        }
    } else {
        /* Case 4 */
        tmp = malloc(n);
        if (tmp != NULL) {
            cry_memxor2(tmp, src1, src2, n);
            memcpy(dst, tmp, n);
            free(tmp);
        }
    }
}

void cry_memxor(unsigned char *dst, const unsigned char *src,
                size_t size)
{
    size_t n = size;

    if (dst <= src) {
        while (n--)
            *dst++ ^= *src++;
    } else {
        while (n--)
            dst[n] ^= src[n];
    }
}
