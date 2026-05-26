
/********************************************************************************************************
 * @file    utility.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#ifndef SUPPORT_STATIC_ANALYSIS
#define cast2uint32(a)       ((unsigned int)(a))
#define get_max_len(a, b)    (((a) > (b)) ? (a) : (b))
#define get_min_len(a, b)    (((a) > (b)) ? (b) : (a))
#define get_word_len(bitLen) (((bitLen) + 31u) >> 5)
#define get_byte_len(bitLen) (((bitLen) + 7u) >> 3)
#else
unsigned int get_max_len(unsigned int a, unsigned int b);
unsigned int get_word_len(unsigned int bit_len);
unsigned int get_byte_len(unsigned int bit_len);
#endif


#define print_BN_buf_U32         print_bn_buf_u32
#define print_buf_U32            print_buf_u32
#define print_buf_U8             print_buf_u8
#define uint8_BigNum_Check_Zero  uint8_bignum_check_zero
#define uint32_BigNum_Check_Zero uint32_bignum_check_zero
#define Bigint_Check_p_1         bigint_check_p_1
#define Big_Div2n                big_div_2n
#define Bigint_Check_1           bigint_check_1
#define Get_Multiple2_Number     get_multiple2_number
#define uint32_BigNumCmp         uint32_big_num_cmp
#define uint8_XOR                uint8_xor
#define uint32_XOR               uint32_xor

    static inline unsigned int cast2uint32(unsigned int a)
    {
        return a;
    }

    // Maintenance API
    /**
 * @brief           Compare big integer a and b
 * @param[in]       a                    - big integer a
 * @param[in]       a_wlen               - Word length of a
 * @param[in]       b                    - big integer b
 * @param[in]       b_wlen               - Word length of b
 * @return          0 (a = b), 1 (a > b), -1 (a < b)
 * @note            Ensure that neither of a or b is NULL.
 */
    int big_integer_compare(const unsigned int *a, unsigned int a_wlen, const unsigned int *b, unsigned int b_wlen);

    /**
 * @brief           a = a / (2^n)
 * @param[in,out]   a                    - Big integer a, will be modified in place
 * @param[in]       a_wlen               - Word length of a
 * @param[in]       n                    - Exponent of 2^n
 * @return          Word length of a after division by 2^n
 * @note            
 *        1. Ensure that a is not NULL.
 *        2. Ensure that a_wlen is the real word length of a.
 *        3. Ensure that a_wlen * 32 is not less than n. a_wlen
 */
    unsigned int div2n_u32(unsigned int *a, unsigned int a_wlen, unsigned int n);
    /**
 * @brief           APIs
 */


#ifdef UTILITY_PRINT_BUF
    /**
 * @brief           Prints a buffer of unsigned 8-bit integers.
 * @param[in]       buf                  - Pointer to the buffer of unsigned 8-bit integers.
 * @param[in]       byteLen              - Length of the buffer in bytes.
 * @param[in]       name                 - Name or description of the buffer for printing purposes.
 * @return          None
 */
    extern void print_buf_u8(const unsigned char *buf, unsigned int byteLen, char *name);

    /**
 * @brief           Prints a buffer of unsigned 32-bit integers.
 * @param[in]       buf                  - Pointer to the buffer of unsigned 32-bit integers.
 * @param[in]       wlen                 - Length of the buffer in terms of number of unsigned 32-bit integers.
 * @param[in]       name                 - Name or description of the buffer for printing purposes.
 * @return          None
  */
    extern void print_buf_u32(const unsigned int *buf, unsigned int wlen, char *name);

    /**
 * @brief           Prints a buffer of unsigned 32-bit integers representing a big number (BN).
 * @param[in]       buf                  - Pointer to the buffer of unsigned 32-bit integers.
 * @param[in]       wlen                 - Length of the buffer in terms of number of unsigned 32-bit integers.
 * @param[in]       name                 - Name or description of the buffer for printing purposes.
 * @return          None
  */
    extern void print_bn_buf_u32(const unsigned int *buf, unsigned int wlen, char *name);
#endif

#define memcpy_ memcpy
#define memset_ memset
#define memcmp_ memcmp

#if 0
    /**
 * @brief           Memory copy, like memcpy()
 * @param[out]      dst                  - buffer
 * @param[in]       src                  - buffer
 * @param[in]       size                 - Number of bytes to copy (size of src or dst buffer)
 * @return          None
 * @note            
 *        1. Ensure that neither of dst nor src is NULL.
 *        2. Ensure that the dst and src buffers do not overlap.
 */
    void memcpy_(void *dst, const void *src, unsigned int size);

    /**
 * @brief           Memory set, like memset()
 * @param[out]      dst                  - buffer
 * @param[in]       value                - Unsigned char value to set
 * @param[in]       size                 - Number of bytes to set in the dst buffer
 * @return          None
 * @note            Ensure that dst is not NULL.
 */
    void memset_(void *dst, unsigned char value, unsigned int size);

    /**
 * @brief           Memory compare, like memcmp()
 * @param[in]       m1                   - Unsigned char buffer m1
 * @param[in]       m2                   - Unsigned char buffer m2
 * @param[in]       size                 - Number of bytes to compare in buffers m1 or m2
 * @return          0 (m1 = m2), non-zero (m1 != m2)
 * @note            
 *        1.Ensure that neither of m1 nor m2 is NULL.
 */
    unsigned char memcmp_(const void *m1, const void *m2, unsigned int size);
#endif

    /**
 * @brief           Set uint32 buffer
 * @param[out]      a                    - word buffer
 * @param[in]       value                - word value to set
 * @param[in]       wlen                 - Word length of buffer a
 * @return          None
 * @note            
 *        1.Ensure that a is not NULL.
 */
    void uint32_set(unsigned int *a, unsigned int value, unsigned int wlen);

    /**
 * @brief           Copy uint32 buffer
 * @param[out]      dst                  - word buffer
 * @param[in]       src                  - word buffer
 * @param[in]       wlen                 - Word length of buffer dst or src
 * @return          None
 * @note            
 *        1. Ensure that neither of dst nor src is NULL.
 */
    void uint32_copy(unsigned int *dst, const unsigned int *src, unsigned int wlen);

    /**
 * @brief           Copy uint32 buffer of 8 words
 * @param[out]      dst                  - word buffer
 * @param[in]       src                  - word buffer
 * @return          None
 * @note            
 *        1. Ensure that neither of dst nor src is NULL.
 */
    void uint32_copy_8_words(unsigned int *dst, const unsigned int *src);

    /**
 * @brief           Clear uint32 buffer
 * @param[in,out]   a                    - Word buffer a, will be cleared in place
 * @param[in]       a_wlen               - Word length of buffer a
 * @return          None
 * @note            
 *        1. Ensure that a is not NULL.
 */
    void uint32_clear(unsigned int *a, unsigned int a_wlen);

    /**
 * @brief           Clear uint32 buffer of 8 words
 * @param[in,out]   a                    - Word buffer a, will be cleared in place
 * @return          None
 * @note            
 *        1. Ensure that a is not NULL.
 */
    void uint32_clear_8_words(unsigned int *a);

    /**
 * @brief           Sleep for a while
 * @param[in]       count                - Count for sleeping
 * @param[in]       rand_bit             - Random bit, only the LSB works
 * @return          None
 */
    void uint32_sleep(unsigned int count, unsigned char rand_bit);

    /**
 * @brief           Reverse byte array
 * @param[in]       in                   - buffer
 * @param[out]      out                  - buffer
 * @param[in]       byteLen              - Byte length of input or output buffer
 * @return          None
 *        1. Ensure that neither of in nor out is NULL.
 *        2. in and out could point to the same buffer.
 */
    void reverse_byte_array(const unsigned char *in, unsigned char *out, unsigned int byteLen);

    /**
 * @brief           Reverse byte array
 * @param[in]       data_in              - buffer (32 bytes)
 * @param[out]      data_out             - buffer (8 words)
 * @return          None
 * @note            
 *        1. Ensure that neither of in nor out is NULL.
 *        2. in and out cannot point to the same buffer.
 *        3. This is for big numbers of 256 bits in SM2, SM9, etc.
 */
    void u8big_to_u32little_256bits(const unsigned char *data_in, unsigned int *data_out);

    /**
 * @brief           Reverse byte array
 * @param[in,out]   a                    - buffer (8 words)
 * @return          None
 * @note            
 *        1. Ensure that a is not NULL.
 *        2. This is for big numbers of 256 bits in SM2, SM9, etc.
 */
    void u8big_to_u32little_256bits_self(unsigned int *a);

    /**
 * @brief           Reverse byte array
 * @param[in]       in                   - buffer (8 words)
 * @param[out]      out                  - buffer (32 bytes)
 * @return          None
 * @note            
 *        1. Ensure that neither of in nor out is NULL.
 *        2. in and out cannot point to the same buffer.
 *        3. This is for big numbers of 256 bits in SM2, SM9, etc.
 */
    void u32little_to_u8big_256bits(const unsigned int *in, unsigned char *out);

    /**
 * @brief           C = A XOR B
 * @param[in]       A                    - byte buffer a
 * @param[in]       B                    - byte buffer b
 * @param[out]      C                    - byte buffer, C = A XOR B
 * @param[in]       byteLen              - Byte length of buffers A, B, and C
 * @return          None
 * @note            
 *        1. Ensure that none of A, B, or C is NULL.
 */
    void uint8_xor(const unsigned char *A, const unsigned char *B, unsigned char *C, unsigned int byteLen);

    /**
 * @brief           C = A XOR B
 * @param[in]       A                    - word buffer a
 * @param[in]       B                    - word buffer b
 * @param[out]      C                    - word buffer, C = A XOR B
 * @param[in]       wlen                 - Word length of buffers A, B, and C
 * @return          None
 * @note            
 *        1. Ensure that none of A, B, or C is NULL.
 */
    void uint32_xor(const unsigned int *A, const unsigned int *B, unsigned int *C, unsigned int wlen);

    /**
 * @brief           Get aimed bit value of big integer a
 * @param[in]       a                    - big integer a
 * @param[in]       bit_index            - aimed bit location
 * @return          Bit value of aimed bit (0 or 1)
 * @note            
 *        1. Ensure that a is not NULL.
 *        2. For the LSB, bit index is 0.
 */
    unsigned int get_bit_value_by_index(const unsigned int *a, unsigned int bit_index);

    /**
 * @brief           Get real bit length of big number a of wlen words
 * @param[in]       a                    - Input, big integer a
 * @param[in]       wlen                 - Input, word length of a
 * @return          Real bit length of big number a
 * @note            
 *        1. Ensure that a is not NULL.
 */
    unsigned int get_valid_bits(const unsigned int *a, unsigned int wlen);

    /**
 * @brief           Get real word length of big number a of max_words words
 * @param[in]       a                    - big integer a
 * @param[in]       max_words            - maximum word length of a
 * @return          Real word length of big number a
 * @note            
 *        1. Ensure that a is not NULL.
 */
    unsigned int get_valid_words(const unsigned int *a, unsigned int max_words);

    /**
 * @brief           Check whether big number or unsigned char buffer a is all zero or not
 * @param[in]       a                    - Input, byte buffer a
 * @param[in]       a_len                - Input, byte length of a
 * @return          0 (a is not zero), 1 (a is all zero)
 * @note            
 *        1. Ensure that a is not NULL.
 */
    unsigned int uint8_bignum_check_zero(const unsigned char *a, unsigned int a_len);

    /**
 * @brief           Check whether big number or unsigned int buffer a is all zero or not
 * @param[in]       a                    - big integer or word buffer a
 * @param[in]       a_wlen               - word length of a
 * @return          0 (a is not zero), 1 (a is all zero)
 * @note            
 *        1. Ensure that a is not NULL.
 */
    unsigned int uint32_bignum_check_zero(const unsigned int *a, unsigned int a_wlen);

    /**
 * @brief           a = a + b
 *                  @param[in,out] a big number a, unsigned char big-endian
 * @param[in]       a_bytes              - byte length of a
 * @param[in]       b                    - unsigned char integer b
 * @param[in]       is_secure            - is secure implementation, 0 (not), other (yes)
 * @return          0 (no overflow), 1 (overflow)
 * @note            
 *        1. Ensure that a is not NULL.
 *        2. This is mainly used for counter++ in SKE, KDF, etc.
 */
    unsigned int uint8_big_num_big_endian_add_little(unsigned char *a, unsigned int a_bytes, unsigned char b, unsigned char is_secure);

    /**
 * @brief           a = a + b
 *                  @param[in,out] a big number a, unsigned int little-endian
 * @param[in]       a_words              - word length of a
 * @param[in]       b                    - unsigned int integer b
 * @param[in]       is_secure            - is secure implementation, 0 (not), other (yes)
 * @return          0 (no overflow), 1 (overflow)
 * @note            
 *        1. Ensure that a is not NULL.
 *        2. This is mainly used for public key algorithm implementation.
 */
    unsigned int uint32_big_num_little_endian_add_little(unsigned int *a, unsigned int a_words, unsigned int b, unsigned char is_secure);

    /**
 * @brief           Compare big integer a and b
 * @param[in]       a                    - big integer a
 * @param[in]       a_wlen               - word length of a
 * @param[in]       b                    - big integer b
 * @param[in]       b_wlen               - word length of b
 * @return          0 (a = b), 1 (a > b), -1 (a < b)
 * @note            
 *        1. Ensure that neither a nor b is NULL.
 */
    int uint32_big_num_cmp(const unsigned int *a, unsigned int a_wlen, const unsigned int *b, unsigned int b_wlen);

    /**
 * @brief           For a = b*2^t, where b is odd, get t
 * @param[in]       a                    - big integer a
 * @return          Number of multiples by 2 for a
 * @note            
 *        1. Ensure that a is not NULL.
 *        2. Ensure that a != 0.
 */
    unsigned int get_multiple2_number(const unsigned int *a);

    /**
 * @brief           a = a / (2^n)
 * @param[in]       a                    - big integer a
 * @param[in]       a_wlen               - Word length of a
 * @param[in]       n                    - Exponent of 2^n
 * @return          Word length of a after division by 2^n
 * @note            
 *        1. Ensure that a is not NULL.
 *        2. Ensure that a_wlen is the real word length of a.
 *        3. Ensure that a_wlen * 32 is not less than n.
 */
    unsigned int big_div_2n(unsigned int *a, unsigned int a_wlen, unsigned int n);

    /**
 * @brief           Check whether a is equal to 1 or not
 * @param[in]       a                    - Pointer to unsigned int big integer a
 * @param[in]       a_wlen               - Word length of big integer a
 * @return          1 (a is 1), 0 (a is not 1)
 * @note            
 *        1. Ensure that a is not NULL.
 */
    unsigned int bigint_check_1(const unsigned int *a, unsigned int a_wlen);

    /**
 * @brief           Check whether a is equal to p-1 or not
 * @param[in]       a                    - Pointer to unsigned int big integer a
 * @param[in]       p                    - Pointer to unsigned int big integer p, p must be odd
 * @param[in]       wlen                 - Word length of a and p
 * @return          1 (a is p-1), 0 (a is not p-1)
 * @note            
 *        1. Ensure that neither a nor p is NULL.
 *        2. Ensure that p is odd.
 */
    unsigned int bigint_check_p_1(const unsigned int *a, const unsigned int *p, unsigned int wlen);

    /**
 * @brief           Check if integer k is in range [1, n-1]
 * @param[in]       k                    - Big integer k 
 * @param[in]       n                    - Big integer n 
 * @param[in]       wlen                 - Word length of k and n
 * @param[in]       ret_zero             - Return value if k is zero
 * @param[in]       ret_big              - Return value if k >= n
 * @param[in]       ret_success          - Return value if k is in [1, n-1]
 * @return          One of the provided return values based on k's value
 * @note            
 *        1. ret_zero: k is zero
 *        2. ret_big: k is greater than or equal to n
 *        3. ret_success: k is in [1, n-1]
 */
    unsigned int uint32_integer_check(const unsigned int *k, const unsigned int *n, unsigned int wlen, unsigned int ret_zero, unsigned int ret_big, unsigned int ret_success);

#ifdef __cplusplus
}
#endif

#endif
