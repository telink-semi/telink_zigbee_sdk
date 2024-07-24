/********************************************************************************************************
 * @file    trng.h
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
/**	@page TRNG
 *
 *	Introduction
 *	===============
 *	B92 supports hardware trng.
 *
 *	API Reference
 *	===============
 *	Header File: trng.h
 */
#ifndef TRNG_H_
#define TRNG_H_

#include "lib/include/sys.h"
#include "reg_include/register.h"
#include "stimer.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * trng error timeout(us),a large value is set by default,can set it by trng_set_error_timeout().
 */
extern unsigned int g_trng_error_timeout_us;

/**
 * @brief     This function serves to trng hardware reset(the hardware device is reset to restore it to its initial state (restart the device), the initialization needs to be reconfigured).
 * @return    none.
 */
void trng_hw_reset(void);

/**
 * @brief     This function serves to set the trng timeout(us).
 * @param[in] timeout_us - the timeout(us).
 * @return    none.
 * @note      The default timeout (g_trng_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
 *            g_trng_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
 *            g_trng_error_timeout_us the minimum time must meet the following conditions:
 *            1. at least 1ms;
 *            2. maximum interrupt processing time;
 */
void trng_set_error_timeout(unsigned int timeout_us);

/**
 * @brief     This function serves to record the api status.
 * @param[in] trng_error_timeout_code - trng_api_error_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario,The parameters of the interface are useless(only one reason for an error,do not need to use enumeration to distinguish it),
 *            if record the details of the reason, can implement it by yourself,trng_hw_reset must be called,the.c static library, shown here is the drive implementation part of the interface:
 *            __attribute__((weak)) void trng_timeout_handler(unsigned int trng_error_timeout_code)
 *            {
 *	             (void)trng_error_timeout_code;
 *               trng_hw_reset();
 *            }
 *
 */
__attribute__((weak)) void trng_timeout_handler(unsigned int trng_error_timeout_code);

/**
 * @brief     This function performs to get one random number.If chip in suspend TRNG module should be close.
 *            else its current will be larger.
 * @return    DRV_API_SUCCESS: operation successful;
 *            DRV_API_TIMEOUT: timeout exit(g_trng_error_timeout_us refer to the note for trng_set_error_timeout,the solution processing is already done in trng_timeout_handler, so just re-invoke the interface);
 */
drv_api_status_e trng_init(void);

/**
 * @brief     This function performs to get one random number.
 * @return    the value of one random number
 **/
unsigned int trng_rand(void);

#endif
