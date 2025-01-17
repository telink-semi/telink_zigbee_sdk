/********************************************************************************************************
 * @file    pke_utility.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/********* pke version:1.1 *********/
#ifndef PKE_UTILITY_H
#define PKE_UTILITY_H


#ifdef __cplusplus
extern "C"
{
#endif


#define valid_bits_get       get_valid_bits
#define valid_words_get      get_valid_words
#define big_integer_compare  uint32_BigNumCmp


#define GET_MAX_LEN(a, b)    (((a) > (b)) ? (a) : (b))
#define GET_MIN_LEN(a, b)    (((a) > (b)) ? (b) : (a))
#define GET_WORD_LEN(bitLen) (((bitLen) + 31) / 32)
#define GET_BYTE_LEN(bitLen) (((bitLen) + 7) / 8)


    //APIs

    /**
 * @brief       set uint32 buffer.
 * @param[out]  a       - output word buffer.
 * @param[in]   value   - input word value.
 * @param[in]   wordLen - word length of buffer a.
 * @return      none.
 */
    void uint32_set(volatile unsigned int *a, unsigned int value, unsigned int wordLen);

    /**
 * @brief       copy uint32 buffer.
 * @param[out]  dst     - output word buffer.
 * @param[in]   src     - input word value.
 * @param[in]   wordLen - word length of buffer dst or src.
 * @return      none.
 */
    void uint32_copy(volatile unsigned int *dst, volatile const unsigned int *src, unsigned int wordLen);

    /**
 * @brief           clear uint32 buffer.
 * @param[in&out]   a        - word buffer a.
 * @param[in]       aWordLen - word length of buffer a.
 * @return          none.
 */
    void uint32_clear(volatile unsigned int *a, unsigned int wordLen);

    /**
 * @brief       sleep for a while.
 * @param[in]   count - count value.
 * @return      none.
 */
    void uint32_sleep(volatile unsigned int count);

    /**
 * @brief       reverse byte array.
 * @param[in]   in      - input buffer.
 * @param[out]  out     - output buffer.
 * @param[in]   byteLen - byte length of in or out.
 * @return      none.
 * @caution     in and out could point the same buffer.
 */
    void reverse_byte_array(volatile const unsigned char *in, unsigned char *out, unsigned int byteLen);

    /**
 * @brief       reverse byte order in every unsigned int word.
 * @param[in]   in      - input byte buffer.
 * @param[out]  out     - output word buffer.
 * @param[in]   byteLen - byte length of buffer in or out.
 * @return      none.
 * @caution     byteLen must be multiple of 4.
 */
    //void reverse_word(unsigned char *in, unsigned char *out, unsigned int bytelen);

    /**
 * @brief       reverse word order.
 * @param[in]   in           - input word buffer.
 * @param[out]  out          - output word buffer.
 * @param[in]   wordLen      - word length of buffer in or out.
 * @param[in]   reverse_word - whether to reverse byte order in every word, 0:no, other:yes.
 * @return      none.
 * @caution     in DAM mode, the memory may be accessed by words, not by bytes, this function is designed for the case
 */
    //void dma_reverse_word_array(unsigned int *in, unsigned int *out, unsigned int wordlen, unsigned int reverse_word);

    /**
 * @brief       C = A XOR B.
 * @param[in]   A       - byte buffer a.
 * @param[in]   B       - byte buffer b.
 * @param[out]  C       - C = A XOR B.
 * @param[in]   byteLen - byte length of A,B,C.
 * @return      none.
 */
    void uint8_XOR(unsigned char *A, unsigned char *B, unsigned char *C, unsigned int byteLen);

    /**
 * @brief       get real bit length of big number a of wordLen words.
 * @param[in]   a       - word buffer a.
 * @param[in]   wordLen - word length of buffer a.
 * @return      none.
 */
    unsigned int get_valid_bits(const unsigned int *a, unsigned int wordLen);

    /**
 * @brief       get real word length of big number a of max_words words.
 * @param[in]   a         - big integer a.
 * @param[in]   max_words - max word length of a.
 * @return      real word length of big number a.
 */
    unsigned int get_valid_words(volatile const unsigned int *a, unsigned int max_words);

    /**
 * @brief       check whether big number or unsigned char buffer a is all zero or not.
 * @param[in]   a         - byte buffer a.
 * @param[in]   aByteLen  - byte length of a.
 * @return      0(a is not zero), 1(a is all zero).
 */
    unsigned char uint8_BigNum_Check_Zero(unsigned char a[], unsigned int aByteLen);

    /**
 * @brief       check whether big number or uint32_t buffer a is all zero or not.
 * @param[in]   a        - big integer or word buffer a.
 * @param[in]   aWordLen - word length of a.
 * @return      0(a is not zero), 1(a is all zero).
 */
    unsigned char uint32_BigNum_Check_Zero(unsigned int a[], unsigned int aWordLen);

    /**
 * @brief       compare big integer a and b.
 * @param[in]   a        - big integer a.
 * @param[in]   aWordLen - word length of a.
 * @param[in]   b        - big integer b.
 * @param[in]   bWordLen - word length of b.
 * @return      0:a=b,   1:a>b,   -1: a<b.
 */
    int uint32_BigNumCmp(volatile const unsigned int *a, unsigned int aWordLen, volatile const unsigned int *b, unsigned int bWordLen);

    /**
 * @brief       for a = b*2^t, b is odd, get t.
 * @param[in]   a   - big integer a.
 * @return      number of multiple by 2, for a.
 * @caution     make sure a != 0.
 */
    unsigned int Get_Multiple2_Number(unsigned int a[]);

    /**
 * @brief       a = a/(2^n).
 * @param[in]   a        - big integer a.
 * @param[in]   aWordLen - word length of a.
 * @param[in]   n        - exponent of 2^n.
 * @return      word length of a = a/(2^n).
 * @caution     1. make sure aWordLen is real word length of a.
 *              2. a may be 0, then aWordLen is 0, to make sure aWordLen-1 is available, so data type of aWordLen is int32_t, not uint32_t.
 */
    unsigned int Big_Div2n(volatile unsigned int a[], signed int aWordLen, unsigned int n);

    /**
 * @brief       check whether a is equal to 1 or not.
 * @param[in]   a        - pointer to uint32_t big integer a.
 * @param[in]   aWordLen - word length of big integer a.
 * @return      1(a is 1), 0(a is not 1).
 */
    unsigned char Bigint_Check_1(volatile unsigned int a[], unsigned int aWordLen);

    /**
 * @brief       check whether a is equal to p-1 or not.
 * @param[in]   a        - pointer to unsigned int big integer a.
 * @param[in]   p        - pointer to unsigned int big integer p, p must be odd.
 * @param[in]   aWordLen - word length of a and p.
 * @return      1(a is 1), 0(a is not 1).
 * @caution     make sure p is odd.
 */
    unsigned char Bigint_Check_p_1(unsigned int a[], unsigned int p[], unsigned int wordLen);


#ifdef __cplusplus
}
#endif

#endif
