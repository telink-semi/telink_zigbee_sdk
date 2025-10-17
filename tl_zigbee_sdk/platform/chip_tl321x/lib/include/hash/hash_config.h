/********************************************************************************************************
 * @file    hash_config.h
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
#ifndef DRIVERS_LIB_INCLUDE_HASH_HASH_CONFIG_H_
#define DRIVERS_LIB_INCLUDE_HASH_HASH_CONFIG_H_
/************************************************************************************
 *******************************    HASH config    **********************************
 ************************************************************************************/

//hash IP base address
// #define HASH_BASE_ADDR            (0x80000000U)//(0x43C00000U)  //HASH register base address

//supported hash algorithms.
#include "chip_config.h"
//#define SUPPORT_HASH_SM3//hw not support
#if(COMPATIBLE_WITH_TL321X_AND_TL323X == 0)
#define SUPPORT_HASH_MD5
#endif
#define SUPPORT_HASH_SHA256
// #define SUPPORT_HASH_SHA384
// #define SUPPORT_HASH_SHA512
#define SUPPORT_HASH_SHA1
#define SUPPORT_HASH_SHA224
// #define SUPPORT_HASH_SHA512_224
// #define SUPPORT_HASH_SHA512_256
//#define SUPPORT_HASH_SHA3_224 //hw not support
//#define SUPPORT_HASH_SHA3_256 //hw not support
//#define SUPPORT_HASH_SHA3_384 //hw not support
//#define SUPPORT_HASH_SHA3_512 //hw not support

//#define HASH_CPU_BIG_ENDIAN        //endian choice.

//#define HMAC_SECURE_PORT_FUNCTION

//support hash dma function
#define HASH_DMA_FUNCTION

#ifdef HASH_DMA_FUNCTION
//DMA in/out address with high 32bit and low 32bit
//#define CONFIG_HASH_SUPPORT_ADDRESS_HIGH_LOW

//ram for hash dma, just for temporary use
// #define HASH_DMA_RAM_BASE         (0x60000000U)//(0x80000000U)
#endif

//support multiple thread function
// #define CONFIG_HASH_SUPPORT_MUL_THREAD

//support node style
#define SUPPORT_HASH_NODE

#ifdef HASH_DMA_FUNCTION
//support dma node style
#define SUPPORT_HASH_DMA_NODE
#endif

//support PBKDF2
#define SUPPORT_PBKDF2

#ifdef SUPPORT_PBKDF2
#define PBKDF2_HIGH_SPEED
#endif

//support ansi_x9.63_kdf
#define SUPPORT_ANSI_X9_63_KDF






#endif/* DRIVERS_LIB_INCLUDE_HASH_HASH_CONFIG_H_ */

