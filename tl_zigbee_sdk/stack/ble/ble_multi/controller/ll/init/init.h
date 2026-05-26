/********************************************************************************************************
 * @file    init.h
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
#ifndef INIT_H_
#define INIT_H_


/**
 * @brief      This function is used to cancel the HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection commands.
 *             This command shall only be issued after the HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection commands have been issued.
 * @param      none
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_createConnectionCancel(void);


/**
 * @brief      This function is used to set the timeout of ACL connection establishment
 * @param[in]  timeout_ms - timeout of ACL connection establishment, unit: mS. If not set, default value is 4000 mS
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setCreateConnectionTimeout(u32 timeout_ms);


/**
 * @brief      This function is used to check if initiation is busy
 * @param[in]  none
 * @return     1: initiation is ongoing;  0: no initiation or initiation finished
 */
bool blc_ll_isInitiationBusy(void);


/**
 * @brief      This function is used to set create connection retry number
 * @param[in]  number - retry number, must be in range of 0 ~ 5
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setCreateConnectionRetryNumber(u8 number);


/**
 * @brief      disable connection update high authority for master.
 * @param[in]  none
 * @return     none
 */
void blc_ll_disableConnUpdHighAuthority(void);

#endif /* INIT_H_ */
