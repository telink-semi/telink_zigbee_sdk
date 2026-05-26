/********************************************************************************************************
 * @file    signaling.h
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

#pragma once


/**
 * @brief MPS - Maximum PDU Payload Size
 *        MTU - Maximum Transmission Unit
 *        CID - Channel Identifier
 *        SPSM - Simplified Protocol/Service Multiplexer
 */
#define SIGNAL_MINIMUM_MTU        23
#define SIGNAL_MINIMUM_MPS        23
#define SIGNAL_MAXIMUM_MPS        65533

#define SIGNAL_CREDIT_MINIMUM_MTU 64
#define SIGNAL_CREDIT_MINIMUM_MPS 64
#define SIGNAL_CREDIT_MAXIMUM_MPS 65533

/**
 * @brief   data structure of CoC channel
 */
typedef struct
{
    u16 connHandle;
    u16 SPSM;
    u16 srcCID;
    u16 dstCID;
    u16 mtu;
    u16 mps;
    u16 sduLen;
    u16 recvLen;
    u16 sendTotalLen;
    u16 sendOffsetLen;
    u16 recvCredits;
    u16 sendCredits;

    u8 *pRxSdu;
    u8 *pTxSdu;
} l2cap_coc_cid_t;

/**
 * @brief   data structure of CoC CID
 *          LE Only supported CID 0x0040 to 0x007F
 *          0x0020 to 0x003E LE-U Reserved for future use, 0xFFFF for send reconfigure request
 */
typedef struct
{
    u16 connHandle;
    u16 SPSM;
    u16 dstCIDNum;
    u16 dstCID[5];
} l2cap_coc_acl_t;

/**
 * @brief   tx or rx packet, for MTU size
 */
#define COC_MODULE_BUFFER_SIZE(createConnCnt, cocCidCnt, eattCidCnt, mtu) \
    (                                                                     \
        (createConnCnt) * sizeof(l2cap_coc_acl_t) + (cocCidCnt + eattCidCnt) * sizeof(l2cap_coc_cid_t) + (mtu) * (cocCidCnt + 2 * eattCidCnt))

/**
 * @brief   data structure of CoC initial parameter
 */
typedef struct
{
    u16 SPSM;          //supported SPSM.
    u16 MTU;           //CID receive MTU.
    u16 createConnCnt; //supported maximum ACL connect, create COC connect.
    u16 cocCidCnt;     //supported maximum COC CID count.
    u16 eattCidCnt;    //supported maximum EATT CID count.
} blc_coc_initParam_t;

/**
 * @brief   This function is used to register CoC module
 * @param   param - CoC initialized parameters
 * @param   pBuffer - CoC buffer
 * @param   buffLen - CoC buffer length
 * @return  state - 0: command succeeded; others: failed
 */

int blc_l2cap_registerCocModule(blc_coc_initParam_t *param, u8 *pBuffer, u16 buffLen);

/**
 * @brief   This function is used to disconnect CoC channel
 * @param   connHandle - connection handle
 * @param   srcCID - source channel identifier
 * @return  BLE_SUCCESS
 */

ble_sts_t blc_l2cap_disconnectCocChannel(u16 connHandle, u16 srcCID);

/**
 * @brief   This function is used to create LE credit based connection
 * @param   connHandle - connection handle
 * @return  BLE_SUCCESS
 */

ble_sts_t blc_l2cap_createLeCreditBasedConnect(u16 connHandle);

/**
 * @brief   This function is used to create credit based connection
 * @param   connHandle - connection handle
 * @param   srcCnt - source channel count
 * @return  BLE_SUCCESS
 */

ble_sts_t blc_l2cap_createCreditBasedConnect(u16 connHandle, u8 srcCnt);

/**
 * @brief   This function is used to send CoC data
 * @param   connHandle - connection handle
 * @param   srcCID - source channel identifier
 * @param   data - data to be sent
 * @param   dataLen - data length
 * @return  BLE_SUCCESS
 */

ble_sts_t blc_l2cap_sendCocData(u16 connHandle, u16 srcCID, u8 *data, u16 dataLen);

/**
 * @brief   CoC main loop
 * @param   none
 * @return  none
 */

void blc_l2cap_cocMainLoop(void);


/**
 * @brief   This function is used to send connection parameter update request
 * @param   connHandle - connection handle
 * @param   min_interval - minimum connection interval
 * @param   max_interval - maximum connection interval
 * @param   latency - connection latency
 * @param   timeout - connection timeout
 * @return  BLE_SUCCESS
 */

ble_sts_t blc_signal_sendConnectParameterUpdateReq(u16 connHandle, u16 min_interval, u16 max_interval, u16 latency, u16 timeout);

/**
 * @brief   This function is used to send connection parameter update response
 * @param   connHandle - connection handle
 * @param   identifier - request packet identifier
 * @param   result - connection parameter update result
 * @return  BLE_SUCCESS
 */

ble_sts_t blc_signal_sendConnectParameterUpdateRsp(u16 connHandle, u8 identifier, conn_para_up_rsp result);
