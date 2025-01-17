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
 *                                           global variable                                                             *
 *********************************************************************************************************************/
/**
 *  @note Time out settings,this variable is applicable to the module that does reboot handle when the times out,can set it by drv_set_error_timeout().
*/
extern unsigned int g_drv_api_error_timeout_us;

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 *  @note:For newly added interfaces, the corresponding status is returned according to the enumeration.
 *        For earlier interfaces, only the newly returned status is returned using the enumeration for compatibility issues.
 */
typedef enum
{
    DRV_API_SUCCESS       = 0x00,
    DRV_API_FAILURE       = 0x01,
    DRV_API_TIMEOUT       = 0x02,
    DRV_API_INVALID_PARAM = 0x03
} drv_api_status_e;

/**
 *  @brief  Timeout error code,this enumeration is applicable to the module that does reboot handle when the times out.
 */
typedef enum
{
    DRV_API_ERROR_NONE                  = 0,
    DRV_API_ERROR_TIMEOUT_RC_24M_CAL    = 0x01,
    DRV_API_ERROR_TIMEOUT_RC_32K_CAL    = 0x02,
    DRV_API_ERROR_TIMEOUT_MSPI_WAIT     = 0x03,
    DRV_API_ERROR_TIMEOUT_ANALOG_WAIT   = 0x04,
    DRV_API_ERROR_TIMEOUT_ANA_TX_BUFCNT = 0x05,
    DRV_API_ERROR_TIMEOUT_PLL_DONE      = 0x06,
} drv_api_error_code_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern drv_api_error_code_e g_drv_api_error_code;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief     This function serves to return the error code.
 * @return    none.
 */
drv_api_error_code_e drv_get_error_code(void);

/**
 * @brief     This function serves to record the error code and restart the system.
 * @param[in] error_code - The error code indicating the type of timeout error.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario.
 */
__attribute__((weak)) _attribute_ram_code_sec_noinline_ void drv_timeout_handler(unsigned int error_code);

/**
 * @brief     This function serves to set the error timeout(us).
 * @param[in] timeout_us - the error timeout(us).
 * @return    none.
 */
void drv_set_error_timeout(unsigned int timeout_us);
#endif
