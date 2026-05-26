/********************************************************************************************************
 * @file    attc.h
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
 * @brief       Send ATT Mtu Size Exchange Request.
 * @param[in]   connHandle - connection handle.
 * @param[in]   mtuSize - ATT MTU size.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendMtuSizeExchangeRequest(u16 connHandle, u16 mtuSize);

/**
 * @brief       Send ATT Prepare Write Request.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   attHandle   - ATT handle.
 * @param[in]   offset      - The offset of the first octet to be written.
 * @param[in]   p           - Pointer point to application data buffer.
 * @param[in]   len         - Pointer point to application data buffer.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendPrepareWriteRequest(u16 connHandle, u16 attHandle, u16 offset, u8 *p, u16 len);

/**
 * @brief       Send ATT Execute Write Request.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   attHandle   - ATT handle.
 * @param[in]   flags       - 0x00: Cancel all prepared writes
 *                          - 0x01: Immediately write all pending prepared values
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendExecuteWriteRequest(u16 connHandle, u16 attHandle, u8 flags);

/**
 * @brief       Send ATT Write Request.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   attHandle   - ATT handle.
 * @param[in]   p           - Pointer point to application data buffer.
 * @param[in]   len         - Pointer point to application data buffer.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendWriteRequest(u16 connHandle, u16 attHandle, u8 *p, u16 len);

/**
 * @brief       Send ATT Write Command.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   attHandle   - ATT handle.
 * @param[in]   p           - Pointer point to application data buffer.
 * @param[in]   len         - Pointer point to application data buffer.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendWriteCommand(u16 connHandle, u16 attHandle, u8 *p, u16 len);

/**
 * @brief       Send ATT Find Info Request.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   startAttHdl - the start of ATT handle.
 * @param[in]   endAttHdl   - the end of ATT handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendFindInfoRequest(u16 connHandle, u16 startAttHdl, u16 endAttHdl);

/**
 * @brief       Send ATT Find By Type Value Request.
 * @param[in]   startAttHdl    - the start of ATT handle.
 * @param[in]   endAttHdl      - the end of ATT handle.
 * @param[in]   uuid           - attribute type.
 * @param[in]   attr_value     - Pointer point to value buffer.
 * @param[in]   len            - the length of value.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendFindByTypeValueRequest(u16 connHandle, u16 startAttHdl, u16 endAttHdl, u16 uuid, u8 *attrVal, u16 len);

/**
 * @brief       Send ATT Read By Type Request.
 * @param[in]   connHandle     - connection handle.
 * @param[in]   startAttHdl    - the start of ATT handle.
 * @param[in]   endAttHdl      - the end of ATT handle.
 * @param[in]   uuid           - attribute type.
 * @param[in]   uuid_len       - the length of attribute type.
 */
ble_sts_t blc_attc_sendReadByTypeRequest(u16 connHandle, u16 startAttHdl, u16 endAttHdl, u8 *uuid, u8 uuid_len);

/**
 * @brief       Send ATT Read By Group Type Request.
 * @param[in]   connHandle     - connection handle.
 * @param[in]   startAttHdl    - the start of ATT handle.
 * @param[in]   endAttHdl      - the end of ATT handle.
 * @param[in]   uuid           - Pointer point to attribute type.
 * @param[in]   uuid_len       - the length of attribute type.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendReadByGroupTypeRequest(u16 connHandle, u16 startAttHdl, u16 endAttHdl, u8 *uuid, u8 uuid_len);

/**
 * @brief       Send ATT Read Request.
 * @param[in]   connHandle      - connection handle.
 * @param[in]   attHandle       - ATT handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendReadRequest(u16 connHandle, u16 attHandle);

/**
 * @brief       Send ATT Read Blob Request.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   attHandle   - ATT handle.
 * @param[in]   offset      - ATT value offset.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendReadBlobRequest(u16 connHandle, u16 attHandle, u16 offset);

/**
 * @brief       Send ATT Read Multiple Request.
 * @param[in]   connHandle - connection handle.
 * @param[in]   numHandles -.
 * @param[in]   pHandle -.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendReadMultReq(u16 connHandle, u8 numHandles, u16 *pHandle);

/**
 * @brief       Send ATT Read Multiple Variable Request.
 * @param[in]   connHandle - connection handle.
 * @param[in]   numHandles -.
 * @param[in]   pHandle -.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendReadMultVarReq(u16 connHandle, u8 numHandles, u16 *pHandle);

/**
 * @brief       Send ATT Handle Value Confirm.
 * @param[in]   connHandle      - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_attc_sendAttHdlValueCfm(u16 connHandle);


/**
 * @brief      This function is used to set whether to hold the ATT Response PDU during the pairing phase 3.
 * @param[in]  hold_enable - 1: enable, holding ATT Response PDU during in pairing phase
 *                      0: disable, allowing ATT Response PDU during in pairing phase
 * @return     none.
 */
void blc_att_holdAttributeResponsePayloadDuringPairingPhase(u8 hold_enable);
