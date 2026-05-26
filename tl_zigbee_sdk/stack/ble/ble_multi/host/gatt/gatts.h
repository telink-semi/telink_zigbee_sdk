/********************************************************************************************************
 * @file    gatts.h
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
 * @brief
 * @param[in]
 * @param[out]
 * @return      ble_sts_t.
 */
atts_group_t *blc_gatts_getAttributeServiceGroup(u16 connHandle);

/**
 * @brief
 * @param[in]
 * @param[out]
 * @return      ble_sts_t.
 */
void blc_gatts_addAttributeServiceGroup(atts_group_t *pGroup);

/**
 * @brief
 * @param[in]
 * @param[out]
 * @return      ble_sts_t.
 */
void blc_gatts_removeAttributeServiceGroup(u16 startHandle);

/**
 * @brief gatt server get attribute value and attribute value length
 * @param[in] connHandle
 * @param[in] handle --- the attribute handle
 * @param[out] attrValue --- the attribute value pointer
 * @param[out] attrValueLen --- the attribute value length pointer
 * @return      ble_sts_t.
 */
ble_sts_t blc_gatts_getAttributeInformationByHandle(u16 connHandle, u16 handle, u8 **attrValue, u16 **attrValueLen);

u8 *blc_gatts_getAttributeValueByHandle(u16 connHandle, u16 handle);
u8 *blc_gatts_getReportReferenceValue(u16 connHandle, u16 handle);

bool blc_gatts_calculateDatabaseHash(u16 connHandle, u8 *databaseHash);


ble_sts_t blc_gatts_notifyValue(u16 connHandle, u16 handle, u8 *value, u16 valueLen);
ble_sts_t blc_gatts_notifyAttr(u16 connHandle, u16 handle);
void      blc_gatts_notifyLoop(void);
void      blc_gatts_notifyDisconnect(u16 connHandle);

typedef void (*gatts_cfmCb)(u16 connHandle, u16 scid);

typedef struct __attribute__((packed))
{
    u16         connHandle;
    u16         scid;
    u16         attrHandle;
    void       *value;
    u16         valueLen;
    gatts_cfmCb cb;
} gattsIndValue_t;

ble_sts_t blc_gatts_indicateValue(gattsIndValue_t *ind);
