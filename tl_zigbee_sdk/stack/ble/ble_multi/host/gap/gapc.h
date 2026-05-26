/********************************************************************************************************
 * @file    gapc.h
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

#define BLT_GAPC_DEBUG(fmt, ...)   BLT_HOST_DBUG(DBG_GAPC_LOG, "[GAPC]" fmt, ##__VA_ARGS__)

#define GAPC_DISCOVERY_MAX_NUM     2
#define GAPC_DISC_MAX_ATTR_INFO    2
#define GAPC_DISC_MAX_INCLUDE_INFO 3

typedef enum
{
    GAPC_CHAR_FIND_DESCRIPTOR        = BIT(2),
    GAPC_CHAR_SUBSCRIBE_CCC_NOTIFY   = BITS(0, 2),
    GAPC_CHAR_SUBSCRIBE_CCC_INDICATE = BITS(1, 2),
    GAPC_CHAR_SUBSCRIBE_CCC_ALL      = BITS(0, 1, 2),
    GAPC_CHAR_READ_CHARACTER_VALUE   = BIT(3),
} blc_gapc_char_setting_enum;

typedef struct __attribute__((packed))
{
    u8  properties; //supported CHAR_PROP_READ, CHAR_PROP_NOTIFY, CHAR_PROP_INDICATE
    u16 valueHandle;
    u16 cccHandle;
} blc_gapc_charInfo_t;

/*
 * @param connHandle --- acl connection handle.
 * @param err --- ATT_READ_REQ request return error.
 * @param pRdCfg --- gattc read config struct.
 */
typedef void (*gapc_read_func_t)(u16 connHandle, u8 err, gattc_read_cfg_t *pRdCfg);
/*
 * @param connHandle --- acl connection handle.
 * @param count --- 0xFF:mean not found service uuid. 0x00: mean found service uuid ending. 0x01-0xFE: found service uuid count.
 * @param startHandle --- service uuid starting attribute handle.
 * @param endHandle --- service uuid ending attribute handle.
 */
typedef void (*gapc_foundService_func_t)(u16 connHandle, u8 count, u16 startHandle, u16 endHandle);
/*
 * @param connHandle --- acl connection handle.
 * @param serviceCount --- RFU.
 * @param properties --- characteristic properties. reference ATT_READ_BY_TYPE_RSP.
 * @param valueHandle --- characteristic value handle.
 */
typedef void (*gapc_foundChar_fun_t)(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle);
/*
 * @param connHandle --- acl connection handle.
 * @param uuid --- unknown characteristic uuid.
 * @param properties --- characteristic properties. reference ATT_READ_BY_TYPE_RSP.
 * @param valueHandle --- characteristic value handle.
 */
typedef void (*gapc_foundUnknownChar_func_t)(u16 connHandle, uuid_t *uuid, u8 properties, u16 valueHandle);
/*
 * @param connHandle --- acl connection handle.
 * @param uuid --- characteristic Descriptors uuid. reference ATT_FIND_INFORMATION_RSP.
 * @param attrHandle --- characteristic Descriptors attribute handle.
 */
typedef void (*gapc_foundCharDesc_func_t)(u16 connHandle, uuid_t *uuid, u16 attrHandle);
/*
 * @param connHandle --- acl connection handle.
 * @param attrHandle --- read characteristic value attribute handle.
 * @param[out] read --- read characteristic value storage location.
 * @param[out] readLen --- read characteristic value length storage location.
 * @param[out] readMaxSize --- can read characteristic value maximum length.
 * @param[out] rdCbFunc --- read gattc callback function.
 * if read is not NULL, must set readMaxSize. readLen Not necessary.
 * use read/readLen/readMaxSize, gap layer will write characteristic value to read.
 * use rdCbFunc:GAP layer will GATT layer callback pass to higher level.
 */
typedef void (*gapc_startReadChar_func_t)(u16 connHandle, u16 attrHandle, u8 **read, u16 **readLen, u16 *readMaxSize, gapc_read_func_t *rdCbFunc);
/*
 * @param connHandle --- acl connection handle.
 * @param cccHandle --- CCC value handle.
 * @param result --- subscribe client characteristic configuration result. ATT_ERROR_CODE.
 */
typedef void (*gapc_subscribeCcc_func_t)(u16 connHandle, u16 cccHandle, u8 result);
/*
 * @param connHandle --- acl connection handle.
 * @param startHandle --- found include uuid start attribute handle.
 * @param endHandle --- found include uuid end attribute handle.
 * @return true --- will discovery include characteristic uuid.
 */
typedef bool (*gapc_foundInclude_func_t)(u16 connHandle, u16 startHandle, u16 endHandle);
/*
 * @param connHandle --- acl connection handle.
 * @param uuid --- unknown include uuid.
 * @param startHandle --- found include uuid start attribute handle.
 * @param endHandle --- found include uuid end attribute handle.
 */
typedef void (*gapc_foundUnknownInclude_func_t)(u16 connHandle, uuid_t *uuid, u16 startHandle, u16 endHandle);
/*
 * @param connHandle --- acl connection handle.
 * @param charInfo --- get characteristic information point.
 * @return [0, 1] --- the size that charInfo.
 */
typedef int (*gapc_getCharInfo_fun_t)(u16 connHandle, blc_gapc_charInfo_t *charInfo);

/*
 * @param connHandle --- acl connection handle.
 * @param count --- the number of reconnect include uuid. 1, 2, ...
 * @return true --- can reconnect this include service.
 *          false --- not supported reconnect include.
 */
typedef bool (*gapc_reconnIncl_fun_t)(u16 connHandle, int count);

/*
 * @param connHandle --- acl connection handle.
 * @param count --- the number of reconnect service uuid. 1, 2, ...
 *                  0 mean reconnect service ending
 * @return true --- can reconnect this service.
 *          false --- not supported reconnect service.
 */
typedef bool (*gapc_reconnService_fun_t)(u16 connHandle, int count);

typedef struct __attribute__((packed))
{
    //service uuid.
    uuid_t uuid;
    //found service uuid callback function.
    gapc_foundService_func_t sfun;
} blc_gapc_discService_t;

typedef struct __attribute__((packed))
{
    union
    {
        u8 setting; //blc_gapc_char_setting_enum

        struct
        {
            //Automatically subscribe to the notify property, if had.
            bool subscribeNtf : 1;
            //Automatically subscribe to the indicate property, if had.
            bool subscribeInd : 1;
            //found Descriptors, if had.
            bool findDecs : 1;
            //Automatically read characteristic value, if had read property.
            bool readValue : 1;
        };
    };

    //characteristic uuid.
    uuid_t uuid;
    //found characteristic uuid callback function.
    gapc_foundChar_fun_t cfun;
    //found characteristic Descriptors uuid callback function.
    gapc_foundCharDesc_func_t dfun;
    //subscribe client characteristic configuration callback function.
    gapc_subscribeCcc_func_t scfun;
    //want read characteristic value callback function.
    gapc_startReadChar_func_t rfun;
} blc_gapc_discChar_t;

typedef struct __attribute__((packed))
{
    //supported characteristic uuid size.
    u8 size;
    //discovery characteristic uuid table.
    const blc_gapc_discChar_t *characteristic;
    //found unknown characteristic uuuid callback function.
    gapc_foundUnknownChar_func_t ufun;
} blc_gapc_discCharTable_t;

typedef struct __attribute__((packed))
{
    //discovery include service uuid.
    uuid_t uuid;
    //include characteristic uuid table.
    blc_gapc_discCharTable_t characteristic;
    //found include service uuid callback function.
    gapc_foundInclude_func_t ifun;
} blc_gapc_discInclude_t;

typedef struct __attribute__((packed))
{
    //supported discovery include uuid size.
    u8 size;
    //include discovery information table.
    const blc_gapc_discInclude_t *include[GAPC_DISC_MAX_INCLUDE_INFO];
    //found unknown include uuid callback function.
    gapc_foundUnknownInclude_func_t uifun;
} blc_gapc_discIncludeTable_t;

typedef struct __attribute__((packed))
{
    //discovery service uuid maximum count.
    u8                                maxServiceCount;
    const blc_gapc_discService_t     *service;
    const blc_gapc_discIncludeTable_t includeTable;
    const blc_gapc_discCharTable_t    characteristicTable;
} blc_gapc_discList_t;

typedef struct __attribute__((packed))
{
    //get characteristic information callback function.
    gapc_getCharInfo_fun_t ifun;
    //want read characteristic value callback function.
    gapc_startReadChar_func_t rfun;
} blc_gapc_reconnChar_t;

typedef struct __attribute__((packed))
{
    //supported reconnect characteristic size.
    u8 size;
    //reconnect characteristic list.
    const blc_gapc_reconnChar_t *characteristic;
} blc_gapc_reconnCharTable_t;

typedef struct __attribute__((packed))
{
    //gapc reconnect include characteristic callback function. if not set, default 1.
    gapc_reconnIncl_fun_t reifun;
    // reconnect include characteristic table list.
    const blc_gapc_reconnCharTable_t charTb;
} blc_gapc_reconnInclTable_t;

typedef struct __attribute__((packed))
{
    const uuid_t serviceUuid;
    //gapc reconnect service characteristic callback function. if not set, default 1.
    gapc_reconnService_fun_t resfun;

    // reconnect service characteristic table list.
    const blc_gapc_reconnCharTable_t charTb;

    //supported discovery include uuid size.
    u8 inclSize;
    //include reconnect information table.
    const blc_gapc_reconnInclTable_t *includeCharTb[GAPC_DISC_MAX_INCLUDE_INFO];

} blc_gapc_reconnList_t;

typedef void (*gapc_write_func_t)(u16 connHandle, u8 err, void *data);

/** @brief GAP Write procedure parameters configuration */
typedef struct __attribute__((packed))
{
    /** Response callback */
    gapc_write_func_t func;
    /** Attribute handle */
    u16 handle;
    /** Data to be written if length > MTU-3, data must global variables*/
    void *data;
    /** Length of the data */
    u16 length;
    /** If true use Write command procedure. */
    bool withoutRsp;
    /** response callback input data*/
    void *cbData;
} gapc_write_cfg_t;

/** @brief GAP read procedure parameters configuration */
typedef struct  __attribute__((packed))
{
    /** Response callback */
    gapc_read_func_t func;
    /** Attribute handle */
    u16 handle;
    /** Read data pointer*/
    u8 *wBuff;
    /** Read data length pointer*/
    u16 *wBuffLen;
    /** read data maximum length*/
    u16 maxLen;
} gapc_read_cfg_t;

/**
 * @brief GAP client discovery specific service loop, needs to be called in the main loop and is based on the L2CAP version 5.3 interface.
 * @return      NULL.
 */
void blc_gapc_discoveryOrReconnectService_loop(void);

/**
 * @brief GAP discovery service initialization function.
 * @param[in] connHandle --- acl connection handle.
 * @param[in] list --- discovery service list, service uuid, characteristic uuid, include uuid,
 * @return ble_sts_t --- only BLE_SUCCESS init successful.
 */
ble_sts_t blc_gapc_registerDiscoveryService(u16 connHandle, const blc_gapc_discList_t *list);

/**
 * @brief GAP reconnect service initialization function.
 * @param[in] connHandle --- acl connection handle.
 * @param[in] list --- reconnect service list, service uuid, characteristic uuid, include uuid,
 * @return ble_sts_t --- only BLE_SUCCESS init successful.
 */
ble_sts_t blc_gapc_registerReconnectService(u16 connHandle, const blc_gapc_reconnList_t *list);

/**
 * @brief get current discovery service uuid.
 * @param[in] connHandle --- acl connection handle.
 * return NULL --- no discovery,
 */
const uuid_t *blc_gapc_getDiscoveryServiceUUID(u16 connHandle);

/**
 * @brief gap client write characteristic value.
 * @param[in] connHandle --- acl connection handle.
 * @param[in] pGapWrCfg -- write value struct.
 * @return ble_sts_t.
 */
ble_sts_t blc_gapc_writeAttributeValue(u16 connHandle, gapc_write_cfg_t *pGapWrCfg);

/**
 * @brief gap client read attribute value.
 * @param[in] connHandle --- acl connection handle.
 * @param[in] pGapWrCfg -- read attribute value struct.
 * @return ble_sts_t.
 */
ble_sts_t blc_gapc_readAttributeValue(u16 connHandle, gapc_read_cfg_t *pGapReCfg);
