/********************************************************************************************************
 * @file    whitelist.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef LL_WHITELIST_H_
#define LL_WHITELIST_H_


/**
 * @brief      This function is used to clear WhiteList
 * @param[in]  none
 * @return     Status - 0x00: command succeeded;
 *  *                   Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ll_clearWhiteList(void);


/**
 * @brief      This function is used to add a device to WhiteList
 *             attention: maximum device number in Whitelist list is 4 by default. Please contact Telink if this number can not meet your requirement.
 * @param[in]  adr_type - device address type
 * @param[in]  addr - device address
 * @return     Status - 0x00: command succeeded;
 *                      Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ll_addDeviceToWhiteList(u8 adr_type, u8 *addr);


/**
 * @brief      This function is used to delete a device from WhiteList
 * @param[in]  adr_type - device address type
 * @param[in]  addr - device address
 * @return     Status - 0x00: command succeeded;
 *                      Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ll_removeDeviceFromWhiteList(u8 adr_type, u8 *addr);


#endif /* LL_WHITELIST_H_ */
