/********************************************************************************************************
 * @file    utility.h
 *
 * @brief   This is the header file for TL321X
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


#include "lib/include/crypto_common/common_config.h"


#ifdef __cplusplus
extern "C"
{
#endif


//#ifndef SUPPORT_STATIC_ANALYSIS
#define CAST2UINT32(a)       ((unsigned int)(a))
#define GET_MAX_LEN(a, b)    (((a) > (b)) ? (a) : (b))
#define GET_MIN_LEN(a, b)    (((a) > (b)) ? (b) : (a))
#define GET_WORD_LEN(bitLen) (((bitLen) + 31u) >> 5)
#define GET_BYTE_LEN(bitLen) (((bitLen) + 7u) >> 3)
    //#else
    //unsigned int GET_MAX_LEN(unsigned int a, unsigned int b);
    //unsigned int GET_WORD_LEN(unsigned int bit_len);
    //unsigned int GET_BYTE_LEN(unsigned int bit_len);
    //#endif


    //APIs

#ifdef UTILITY_PRINT_BUF
    extern void print_buf_U8(const unsigned char *buf, unsigned int byteLen, char *name);
    extern void print_buf_U32(const unsigned int *buf, unsigned int wordLen, char *name);
    extern void print_BN_buf_U32(const unsigned int *buf, unsigned int wordLen, char *name);
#endif

#define memcpy_ memcpy
#define memset_ memset
#define memcmp_ memcmp

#if 0
/**
 * @brief       memory copy, like memcpy()
 * @param[in]   dst         - output, output buffer.
 * @param[in]   src         - input, input buffer.
 * @param[in]   size        - input, bytes of src or dst buffer.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure neither of dst,src is NULL.
      -# 2. 2. please make sure dst buffer and src buffer do not have common part.
   @endverbatim
 */
void memcpy_(void *dst, const void *src, unsigned int size);

/**
 * @brief       memory set, like memset()
 * @param[in]   dst         - output, output buffer.
 * @param[in]   value       - input, unsigned char value.
 * @param[in]   size        - input, bytes of dst buffer.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure dst is not NULL.
   @endverbatim
 */
void memset_(void *dst, unsigned char value, unsigned int size);

/**
 * @brief       memory set, like memset()
 * @param[in]   m1         - input, unsigned char buffer m1.
 * @param[in]   m2         - input, unsigned char buffer m2
 * @param[in]   size       - input, bytes of buffer m1 or m2.
 * @return      0(m1 = m2), other(m1 != m2)
 * @note
   @verbatim
      -# 1. please make sure neither of m1,m2 is NULL.
   @endverbatim
 */
unsigned char memcmp_(const void *m1, const void *m2, unsigned int size);

#endif

    /**
 * @brief       set uint32 buffer
 * @param[in]   a             - output, output word buffer.
 * @param[in]   value         - input, input word value.
 * @param[in]   wordLen       - input, word length of buffer a.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    void uint32_set(unsigned int *a, unsigned int value, unsigned int wordLen);

    /**
 * @brief       copy uint32 buffer
 * @param[in]   dst             - output, output word buffer.
 * @param[in]   src             - input, input word buffer.
 * @param[in]   wordLen         - input, word length of buffer dst or src.
 * @return      none
 * @note
   @verbatim
      -#  please make sure neither of dst,src is NULL.
   @endverbatim
 */
    void uint32_copy(unsigned int *dst, const unsigned int *src, unsigned int wordLen);

    /**
 * @brief       copy uint32 buffer
 * @param[in]   dst             - output, output word buffer.
 * @param[in]   src             - input, input word buffer.
 * @param[in]   wordLen         - input, word length of buffer dst or src.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure neither of dst,src is NULL.
   @endverbatim
 */
    void uint32_copy_8_words(unsigned int *dst, const unsigned int *src);

    /**
 * @brief       clear uint32 buffer
 * @param[in]   a             - input&output, word buffer a.
 * @param[in]   wordLen      - input, word length of buffer a.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    void uint32_clear(unsigned int *a, unsigned int wordLen);

    /**
 * @brief       clear uint32 buffer of 8 words
 * @param[in]   a             - input&output, word buffer a.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    void uint32_clear_8_words(unsigned int *a);

    /**
 * @brief       sleep for a while
 * @param[in]   count             - input, count.
 * @param[in]   rand_bit          - input, random bit, only the LSB works.
 * @return      none
 */
    void uint32_sleep(unsigned int count, unsigned char rand_bit);

    /**
 * @brief       convert 0x1122334455667788 to 0x4433221188776655
 * @param[in]   in             - source address.
 * @param[out]  out            - destination address.
 * @param[in]   wordLen        - word length of in/out.
 * @return      none
 */
    void uint32_endian_reverse(unsigned char *in, unsigned char *out, unsigned int wordLen);

#if 0
/**
 * @brief       reverse word array
 * @param[in]   in             - input, input buffer.
 * @param[out]  out            - output, output buffer.
 * @param[in]   wordLen        - input, word length of in or out.
 * @return      none
 * @note
   @verbatim
      -# 1. in and out could point the same buffer.
   @endverbatim
 */
void reverse_word_array(unsigned char *in, unsigned int *out, unsigned int wordLen)
#endif

    /**
 * @brief       reverse byte array
 * @param[in]   in             - input, input buffer.
 * @param[out]  out            - output, output buffer.
 * @param[in]   wordLen        - input, word length of in or out.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure neither of in,out is NULL.
      -# 2. in and out could point the same buffer
   @endverbatim
 */
    void reverse_byte_array(const unsigned char *in, unsigned char *out, unsigned int byteLen);

#if 0
/**
 * @brief      reverse byte order in every unsigned int word
 * @param[in]   in             - input, input byte buffer.
 * @param[out]  out            - output, output word buffer.
 * @param[in]   bytelen        - input, byte length of buffer in or out.
 * @return      none
 * @note
   @verbatim
      -# 1. byteLen must be a multiple of 4.
   @endverbatim
 */
void reverse_word(unsigned char *in, unsigned char *out, unsigned int bytelen);

/**
 * @brief     reverse word order
 * @param[in]   in             - input, input word buffer.
 * @param[out]  out            - output, output word buffer.
 * @param[in]   bytelen        - input, word length of buffer in or out.
 * @param[in]   reverse_word   - input, whether to reverse byte order in every word, 0:no, other:yes.
 * @return      none
 * @note
   @verbatim
      -# 1. in DAM mode, the memory may be accessed by words, not by bytes, this function is designed for the case.
   @endverbatim
 */
void dma_reverse_word_array(unsigned int *in, unsigned int *out, unsigned int wordlen, unsigned int reverse_word);
#endif

    /**
 * @brief     reverse byte array
 * @param[in]   in             - input, input buffer, 32 bytes.
 * @param[out]  out            - output, output buffer, 8 words.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure neither of in,out is NULL.
      -# 2. in and out can not point the same buffer.
      -# 3. this is for big number of 256 bits in SM2, SM9, etc.
   @endverbatim
 */
    void u8big_to_u32little_256bits(const unsigned char *in, unsigned int *out);

    /**
 * @brief     reverse byte array
 * @param[in&out]   a             - input&output, 8 words.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
      -# 2. this is for big number of 256 bits in SM2, SM9, etc.
   @endverbatim
 */
    void u8big_to_u32little_256bits_self(unsigned int *a);

    /**
 * @brief     reverse byte array
 * @param[in]   a             - input, input buffer, 8 words.
 * @param[out]  output        - output, output buffer, 32 bytes.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure neither of in,out is NULL.
      -# 2. in and out can not point the same buffer.
      -# 3. this is for big number of 256 bits in SM2, SM9, etc.
   @endverbatim
 */
    void u32little_to_u8big_256bits(const unsigned int *in, unsigned char *out);

    /**
 * @brief    C = A XOR B
 * @param[in]   A             - input, byte buffer a.
 * @param[in]   B             - input, byte buffer b.
 * @param[in]   C             - output, C = A XOR B.
 * @param[in]   byteLen       - input, byte length of A,B,C.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure none of A,B,C is NULL.
   @endverbatim
 */
    void uint8_XOR(const unsigned char *A, const unsigned char *B, unsigned char *C, unsigned int byteLen);

    /**
 * @brief    C = A XOR B
 * @param[in]   A             - input, word buffer a.
 * @param[in]   B             - input, word buffer b.
 * @param[in]   C             - output, C = A XOR B.
 * @param[in]   byteLen       - input, word length of A,B,C.
 * @return      none
 * @note
   @verbatim
      -# 1. please make sure none of A,B,C is NULL.
   @endverbatim
 */
    void uint32_XOR(const unsigned int *A, const unsigned int *B, unsigned int *C, unsigned int wordLen);

    /**
 * @brief   get aimed bit value of big integer a
 * @param[in]   a             - input, big integer a.
 * @param[in]   bit_index     - input, aimed bit location.
 * @return      bit value of aimed bit
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
      -# 2. for the LSB, bit index is 0.
   @endverbatim
 */
    unsigned int get_bit_value_by_index(const unsigned int *a, unsigned int bit_index);

    /**
 * @brief   get real bit length of big number a of wordLen words
 * @param[in]   a             - input, big integer a.
 * @param[in]   wordLen       - input, word length of a.
 * @return      real bit length of big number a
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    unsigned int get_valid_bits(const unsigned int *a, unsigned int wordLen);

    /**
 * @brief   get real word length of big number a of max_words words
 * @param[in]   a             - input, big integer a.
 * @param[in]   max_words     - input, max word length of a.
 * @return     real word length of big number a.
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    unsigned int get_valid_words(const unsigned int *a, unsigned int max_words);

    /**
 * @brief   check whether big number or unsigned char buffer a is all zero or not
 * @param[in]   a             - input, byte buffer a.
 * @param[in]   aByteLen      - input, byte length of a.
 * @return    0(a is not zero),1(a is all zero)
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    unsigned int uint8_BigNum_Check_Zero(const unsigned char *a, unsigned int aByteLen);

    /**
 * @brief   check whether big number or unsigned int buffer a is all zero or not
 * @param[in]   a             - input, big integer or word buffer a.
 * @param[in]   aByteLen      - input, word length of a.
 * @return     0(a is not zero), 1(a is all zero)
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    unsigned int uint32_BigNum_Check_Zero(const unsigned int *a, unsigned int aWordLen);

    /**
 * @brief   a = a + b
 * @param[in]   a             - input, big number a, unsigned char big-endian.
 * @param[in]   a_bytes       - input, byte length of a.
 * @param[in]   b             - input, unsigned char integer b.
 * @param[in]   is_secure     - input, is secure implementation, 0(not), other(yes).
 * @return     0(not overflow),1(overflow)
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
      -# 2. this is mainly used for counter++ in SKE, KDF, etc.
   @endverbatim
 */
    unsigned int uint8_big_num_big_endian_add_little(unsigned char *a, unsigned int a_bytes, unsigned char b, unsigned char is_secure);

    /**
 * @brief   a = a + b
 * @param[in]   a             - input, big number a, unsigned int big-endian.
 * @param[in]   a_words       - input, word length of a.
 * @param[in]   b             - input, unsigned int integer b.
 * @param[in]   is_secure     - input, is secure implementation, 0(not), other(yes).
 * @return     0(not overflow),1(overflow)
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
      -# 2. this is mainly used for public key algorithm implementation.
   @endverbatim
 */
    unsigned int uint32_big_num_little_endian_add_little(unsigned int *a, unsigned int a_words, unsigned int b, unsigned char is_secure);

    /**
 * @brief   compare big integer a and b
 * @param[in]   a             - input, big integer a.
 * @param[in]   aWordLen      - input, word length of a.
 * @param[in]   b             - input, big integer b.
 * @param[in]   bWordLen      - input, word length of b.
 * @return     0:a=b,   1:a>b,   -1: a<b
 * @note
   @verbatim
      -# 1. please make sure neither of a,b is NULL.
   @endverbatim
 */
    int32_t uint32_BigNumCmp(const unsigned int *a, unsigned int aWordLen, const unsigned int *b, unsigned int bWordLen);

    /**
 * @brief   for a = b*2^t, b is odd, get t
 * @param[in]   a             - big integer a.
 * @return     number of multiple by 2, for a
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
      -# 2. make sure a != 0.
   @endverbatim
 */
    unsigned int Get_Multiple2_Number(const unsigned int *a);

    /**
 * @brief   a = a/(2^n)
 * @param[in]   a             - big integer a.
 * @param[in]   aWordLen      - word length of a.
 * @param[in]   n             - exponent of 2^n.
 * @return     word length of a = a/(2^n),
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
      -# 2. make sure aWordLen is real word length of a.
      -# 3. please make sure aWordLen*32 is not less than n.
   @endverbatim
 */
    unsigned int Big_Div2n(unsigned int *a, unsigned int aWordLen, unsigned int n);

    /**
 * @brief   check whether a is equal to 1 or not
 * @param[in]   a             - pointer to unsigned int big integer a.
 * @param[in]   aWordLen      - word length of big integer a.
 * @return     1(a is 1), 0(a is not 1)
 * @note
   @verbatim
      -# 1. please make sure a is not NULL.
   @endverbatim
 */
    unsigned int Bigint_Check_1(const unsigned int *a, unsigned int aWordLen);

    /**
 * @brief   check whether a is equal to p-1 or not
 * @param[in]   a             - pointer to unsigned int big integer a.
 * @param[in]   p             - pointer to unsigned int big integer p, p must be odd.
 * @param[in]   wordLen       - word length of a and p.
 * @return     1(a is p-1), 0(a is not p-1)
 * @note
   @verbatim
      -# 1. please make sure neither of a,p is NULL.
      -# 2. please make sure p is odd.
   @endverbatim
 */
    unsigned int Bigint_Check_p_1(const unsigned int *a, const unsigned int *p, unsigned int wordLen);

    /**
 * @brief   check whether integer k is in [1, n-1]
 * @param[in]   k             - input, big number k.
 * @param[in]   n             - input, big number n.
 * @param[in]   wordLen       - input, word length of k and n.
 * @return     ret_zero      k is zero
 *             ret_big       k is greater/bigger than or equal to n
 *             ret_success   k is in [1, n-1]
 */
    unsigned int uint32_integer_check(const unsigned int *k, const unsigned int *n, unsigned int wordLen, unsigned int ret_zero, unsigned int ret_big, unsigned int ret_success);

#ifdef __cplusplus
}
#endif

#endif
