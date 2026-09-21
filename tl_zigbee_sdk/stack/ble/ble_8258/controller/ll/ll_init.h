/********************************************************************************************************
 * @file    ll_init.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
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
#ifndef LL_INIT_H_
#define LL_INIT_H_



/**
 * @brief      This function is used to initialize legacy initiating module
 * 			   notice that only one module can be selected between legacy initiating module and extended initiating module
 * @param	   none
 * @return     none
 */
void	 	blc_ll_initInitiating_module(void);



/**
 * @brief      This function is used to create an ACL connection to a connectable advertiser.
 * 			   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.12 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  scan_interval - recommendations from the Host on how frequently (LE_Scan_Interval) the Controller should scan.
 * @param[in]  scan_window - recommendations from the Host on how long (LE_Scan_Window) the Controller should scan.
 * @param[in]  filter_policy - used to determine whether the White List is used.
 * @param[in]  adr_type - indicates the type of address used in the connectable advertisement sent by the peer.
 * @param[in]  mac - indicates the Peer's Public Device Address.
 * @param[in]  own_adr_type - indicates the type of address being used in the connection request packets.
 * @param[in]  conn_min - the minimum allowed connection interval.
 * @param[in]  conn_max - the maximum allowed connection interval.
 * @param[in]  conn_latency - he maximum allowed connection latency
 * @param[in]  timeout - defines the link supervision timeout for the connection.
 * @param[in]  ce_min - informative parameters providing the Controller with the expected minimum length of the connection events.
 * @param[in]  ce_max - informative parameters providing the Controller with the expected maximum length of the connection events.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_ll_createConnection( scan_inter_t scanInter, scan_wind_t scanWindow, init_fp_t initiator_fp, u8 peerAdrType,    u8 *peerAddr, own_addr_type_t ownAdrType,
									 conn_inter_t conn_min,  conn_inter_t conn_max,  u16 conn_latency,       conn_tm_t timeout, u16 ce_min,   u16 ce_max );


/**
 * @brief      This function is used to cancel create connection process .
 * 			   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.13 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param	   none
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_createConnectionCancel (void);


/**
 * @brief      This function is used to set create connection process timeout.
 * @param	   timeout_ms - timeout threshold
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t   blc_ll_setCreateConnectionTimeout (u32 timeout_ms);



#endif /* LL_INIT_H_ */
