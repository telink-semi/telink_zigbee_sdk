/********************************************************************************************************
 * @file    ble_common.h
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
#ifndef BLE_COMMON_H
#define BLE_COMMON_H

#include "tl_common.h"
#include "myudb.h"
#include "tlkapi_debug.h"

typedef enum
{
    BLE_SUCCESS = 0,

    //// HCI Status, See the Core_v5.0(Vol 2/Part D/1.3 "list of Error Codes") for more information)
    HCI_ERR_UNKNOWN_HCI_CMD                       = 0x01,
    HCI_ERR_UNKNOWN_CONN_ID                       = 0x02,
    HCI_ERR_HW_FAILURE                            = 0x03,
    HCI_ERR_PAGE_TIMEOUT                          = 0x04,
    HCI_ERR_AUTH_FAILURE                          = 0x05,
    HCI_ERR_PIN_KEY_MISSING                       = 0x06,
    HCI_ERR_MEM_CAP_EXCEEDED                      = 0x07,
    HCI_ERR_CONN_TIMEOUT                          = 0x08,
    HCI_ERR_CONN_LIMIT_EXCEEDED                   = 0x09,
    HCI_ERR_SYNCH_CONN_LIMIT_EXCEEDED             = 0x0A,
    HCI_ERR_CONN_ALREADY_EXISTS                   = 0x0B,
    HCI_ERR_CMD_DISALLOWED                        = 0x0C,
    HCI_ERR_CONN_REJ_LIMITED_RESOURCES            = 0x0D,
    HCI_ERR_CONN_REJECTED_SECURITY_REASONS        = 0x0E,
    HCI_ERR_CONN_REJECTED_UNACCEPTABLE_BDADDR     = 0x0F,
    HCI_ERR_CONN_ACCEPT_TIMEOUT_EXCEEDED          = 0x10,
    HCI_ERR_UNSUPPORTED_FEATURE_PARAM_VALUE       = 0x11,
    HCI_ERR_INVALID_HCI_CMD_PARAMS                = 0x12,
    HCI_ERR_REMOTE_USER_TERM_CONN                 = 0x13,
    HCI_ERR_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES = 0x14,
    HCI_ERR_REMOTE_DEVICE_TERM_CONN_POWER_OFF     = 0x15,
    HCI_ERR_CONN_TERM_BY_LOCAL_HOST               = 0x16,
    HCI_ERR_REPEATED_ATTEMPTS                     = 0x17,
    HCI_ERR_PAIRING_NOT_ALLOWED                   = 0x18,
    HCI_ERR_UNKNOWN_LMP_PDU                       = 0x19,
    HCI_ERR_UNSUPPORTED_REMOTE_FEATURE            = 0x1A,
    HCI_ERR_SCO_OFFSET_REJ                        = 0x1B,
    HCI_ERR_SCO_INTERVAL_REJ                      = 0x1C,
    HCI_ERR_SCO_AIR_MODE_REJ                      = 0x1D,
    HCI_ERR_INVALID_LMP_PARAMS                    = 0x1E,
    HCI_ERR_UNSPECIFIED_ERROR                     = 0x1F,
    HCI_ERR_UNSUPPORTED_LMP_PARAM_VAL             = 0x20,
    HCI_ERR_ROLE_CHANGE_NOT_ALLOWED               = 0x21,
    HCI_ERR_LMP_LL_RESP_TIMEOUT                   = 0x22,
    HCI_ERR_LMP_ERR_TRANSACTION_COLLISION         = 0x23,
    HCI_ERR_LMP_PDU_NOT_ALLOWED                   = 0x24,
    HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE           = 0x25,
    HCI_ERR_LINK_KEY_CAN_NOT_BE_CHANGED           = 0x26,
    HCI_ERR_REQ_QOS_NOT_SUPPORTED                 = 0x27,
    HCI_ERR_INSTANT_PASSED                        = 0x28,
    HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED   = 0x29,
    HCI_ERR_DIFFERENT_TRANSACTION_COLLISION       = 0x2A,
    HCI_ERR_RESERVED1                             = 0x2B,
    HCI_ERR_QOS_UNACCEPTABLE_PARAM                = 0x2C,
    HCI_ERR_QOS_REJ                               = 0x2D,
    HCI_ERR_CHAN_ASSESSMENT_NOT_SUPPORTED         = 0x2E,
    HCI_ERR_INSUFFICIENT_SECURITY                 = 0x2F,
    HCI_ERR_PARAM_OUT_OF_MANDATORY_RANGE          = 0x30,
    HCI_ERR_RESERVED2                             = 0x31,
    HCI_ERR_ROLE_SWITCH_PENDING                   = 0x32,
    HCI_ERR_RESERVED3                             = 0x33,
    HCI_ERR_RESERVED_SLOT_VIOLATION               = 0x34,
    HCI_ERR_ROLE_SWITCH_FAILED                    = 0x35,
    HCI_ERR_EXTENDED_INQUIRY_RESP_TOO_LARGE       = 0x36,
    HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST  = 0x37,
    HCI_ERR_HOST_BUSY_PAIRING                     = 0x38,
    HCI_ERR_CONN_REJ_NO_SUITABLE_CHAN_FOUND       = 0x39,
    HCI_ERR_CONTROLLER_BUSY                       = 0x3A,
    HCI_ERR_UNACCEPTABLE_CONN_INTERVAL            = 0x3B,
    HCI_ERR_UNACCEPTABLE_CONN_PARAMETERS          = 0x3B,
    HCI_ERR_ADVERTISING_TIMEOUT                   = 0x3C,
    HCI_ERR_CONN_TERM_MIC_FAILURE                 = 0x3D,
    HCI_ERR_CONN_FAILED_TO_ESTABLISH              = 0x3E,
    HCI_ERR_MAC_CONN_FAILED                       = 0x3F,
    HCI_ERR_COARSE_CLOCK_ADJUSTMENT_REJECT        = 0x40,
    HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED              = 0x41,
    HCI_ERR_UNKNOWN_ADV_IDENTIFIER                = 0x42,
    HCI_ERR_LIMIT_REACHED                         = 0x43,
    HCI_ERR_OP_CANCELLED_BY_HOST                  = 0x44,
    HCI_ERR_PACKET_TOO_LONG                       = 0x45,
    HCI_ERR_TOO_LATE                              = 0x46,
    HCI_ERR_TOO_EARLY                             = 0x47,
    HCI_ERR_INSUFFICIENT_CHANNELS                 = 0x48,
    //DBG used only for CIS
    HCI_ERR_CONN_TERM_CIS_MIC_FAILURE = 0xD3,


    ///////////////////////// TELINK define status /////////////////////////////

    //LL status
    LL_ERR_CONNECTION_NOT_ESTABLISH = 0x80,
    LL_ERR_TX_FIFO_NOT_ENOUGH,
    LL_ERR_ENCRYPTION_BUSY,
    LL_ERR_CURRENT_STATE_NOT_SUPPORTED_THIS_CMD,
    LL_ERR_INVALID_PARAMETER,
    LL_ERR_UNKNOWN_OPCODE,


    //L2CAP status
    L2CAP_ERR_INVALID_PARAMETER = 0x90,
    L2CAP_ERR_INVALID_HANDLE,
    L2CAP_ERR_INSUFFICIENT_RESOURCES,
    L2CAP_ERR_PSM_NOT_REGISTER,
    L2CAP_ERR_CONTROL_NOT_READY,
    L2CAP_ERR_COC_CREATING,
    L2CAP_ERR_COC_DATA_STILL_SENT,
    L2CAP_ERR_NO_CID_AVAILABLE,
    L2CAP_ERR_ALL_CID_ALLOCATED,
    L2CAP_ERR_NO_CREATE_COC_HANDLER,

    //SMP status
    SMP_ERR_INVALID_PARAMETER = 0xA0,
    SMP_ERR_PAIRING_BUSY,
    SMP_ERR_SC_MTU_TOO_SHORT,

    //GATT status
    GATT_ERR_INVALID_PARAMETER = 0xB0,
    GATT_ERR_PREVIOUS_INDICATE_DATA_HAS_NOT_CONFIRMED,
    GATT_ERR_SERVICE_DISCOVERY_TIMEOUT,
    GATT_ERR_NOTIFY_INDICATION_BUSY,
    GATT_ERR_DATA_PENDING_DUE_TO_SERVICE_DISCOVERY_BUSY,
    GATT_ERR_DATA_LENGTH_EXCEED_MTU_SIZE,
    GATT_ERR_DATA_LENGTH_EXCEED_MEM_RESTRICTION,
    GATT_ERR_UNSPECIFIED,

    //GAP status
    GAP_ERR_INVALID_PARAMETER = 0xC0,
    GAP_ERR_STATE_NO_IDLE,
    GAP_ERR_WRITE_BUSY,

    //IAL
    IAL_ERR_SDU_LEN_EXCEED_SDU_MAX,
    IAL_ERR_LOSS_SDU_INTERVAL,
    IAL_ERR_ISO_TX_FIFO_NOT_ENOUGH,
    IAL_ERR_SDU_BUFF_INVALID,
    IAL_ERR_EVENT_PASSED,
    IAL_ERR_INVALID_PDU,
    IAL_HCI_BUFFER_INVALID,

    //Service status
    SERVICE_ERR_INVALID_PARAMETER = 0xD0,

    //Profile common error
    PRF_ERR_INVALID_ATTR_HANDLE,
    PRF_ERR_INVALID_PARAMETER,

    //Audio Profile status
    AUDIO_ERR_NO_MEMORY = 0xE0,
    AUDIO_ERR_INVALID_PARAMETER,
    AUDIO_ERR_DISCOVERY_FAILED,
    AUDIO_ERR_BUSY,
    AUDIO_ERR_STATUS,

    //LE Audio Server
    LE_AUDIO_SERVER_INVALID_SERVICE = 0xF0,
    LE_AUDIO_SERVER_INVALID_HANDLE,

} ble_sts_t;


/**
 *  @brief  Definition for Error Response of ATTRIBUTE PROTOCOL PDUS
 *  See the Core_v5.0(Vol 3/Part F/3.4.1.1, "Error Response") for more information.
 */
typedef enum
{

    ATT_SUCCESS = 0,

    ATT_ERR_INVALID_HANDLE         = 0x01, //!< The attribute handle given was not valid on this server
    ATT_ERR_READ_NOT_PERMITTED     = 0x02, //!< The attribute cannot be read
    ATT_ERR_WRITE_NOT_PERMITTED    = 0x03, //!< The attribute cannot be written
    ATT_ERR_INVALID_PDU            = 0x04, //!< The attribute PDU was invalid
    ATT_ERR_INSUFFICIENT_AUTH      = 0x05, //!< The attribute requires authentication before it can be read or written
    ATT_ERR_REQ_NOT_SUPPORTED      = 0x06, //!< Attribute server does not support the request received from the client
    ATT_ERR_INVALID_OFFSET         = 0x07, //!< Offset specified was past the end of the attribute
    ATT_ERR_INSUFFICIENT_AUTHOR    = 0x08, //!< The attribute requires authorization before it can be read or written
    ATT_ERR_PREPARE_QUEUE_FULL     = 0x09, //!< Too many prepare writes have been queued
    ATT_ERR_ATTR_NOT_FOUND         = 0x0A, //!< No attribute found within the given attribute handle range
    ATT_ERR_ATTR_NOT_LONG          = 0x0B, //!< The attribute cannot be read or written using the Read Blob Request
    ATT_ERR_INSUFFICIENT_KEY_SIZE  = 0x0C, //!< The Encryption Key Size used for encrypting this link is insufficient
    ATT_ERR_INVALID_ATTR_VALUE_LEN = 0x0D, //!< The attribute value length is invalid for the operation
    ATT_ERR_UNLIKELY_ERR           = 0x0E, //!< The attribute request that was requested has encountered an error that was unlikely, and therefore could not be completed as requested
    ATT_ERR_INSUFFICIENT_ENCRYPT   = 0x0F, //!< The attribute requires encryption before it can be read or written
    ATT_ERR_UNSUPPORTED_GRP_TYPE   = 0x10, //!< The attribute type is not a supported grouping attribute as defined by a higher layer specification
    ATT_ERR_INSUFFICIENT_RESOURCES = 0x11, //!< Insufficient Resources to complete the request
    ATT_ERR_DATABASE_OUT_OF_SYNC   = 0x12, //!< The server requests the client to rediscover the database
    ATT_ERR_VALUE_NOT_ALLOWED      = 0x13, //!< The attribute parameter value was not allowed

//#ifdef BLC_ZIGBEE_INTEGRATION
    ATT_ERR_ZIGBEE_DIRECT_APP_ERROR = 0x80,//!<The attribute PDU was invalid for zigbee direct
//#endif /* BLC_ZIGBEE_INTEGRATION */

    /* List of Common Profile and Service Error Codes */
    ATT_ERR_WRITE_REQUEST_REJECT = 0xFC,          //!< Write Request Rejected
    ATT_ERR_CCC_DESCRIPTOR_IMPROPERLY_CONFIGURED, //!< Client Characteristic Configuration Descriptor Improperly Configured
    ATT_ERR_PROCEDURE_ALREADY_IN_PROGRESS,        //!< Procedure Already in Progress
    ATT_ERR_OUT_OF_RANGE,                         //!< Out of Range

} att_err_t;


/**
 *  @brief  error code for user initialization error
 */
typedef enum
{
    INIT_SUCCESS = 0,

    ///////// Controller ///////////
    LL_ACL_RX_BUF_NO_INIT = 0x1001,
    LL_ACL_RX_BUF_PARAM_INVALID,
    LL_ACL_RX_BUF_SIZE_NOT_MEET_MAX_RX_OCT,
    LL_ACL_TX_BUF_NO_INIT,
    LL_ACL_TX_BUF_PARAM_INVALID,
    LL_ACL_TX_BUF_SIZE_NOT_MEET_MAX_TX_OCT,

    HCI_ACL_DATA_BUF_PARAM_INVALID,
    HCI_ACL_DATA_BUF_SIZE_NOT_MEET_MAX_TX_OCT,

    LL_ACL_CONN_NUM_NOT_SUPPORTED,


    LL_ACL_TX_BUF_SIZE_MUL_NUM_EXCEED_4K = 0x1071, /* special, B91 only */


    LL_BIS_TX_BUF_NO_INIT = 0x1081,
    LL_BIS_TX_BUF_PARAM_INVALID,
    LL_BIS_RX_BUF_NO_INIT,
    LL_BIS_RX_BUF_PARAM_INVALID,
    LL_BIS_RX_PDU_INVALID,
    LL_BIS_RX_PDU_EMPTY,
    LL_BIS_RX_EVT_BUF_NO_INIT,
    LL_BIS_RX_EVT_BUF_PARAM_INVALID,
    LL_BIS_RX_IAL_BUF_NO_INIT,
    LL_BIS_TX_IAL_BUF_NO_INIT,


    LL_CIS_RX_BUF_NO_INIT = 0x1091,
    LL_CIS_RX_BUF_PARAM_INVALID,
    LL_CIS_TX_BUF_NO_INIT,
    LL_CIS_TX_BUF_PARAM_INVALID,
    LL_CIS_RX_EVT_BUF_NO_INIT,
    LL_CIS_RX_EVT_BUF_PARAM_INVALID,
    LL_CIS_RX_IAL_BUF_NO_INIT,
    LL_CIS_TX_IAL_BUF_NO_INIT,


    ////////////////// Host /////////////////////
    //GAP
    INIT_ERR_GAP_PARAM_INVALID = 0x2001,

    //L2CAP
    INIT_ERR_L2CAP_PARAM_INVALID = 0x2101,


    //ATT
    INIT_ERR_ATT_PARAM_INVALID = 0x2201,


    //GATT
    INIT_ERR_GATT_PARAM_INVALID = 0x2301,

    //SMP
    INIT_ERR_SMP_PARAM_INVALID = 0x2401,
    INIT_ERR_SMP_BONDING_MAX_NUMBER_EXCEED,

    ////////////////// system /////////////////////
    SYS_ERR_CLK_TOO_SLOW = 0x3001, //System clock is too slow: Triggered when the system clock frequency is below the minimum required threshold

} init_err_t;

/////////////////////////////// BLE  MAC ADDRESS //////////////////////////////////////////////
#define BLE_ADDR_PUBLIC         0 //BDA: Public
#define BLE_ADDR_RANDOM         1 //BDA: Random
#define BLE_ADDR_PUBLIC_ID      2 //BDA: Public Identity Address, RPA
#define BLE_ADDR_RANDOM_ID      3 //BDA: Random (Static) Identity Address, RPA

#define BLE_ADDR_INVALID 0xff
#define BLE_ADDR_LEN     6


//Definition for BLE Common Address Type
/*
 *
 *                |--public  ..................................................... BLE_DEVICE_ADDRESS_PUBLIC
 *                |
 * Address Type --|           |-- random static  ................................. BLE_DEVICE_ADDRESS_RANDOM_STATIC
 *                |           |
 *                |--random --|
 *                            |                    |-- non_resolvable private  ... BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE
 *                            |-- random private --|
 *                                                 |-- resolvable private  ....... BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE
 *
 */

#define BLE_DEVICE_ADDRESS_PUBLIC                 1
#define BLE_DEVICE_ADDRESS_RANDOM_STATIC          2
#define BLE_DEVICE_ADDRESS_NON_RESOLVABLE_PRIVATE 3
#define BLE_DEVICE_ADDRESS_RESOLVABLE_PRIVATE     4


#define IS_PUBLIC_ADDR(Type, Addr)                      ( (Type)==BLE_ADDR_PUBLIC) )
#define IS_RANDOM_STATIC_ADDR(Type, Addr)          ((Type) == BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0xC0)
#define IS_NON_RESOLVABLE_PRIVATE_ADDR(Type, Addr) ((Type) == BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x00)
#define IS_RESOLVABLE_PRIVATE_ADDR(Type, Addr)     ((Type) == BLE_ADDR_RANDOM && (Addr[5] & 0xC0) == 0x40)


#define MAC_MATCH16(md, ms)                        (md[0] == ms[0] && md[1] == ms[1] && md[2] == ms[2])

/////////////////////////////////////////////////////////////////////////////


/******************************************** ATT ***************************************************************/
/**
 *  @brief  Definition for Attribute protocol PDUs
 */
typedef enum
{
    ATT_OP_ERROR_RSP              = 0x01,
    ATT_OP_EXCHANGE_MTU_REQ       = 0x02,
    ATT_OP_EXCHANGE_MTU_RSP       = 0x03,
    ATT_OP_FIND_INFORMATION_REQ   = 0x04,
    ATT_OP_FIND_INFO_REQ          = 0x04,
    ATT_OP_FIND_INFORMATION_RSP   = 0x05,
    ATT_OP_FIND_INFO_RSP          = 0x05,
    ATT_OP_FIND_BY_TYPE_VALUE_REQ = 0x06,
    ATT_OP_FIND_BY_TYPE_VALUE_RSP = 0x07,
    ATT_OP_READ_BY_TYPE_REQ       = 0x08,
    ATT_OP_READ_BY_TYPE_RSP       = 0x09,
    ATT_OP_READ_REQ               = 0x0A,
    ATT_OP_READ_RSP               = 0x0B,
    ATT_OP_READ_BLOB_REQ          = 0x0C,
    ATT_OP_READ_BLOB_RSP          = 0x0D,
    ATT_OP_READ_MULTIPLE_REQ      = 0x0E,
    ATT_OP_READ_MULTI_REQ         = 0x0E,
    ATT_OP_READ_MULTIPLE_RSP      = 0x0F,
    ATT_OP_READ_BY_GROUP_TYPE_REQ = 0x10,
    ATT_OP_READ_BY_GROUP_TYPE_RSP = 0x11,
    ATT_OP_WRITE_REQ              = 0x12,
    ATT_OP_WRITE_RSP              = 0x13,
    ATT_OP_PREPARE_WRITE_REQ      = 0x16,
    ATT_OP_PREPARE_WRITE_RSP      = 0x17,
    ATT_OP_EXECUTE_WRITE_REQ      = 0x18,
    ATT_OP_EXECUTE_WRITE_RSP      = 0x19,

    ATT_OP_HANDLE_VALUE_NTF  = 0x1B,
    ATT_OP_HANDLE_VALUE_NOTI = 0x1B,
    ATT_OP_HANDLE_VALUE_IND  = 0x1D,
    ATT_OP_HANDLE_VALUE_CFM  = 0x1E,

    ATT_OP_READ_MULTIPLE_VARIABLE_REQ = 0x20, //core_5.2
    ATT_OP_READ_MULTIPLE_VARIABLE_RSP = 0x21, //core_5.2
    ATT_OP_MULTIPLE_HANDLE_VALUE_NTF  = 0x23, //core_5.2

    ATT_OP_WRITE_CMD        = 0x52,
    ATT_OP_SIGNED_WRITE_CMD = 0xD2,
} att_pdu_type;

/**
 * @brief   In Spec and HCI final pkt, HCI ACL DATA buffer length = LE_ACL_Data_Packet_Length + 4.
 *          pkt_len is integer multiple of 4, so result is 4 Byte align.
 *          See the Core_v5.0(Vol 3/Part F/3.4.1.1, "Error Response") for more information.
 *          However when dealing with RxFifo to HCI, Telink use a byte to PBFlag.
 *          So Telink Controller used 5 = 2(connHandle) + 1(PBFlag) + 2(length)
 */
#define CALCULATE_HCI_ACL_DATA_FIFO_SIZE(pkt_len) ((pkt_len + 5 + 3) / 4 * 4)


/**
 * @brief   6 = header(2)+l2cap_len(2)+CID(2)
 */
#define CAL_L2CAP_BUFF_SIZE(n) (((n + 6) + 3) / 4 * 4)

/**
 * @brief   12 = type(1) + len(1) + l2cap_len(2) + cid(2) + sud_len(2) + mic(4)
 */
#define L2CAP_ALIGN4_KFRAM_DMA_BUFF(n) (((n + 12) + 3) / 4 * 4)

/**
 * @brief   CIS TX FIFO Size = n + CIS_TX_PDU_BUF_EXT_LEN + TLK_RF_TX_EXT_LEN
 * CIS_TX_PDU_BUF_EXT_LEN is extra data length designed by Stack
 * TLK_RF_TX_EXT_LEN depend on MCU
 * CIS TX FIFO do not use hardware FIFO, so no 16 byte align limitation
 * TX dma_len must be 4 byte align, so total buffer need 4 byte align */
#define CAL_LL_CIS_TX_FIFO_SIZE(n) DATA_LENGTH_ALIGN4(n + CIS_TX_PDU_BUF_EXT_LEN + TLK_RF_TX_EXT_LEN)

#define BIS_PDU_ALIGN4_TXBUFF(n)   DATA_LENGTH_ALIGN4((CAL_LL_ISO_TX_FIFO_SIZE(n) + 12)) //12=OFFSETOF(bis_tx_pdu_t, isoTxPdu)

#define BIS_PDU_ALIGN4_RXBUFF(n)   DATA_LENGTH_ALIGN4(BIS_LL_RX_PDU_FIFO_SIZE(n))


/*
++++++++++
| 2            | 2            | 4          | 2          | 1          | 1       | 1  | 1  | iso_sdu_len  |
++++++++++
| pkt_seq_num  | iso_sdu_len  | timestamp  | sduOffset  | numHciPkt  | pkt_st  | PB | TS | SDU_Data     |
++++++++++

total len = 14
*/

#define IAL_SDU_HDR_EN               14
#define CAL_IAL_ISO_SDU_FIFO_SIZE(n) (((n + IAL_SDU_HDR_EN) + 3) / 4 * 4)


/*
HCI TX fifo include ACL data report, HCI Event report (controller to host)

1. HCI ACL out Data format in telink
++++++
| 2         | 1     | 2       | 2         | n               |
++++++
| uart_len  | type  | handle  | data_len  | data_total_len  |
++++++
 */
#define HCI_ACL_OUT_FIFO_SIZE(n) ((n + 7 + 3) / 4 * 4)

/* HCI ISO out DATA format in telink
+++++++++
| 2         | 1     | 2       | 2                     | 4          | 2                    | 2               | n         |
+++++++++
| uart_len  | type  | handle  | ISO_data_load_length  | timestamp  | packet_sequence_num  | iso_sdu_length  | sdu_data  |
+++++++++
 */
#define HCI_ISO_OUT_FIFO_SIZE(n) ((n + 15 + 3) / 4 * 4)


//HCI OUT hold ISO data and ACL data, Event
#define HCI_OUT_FIFO_MAX_SIZE(acl_len_max, iso_len_max) (max2(HCI_ACL_OUT_FIFO_SIZE(acl_len_max), HCI_ISO_OUT_FIFO_SIZE(iso_len_max)))


/* HCI ACL in data format in telink  (host to controller)
 *
+++++
| 1     | 2       | 2         | n               |
+++++
| type  | handle  | data_len  | data_total_len  |
+++++
 */
#define HCI_ACL_IN_FIFO_SIZE(n) ((n + 5 + 3) / 4 * 4)

/* HCI ISO in data format in telink  (host to controller)
++++++++
| 1     | 2       | 2                     | 4          | 2                    | 2               | n         |
++++++++
| type  | handle  | ISO_data_load_length  | timestamp  | packet_sequence_num  | iso_sdu_length  | sdu_data  |
++++++++
 */
#define HCI_ISO_IN_FIFO_SIZE(n) (((n + 13) + 3) / 4 * 4)


/*
 * HCI in fifo include HCI ACL data and CMD, ISO data
 */
#define HCI_IN_FIFO_MAX_SIZE(acl_len_max, iso_len_max) (max2(HCI_ACL_IN_FIFO_SIZE(acl_len_max), HCI_ISO_IN_FIFO_SIZE(iso_len_max)))


/* for BLE B91 old SDK macro compatible with new SDK */
/**
 * @brief   6 = header(2)+l2cap_len(2)+CID(2)
 */
#define CAL_MTU_BUFF_SIZE(n) (((n + 6) + 3) / 4 * 4)


/**
 * @brief      ota crc16 related function.
 * @param[in]  pD: input data.
 * @param[in]  len: data length.
 * @return     crc result.
 */
unsigned short blt_Crc16ComputeInternal (unsigned char *pD, int len);


/**
 * @brief      get library build information (branch, commit, timestamps)
 * @param[in]  pbuf - the pointer to the lib info string.
 *             eg. "develop 105b2c862 Thu Jun 19 21:09:30 2025 +0800 Dirty 2025-06-19 23:29:58"
 * @param[in]  number - the size of the buffer.
 * @return     the actual length of lib info string.
 */
unsigned char blc_get_sdk_ble_lib_info(unsigned char *pbuf, unsigned char number);

#endif
