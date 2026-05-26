/********************************************************************************************************
 * @file    gatt_v0.h
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
#ifndef GATT_H_
#define GATT_H_

/* GATT data handle call-back. */
typedef int (*gatt_handler_t)(u16 conn, u8 *p);
extern gatt_handler_t gatt_data_handler;


/**
 * @brief       Register GATT data handle call-back.
 * @param[in]   handler  -
 * @return      none.
 */
void blc_gatt_register_data_handler(gatt_handler_t handler);

/**
 * @brief       Send ATT Value Notify.
 * @param[in]   connHandle   - connection handle.
 * @param[in]   attHandle    - ATT handle.
 * @param[in]   p            - Pointer point to application data buffer.
 * @param[in]   len          - the length of data.
 * @return      none.
 */
ble_sts_t blc_gatt_pushHandleValueNotify(u16 connHandle, u16 attHandle, u8 *p, int len);

/**
 * @brief       Send ATT Indicate.
 * @param[in]   connHandle   - connection handle.
 * @param[in]   attHandle    - ATT handle.
 * @param[in]   p            - Pointer point to application data buffer.
 * @param[in]   len          - the length of data.
 * @return      none.
 */
ble_sts_t blc_gatt_pushHandleValueIndicate(u16 connHandle, u16 attHandle, u8 *p, int len);

/**
 * @brief       Send ATT Prepare Write Request.
 * @param[in]   connHandle   - connection handle.
 * @param[in]   attHandle    - ATT handle.
 * @param[in]   offset       - The offset of the first octet to be written.
 * @param[in]   p            - Pointer point to application data buffer.
 * @param[in]   len          - the length of data, it should be <= ATT_MTU-5.
 * @return      ble_sts_t.
 */
ble_sts_t blc_gatt_pushPrepareWriteRequest(u16 connHandle, u16 attHandle, u16 offset, u8 *p, int len);

/**
 * @brief       Send ATT Execute Write Request.
 * @param[in]   connHandle   - connection handle.
 * @param[in]   flags        - 0x00: Cancel all prepared writes
 *                           - 0x01: Immediately write all pending prepared values
 * @return      ble_sts_t.
 */
ble_sts_t blc_gatt_pushExecuteWriteRequest(u16 connHandle, u8 flags);

/**
 * @brief       Send ATT Write Command.
 * @param[in]   connHandle   - connection handle.
 * @param[in]   attHandle    - ATT handle.
 * @param[in]   p            - Pointer point to application data buffer.
 * @param[in]   len          - the length of data.
 * @return      none.
 */
ble_sts_t blc_gatt_pushWriteCommand(u16 connHandle, u16 attHandle, u8 *p, int len);

/**
 * @brief       Send ATT Write Request.
 * @param[in]   connHandle   - connection handle.
 * @param[in]   attHandle    - ATT handle.
 * @param[in]   p            - Pointer point to application data buffer.
 * @param[in]   len          - the length of data.
 * @return      none.
 */
ble_sts_t blc_gatt_pushWriteRequest(u16 connHandle, u16 attHandle, u8 *p, int len);

/**
 * @brief       Send ATT Find Info Request.
 * @param[in]   connHandle         - connection handle.
 * @param[in]   start_attHandle    - the start of ATT handle.
 * @param[in]   end_attHandle      - the end of ATT handle.
 * @return      none.
 */
ble_sts_t blc_gatt_pushFindInformationRequest(u16 connHandle, u16 start_attHandle, u16 end_attHandle);

/**
 * @brief       Send ATT Find By Type Value Request.
 * @param[in]   connHandle         - connection handle.
 * @param[in]   start_attHandle    - the start of ATT handle.
 * @param[in]   end_attHandle      - the end of ATT handle.
 * @param[in]   uuid               - attribute type.
 * @param[in]   attr_value         - Pointer point to value buffer.
 * @param[in]   len                - the length of value.
 * @return      none.
 */
ble_sts_t blc_gatt_pushFindByTypeValueRequest(u16 connHandle, u16 start_attHandle, u16 end_attHandle, u16 uuid, u8 *attr_value, int len);
/**
 * @brief       Send ATT Read By Type Request.
 * @param[in]   connHandle         - connection handle.
 * @param[in]   start_attHandle    - the start of ATT handle.
 * @param[in]   end_attHandle      - the end of ATT handle.
 * @param[in]   uuid               - attribute type.
 * @param[in]   uuid_len           - the length of attribute type.
 * @return      none.
 */
ble_sts_t blc_gatt_pushReadByTypeRequest(u16 connHandle, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);

/**
 * @brief       Send ATT Read Request.
 * @param[in]   connHandle         - connection handle.
 * @param[in]   attHandle          - ATT handle.
 * @return      none.
 */
ble_sts_t blc_gatt_pushReadRequest(u16 connHandle, u16 attHandle);

/**
 * @brief       Send ATT Read Blob Request.
 * @param[in]   connHandle         - connection handle.
 * @param[in]   attHandle          - ATT handle.
 * @param[in]   offset             - ATT value offset.
 * @return      none.
 */
ble_sts_t blc_gatt_pushReadBlobRequest(u16 connHandle, u16 attHandle, u16 offset);

/**
 * @brief       Send ATT Read By Group Type Request.
 * @param[in]   connHandle         - connection handle.
 * @param[in]   start_attHandle    - the start of ATT handle.
 * @param[in]   end_attHandle      - the end of ATT handle.
 * @param[in]   uuid               - Pointer point to attribute type.
 * @param[in]   uuid_len           - the length of attribute type.
 * @return      none.
 */
ble_sts_t blc_gatt_pushReadByGroupTypeRequest(u16 connHandle, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);

/**
 * @brief       Send ATT Err Response.
 * @param[in]   connHandle         - connection handle.
 * @param[in]   reqOpcode          - reqOpcode.
 * @param[in]   attHdlInErr        - attHdlInErr.
 * @param[in]   ErrorCode
 * @return      ble_sts_t.
 */
ble_sts_t blc_gatt_pushErrResponse(u16 connHandle, u8 reqOpcode, u16 attHdlInErr, u8 ErrorCode);

/**
 * @brief       Send ATT handle value confirm.
 * @param[in]   connHandle         - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_gatt_pushHandleValueConfirm(u16 connHandle);


#endif /* GATT_H_ */
