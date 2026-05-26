/********************************************************************************************************
 * @file    gattc.h
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

/** GATT Discover types */
enum
{
    /** Discover Primary Services. */
    GATT_DISCOVER_PRIMARY, //Primary Service Discovery: Discover All Primary Services / Discover Primary Service by Service UUID
    /** Discover Secondary Services. */
    GATT_DISCOVER_SECONDARY,
    /** Discover Included Services. */
    GATT_DISCOVER_INCLUDE, //Relationship Discovery: Find Included Services
    /** Discover Characteristic Value and its properties. */
    GATT_DISCOVER_CHARACTERISTIC, //Characteristic Discovery: Discover All Characteristics of a Service / Discover Characteristics by UUID
    /** Discover Attributes which are not services or characteristics. */
    GATT_DISCOVER_DESCRIPTOR,
    /** Discover Attributes of any type. */
    GATT_DISCOVER_ATTRIBUTE,
    /** Discover standard characteristic descriptor values. */
    GATT_DISCOVER_STD_CHAR_DESC, //Characteristic Descriptor Discovery
};

/* GATT interaction process type */
enum
{
    GATT_PROC_END,
    GATT_PROC_CONT,
};


struct gattc_sdp_cfg;
struct gattc_read_cfg;
struct gattc_write_cfg;
struct gattc_sub_ccc_msg;
struct gattc_sub_ccc_cfg;


typedef u8 (*gattc_sdp_func_t)(u16 connHandle, gatt_attr_t *attr, struct gattc_sdp_cfg *params);

typedef u8 (*gattc_read_func_t)(u16 connHandle, u8 err, gatt_read_data_t *rdData, struct gattc_read_cfg *params);

typedef void (*gattc_sub_ccc_msg_func_t)(u16 connHandle, u16 attHdl, u8 *val, u16 valLen);

typedef void (*gattc_sub_ccc_func_t)(u16 connHandle, u8 err, struct gattc_sub_ccc_cfg *params);

typedef void (*gattc_write_func_t)(u16 connHandle, u8 err, struct gattc_write_cfg *params);


/* GATTC indicate/notify data handle call-back. */
typedef int (*gattc_sub_data_cb_t)(u16 connHandle, attr_pkt_t *attr, u16 attrLen);

/** @brief GATT Discovery procedure parameters configuration */
typedef struct gattc_sdp_cfg
{
    /** Discover UUID type */
    uuid_t *uuid; //UUID size only set 2 on 16
    /** Discover attribute callback */
    gattc_sdp_func_t func;

    union
    {
        struct
        {
            /** Include service attribute declaration handle */
            u16 attrHdl;
            /** Included service start handle */
            u16 startHdl;
            /** Included service end handle */
            u16 endHdl;
        } _included;

        /** Discover start handle */
        u16 startHdl;
    };

    /** Discover end handle */
    u16 endHdl;
    /** Discover type */
    u8 type;
    /** Discover descriptor */
    u8 properties;

    u8 reserved[2];

} gattc_sdp_cfg_t;

/** @brief GATT Read procedure parameters configuration */
typedef struct __attribute__((packed)) gattc_read_cfg
{
    /** Read attribute callback. */
    gattc_read_func_t func;
    /** If == 1: single.handle and single.offset are used.
     *  If > 1:  multiple.handles are used.
     *  If == 0: byUuid is used for Read Using Characteristic UUID. */
    u16 hdlCnt;

    union
    {
        struct
        {
            /** Attribute handle. */
            u16 handle;
            /** Attribute data offset. */
            u16  offset;
            u8  *wBuff;
            u16 *wBuffLen;
            u16  maxLen;
        } single;

        struct
        {
            /** Attribute handles to read with Read Multiple Characteristic Values. */
            u16 *handles;
            /** If TRUE: use Read Multiple Variable Length Characteristic Values procedure.
             *  If FALSE: use Read Multiple Characteristic Values procedure. */
            bool variable;
        } multiple;

        struct
        {
            /** First requested handle number. */
            u16 startHdl;
            /** Last requested handle number. */
            u16 endHdl;
            /** 2 Byte or 16 Byte UUID. */
            uuid_t *uuid;
        } byUuid;
    };
} gattc_read_cfg_t;

/** @brief GATT Write procedure parameters configuration */
typedef struct gattc_write_cfg
{
    /** Response callback */
    gattc_write_func_t func;
    /** Attribute handle */
    u16 handle;
    /** Attribute data offset */
    u16 offset;
    /** Data to be written */
    void *data;
    /** Length of the data */
    u16 length;
    /** If true use Write command procedure. */
    bool withoutRsp;
    u8   reserved;
} gattc_write_cfg_t;

typedef struct gattc_sub_ccc_msg
{
    /* queue's node */
    struct gattc_sub_ccc_msg *pNext;
    gattc_sub_ccc_msg_func_t  ntfOrIndFunc;
    u16                       startHdl;
    u16                       endHdl;
} gattc_sub_ccc_msg_t;

typedef struct gattc_sub_ccc_cfg
{
    /** Subscribe characteristic UUID type */
    uuid_t *uuid; //UUID size only set 2 on 16

    /** Subscribe value callback */
    gattc_sub_ccc_func_t func;

    /** Subscribe value handle (as start handle: for automatic discovery) */
    u16 valueHdl;

    /** Subscribe value, BIT(0):subscribe notify, BIT(1):subscribe indicate */
    u16 value;
} gattc_sub_ccc_cfg_t;

/**
 * @brief       GATTC Discover: Primary / Include Service / Characteristic / Descriptors
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_sdp_cfg_t  -
 * @return      ble_sts_t.
 */
ble_sts_t blc_gattc_discovery(u16 connHandle, gattc_sdp_cfg_t *pSdpCfg);

/**
 * @brief       Read Attribute Value by handle
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_read_cfg_t  -
 * @return      ble_sts_t.
 */
ble_sts_t blc_gattc_readAttributeValue(u16 connHandle, gattc_read_cfg_t *pRdCfg);

/**
 * @brief       GATTC Write Attribute Value by handle
 * @param[in]   connHandle   - connection handle.
 * @param[in]   pWrCfg  -
 * @return      ble_sts_t.
 */
ble_sts_t blc_gattc_writeAttributeValue(u16 connHandle, gattc_write_cfg_t *pWrCfg);

/**

 */
bool      blc_gattc_addSubscribeCCCNode(u16 connHandle, gattc_sub_ccc_msg_t *pSubNode);
ble_sts_t blc_gattc_writeSubscribeCCCRequest(u16 connHandle, gattc_sub_ccc_cfg_t *pSubCccCfg);


/**
 * @brief       GATTC xxx
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_sub_ccc_msg_t  -
 * @return      ble_sts_t.
 */
void blc_gattc_removeSubscribeCCCNode(u16 connHandle, gattc_sub_ccc_msg_t *pSubNode);

/**
 * @brief       GATTC xxx
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
void blc_gattc_cleanAllSubscribeCCCNode(u16 connHandle);
