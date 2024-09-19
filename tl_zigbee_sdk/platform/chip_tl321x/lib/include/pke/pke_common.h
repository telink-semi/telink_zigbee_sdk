/********************************************************************************************************
 * @file    pke_common.h
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
#ifndef PKE_COMMON_H
#define PKE_COMMON_H



#include "lib/include/pke/pke_config.h"
#include "reg_include/pke_reg.h"




//ECC point conversion form
#define POINT_COMPRESSED          (0x02U)   //pc||x, pc = 0x02|LSB(y)
#define POINT_UNCOMPRESSED        (0x04U)   //pc||x||y, pc=0x04
typedef unsigned char EC_POINT_FORM;



//define KDF
typedef void *(*KDF_FUNC)(const void *input, unsigned int byteLen, unsigned char *key, unsigned int keyByteLen);


//APIs

/**
 * @brief       load input operand to baseaddr
 * @param[out]  baseaddr     - destination data
 * @param[in]   data         - source data
 * @param[in]   wordlen      - word length of data
 * @return      0:success     other:error
 */
void pke_load_operand(unsigned int *baseaddr, const unsigned int *data, unsigned int wordLen);

/**
 * @brief       get result operand from baseaddr
 * @param[out]  baseaddr     - source data
 * @param[in]   data         - destination data
 * @param[in]   wordlen      - word length of data
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.operands are both U32 little-endian
  @endverbatim
 */
void pke_read_operand(unsigned int *baseaddr, unsigned int *data, unsigned int wordLen);

/**
 * @brief       load input operand(U8 big-endian) to baseaddr
 * @param[out]  baseaddr     - destination data
 * @param[in]   data         - source data, U8 big-endian
 * @param[in]   byteLen      - byte length of data
 * @return      0:success     other:error
 */
void pke_load_operand_U8(unsigned int *baseaddr, const unsigned char *data, unsigned int byteLen);
/**
 * @brief       get result operand(U8 big-endian) from baseaddr
 * @param[in]   baseaddr     - source data
 * @param[out]  data         - destination data, U8 big-endian
 * @param[in]   byteLen      - byte length of data
 * @return      0:success     other:error
 */
void pke_read_operand_U8(unsigned int *baseaddr, unsigned char *data, unsigned int byteLen);

/**
 * @brief       set operand with an unsigned int value
 * @param[out]  baseaddr      - operand
 * @param[in]   wordLen       - word length of operand
 * @param[in]   b             - unsigned int value b
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.wordLen can not be 0
  @endverbatim
 */
void pke_set_operand_uint32_value(unsigned int *baseaddr, unsigned int wordLen, unsigned int b);



#endif

