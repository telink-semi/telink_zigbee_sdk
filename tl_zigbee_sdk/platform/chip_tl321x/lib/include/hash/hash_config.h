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


//#define SUPPORT_HASH_SM3//hw not support
#define SUPPORT_HASH_MD5
#define SUPPORT_HASH_SHA256
//#define SUPPORT_HASH_SHA384//hw not support
//#define SUPPORT_HASH_SHA512//hw not support//Add a software implementation of the SHA-512 hash driver file to solve the problem of hardware not supporting SHA-512 hash.
#define SUPPORT_HASH_SHA1
#define SUPPORT_HASH_SHA224
//#define SUPPORT_HASH_SHA512_224//hw not support
//#define SUPPORT_HASH_SHA512_256//hw not support
//#define SUPPORT_HASH_SHA3_224 //hw not support
//#define SUPPORT_HASH_SHA3_256 //hw not support
//#define SUPPORT_HASH_SHA3_384 //hw not support
//#define SUPPORT_HASH_SHA3_512 //hw not support


//#define HASH_CPU_BIG_ENDIAN        //endian choice.
#define HASH_DMA_FUNCTION
//support multiple thread function
#define CONFIG_HASH_SUPPORT_MUL_THREAD
//support node style
#define SUPPORT_HASH_NODE
//support PBKDF2
#define SUPPORT_PBKDF2
//support ansi_x9.63_kdf
#define SUPPORT_ANSI_X9_63_KDF
#define HMAC_SECURE_PORT_FUNCTION


#ifdef SUPPORT_PBKDF2
    #define PBKDF2_HIGH_SPEED
#endif

#ifdef HASH_DMA_FUNCTION
//extern int buf_hash_dma[0x600];
    //#define CONFIG_HASH_SUPPORT_ADDRESS_HIGH_LOW
//    #define HASH_DMA_RAM_BASE (buf_hash_dma)
    //support dma node style
    #define SUPPORT_HASH_DMA_NODE
#endif

#ifdef SUPPORT_PBKDF2
    #define PBKDF2_HIGH_SPEED
#endif

#endif /* DRIVERS_LIB_INCLUDE_HASH_HASH_CONFIG_H_ */
