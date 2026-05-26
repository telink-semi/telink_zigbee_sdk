/********************************************************************************************************
 * @file    ext_init.h
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
#ifndef EXT_INIT_H_
#define EXT_INIT_H_


/**
 * @brief      for user to initialize extended initiating module
 *             notice that only one module can be selected between legacy initiating module and extended initiating module
 * @param      none
 * @return     none
 */
void blc_ll_initExtendedInitiating_module(void);


/**
 * @brief      This function is used to create an ACL connection to a connectable advertiser.
 *
 * @param[in]  filter_policy - used to determine whether the WhiteList is used. If the White List is not used, the Peer_Address_Type and the
                               Peer_Address parameters specify the address type and address of the advertising device to connect to.
 * @param[in]  ownAdr_type - indicates the type of address being used in the connection request packets.
 * @param[in]  peerAdrType - indicates the type of address used in the connectable advertisement sent by the peer.
 * @param[in]  *peerAddr - indicates the Peer's Public Device Address, Random (static) Device Address, Non-Resolvable Private Address, or
                            Resolvable Private Address depending on the Peer_Address_Type parameter.
 * @param[in]  init_phys - indicates the PHY(s) on which the advertising packets should be received on the primary advertising physical channel and
                            the PHYs for which connection parameters have been specified.
 *
 *             Attention:
 *             scanInter_0/scanWindow_0/conn_min_0/conn_max_0/timeout_0 are only for    1M PHY.  If    1M PHY is not supported, these parameters are ignored.
 *             scanInter_1/scanWindow_1/conn_min_1/conn_max_1/timeout_1 are only for    2M PHY.  If    2M PHY is not supported, these parameters are ignored.
 *             scanInter_2/scanWindow_2/conn_min_2/conn_max_2/timeout_2 are only for Coded PHY.  If Coded PHY is not supported, these parameters are ignored.
 *
 * @param[in]  scanInter_0 - for 1M PHY: recommendations from the Host on how frequently (LE_Scan_Interval) the Controller should scan.
 * @param[in]  scanWindow_0 - for 1M PHY: recommendations from the Host on how long (LE_Scan_Window) the Controller should scan.
 * @param[in]  conn_min_0 - for 1M PHY: the minimum allowed connection interval.
 * @param[in]  conn_max_0 - for 1M PHY: the maximum allowed connection interval.
 * @param[in]  timeout_0 - for 1M PHY: Supervision timeout for the LE Link.
 * @param[in]  scanInter_1 - for 2M PHY: recommendations from the Host on how frequently (LE_Scan_Interval) the Controller should scan.
 * @param[in]  scanWindow_1 - for 2M PHY: recommendations from the Host on how long (LE_Scan_Window) the Controller should scan.
 * @param[in]  conn_min_1 - for 2M PHY: the minimum allowed connection interval.
 * @param[in]  conn_max_1 - for 2M PHY: the maximum allowed connection interval.
 * @param[in]  timeout_1 - for 2M PHY: Supervision timeout for the LE Link.
 * @param[in]  scanInter_2 - for Coded PHY: recommendations from the Host on how frequently (LE_Scan_Interval) the Controller should scan.
 * @param[in]  scanWindow_2 - for Coded PHY: recommendations from the Host on how long (LE_Scan_Window) the Controller should scan.
 * @param[in]  conn_min_2 - for Coded PHY: the minimum allowed connection interval.
 * @param[in]  conn_max_2 - for Coded PHY: the maximum allowed connection interval.
 * @param[in]  timeout_2 - for Coded PHY: Supervision timeout for the LE Link.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_extended_createConnection(init_fp_t filter_policy, own_addr_type_t ownAdrType, u8 peerAdrType, u8 *peerAddr, init_phy_t init_phys, scan_inter_t scanInter_0, scan_wind_t scanWindow_0, conn_inter_t conn_min_0, conn_inter_t conn_max_0, conn_tm_t timeout_0, scan_inter_t scanInter_1, scan_wind_t scanWindow_1, conn_inter_t conn_min_1, conn_inter_t conn_max_1, conn_tm_t timeout_1, scan_inter_t scanInter_2, scan_wind_t scanWindow_2, conn_inter_t conn_min_2, conn_inter_t conn_max_2, conn_tm_t timeout_2);


#endif /* EXT_INIT_H_ */
