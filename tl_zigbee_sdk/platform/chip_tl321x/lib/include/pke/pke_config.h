/********************************************************************************************************
 * @file    pke_config.h
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
#ifndef PKE_CONFIG_H
#define PKE_CONFIG_H


#include "lib/include/crypto_common/common_config.h"


#ifdef __cplusplus
extern "C"
{
#endif


/************************************************************************************
 *******************************    PKE config    ***********************************
 ************************************************************************************/

/*
 *function: define available PKE IP
 *caution:
 */
#define PKE_LP


/*
 *function: define secure version(hardware & software driver)
 *caution:
 *     1. pke lp not support sec version
 */
//#define PKE_SEC


/*
 *function: pke IP base address
 *caution:
 */
//#define PKE_BASE_ADDR            (0x80110000)  //PKE register base address


/*
 *function: some PKE algorithm operand maximum bit length
 *caution:
 */
#define OPERAND_MAX_BIT_LEN (4096u)
#define ECCP_MAX_BIT_LEN    (521u)
#define RSA_MAX_BIT_LEN     OPERAND_MAX_BIT_LEN
#define DH_MAX_BIT_LEN      OPERAND_MAX_BIT_LEN


/*
 *function: supported pke_lp algorithms
 *caution:
 */
#define SUPPORT_RSA
#define SUPPORT_DH
#define SUPPORT_ECDH
#define SUPPORT_ECDSA
#define SUPPORT_ECIES
//#define SUPPORT_SM2//hardware not support
//#define SUPPORT_SM9//hardware not support
#define SUPPORT_C25519


#ifdef SUPPORT_RSA
//#define SUPPORT_RSAES_OAEP
//#define SUPPORT_RSASSA_PSS //hardware not support
#endif


    /*
 *function: pre_mont style
 *caution:
 *     1. if close this micro, all pre_mont for modexp through hardware
 *     2. if open this micro, some pre_mont for modexp through software
 *     3. depending on the compiler, there may be cases that software calc faster than hardware
 *     4. closed by default
 */
    //#define PKE_CONFIG_ALL_MODEXP_PRE_CALC_WITH_MGMR_MICROCODE


#ifdef PKE_SEC

    #ifdef SUPPORT_RSA
        #define RSA_SEC
    #endif

    #ifdef SUPPORT_DH
        #define DH_SEC
    #endif

    #ifdef SUPPORT_ECDH
        #define ECDH_SEC
    #endif

    #ifdef SUPPORT_ECDSA
        #define ECDSA_SEC
    #endif

    #ifdef SUPPORT_SM2
        #define SM2_SEC
    #endif

    #ifdef SUPPORT_SM9
        #define SM9_SEC
    #endif

#endif


#ifdef __cplusplus
}
#endif


#endif
