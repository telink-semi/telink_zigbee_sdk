/********************************************************************************************************
 * @file    zbd_common.h
 *
 * @brief   This is the header file for zbd_common
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/
#ifndef _ZBD_COMMON_H_
#define _ZBD_COMMON_H_

#define ZIGBEE_DIRECT_DEBUG_EN              0

//BLE RECOMMENDED VALUE
#define DIRECT_ADVERTISING_INTERVAL         152.5   //ms
#define DIRECT_CONNECTION_INTERVAL_MIN      7.5     //ms
#define DIRECT_CONNECTION_INTERVAL_MAX      100     //ms
#define DIRECT_ATT_MTU_MIN                  185
#define DIRECT_ATT_MTU_RECOMMENDED          247
#define DIRECT_CONNECTION_MAX               1

/**
 * @brief This indicates the domain or a particular operation that the following status code pertains to.
 */
typedef enum {
    ZBD_STA_SUCCESS                     = 0x00, /*! success */
    ZBD_STA_BUFF_FAILURE                = 0x01, /*! fail to ger buffer */
    ZBD_STA_EUI_CONFLICT                = 0x02, /*! ZVD/ZCD ieee address conflict */
    ZBD_STA_EDCH_POINT_ERROR            = 0x03, /*! fail to get xk */
    ZBD_STA_MACTAG_GET_ERROR            = 0x04, /*! fail to get mac tag */
    ZBD_STA_LEN_OUT_RANGE               = 0x05, /*! the data length is out of range */
    ZBD_STA_ACCESS_FAILURE              = 0x06, /*! access fail, ble interface off*/
    ZBD_STA_KEY_METHOD_UNSUPPORTED      = 0x07, /*! key method is unsupported */
    ZBD_STA_NWKKEY_SEQNUM_UNSUPPORTED   = 0x08, /*! the network sequence number is not record*/
    ZBD_STA_NOT_PERMIT_PROVISSION       = 0x09, /*! not allow the provisioning session establish*/
    ZBD_STA_KEY_IS_NULL                 = 0x0A, /*! the key is null*/
    ZBD_STA_WAIT_TIMEOUT                = 0x0B, /*! Wait for the response timeout*/
    ZBD_STA_VALIDATE_FAIL               = 0x0C, /*! validate value error*/
    ZBD_STA_TLV_PARSE_ERROR             = 0x0D, /*! tlv parse error*/
    ZDB_STA_TLVID_UNSUPPORTED           = 0x0E, /*! the tlv id is unsupported for this command*/
    ZBD_STA_TLV_NOT_FOUND               = 0x0F, /*! not find the tlv*/
    ZBD_STA_OPERCODE_INVALID            = 0x10, /*! invalid operate code*/

    ZBD_STA_ENCRYPT_FAIL                = 0x11, /*! encrypt fail*/
    ZBD_STA_DECRYPT_FAIL                = 0x12, /*! decrypt fail*/
    ZBD_STA_INFRAMECOUNTER_INVALID      = 0x13, /*! incoming frame counter verification failed*/
    ZBD_STA_OUTFRAMECOUNTER_INVALID     = 0x14, /*! outgoing frame counter out of range*/
    ZBD_STA_INSUFF_AUTH_LEVEL           = 0x15, /*! insufficient authentication level*/

	ZBD_STA_PREPROCESS_ONGOING          = 0x20, /*! the previous command has not been processed*/
    ZBD_STA_NETWORK_STATE_NOT_ALLOWED   = 0x21, /*! the command is not allowed because of zigbee network status*/
    ZBD_STA_DEVICE_TYPE_NOT_ALLOWED     = 0x22, /*! the command is not allowed because of zigbee device type*/
    ZBD_STA_STACK_INTERNAL_ERROR        = 0x23, /*! the command processing failed */
    ZBD_STA_COMMAND_HANDLING            = 0x24, /*! the command for zigbee stack to handle*/
    ZBD_STA_DATA_OUT_RANGE              = 0x25, /*! the data is out of range*/
    ZBD_STA_BLE_STACK_ERROR             = 0x26, /*! the data send fail because of ble stack*/
    ZBD_STA_BLE_DATA_PENDING            = 0x27, /*! the data is pending to send*/

    ZBD_STA_FAILURE                     = 0xFF,
} ZBD_RET_E;

#endif
