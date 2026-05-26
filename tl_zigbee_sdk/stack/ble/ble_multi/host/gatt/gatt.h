/********************************************************************************************************
 * @file    gatt.h
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


/* Client Supported Features bit assignments */
#define CLIENT_SUPP_FEAT_ROBUST_CACHING        BIT(0) //The client supports robust caching
#define CLIENT_NOT_SUPP_FEAT_ROBUST_CACHING    0
#define CLIENT_SUPP_FEAT_EATT_BEARER           BIT(1) //The client supports Enhanced ATT bearer
#define CLIENT_NOT_SUPP_FEAT_EATT_BEARER       0
#define CLIENT_SUPP_FEAT_MULTI_HDL_VAL_NTF     BIT(2) //The client supports Enhanced ATT bearer
#define CLIENT_NOT_SUPP_FEAT_MULTI_HDL_VAL_NTF 0

/* Server Supported Features bit assignments */
#define SERVER_SUPP_FEAT_EATT_BEARER     BIT(0) //The server supports Enhanced ATT bearer
#define SERVER_NOT_SUPP_FEAT_EATT_BEARER 0

/** @brief Service Attribute Value. */
typedef struct __attribute__((packed))
{
    /** Service UUID. */
    const uuid_t *uuid;
    /** Service end handle. */
    u16 endHdl;
} gatt_service_val_t;

/** @brief Include Attribute Value. */
typedef struct __attribute__((packed))
{
    /* Service start handle. */
    u16 startHdl;
    /* Service end handle. */
    u16 endHdl;
    /* Service UUID. */
    uuid_t uuid;
} gatt_include_t;

/** @brief GATT Attribute structure. */
typedef struct __attribute__((packed))
{
    /* Attribute_handle: handle */
    u16 handle;
    /* Attribute_types: UUID */
    const uuid_t *uuid;
    /* Attribute_value: User data */
    void *user_data;

} gatt_attr_t;

#define GATT_RD_CONT  BIT(0)
#define GATT_RD_CMPLT BIT(1)

typedef struct
{
    u16 rdState;
    u16 dataLen;
    u8 *dataVal;
} gatt_read_data_t;

/** GATT Characteristic Properties bit field */
#define CHAR_PROP_BROADCAST         BIT(0)
#define CHAR_PROP_READ              BIT(1)
#define CHAR_PROP_WRITE_WITHOUT_RSP BIT(2)
#define CHAR_PROP_WRITE             BIT(3)
#define CHAR_PROP_NOTIFY            BIT(4)
#define CHAR_PROP_INDICATE          BIT(5)
#define CHAR_PROP_AUTHEN            BIT(6)
#define CHAR_PROP_EXTENDED          BIT(7)

/** @brief Characteristic Attribute Value. */
typedef struct __attribute__((packed))
{
    /** GATT Characteristic attribute Handle. */
    u16 attrHdl;
    /** GATT Characteristic Properties. */
    u8 properties;
    /** GATT Characteristic Value Attribute Handle. */
    u16 valueHdl;
    /** GATT Characteristic UUID. */
    uuid_t uuid;
} gatt_chrc_t;

/** Characteristic Extended Properties bit field */
#define CHAR_EXT_PROP_RELIABLE_WR  0x0001
#define CHAR_EXT_PROP_WRITABLE_AUX 0x0002

/* Characteristic Extended Properties Attribute Value. */
typedef struct
{
    /** Characteristic Extended properties */
    u16 properties;
} gatt_cep_t;

/** Client Characteristic Configuration bits value. */
#define CLT_CHAR_CFG_NTF 0x0001
#define CLT_CHAR_CFG_IND 0x0002

/** Client Characteristic Configuration Attribute Value */
typedef struct
{
    /** Client Characteristic Configuration flags */
    u16 flags;
} gatt_ccc_t;

/* Server Characteristic Configuration bits value. */
#define SVR_CHAR_CFG_BROADCAST 0x0001

/** Server Characteristic Configuration Attribute Value */
typedef struct
{
    /** Server Characteristic Configuration flags */
    u16 flags;
} gatt_scc_t;

/** @brief Characteristic Presentation Format declaration. */
typedef struct __attribute__((packed))
{
    /* Format of the value of the characteristic */
    u8 format;
    /* Exponent field to determine how the value of this characteristic is further formatted */
    s8 exponent;
    /* Unit of the characteristic */
    u16 unit;
    /* Name space of the description */
    u8 name_space;
    /* Description of the characteristic as defined in a higher layer profile */
    u16 description;
} gatt_cpf_t;
