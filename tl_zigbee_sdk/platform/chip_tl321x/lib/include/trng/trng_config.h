/********************************************************************************************************
 * @file    trng_config.h
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
#ifndef TRNG_CONFIG_H
#define TRNG_CONFIG_H


#include "lib/include/crypto_common/common_config.h"

#include <stdio.h>


/************************************************************************************
 *******************************    TRNG config    **********************************
 ************************************************************************************/

//trng IP base address
#define TRNG_BASE_ADDR (0x80103000U) //TRNG register base address

#define CONFIG_TRNG_GENERATE_BY_HARDWARE

#define TRNG_RO_ENTROPY //enable Ring Oscillator entropy
//#define TRNG_TERO_ENTROPY        //enable Transient Effect Ring Oscillator entropy

#define TRNG_DELAY_COUNTER             (10U)
#define TRNG_TIMEOUT_COUNTER_THRESHOLD (100000U)
#define TRNG_ERROR_COUNTER_THRESHOLD   (20U)


#ifdef __cplusplus
}
#endif


#endif
