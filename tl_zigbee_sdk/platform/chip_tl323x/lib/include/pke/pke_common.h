/********************************************************************************************************
 * @file    pke_common.h
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
#ifndef PKE_COMMON_H
#define PKE_COMMON_H


#include "lib/include/pke/pke_config.h"
#include "reg_include/pke_reg.h"


/**
 * @brief           ECC point conversion form
 */
#define POINT_COMPRESSED   (0x02U) /**< pc||x, pc = 0x02|LSB(y) */
#define POINT_UNCOMPRESSED (0x04U) /**< pc||x||y, pc=0x04 */
typedef unsigned char ec_point_form_e;

/**
 * @brief           define KDF
 */
typedef void *(*KDF_FUNC)(const void *input, unsigned int byteLen, unsigned char *key, unsigned int key_len);

// APIs
/**
 * @brief           Load input operand to baseaddr
 * @param[out]      baseaddr             - destination data
 * @param[in]       data                 - source data
 * @param[in]       wlen                 - word length of data
 * @return          None
 * @note            
 *        1. Operands are both unsigned int little-endian.
 */
void pke_load_operand(unsigned int *baseaddr, const unsigned int *data, unsigned int wlen);

/**
 * @brief           Load input operand of 256 bits to baseaddr
 * @param[out]      baseaddr             - destination data
 * @param[in]       data                 - source data
 * @return          None
 * @note            
 *        1. Operands are both unsigned int little-endian.
 *        2. Operands are both of 256 bits for SM2, SM9, etc.
 */
void pke_load_operand_256bits(unsigned int *baseaddr, const unsigned int *data);

/**
 * @brief           Get result operand from baseaddr
 * @param[in]       baseaddr             - source data
 * @param[out]      data                 - destination data
 * @param[in]       wlen                 - word length of data
 * @return          None
 * @note            
 *        1. Operands are both unsigned int little-endian.
 */
void pke_read_operand(const unsigned int *baseaddr, unsigned int *data, unsigned int wlen);

/**
 * @brief           Get result operand of 256 bits from baseaddr
 * @param[in]       baseaddr             - source data
 * @param[out]      data                 - destination data
 * @return          None
 * @note            
 *        1. Operands are both unsigned int little-endian.
 *        2. Operands are both of 256 bits for SM2, SM9, etc.
 */
void pke_read_operand_256bits(const unsigned int *baseaddr, unsigned int *data);

/**
 * @brief           Load input operand (unsigned char big-endian) to baseaddr
 * @param[in]       baseaddr             - destination data
 * @param[out]      data                 - source data, unsigned char big-endian
 * @param[in]       byteLen              - byteLen Input, byte length of data
 * @return          None
 */
void pke_load_operand_U8(unsigned int *baseaddr, const unsigned char *data, unsigned int byteLen);

/**
 * @brief           Get result operand (unsigned char big-endian) from baseaddr
 * @param[in]       baseaddr             - source data
 * @param[out]      data                 - destination data, unsigned char big-endian
 * @param[in]       byteLen              - byte length of data
 * @return          None
 */
void pke_read_operand_U8(const unsigned int *baseaddr, unsigned char *data, unsigned int byteLen);

/**
 * @brief           Set operand with an unsigned int value
 * @param[out]      baseaddr             - operand
 * @param[in]       wlen                 - word length of operand
 * @param[in]       b                    - unsigned int value b
 * @return          None
 * @note            
 *        1. wlen cannot be 0.
 */
void pke_set_operand_uint32_value(unsigned int *baseaddr, unsigned int wlen, unsigned int b);

/**
 * @brief           Set operand of 256 bits with an unsigned int value
 * @param[out]      baseaddr             - operand
 * @param[in]       b                    - unsigned int value b
 * @return          None
 * @note            
 *        1. Operand is unsigned int little-endian.
 *        2. Operand is of 256 bits for SM2, SM9, etc.
 */
void pke_set_operand_uint32_value_256bits(unsigned int *baseaddr, unsigned int b);

/**
 * @brief           Check whether k is equal to (n-1), here n is odd.
 * @param[in]       k                    - big number k
 * @param[in]       n                    - big number n
 * @param[in]       words                - word length of k and n
 * @return          0 if k is n-1, other value if k is not n-1
 * @note            
 *        1. n must be odd.
 */
unsigned int is_k_equal_to_n_minus_1(const unsigned int *k, const unsigned int *n, unsigned int words);

#endif
