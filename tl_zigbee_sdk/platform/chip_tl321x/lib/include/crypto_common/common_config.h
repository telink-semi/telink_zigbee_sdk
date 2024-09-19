/********************************************************************************************************
 * @file    common_config.h
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
#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H


#include <stdint.h>    //including definitions of int32_t, unsigned int, etc.

#include <string.h>    //including definition of NULL




/************************************************************************************
 ******************************    common config    *********************************
 ************************************************************************************/

//print buffer functions
#define UTILITY_PRINT_BUF

#ifdef UTILITY_PRINT_BUF
#include <stdio.h>
#endif

#define UTILITY_SEC


//only one of the following two macro could be enabled
#define MEM_VOLATILE
//#define MEM_VOLATILE volatile


//#define CONFIG_UNIT_TEST

//C keyword static, keep it by default, or define it empty just for some testing.
#ifdef CONFIG_UNIT_TEST
#define FLAG_STATIC
#else
#define FLAG_STATIC   static   //default
#endif


#define SUPPORT_STATIC_ANALYSIS




#endif

