/********************************************************************************************************
 * @file    utility.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef UTILITY_H
#define UTILITY_H


#include <stdint.h> //including int definition
#include <string.h>


#ifdef __cplusplus
extern "C"
{
#endif


#define GET_MAX_LEN(a, b)    (((a) > (b)) ? (a) : (b))
#define GET_MIN_LEN(a, b)    (((a) > (b)) ? (b) : (a))
#define GET_WORD_LEN(bitLen) (((bitLen) + 31) / 32)
#define GET_BYTE_LEN(bitLen) (((bitLen) + 7) / 8)


    //APIs

#define PKE_PRINT_BUF
#ifdef PKE_PRINT_BUF
    extern void print_buf_U8(unsigned char buf[], unsigned int byteLen, char name[]);
    extern void print_buf_U32(unsigned int buf[], unsigned int wordLen, char name[]);
    extern void print_BN_buf_U32(unsigned int buf[], unsigned int wordLen, char name[]);
#endif

#define memcpy_ memcpy
#define memset_ memset
#define memcmp_ memcmp

    /**
 * @brief       set uint32 buffer.
 * @param[out]  a                - output word buffer.
 * @param[in]   value            - input word value.
 * @param[in]   wordLen          - word length of buffer a.
 * @return      none
 */
    void uint32_set(volatile unsigned int *a, unsigned int value, unsigned int wordLen);

    /**
 * @brief       copy uint32 buffer.
 * @param[out]  dst          - output word buffer.
 * @param[in]   src          - input word buffer.
 * @param[in]   wordLen      - word length of buffer dst or src.
 * @return      none
 */
    void uint32_copy(volatile unsigned int *dst, volatile unsigned int *src, unsigned int wordLen);

    /**
 * @brief       copy uint32 buffer.
 * @param[in/out]   a            - word buffer a.
 * @param[in]   aWordLen         - word length of buffer a.
 * @return      none
 */
    void uint32_clear(volatile unsigned int *a, unsigned int wordLen);

    /**
 * @brief       sleep for a while.
 * @param[in]   count         - count.
 * @return      none
 */
    void uint32_sleep(unsigned int count, unsigned char rand);

    void uint32_endian_reverse(unsigned char *in, unsigned char *out, unsigned int wordLen);

    /**
 * @brief       reverse word array.
 * @param[in]   in         - input buffer.
 * @param[in]   out        - output buffer.
 * @param[in]   wordLen    - word length of in or out.
 * @return      none
 * @note
  @verbatim
      -# 1. in and out could point the same buffer.
  @endverbatim
  */
    void reverse_byte_array(volatile const unsigned char *in, unsigned char *out, unsigned int byteLen);
#if 0
void reverse_word(unsigned char *in, unsigned char *out, unsigned int bytelen);

void dma_reverse_word_array(unsigned int *in, unsigned int *out, unsigned int wordlen, unsigned int reverse_word);
#endif

    /**
 * @brief       C = A XOR B.
 * @param[in]   A         - byte buffer a.
 * @param[in]   B         - byte buffer b.
 * @param[out]  C         - C = A XOR B.
 * @return      none
 */
    void uint8_XOR(unsigned char *A, unsigned char *B, unsigned char *C, unsigned int byteLen);

    /**
 * @brief       C = A XOR B.
 * @param[in]   A         - word buffer a.
 * @param[in]   B         - word buffer b.
 * @param[out]  C         - C = A XOR B.
 * @param[in]   byteLen   - word length of A,B,C.
 * @return      none
 */
    void uint32_XOR(unsigned int *A, unsigned int *B, unsigned int *C, unsigned int wordLen);

    /**
 * @brief       get real bit length of big number a of wordLen words.
 * @param[in]   a         - pointer to the array of unsigned integers.
 * @param[in]   wordLen   - length of the array in words (unsigned integers).
 * @return      the number of valid bits in the array, or 0 if the array is empty or all elements are zero
 */
    unsigned int get_valid_bits(unsigned int *a, unsigned int wordLen);

    /**
 * @brief       get real word length of big number a of max_words words.
 * @param[in]   a           - big integer a.
 * @param[in]   max_words   - max word length of a.
 * @return      real word length of big number a
 */
    unsigned int get_valid_words(volatile unsigned int *a, unsigned int max_words);

    /**
 * @brief       check whether big number or unsigned char buffer a is all zero or not.
 * @param[in]   a           - byte buffer a.
 * @param[in]   aByteLen    - byte length of a.
 * @return      0(a is not zero),1(a is all zero)
 */
    unsigned char uint8_BigNum_Check_Zero(unsigned char a[], unsigned int aByteLen);

    /**
 * @brief       check whether big number or unsigned int buffer a is all zero or not.
 * @param[in]   a           - big integer or word buffer a.
 * @param[in]   aByteLen    - word length of a.
 * @return      0(a is not zero), 1(a is all zero)
 */
    unsigned int uint32_BigNum_Check_Zero(unsigned int a[], unsigned int aWordLen);

    /**
 * @brief       compare big integer a and b.
 * @param[in]   a           - big integer a.
 * @param[in]   aWordLen    - word length of a.
 * @param[in]   b           - big integer b.
 * @param[in]   bWordLen    - word length of b.
 * @return      0:a=b,   1:a>b,   -1: a<b
 */
    int uint32_BigNumCmp(volatile unsigned int *a, unsigned int aWordLen, volatile unsigned int *b, unsigned int bWordLen);

    /**
 * @brief       for a = b*2^t, b is odd, get t.
 * @param[in]   a           - big integer a.
 * @return      number of multiple by 2, for a
 * @note
   @verbatim
      -# 1. make sure a != 0.
   @endverbatim
 */
    unsigned int Get_Multiple2_Number(unsigned int a[]);

    /**
 * @brief       a = a/(2^n).
 * @param[in]   a                  - big integer a.
 * @param[in]   aWordLen           - word length of a.
 * @param[in]   n                  - exponent of 2^n.
 * @return      word length of a = a/(2^n)
 * @note
   @verbatim
      -# 1. make sure aWordLen is real word length of a.
      -# 2. to make sure aWordLen-1 is available, so data type of aWordLen is int, not unsigned int.
      -# 3. please make sure aWordLen*32 is not less than n.
   @endverbatim
 */
    unsigned int Big_Div2n(unsigned int a[], int aWordLen, unsigned int n);

    /**
 * @brief       check whether a is equal to 1 or not.
 * @param[in]   a                  - pointer to unsigned int big integer a.
 * @param[in]   aWordLen           - word length of big integer a.
 * @return      1(a is 1), 0(a is not 1)
 * @note
   @verbatim
      -# 1. make sure aWordLen is real word length of a.
      -# 2. to make sure aWordLen-1 is available, so data type of aWordLen is int, not unsigned int.
      -# 3. please make sure aWordLen*32 is not less than n.
   @endverbatim
 */
    unsigned char Bigint_Check_1(volatile unsigned int a[], unsigned int aWordLen);

    /**
 * @brief       check whether a is equal to p-1 or not.
 * @param[in]   a                  - pointer to unsigned int big integer a.
 * @param[in]   p                  - pointer to unsigned int big integer p, p must be odd.
 * @param[in]   aWordLen           - word length of a and p.
 * @return      1(a is 1), 0(a is not 1)
 * @note
   @verbatim
      -# 1. make sure p is odd.
   @endverbatim
 */
    unsigned char Bigint_Check_p_1(unsigned int a[], unsigned int p[], unsigned int wordLen);

    /**
 * @brief       check whether integer k is in [1, n-1].
 * @param[in]   k                  - big number k.
 * @param[in]   n                  - big number n.
 * @param[in]   wordLen            - word length of k and n.
 * @return      ret_zero(k is zero)   ret_big(k is greater/bigger than or equal to n)  ret_success(k is in [1, n-1])
 */
    unsigned int uint32_integer_check(unsigned int *k, unsigned int *n, unsigned int wordLen, unsigned int ret_zero, unsigned int ret_big, unsigned int ret_success);

#ifdef __cplusplus
}
#endif

#endif
