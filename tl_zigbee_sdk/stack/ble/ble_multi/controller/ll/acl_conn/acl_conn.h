/********************************************************************************************************
 * @file    acl_conn.h
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
#ifndef ACL_CONN_H_
#define ACL_CONN_H_

#define blc_ll_getCurrentMasterRoleNumber   blc_ll_getCurrentCentralRoleNumber
#define blc_ll_getCurrentSlaveRoleNumber    blc_ll_getCurrentPeripheralRoleNumber
/**
 * @brief      This function is used to get the current number of ACL connections.
 * @param[in]  none.
 * @return     The number of currently connected ACLs.
 */
int blc_ll_getCurrentConnectionNumber(void); //master + slave connection number


/**
 * @brief      This function is used to obtain the maximum number of connections that can be supported.
 * @param[in]  none.
 * @return     Maximum number of connections that can be supported.
 */
int blc_ll_getSupportedMaxConnNumber(void);


/**
 * @brief      This function is used to obtain the number of ACL connections of the Master role.
 * @param[in]  none.
 * @return     The number of currently connected master ACLs.
 */
int blc_ll_getCurrentCentralRoleNumber(void); //master role number


/**
 * @brief      This function is used to obtain the number of ACL connections of the Slave role.
 * @param[in]  none.
 * @return     The number of currently connected slave ACLs.
 */
int blc_ll_getCurrentPeripheralRoleNumber(void); //slave  role number


/**
 * @brief      This function is used to configure the number of master and slave connections that the protocol stack can support.
 * @param[in]  max_master_num - Number of master ACL connections supported.
 * @param[in]  max_slave_num - Number of slave ACL connections supported.
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_setMaxConnectionNumber(int max_master_num, int max_slave_num);


/**
 * @brief      This function is used to obtain the currently available TX FIFO numbers according to the ACL handle.
 * @param[in]  connHandle - ACL connection handle.
 * @return     available TX FIFO numbers
 */
u8 blc_ll_getTxFifoNumber(u16 connHandle);


/**
 * @brief      This function is used to disconnect the device with the specified ACL connection handle and specify the reason for the disconnection.
 * @param[in]  connHandle - ACL connection handle.
 * @param[in]  reason - Reason for disconnection..
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_disconnect(u16 connHandle, u8 reason);


/**
 * @brief      This function is used to obtain the connection establishment time point corresponding to the current ACL connection handle.
 * @param[in]  connHandle - ACL connection handle.
 * @param[in]  reason - Reason for disconnection..
 * @return     The connection establishment time point corresponding to the current ACL connection handle: Based on 16M system clock ticks.
 */
u32 blc_ll_getConnectionStartTick(u16 connHandle);


/**
 * @brief      This function is used to read remote version with the specified ACL connection handle.
 * @param[in]  connHandle - ACL connection handle.
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_readRemoteVersion(u16 connHandle);


/**
 * @brief      for user to initialize ACL connection module, this is must if user want use ACL master role or ACL slave role.
 * @param      none
 * @return     none
 */
void blc_ll_initAclConnection_module(void);


/**
 * @brief      for user to initialize LinkLayer ACL connection RX FIFO.
 *             all connection will share the FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_initAclConnRxFifo(u8 *pRxbuf, int fifo_size, int fifo_number);


/**
 * @brief      this function is used to initialize cache Tx FIFO.
 * @param      pTxbuf  - TX FIFO buffer address.
 * @param      fifo_size - Tx FIFO size
 * @param      fifo_number - TX FIFO number, can only be 16 or 32
 * @return     status, 0x00:  succeed, no buffer error
 *                     other: buffer error code
 */
ble_sts_t blc_ll_initAclConnCacheTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number);

/**
 * @brief      check ACL whether is in establish state.
 * @param[in]  connHandle - ACL connection handle.
 * @return     status, 0:  connection not established(disconnection or connection complete but not established)
 *                     1:  connection established state
 */
bool blc_ll_isAclConnEstablished(u16 connHandle);


/**
 * @brief      set ACL data length for Master and Slave.
 * @param[in]  maxRxOct - ACL max RX Oct.
 * @param[in]  maxTxOct_master - ACL master max TX Oct (Range:27 - 251)
 * @param[in]  maxTxOct_slave - ACL slave max TX Oct  (Range:27 - 251)
 * @return     status, 0x00 :  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_setAclConnMaxOctetsNumber(u8 maxRxOct, u8 maxTxOct_master, u8 maxTxOct_slave);


/**
 * @brief       For user to configure if auto exchange data length.
 *              if not call this API, this feature is enabled by default.
 * @param[in]   auto_dle_en - 1: auto exchange data length by stack; 0: user send length request on application layer
 * @return      none
 */
void blc_ll_setAutoExchangeDataLengthEnable(int auto_dle_en);


/**
 * @brief       This function is used to set ll_length_req pending timing after connection created
 * @param[in]   time_ms - pending timing, unit: ms
 * @return      none
 */
void blc_ll_setDataLengthReqSendingTime_after_connCreate(int time_ms);


/**
 * @brief       this function is used to set PHY type for connection
 * @param[in]   connHandle - Connection_Handle Range:0x0000 to 0x0EFF
 * @param[in]   all_phys - preference PHY for TX & RX
 * @param[in]   tx_phys - preference PHY for TX
 * @param[in]   rx_phys - preference PHY for RX
 * @param[in]   phy_options - LE coding indication prefer
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_setPhy(u16 connHandle, le_phy_prefer_mask_t all_phys, le_phy_prefer_type_t tx_phys, le_phy_prefer_type_t rx_phys, le_phy_option_prefer_t phy_options);


/**
 * @brief       This function is used to set LE Coded PHY preference, S2 or S8, or no specific preference.
 * @param[in]   prefer_CI - Reference structure: hci_le_readPhyCmd_retParam_t.
 * @return      status, 0x00:  succeed
 *                      other: failed
 */
ble_sts_t blc_ll_setDefaultConnCodingIndication(le_ci_prefer_t prefer_CI);


/**
 * @brief       this function is used to allows the Host to specify its preferred values for the transmitter PHY and
 *              receiver PHY to be used for all subsequent connections over the LE transport.
 * @param[in]   all_phys - Reference structure: le_phy_prefer_mask_t:
 *                         bit0: The Host has no preference among the transmitter PHYs supported by the Controller
 *                         bit1: The Host has no preference among the receiver PHYs supported by the Controller
 *                         All other bits: Reserved for future use
 * @param[in]   tx_phys - Reference structure: le_phy_prefer_mask_t:
 *                         bit0: The Host prefers to use the LE 1M transmitter PHY (possibly among others)
 *                         bit1: The Host prefers to use the LE 2M transmitter PHY (possibly among others)
 *                         bit2: The Host prefers to use the LE Coded transmitter PHY (possibly among others)
 *                         All other bits: Reserved for future use
 * @param[in]   rx_phys - Reference structure: le_phy_prefer_mask_t
 * @return      status, 0x00:  succeed
 *                      other: failed
 */
ble_sts_t blc_ll_setDefaultPhy(le_phy_prefer_mask_t all_phys, le_phy_prefer_type_t tx_phys, le_phy_prefer_type_t rx_phys);


/**
 * @brief       this function is used to read the current transmitter PHY and receiver PHY on the connection identified
 *              by the Connection_Handle.
 * @param[in]   connHandle - Connection_Handle Range:0x0000 to 0x0EFF
 * @param[out]  para - Reference structure: hci_le_readPhyCmd_retParam_t:
 *                     Status -  0x00 HCI_LE_Read_PHY command succeeded; 0x01 to 0xFF: HCI_LE_Read_PHY command failed
 *                     Connection_Handle - Connection_Handle Range:0x0000 to 0x0EFF
 *                     TX_PHY - 1:LE 1M; 2: LE 2M; 3: LE Coded.
 *                     RX_PHY - 1:LE 1M; 2: LE 2M; 3: LE Coded.
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_readPhy(u16 connHandle, hci_le_readPhyCmd_retParam_t *para);


/**
 * @brief      This function is used to obtain the latest average RSSI of ACL connections.
 * @param[in]  connHandle - ACL connection handle.
 * @return     The value of latest average RSSI.
 */
u8 blc_ll_getAclLatestAvgRSSI(u16 connHandle);

/**
 * @brief      This function is used to obtain the latest RSSI of ACL connections.
 * @param[in]  connHandle - ACL connection handle.
 * @return     The value of latest average RSSI.
 */
u8 blc_ll_getAclLatestRSSI(u16 connHandle);

/**
 * @brief      for user to read current ACL connection interval
 * @param[in]  connHandle - ACL connection handle.
 * @return     0    :  connHandle invalid, not match a connection
 *             other:  connection interval, unit: 1.25mS
 */
u16 blc_ll_getAclConnectionInterval(u16 connHandle);


/**
 * @brief      for user to read current ACL connection latency
 * @param[in]  connHandle - ACL connection handle.
 * @return     0    :  connHandle invalid, not match a connection
 *             other:  connection latency
 */
u16 blc_ll_getAclConnectionLatency(u16 connHandle);


/**
 * @brief      for user to read current ACL connection supervision timeout
 * @param[in]  connHandle - ACL connection handle.
 * @return     0    :  connHandle invalid, not match a connection
 *             other:  connection supervision timeout, unit: 10 mS
 */
u16 blc_ll_getAclConnectionTimeout(u16 connHandle);


/**
 * @brief      This function is used to disable DLE exchange automatically.
 *             User can call this API in connection callback(HCI_LE_EVT_MASK_CONNECTION_COMPLETE/HCI_SUB_EVT_LE_ENHANCED_CONNECTION_COMPLETE).
 * @param[in]  connHandle - ACL connection handle.
 * @return     none.
 */
void blc_ll_dataLenAutoExgDisable(u16 connHandle);

ble_sts_t blc_hci_readSuggestedDefaultTxDataLength(u8 *tx, u8 *txtime);
ble_sts_t blc_hci_writeSuggestedDefaultTxDataLength(u16 tx, u16 txtime);
ble_sts_t blc_hci_readMaximumDataLength(hci_le_readMaxDataLengthCmd_retParam_t *para);

#endif /* ACL_CONN_H_ */
