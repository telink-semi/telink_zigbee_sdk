/********************************************************************************************************
 * @file    ll_master.h
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
#ifndef LL_MASTER_H_
#define LL_MASTER_H_




/**
 * @brief      for user to initialize ACL connection in master role.
 * @param[in]  none
 * @return     none
 */
void		blc_ll_initMasterRoleSingleConn_module(void);


/**
 * @brief      for user to check if RF Machine is busy.
 * @param[in]  none
 * @return     status. 1:  busy
 * 					   other: not busy
 */
bool		blm_ll_isRfStateMachineBusy(void);


/**
 * @brief      for user to start disconnection process in master role .
 * @param[in]  handle - connect handle
 * @param[in]  reason - terminate reason
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_disconnect (u16 handle, u8 reason);


/**
 * @brief      for user to start update parameter process in master role .
 * 			   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.18 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  connHandle - connect handle
 * @param[in]  conn_min - minimum connection interval
 * @param[in]  conn_max - maximum connection interval
 * @param[in]  conn_latency - connection latency
 * @param[in]  timeout - connection timeout
 * @param[in]  ce_min - not supported
 * @param[in]  ce_max - not supported
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_updateConnection (u16 connHandle,
							  u16 conn_min, u16 conn_max, u16 conn_latency, u16 timeout,
							  u16 ce_min, u16 ce_max );


/**
 * @brief      for user to start update channel map process in master role .
 * @param[in]  handle - connect handle
 * @param[in]  map - pointer of channel map
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_setHostChannel (u16 handle, u8 * map);


/**
 * @brief      l2cap layer processing function in master role.
 * @param[in]  conn - connect handle
 * @param[in]  raw_pkt - pointer of packet
 * @return     address of l2cap layer packet
 */
rf_packet_l2cap_t *		blm_l2cap_packet_pack (u16 conn, u8 * raw_pkt);


/**
 * @brief      for user to read remote feature in master role .
 * @param[in]  handle - connect handle
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blm_ll_readRemoteFeature (u16 handle);



#endif /* LL_MASTER_H_ */
