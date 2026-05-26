/********************************************************************************************************
 * @file    ble_format.h
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
#ifndef BLE_FORMAT_H
#define BLE_FORMAT_H


#include "stack/ble/ble_common.h"

//data type definitions and formats, used Advertising Data(AD), Scan Response(SRD), Additional Controller Advertising Data(ACAD).

typedef struct __attribute__((packed))
{
    unsigned char length;
    unsigned char type;
    unsigned char value[0];
} advData_ltv_t;

//Flags: Optional in this context;  shall not appear more than once in a block.
#define FLAGS_LE_LIMITED_DISCOVERABLE_MODE       BIT(0)
#define FLAGS_LE_GENERAL_DISCOVERABLE_MODE       BIT(1)
#define FLAGS_BR_EDR_NOT_SUPPORTED               BIT(2)
#define FLAGS_CONTROLLER_SUPPORTED_LE_AND_BR_EDR BIT(3)
#define FLAGS_PREVIOUSLY_USED                    BIT(4)

typedef struct __attribute__((packed))
{
    advData_ltv_t header; //refer to DT_FLAGS
    unsigned char flags;
} advData_flags_t;

//Local Name: Optional in this context;  shall not appear more than once in a block.

//Shortened Local Name: utf8s
//For example, if the device name is 'BT_Device_Name' then the shortened name could be 'BT_Device' or 'BT_Dev'.
typedef struct __attribute__((packed))
{
    advData_ltv_t header; //refer to DT_SHORTENED_LOCAL_NAME
    unsigned char shortenedLocalName[0];
} advData_shortenedLocalName_t;

//Complete Local Name: utf8s
typedef struct __attribute__((packed))
{
    advData_ltv_t header; //refer to DT_COMPLETE_LOCAL_NAME
    unsigned char completeLocalName[0];
} advData_completeLocalName_t;

//Service UUID: Optional in this context (may appear more than once in a block).
//GAP and GATT service UUIDs should not be included in a Service UUIDs AD type, for either a complete or incomplete list.
typedef struct __attribute__((packed))
{
    advData_ltv_t  header;  //refer to DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID
    unsigned short uuid[0]; //More 16-bit Service UUIDs available.
} advData_incomplete16BitServiceUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t  header;  //refer to DT_COMPLETE_LIST_16BIT_SERVICE_UUID
    unsigned short uuid[0]; //Complete list of 16-bit Service UUIDs.
} advData_complete16BitServiceUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t header;  //refer to DT_INCOMPLETE_LIST_32BIT_SERVICE_UUID
    unsigned int  uuid[0]; //More 32-bit Service UUIDs available.
} advData_incomplete32BitServiceUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t header;  //refer to DT_COMPLETE_LIST_32BIT_SERVICE_UUID
    unsigned int  uuid[0]; //Complete list of 32-bit Service UUIDs.
} advData_complete32BitServiceUuid_t;

typedef struct __attribute__((packed))
{
    unsigned char uuid[16];
} bit128UUID_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t header;  //refer to DT_INCOMPLETE_LIST_128BIT_SERVICE_UUID
    bit128UUID_t  uuid[0]; //More 128-bit Service UUIDs available.
} advData_incomplete128BitServiceUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t header;  //refer to DT_COMPLETE_LIST_128BIT_SERVICE_UUID
    bit128UUID_t  uuid[0]; //Complete list of 128-bit Service UUIDs.
} advData_complete128BitServiceUuid_t;

//Manufacturer Specific Data: Optional in this context (may appear more than once in a block).
typedef struct __attribute__((packed))
{
    advData_ltv_t  header;    //refer to DT_MANUFACTURER_SPECIFIC_DATA
    unsigned short companyID; //
    unsigned char  manufacturerSpecificData[0];
} advData_manufacturerSpecificData_t;

//TX Power Level: ACAD not used. Optional in this context (may appear more than once in a block).
typedef struct __attribute__((packed))
{
    advData_ltv_t header;       //refer to DT_TX_POWER_LEVEL
    char          txPowerLevel; //0xXX:-127 to +127 dBm.
} advData_txPowerLevel_t;

//Peripheral Connection Interval Range: ACAD not used. Optional in this context (may appear more than once in a block).
//connection interval range:0x0006 to 0x0C80. unit 1.25ms
typedef struct __attribute__((packed))
{
    advData_ltv_t  header; //refer to DT_PERIPHERAL_CONN_INTERVAL_RANGE
    unsigned short minConnInterval;
    unsigned short maxConnInterval;
} advData_connIntervalRange_t;

//Service Solicitation: ACAD not used. Optional in this context (may appear more than once in a block).
typedef struct __attribute__((packed))
{
    advData_ltv_t  header;              //refer to DT_LIST_16BIT_SERVICE_SOLICITATION_UUID
    unsigned short solicitationUuid[0]; //List of 16 bit Service Solicitation UUIDs
} advData_16bitsolicitationUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t header;              //refer to DT_LIST_32BIT_SERVICE_SOLICITATION_UUID
    unsigned int  solicitationUuid[0]; //List of 32 bit Service Solicitation UUIDs
} advData_32bitsolicitationUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t header;              //refer to DT_LIST_128BIT_SERVICE_SOLICITATION_UUID
    bit128UUID_t  solicitationUuid[0]; //List of 128 bit Service Solicitation UUIDs
} advData_128bitsolicitationUuid_t;

//Service Data:Optional in this context (may appear more than once in a block).
typedef struct __attribute__((packed))
{
    advData_ltv_t  header;  //refer to DT_SERVICE_DATA_16BIT_UUID
    unsigned short uuid;    //The first value contains the 16 bit Service UUID.
    unsigned short data[0]; //Any remainder contains additional service data.
} advData_serviceData16BitsUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t  header;  //refer to DT_SERVICE_DATA_32BIT_UUID
    unsigned int   uuid;    //The first value contains the 16 bit Service UUID.
    unsigned short data[0]; //Any remainder contains additional service data.
} advData_serviceData32BitsUuid_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t  header;  //refer to DT_SERVICE_DATA_128BIT_UUID
    bit128UUID_t   uuid;    //The first value contains the 16 bit Service UUID.
    unsigned short data[0]; //Any remainder contains additional service data.
} advData_serviceData128BitsUuid_t;

//Appearance: Optional in this context;  shall not appear more than once in a block and
//              shall not appear in both the AD and SRD of the same extended advertising interval.
typedef struct __attribute__((packed))
{
    advData_ltv_t  header;     //refer to DT_APPEARANCE
    unsigned short appearance; //The Appearance value shall be the enumerated value as defined by Assigned Numbers.
} advData_appearance_t;

//target address
typedef struct __attribute__((packed))
{
    unsigned char addr[6];
} targetAddress_t;

//Public Target Address: Optional in this context;  shall not appear more than once in a block and
//              shall not appear in both the AD and SRD of the same extended advertising interval.
typedef struct __attribute__((packed))
{
    advData_ltv_t header;   //refer to DT_PUBLIC_TARGET_ADDRESS
    targetAddress_t public; //The format of each address is the same as the Public Device Address defined in [Vol 6] Part B, Section 1.3.
} advData_publicTargetAddress_t;

//Random Target Address: Optional in this context;  shall not appear more than once in a block and
//              shall not appear in both the AD and SRD of the same extended advertising interval.
typedef struct __attribute__((packed))
{
    advData_ltv_t   header; //refer to DT_RANDOM_TARGET_ADDRESS
    targetAddress_t random; //The format of each address is the same as the Random Device Address defined in [Vol 6] Part B, Section 1.3.
} advData_randomTargetAddress_t;

//Advertising Interval: Optional in this context; shall not appear more than once in a block.
typedef struct __attribute__((packed))
{
    advData_ltv_t  header;      //refer to DT_ADVERTISING_INTERVAL
    unsigned short advInterval; //units: 0.625ms
} advData_advertisingInterval_t;

typedef struct __attribute__((packed))
{
    advData_ltv_t header; //refer to DT_ADVERTISING_INTERVAL_LONG

    union
    {
        unsigned char advInterval1[3];
        unsigned int  advInterval2;
    }; //units: 0.625ms, must >= 40.96s
} advData_advertisingIntervalLong_t;

//Uniform Resource Identifier: Optional in this context (may appear more than once in a block).
typedef struct __attribute__((packed))
{
    advData_ltv_t header; //refer to DT_URI
    unsigned char uri[0]; //Scheme name string and URI as a UTF-8 string
} advData_uniformResourceIdentifier_t;

//LE Supported Features: Optional in this context; shall not appear more than once in a block.
typedef struct __attribute__((packed))
{
    advData_ltv_t header; //refer to DT_LE_SUPPORTED_FEATURES
    //Each value corresponds to the bit with the same position in the FeatureSet defined in [Vol 6] Part B, Section 4.6.
    unsigned char feature[8];
} advData_leSupportedFeatures_t;

//Channel Map Update Indication: ACAD, Optional in this context; shall not appear more than once in a block.
typedef struct __attribute__((packed))
{
    advData_ltv_t header;     //refer to DT_CHM_UPT_IND
    unsigned char chM[5];     //Channel Map
    unsigned char instant[2]; //Instant
} advData_channelMapUpdateIndication_t;

//BIGInfo: ACAD, Optional in this context; shall not appear more than once in a block.

//Broadcast_Code: OOB only, Optional in this context (may appear more than once in a block)

//Encrypted Data: Optional in this context (may appear more than once in a block)

//Periodic Advertising Response Timing Information: ACAD only,

/* EIR Data Type, Advertising Data Type (AD Type) and OOB Data Type Definitions */
typedef enum
{
    DT_FLAGS                               = 0x01, //  Flag
    DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID  = 0x02, //  Incomplete List of 16-bit Service Class UUIDs
    DT_COMPLETE_LIST_16BIT_SERVICE_UUID    = 0x03, //  Complete List of 16-bit Service Class UUIDs
    DT_INCOMPLETE_LIST_32BIT_SERVICE_UUID  = 0x04, //  Incomplete List of 32-bit Service Class UUIDs
    DT_COMPLETE_LIST_32BIT_SERVICE_UUID    = 0x05, //  Complete List of 32-bit Service Class UUIDs
    DT_INCOMPLETE_LIST_128BIT_SERVICE_UUID = 0x06, //  Incomplete List of 128-bit Service Class UUIDs
    DT_COMPLETE_LIST_128BIT_SERVICE_UUID   = 0x07, //  Complete List of 128-bit Service Class UUIDs
    DT_SHORTENED_LOCAL_NAME                = 0x08, //  Shortened Local Name
    DT_COMPLETE_LOCAL_NAME                 = 0x09, //  Complete Local Name
    DT_TX_POWER_LEVEL                      = 0x0A, //  Tx Power Level

    DT_PERIPHERAL_CONN_INTERVAL_RANGE        = 0x12,
    DT_LIST_16BIT_SERVICE_SOLICITATION_UUID  = 0x14,
    DT_LIST_128BIT_SERVICE_SOLICITATION_UUID = 0x15,
    DT_SERVICE_DATA                          = 0x16, //  Service Data
    DT_SERVICE_DATA_16BIT_UUID               = 0x16,
    DT_PUBLIC_TARGET_ADDRESS                 = 0x17,
    DT_RANDOM_TARGET_ADDRESS                 = 0x18,
    DT_APPEARANCE                            = 0x19, //  Appearance
    DT_ADVERTISING_INTERVAL                  = 0x1A,
    DT_LE_BLUETOOTH_DEVICE_ADDRESS           = 0x1B,
    DT_LE_ROLE                               = 0x1C,

    DT_LIST_32BIT_SERVICE_SOLICITATION_UUID = 0x1F,
    DT_SERVICE_DATA_32BIT_UUID              = 0x20,
    DT_SERVICE_DATA_128BIT_UUID             = 0x21,
    DT_URI                                  = 0x24,
    DT_INDOOR_POSITIONING                   = 0x25,
    DT_TRANSPORT_DISCOVERY_DATA             = 0x26,

    DT_LE_SUPPORTED_FEATURES          = 0x27,
    DT_CHM_UPT_IND                    = 0x28, //  Channel Map Update Indication
    DT_PB_ADV                         = 0x29,
    DT_MESH_MESSAGE                   = 0x2A,
    DT_MESH_BEACON                    = 0x2B,
    DT_BIGINFO                        = 0x2C, //  BIGInfo
    DT_BROADCAST_CODE                 = 0x2D, //  Broadcast_Code
    DT_CSIP_RSI                       = 0x2E,
    DT_ADVERTISING_INTERVAL_LONG      = 0x2F,
    DT_BROADCAST_NAME                 = 0x30, //  Broadcast_Name
    DT_ENCRYPTED_ADVERTISING_DATA     = 0x31,
    DT_PA_RESPONSE_TIMING_INFORMATION = 0x32,
    DT_ELECTRONIC_SHELF_LABEL         = 0x34,
    DT_3D_INFORMATION_DATA            = 0x3D, //  3D Information Data

    DT_MANUFACTURER_SPECIFIC_DATA = 0xFF,     //  Manufacturer Specific Data
} data_type_t;

#define APPEAR_GENERIC_HID  0x0300
#define APPEAR_HID_KEYBOARD 0x03C1
#define APPEAR_HID_MOUSE    0x03C2

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2capLen;
    u16 chanId;
    u8  opcode;
    u16 handle;
    u8  dat[20];
} rf_packet_att_t;

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2cap;
    u16 chanid;

    u8  att;
    u16 handle;

    u8 dat[20];

} rf_packet_att_data_t;

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2capLen;
    u16 chanId;
    u8  opcode;
    u8  datalen;
    u8  data[1]; // character_handle / property / value_handle / value
} rf_packet_att_readByTypeRsp_t;

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2capLen;
    u16 chanId;
    u8  opcode;
    u8  errOpcode;
    u16 errHandle;
    u8  errReason;
} rf_packet_att_errRsp_t;


#endif /* BLE_FORMAT_H */
