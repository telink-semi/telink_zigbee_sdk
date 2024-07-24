/********************************************************************************************************
 * @file    error_handler.c
 *
 * @brief   This is the source file for B92
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

#include "error_handler/error_handler.h"

/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
/**
 *  @note Time out settings,this variable is applicable to the module that does reboot handle when the times out,can set it by drv_set_error_timeout().
*/
unsigned int g_drv_api_error_timeout_us = 0xffffffff;

drv_api_error_code_e g_error_code = DRV_API_ERROR_NONE;

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

/**
 * @brief     This function serves to return the error code.
 * @return    none.
 */
drv_api_error_code_e drv_get_error_code(void)
{
    return g_error_code;
}

/**
 * @brief     This function serves to record the error code and restart the system.
 * @param[in] error_code - The error code indicating the type of timeout error.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario.
 */
__attribute__((weak)) void drv_timeout_handler(unsigned int error_code)
{
    g_error_code = error_code;
    sys_reboot();
}

/**
 * @brief     This function serves to set the error timeout(us).
 * @param[in] timeout_us - the error timeout(us).
 * @return    none.
 */
void drv_set_error_timeout(unsigned int timeout_us){
	g_drv_api_error_timeout_us = timeout_us;
}
