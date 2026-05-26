/********************************************************************************************************
 * @file    efuse.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef EFUSE_H_
#define EFUSE_H_

#include "error_handler/error_handler.h"

/**
* @brief      This function servers to get chip id from EFUSE.
* @param[in]  chip_id_buff - store chip id. Chip ID is 16 bytes.
* @return     DRV_API_SUCCESS: operation successful.
*             DRV_API_TIMEOUT: operation timeout.
*/
drv_api_status_e efuse_get_chip_id(unsigned char *chip_id_buff);

/**
 * @brief      This function serves to read IEEE address from EFUSE.
 * @param[out] buf  - Pointer to IEEE address buffer(IEEE address is 8bytes)
 * @return     DRV_API_SUCCESS: operation successful.
 *             DRV_API_TIMEOUT: operation timeout.
 */
drv_api_status_e efuse_get_ieee_addr(unsigned char *buf);
#endif
