/********************************************************************************************************
 * @file    ble_ad_type.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once
#if (defined(HOST_V2_ENABLE))
/**
 *  @brief LTV(length, type, value) data structure
 *  @details This structure is used to represent LTV data.
 *  It is used for example, advertising data, scan response data, LE Audio metadata, codec configuration, etc.
*/
struct ltv_data
{
    uint8_t length;   /** < Length of the value field */
    uint8_t type;     /** < Type of the value field */
    uint8_t value[0]; /** < Value field */
} __attribute__((packed));

//data type definitions and formats, used Advertising Data(AD), Scan Response(SRD), Additional Controller Advertising Data(ACAD).

//Flags: Optional in this context;  shall not appear more than once in a block.
#define FLAGS_LE_LIMITED_DISCOVERABLE_MODE       BIT(0)
#define FLAGS_LE_GENERAL_DISCOVERABLE_MODE       BIT(1)
#define FLAGS_BR_EDR_NOT_SUPPORTED               BIT(2)
#define FLAGS_CONTROLLER_SUPPORTED_LE_AND_BR_EDR BIT(3)
#define FLAGS_PREVIOUSLY_USED                    BIT(4)

union ad_type_flags
{
    uint8_t flags;

    struct
    {
        uint8_t le_limited_discoverable_mode       : 1;
        uint8_t le_general_discoverable_mode       : 1;
        uint8_t br_edr_not_supported               : 1;
        uint8_t controller_supported_le_and_br_edr : 1;
        uint8_t reserved                           : 4;
    };
};

struct ad_data_flags
{
    struct ltv_data     header; //refer to AD_TYPE_FLAGS
    union ad_type_flags flags;
} __attribute__((packed));

//Local Name: Optional in this context;  shall not appear more than once in a block.

//Shortened Local Name: utf8s
//For example, if the device name is 'BT_Device_Name' then the shortened name could be 'BT_Device' or 'BT_Dev'.
struct ad_data_short_local_name
{
    struct ltv_data header; //refer to AD_TYPE_SHORTENED_LOCAL_NAME
    char            name[];
} __attribute__((packed));

//Complete Local Name: utf8s
struct ad_data_complete_local_name
{
    struct ltv_data header; //refer to AD_TYPE_COMPLETE_LOCAL_NAME
    char            name[];
} __attribute__((packed));

//Service UUID: Optional in this context (may appear more than once in a block).
//GAP and GATT service UUIDs should not be included in a Service UUIDs AD type, for either a complete or incomplete list.
struct ad_data_incomplete_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_INCOMPLETE_LIST_16BIT_SERVICE_UUID
    uint16_t        uuid[0]; //More 16-bit Service UUIDs available.
} __attribute__((packed));

struct ad_data_complete_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_COMPLETE_LIST_16BIT_SERVICE_UUID
    uint16_t        uuid[0]; //Complete list of 16-bit Service UUIDs.
} __attribute__((packed));

struct ad_data_incomplete_32bit_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_INCOMPLETE_LIST_32BIT_SERVICE_UUID
    uint32_t        uuid[0]; //More 32-bit Service UUIDs available.
} __attribute__((packed));

struct ad_data_complete_32bit_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_COMPLETE_LIST_32BIT_SERVICE_UUID
    uint32_t        uuid[0]; //Complete list of 32-bit Service UUIDs.
} __attribute__((packed));

struct server_uuid_128bit
{
    uint8_t uuid[16];
};

struct ad_data_incomplete_128bit_service_uuid
{
    struct ltv_data           header;  //refer to AD_TYPE_INCOMPLETE_LIST_128BIT_SERVICE_UUID
    struct server_uuid_128bit uuid[0]; //More 128-bit Service UUIDs available.
} __attribute__((packed));

struct ad_data_complete_128bit_service_uuid
{
    struct ltv_data           header;  //refer to AD_TYPE_COMPLETE_LIST_128BIT_SERVICE_UUID
    struct server_uuid_128bit uuid[0]; //Complete list of 128-bit Service UUIDs.
} __attribute__((packed));

//Manufacturer Specific Data: Optional in this context (may appear more than once in a block).
struct ad_data_manufacture_specific_data
{
    struct ltv_data header;    //refer to AD_TYPE_MANUFACTURER_SPECIFIC_DATA
    uint16_t        companyID; // company ID.
    uint8_t         data[0];   //Any remainder contains manufacturer specific data.
} __attribute__((packed));

//TX Power Level: ACAD not used. Optional in this context (may appear more than once in a block).
struct ad_data_tx_power_level
{
    struct ltv_data header;       //refer to AD_TYPE_TX_POWER_LEVEL
    char            txPowerLevel; //0xXX:-127 to +127 dBm.
} __attribute__((packed));

//Peripheral Connection Interval Range: ACAD not used. Optional in this context (may appear more than once in a block).
//connection interval range:0x0006 to 0x0C80. unit 1.25ms
struct ad_data_connect_interval_range
{
    struct ltv_data header; //refer to AD_TYPE_PERIPHERAL_CONN_INTERVAL_RANGE
    uint16_t        minConnInterval;
    uint16_t        maxConnInterval;
} __attribute__((packed));

//Service Solicitation: ACAD not used. Optional in this context (may appear more than once in a block).
struct ad_data_16bit_solicitation_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_LIST_16BIT_SERVICE_SOLICITATION_UUID
    uint16_t        uuid[0]; //List of 16 bit Service Solicitation UUIDs
} __attribute__((packed));

struct ad_data_32bit_solicitation_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_LIST_32BIT_SERVICE_SOLICITATION_UUID
    uint32_t        uuid[0]; //List of 32 bit Service Solicitation UUIDs
} __attribute__((packed));

struct ad_data_128bit_solicitation_uuid
{
    struct ltv_data           header;  //refer to AD_TYPE_LIST_128BIT_SERVICE_SOLICITATION_UUID
    struct server_uuid_128bit uuid[0]; //List of 128 bit Service Solicitation UUIDs
} __attribute__((packed));

//Service Data:Optional in this context (may appear more than once in a block).
struct ad_data_service_data_16bit_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_SERVICE_DATA_16BIT_UUID
    uint16_t        uuid;    //The first value contains the 16 bit Service UUID.
    uint8_t         data[0]; //Any remainder contains additional service data.
} __attribute__((packed));

struct ad_data_service_data_32bit_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_SERVICE_DATA_32BIT_UUID
    uint32_t        uuid;    //The first value contains the 16 bit Service UUID.
    uint8_t         data[0]; //Any remainder contains additional service data.
} __attribute__((packed));

struct ad_data_service_data_128bit_uuid
{
    struct ltv_data           header;  //refer to AD_TYPE_SERVICE_DATA_128BIT_UUID
    struct server_uuid_128bit uuid;    //The first value contains the 16 bit Service UUID.
    uint8_t                   data[0]; //Any remainder contains additional service data.
} __attribute__((packed));

//Appearance: Optional in this context;  shall not appear more than once in a block and
//              shall not appear in both the AD and SRD of the same extended advertising interval.
struct ad_data_appearance
{
    struct ltv_data header;     //refer to AD_TYPE_APPEARANCE
    uint16_t        appearance; //The Appearance value shall be the enumerated value as defined by Assigned Numbers.
} __attribute__((packed));

//Public Target Address: Optional in this context;  shall not appear more than once in a block and
//              shall not appear in both the AD and SRD of the same extended advertising interval.
struct ad_data_public_target_address
{
    struct ltv_data header;  //refer to AD_TYPE_PUBLIC_TARGET_ADDRESS
    uint8_t         addr[6]; //The format of each address is the same as the Public Device Address defined in [Vol 6] Part B, Section 1.3.
} __attribute__((packed));

//Random Target Address: Optional in this context;  shall not appear more than once in a block and
//              shall not appear in both the AD and SRD of the same extended advertising interval.
struct ad_data_random_target_address
{
    struct ltv_data header;  //refer to AD_TYPE_RANDOM_TARGET_ADDRESS
    uint8_t         addr[6]; //The format of each address is the same as the Random Device Address defined in [Vol 6] Part B, Section 1.3.
} __attribute__((packed));

//Advertising Interval: Optional in this context; shall not appear more than once in a block.
struct ad_data_advertising_interval
{
    struct ltv_data header;      //refer to AD_TYPE_ADVERTISING_INTERVAL
    uint16_t        advInterval; //units: 0.625ms
} __attribute__((packed));

struct ad_data_advertising_interval_long
{
    struct ltv_data header; //refer to AD_TYPE_ADVERTISING_INTERVAL_LONG

    union
    {
        uint8_t  advInterval1[3];
        uint32_t advInterval2;
    }; //units: 0.625ms, must >= 40.96s
} __attribute__((packed));

//Uniform Resource Identifier: Optional in this context (may appear more than once in a block).
struct ad_data_uniform_resource_identifier
{
    struct ltv_data header; //refer to AD_TYPE_URI
    uint8_t         uri[0]; //Scheme name string and URI as a UTF-8 string
} __attribute__((packed));

//LE Supported Features: Optional in this context; shall not appear more than once in a block.
struct ad_data_le_supported_features
{
    struct ltv_data header; //refer to AD_TYPE_LE_SUPPORTED_FEATURES
    //Each value corresponds to the bit with the same position in the FeatureSet defined in [Vol 6] Part B, Section 4.6.
    uint8_t feature[8];
} __attribute__((packed));

//Channel Map Update Indication: ACAD, Optional in this context; shall not appear more than once in a block.
struct ad_data_channel_map_update_indication
{
    struct ltv_data header;     //refer to AD_TYPE_CHM_UPT_IND
    uint8_t         chM[5];     //Channel Map
    uint8_t         instant[2]; //Instant
} __attribute__((packed));

//BIGInfo: ACAD, Optional in this context; shall not appear more than once in a block.

//Broadcast_Code: OOB only, Optional in this context (may appear more than once in a block)

//Encrypted Data: Optional in this context (may appear more than once in a block)

//Periodic Advertising Response Timing Information: ACAD only,

///* EIR Data Type, Advertising Data Type (AD Type) and OOB Data Type Definitions */
//typedef enum
//{
//    DT_FLAGS                               = 0x01, //  Flag
//    DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID  = 0x02, //  Incomplete List of 16-bit Service Class UUIDs
//    DT_COMPLETE_LIST_16BIT_SERVICE_UUID    = 0x03, //  Complete List of 16-bit Service Class UUIDs
//    DT_INCOMPLETE_LIST_32BIT_SERVICE_UUID  = 0x04, //  Incomplete List of 32-bit Service Class UUIDs
//    DT_COMPLETE_LIST_32BIT_SERVICE_UUID    = 0x05, //  Complete List of 32-bit Service Class UUIDs
//    DT_INCOMPLETE_LIST_128BIT_SERVICE_UUID = 0x06, //  Incomplete List of 128-bit Service Class UUIDs
//    DT_COMPLETE_LIST_128BIT_SERVICE_UUID   = 0x07, //  Complete List of 128-bit Service Class UUIDs
//    DT_SHORTENED_LOCAL_NAME                = 0x08, //  Shortened Local Name
//    DT_COMPLETE_LOCAL_NAME                 = 0x09, //  Complete Local Name
//    DT_TX_POWER_LEVEL                      = 0x0A, //  Tx Power Level
//
//    DT_PERIPHERAL_CONN_INTERVAL_RANGE        = 0x12,
//    DT_LIST_16BIT_SERVICE_SOLICITATION_UUID  = 0x14,
//    DT_LIST_128BIT_SERVICE_SOLICITATION_UUID = 0x15,
//    DT_SERVICE_DATA                          = 0x16, //  Service Data
//    DT_SERVICE_DATA_16BIT_UUID               = 0x16,
//    DT_PUBLIC_TARGET_ADDRESS                 = 0x17,
//    DT_RANDOM_TARGET_ADDRESS                 = 0x18,
//    DT_APPEARANCE                            = 0x19, //  Appearance
//    DT_ADVERTISING_INTERVAL                  = 0x1A,
//    DT_LE_BLUETOOTH_DEVICE_ADDRESS           = 0x1B,
//    DT_LE_ROLE                               = 0x1C,
//
//    DT_LIST_32BIT_SERVICE_SOLICITATION_UUID = 0x1F,
//    DT_SERVICE_DATA_32BIT_UUID              = 0x20,
//    DT_SERVICE_DATA_128BIT_UUID             = 0x21,
//    DT_URI                                  = 0x24,
//    DT_INDOOR_POSITIONING                   = 0x25,
//    DT_TRANSPORT_DISCOVERY_DATA             = 0x26,
//
//    DT_LE_SUPPORTED_FEATURES          = 0x27,
//    DT_CHM_UPT_IND                    = 0x28, //  Channel Map Update Indication
//    DT_PB_ADV                         = 0x29,
//    DT_MESH_MESSAGE                   = 0x2A,
//    DT_MESH_BEACON                    = 0x2B,
//    DT_BIGINFO                        = 0x2C, //  BIGInfo
//    DT_BROADCAST_CODE                 = 0x2D, //  Broadcast_Code
//    DT_CSIP_RSI                       = 0x2E,
//    DT_ADVERTISING_INTERVAL_LONG      = 0x2F,
//    DT_BROADCAST_NAME                 = 0x30, //  Broadcast_Name
//    DT_ENCRYPTED_ADVERTISING_DATA     = 0x31,
//    DT_PA_RESPONSE_TIMING_INFORMATION = 0x32,
//    DT_ELECTRONIC_SHELF_LABEL         = 0x34,
//    DT_3D_INFORMATION_DATA            = 0x3D, //  3D Information Data
//
//    DT_MANUFACTURER_SPECIFIC_DATA = 0xFF,     //    Manufacturer Specific Data
//} data_type_t;
#endif
