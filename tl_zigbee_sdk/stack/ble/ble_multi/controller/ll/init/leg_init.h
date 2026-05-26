/********************************************************************************************************
 * @file    leg_init.h
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
#ifndef LLMS_INIT_H_
#define LLMS_INIT_H_

#include "stack/ble/ble_multi/hci/hci_cmd.h"


/**
 * @brief      for user to initialize legacy initiating module
 *             notice that only one module can be selected between legacy initiating module and extended initiating module
 * @param      none
 * @return     none
 */
void blc_ll_initLegacyInitiating_module(void);


/**
 * @brief      This function is used to create an ACL connection to a connectable advertiser.
 * @param[in]  scan_interval - recommendations from the Host on how frequently (LE_Scan_Interval) the Controller should scan.
 * @param[in]  scan_window - recommendations from the Host on how long (LE_Scan_Window) the Controller should scan.
 * @param[in]  filter_policy - used to determine whether the White List is used.
 * @param[in]  adr_type - indicates the type of address used in the connectable advertisement sent by the peer.
 * @param[in]  *mac - indicates the Peer's Public Device Address.
 * @param[in]  own_adr_type - indicates the type of address being used in the connection request packets.
 * @param[in]  conn_min - the minimum allowed connection interval.
 * @param[in]  conn_max - the maximum allowed connection interval.
 * @param[in]  conn_latency - he maximum allowed connection latency
 * @param[in]  timeout - defines the link supervision timeout for the connection.
 * @param[in]  ce_min - informative parameters providing the Controller with the expected minimum length of the connection events.
 * @param[in]  ce_max - informative parameters providing the Controller with the expected maximum length of the connection events.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_createConnection(scan_inter_t scanInter, scan_wind_t scanWindow, init_fp_t fp, u8 peerAdrType, u8 *peerAddr, own_addr_type_t ownAdrType, conn_inter_t conn_min, conn_inter_t conn_max, u16 conn_latency, conn_tm_t timeout, u16 ce_min, u16 ce_max);


#endif /* LLMS_INIT_H_ */
