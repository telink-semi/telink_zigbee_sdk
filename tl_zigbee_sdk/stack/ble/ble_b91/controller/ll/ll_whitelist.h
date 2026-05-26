/********************************************************************************************************
 * @file     ll_whitelist.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#ifndef LL_WHITELIST_H_
#define LL_WHITELIST_H_

#include "stack/ble/ble_b91/ble_common.h"


/**
 * @brief      reset whitelist
 * @param[in]  none
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t ll_whiteList_reset(void);


/**
 * @brief      add a device form whitelist
 * @param[in]  type - device mac address type
 * @param[in]  addr - device mac address
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t ll_whiteList_add(u8 type, u8 *addr);


/**
 * @brief      delete a device from whitelist
 * @param[in]  type - device mac address type
 * @param[in]  addr - device mac address
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t ll_whiteList_delete(u8 type, u8 *addr);




/**
 * @brief      get whitelist size
 * @param[out] pointer to size
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t ll_whiteList_getSize(u8 *returnPublicAddrListSize) ;




#endif /* LL_WHITELIST_H_ */
