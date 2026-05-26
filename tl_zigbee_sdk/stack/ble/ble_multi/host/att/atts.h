/********************************************************************************************************
 * @file    atts.h
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

#ifdef MCU_CORE_D25F_ENABLE
#include "types.h"
#include "stack/ble/ble_multi/ble_common.h"
#endif

#include "att.h"

#define ATTS_SET_WRITE_CBACK           0x01
#define ATTS_SET_READ_CBACK            0x02
#define ATTS_SET_VARIABLE_LEN          0x04 //allow write value length is variable and can change length
#define ATTS_SET_ALLOW_WRITE           0x08 //allow write value copy to attribute value ptr
#define ATTS_SET_ATTR_VALUE_PROPERTIES 0x10 //attribute value is Characteristic attribute properties

typedef struct __attribute__((packed))
{
    u8   perm;
    u8   uuidLen; //UUID size only set 2 on 16
    u8  *uuid;
    u16 *attrValueLen;
    u16  maxAttrLen;
    u8  *attrValue;
    u8   settings;
} atts_attribute_t;

typedef int (*atts_r_cb_t)(u16 connHandle, u8 opcode, u16 attrHandle, u8 **outValue, u16 *outValueLen);
typedef int (*atts_w_cb_t)(u16 connHandle, u8 opcode, u16 attrHandle, u8 *writeValue, u16 valueLen);

#ifdef MCU_CORE_D25F_ENABLE
typedef struct __attribute__((packed)) atts_group_old
{
    struct atts_group_old  *pNext;
    const atts_attribute_t *pAttr;

    atts_r_cb_t readCback;
    atts_w_cb_t writeCback;
    u16         startHandle;
    u16         endHandle;
} atts_group_t;
#else
typedef struct __attribute__((packed)) atts_group
{
    struct atts_group      *pNext;
    const atts_attribute_t *pAttr;

    atts_r_cb_t readCback;
    atts_w_cb_t writeCback;
    u16         startHandle;
    u16         endHandle;
} atts_group_t;
#endif
typedef struct __attribute__((packed))
{
    u8   serviceNum;  //the number of the service uuid found
    u8   num;         //the number of the characteristic uuid found
    u16  charHandle;  //characteristic handle
    u16 *charDataLen; //characteristic data length
    u8  *charData;    //characteristic data
    u8  *CCC;         //CCC
    u16  cccHandle;   //CCC Handle
} atts_foundCharParam_t;

typedef void (*atts_charFoundCback_t)(atts_foundCharParam_t *charParam, void *p);
typedef bool (*atts_serviceFoundCback_t)(void *p);

typedef struct __attribute__((packed))
{
    u8                    charUuidLen;
    const u8             *charUuid;
    atts_charFoundCback_t foundCback;
} atts_findCharList_t;

typedef struct __attribute__((packed))
{
    u8                         inclUuidLen;
    const u8                  *inclUuid;
    u8                         charSize;
    const atts_findCharList_t *charList;
    atts_serviceFoundCback_t   foundCback;
} atts_findInclList_t;

typedef struct __attribute__((packed))
{
    u8                         serviceUuidLen;
    const u8                  *serviceUuid;
    u8                         charSize;
    const atts_findCharList_t *charList;
    atts_serviceFoundCback_t   foundCback; //RFU, Discovery of multiple services may be supported
    u8                         inclSize;
    const atts_findInclList_t *inclList[5];
} atts_findServiceList_t;

typedef struct __attribute__((packed))
{
    u16 handle;
    u16 length;
    u8 *value;
} atts_multHandleNtf_t;

/**
 * @brief       Send ATT Error Response.
 * @param[in]   connHandle - connection handle.
 * @param[in]   reqOpcode -
 * @param[in]   attHdlInErr -
 * @param[in]   ErrorCode -
 * @return      ble_sts_t.
 */
ble_sts_t blc_atts_sendErrResponse(u16 connHandle, u8 reqOpcode, u16 attHdlInErr, u8 ErrorCode);

/**
 * @brief       Send ATT Handle Value Notify.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   attHandle   - ATT handle
 * @param[in]   p           - Pointer point to application data buffer.
 * @param[in]   len         - Pointer point to application data buffer.
 * @return      ble_sts_t.
 */
ble_sts_t blc_atts_sendHandleValueNotify(u16 connHandle, u16 attHandle, u8 *p, int len);

/**
 * @brief       Send ATT Handle Value Indicate.
 * @param[in]   connHandle  - connection handle.
 * @param[in]   attHandle   - ATT handle
 * @param[in]   p           - Pointer point to application data buffer.
 * @param[in]   len         - Pointer point to application data buffer.
 * @return      ble_sts_t.
 */
ble_sts_t blc_atts_sendHandleValueIndicate(u16 connHandle, u16 attHandle, u8 *p, int len);

/**
 * @brief       Find Char By Service UUID.
 * @param[in]   serviceUuid -
 * @param[in]   uuidLen -
 * @param[in]   charList -
 * @param[in]   charListLen -
 * @param[in]   p -
 * @return      ble_sts_t.
 */
int blc_atts_findCharacteristicByServiceUuid(const u8 *serviceUuid, u8 uuidLen, const atts_findCharList_t *charList, u16 charListLen, void *p);


/**
 * @brief       Find characteristic UUID.
 * @param[in]   serviceList -
 * @param[in]   p -
 * @return      ble_sts_t.
 */
int blc_atts_findCharacteristic(const atts_findServiceList_t *serviceList, void *p);
