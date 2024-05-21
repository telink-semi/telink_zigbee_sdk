/********************************************************************************************************
 * @file    error_handler.h
 *
 * @brief   This is the header file for B92
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
#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

#include "lib/include/sys.h"
#include "clock.h"

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
/**
 *  @note Time out settings for all modules.
*/
#define TIMEOUT_US			5000000

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 *  @brief  Timeout error code.
 */
typedef enum {
	ERROR_NONE						= 0,
	ERROR_TIMEOUT_RC_24M_CAL		= 0x01,
	ERROR_TIMEOUT_RC_32K_CAL		= 0x02,
	ERROR_TIMEOUT_MSPI_WAIT			= 0x03,
	ERROR_TIMEOUT_ANALOG_WAIT		= 0x04,
	ERROR_TIMEOUT_ANA_TX_BUFCNT		= 0x05,
} error_code_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern error_code_e g_error_code;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief     This function serves to return the error code.
 * @return    none.
 */
error_code_e get_error_code(void);

/**
 * @brief     This function serves to record the error code and restart the system.
 * @param[in] error_code - The error code indicating the type of timeout error.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario.
 */
__attribute__((weak)) void timeout_handler(error_code_e error_code);


#endif
