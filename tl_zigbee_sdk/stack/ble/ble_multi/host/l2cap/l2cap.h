/********************************************************************************************************
 * @file    l2cap.h
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
#ifndef L2CAP_H_
#define L2CAP_H_


#define L2CAP_PSM_EATT 0x0027
#define L2CAP_PSM_ATT  0x001f

typedef enum
{
    CONN_PARAM_UPDATE_ACCEPT = 0x0000,
    CONN_PARAM_UPDATE_REJECT = 0x0001,
} conn_para_up_rsp;

/**
 * @brief       initialize l2cap buffer to reassembly link lay PDU to SDU in ACL Central
 * @param[in]   pRxBuf - the pointer of rx buffer in ACL Central
 * @param[in]   rx_buf_size   - the size of of rx buffer in ACL Central
 * @param[in]   pTxBuf - the pointer of tx buffer in ACL Central
 * @param[in]   tx_buf_size   - the size of of tx buffer in ACL Central
 * @return      none.
 */
ble_sts_t blc_l2cap_initAclCentralBuffer(u8 *pRxBuf, u16 rx_buf_size, u8 *pTxBuf, u16 tx_buf_size);


/**
 * @brief       initialize l2cap buffer to reassembly link lay PDU to SDU in ACL Peripheral
 * @param[in]   pRxBuf - the pointer of rx buffer in ACL Peripheral
 * @param[in]   rx_buf_size   - the size of of rx buffer in ACL Peripheral
 * @param[in]   pTxBuf - the pointer of tx buffer in ACL Peripheral
 * @param[in]   tx_buf_size   - the size of of tx buffer in ACL Peripheral
 * @return  none.
 */
ble_sts_t blc_l2cap_initAclPeripheralBuffer(u8 *pRxBuf, u16 rx_buf_size, u8 *pTxBuf, u16 tx_buf_size);


/**
 * @brief       This function is used to set connect request parameter for updating connect parameter in slave
 * @param[in]   connHandle - connection handle
 * @param[in]   min_interval - connect interval minimum
 * @param[in]   max_interval - connect interval maximum
 * @param[in]   latency - connect latency
 * @param[in]   timeout - connect timeout
 * @return      0: success
 *              1: fail
 */
u8 bls_l2cap_requestConnParamUpdate(u16 connHandle, u16 min_interval, u16 max_interval, u16 latency, u16 timeout);


/**
 * @brief       This function is used to send connect parameter update response in master
 * @param[in]   connHandle - connection handle
 * @param[in]   req_id - Request packet identifier
 * @param[in]   result - connect parameter update result
 *              0x0001: CONN_PARAM_UPDATE_REJECT
 *              0x0000: CONN_PARAM_UPDATE_ACCEPT, need to call the API blm_l2cap_processConnParamUpdatePending() later
 * @return      none.
 */
void blc_l2cap_SendConnParamUpdateResponse(u16 connHandle, u8 req_id, conn_para_up_rsp result);


/**
 * @brief       This function is used to host set connect parameter process pending in master
 * @param[in]   connHandle - connection handle
 * @param[in]   min_interval - connect interval minimum
 * @param[in]   max_interval - connect interval maximum
 * @param[in]   latency - connect latency
 * @param[in]   timeout - connect timeout
 * @return      none.
 */
void blm_l2cap_processConnParamUpdatePending(u16 connHandle, u16 min_interval, u16 max_interval, u16 latency, u16 timeout);


/**
 * @brief       This function is used to set the minimal time for send connect parameter update request after connect created
 * @param[in]   connHandle - connection handle
 * @param[in]   time_ms - the unit is millisecond
 * @return      0: success
 *              1: fail
 */
u8 bls_l2cap_setMinimalUpdateReqSendingTime_after_connCreate(u16 connHandle, int time_ms);


/**
 * @brief   This function is used to handler L2CAP data
 * @param   connHandle - connection handle
 * @param   *p - the pointer of l2cap data
 * @return  0
 */
int blc_l2cap_pktHandler(u16 connHandle, u8 *raw_pkt);


/**
 * @brief   This function is used to handler L2CAP data (new version)
 * @param   connHandle - connection handle
 * @param   *p - the pointer of l2cap data
 * @return  0
 */
int blc_l2cap_pktHandler_5_3(u16 connHandle, u8 *raw_pkt);

#endif
