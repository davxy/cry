#ifndef _CRY_MEMXOR_H_
#define _CRY_MEMXOR_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
 * XOR two memory regions.
 *
 * The result is stored in the first operand.
 * The memory segments may overlap and can work inplace.
 *
 * @param dst   First source and destination.
 * @param src   Second source.
 * @param size  Number of bytes to XOR.
 */
void cry_memxor(unsigned char *dst, const unsigned char *src,
                unsigned int size);

/**
 * XOR two memory regions.
 *
 * The memory segments may overlap and can work inplace.
 *
 * @param dst   Destination.
 * @param src1  First source.
 * @param src2  Second source.
 * @param size  Number of bytes to XOR.
 */
void cry_memxor3(unsigned char *dst, const unsigned char *src1,
                 const unsigned char *src2, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_MEMXOR_H_ */
