/********************************************************************************************************
 * @file    mac.h
 *
 * @brief   This is the header file for mac
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#ifndef MAC_H
#define MAC_H

/**
 * @brief Device type
 */
enum mac_dev_type_e {
    MAC_DEV_UNDEFINED = 0,
    MAC_DEV_FFD,
    MAC_DEV_RFD
};

/**
 * @brief MAC frame version
 */
enum mac_frame_version_e {
    MAC_FRAME_IEEE_802_15_4_2003                = 0,
    MAC_FRAME_IEEE_802_15_4_2006                = 1,
    MAC_FRAME_IEEE_802_15_4                     = 2
};

#define MAC_FRAME_VERSION                       MAC_FRAME_IEEE_802_15_4_2003

#define MAC_INVALID_PANID                       0xFFFF
#define MAC_PAN_ID_BROADCAST                    0xFFFF

#define MAC_SHORT_ADDR_MAX                      0xFFF7
#define MAC_SHORT_ADDR_BROADCAST                0xFFFF
#define MAC_SHORT_ADDR_RESERVED                 0xFFFE

#define ZB_TURN_OFF_ORDER                       15

/* According to 802.15.4, should be 0. */
#define ZB_MAC_INTERNAL_EXPIRY_CNT              0
#define ZB_MAC_PENDING_TRANS_QUEUE_NUM          6

#define TL_ZB_MAC_CHANNEL_START                 11
#define TL_ZB_MAC_CHANNEL_NUM                   16
#define TL_ZB_MAC_CHANNEL_STOP                  (TL_ZB_MAC_CHANNEL_START + TL_ZB_MAC_CHANNEL_NUM - 1)

/**
 * @brief Defines scan duration
 */
#define ZB_MAX_SCAN_DURATION_VALUE              200

/**
 * @brief Define the max number of ED scan results supported
 */
#define MAX_ED_SCAN_RESULTS_SUPPORTED           16

/**
 * @brief Define the max number of PAN descriptors supported
 */
#define MAX_PAN_DESC_SUPPORTED                  6


/**
 * @brief Primitive ID of MAC layer
 */
#define MAC_MCPS_DATA_REQ                       0x40 //!< MCPS Data Request
#define MAC_MCPS_DATA_CNF                       0x41 //!< MCPS Data Confirm
#define MAC_MCPS_DATA_IND                       0x42 //!< MCPS Data Indication
#define MAC_MCPS_PURGE_REQ                      0x43 //!< MCPS Purge request
#define MAC_MCPS_PURGE_CNF                      0x44 //!< MCPS Purge Confirm
#define MAC_MLME_ASSOCIATE_REQ                  0x45 //!< MLME Associate Request
#define MAC_MLME_ASSOCIATE_CNF                  0x46 //!< MLME Associate Confirm
#define MAC_MLME_ASSOCIATE_IND                  0x47 //!< MLME Associate Indication
#define MAC_MLME_ASSOCIATE_RES                  0x48 //!< MLME Associate Response
#define MAC_MLME_DISASSOCIATE_REQ               0x49 //!< MLME Disassociate Request
#define MAC_MLME_DISASSOCIATE_CNF               0x4a //!< MLME Disassociate Confirm
#define MAC_MLME_DISASSOCIATE_IND               0x4b //!< MLME Disassociate Indication
#define MAC_MLME_BEACON_NOTIFY_IND              0x4c //!< MLME Beacon Notify Indication
#define MAC_MLME_GET_REQ                        0x4d //!< MLME Get Request
#define MAC_MLME_GET_CNF                        0x4e //!< MLME Get Confirm
#define MAC_MLME_ORPHAN_IND                     0x4f //!< MLME Orphan Indication
#define MAC_MLME_ORPHAN_RES                     0x50 //!< MLME Orphan Response
#define MAC_MLME_RESET_REQ                      0x51 //!< MLME Reset Request
#define MAC_MLME_RESET_CNF                      0x52 //!< MLME Reset Confirm
#define MAC_MLME_RX_ENABLE_REQ                  0x53 //!< MLME RX Enable Request
#define MAC_MLME_RX_ENABLE_CNF                  0x54 //!< MLME RX Enable Confirm
#define MAC_MLME_SCAN_REQ                       0x55 //!< MLME Scan Request
#define MAC_MLME_SCAN_CNF                       0x56 //!< MLME Scan Confirm
#define MAC_MLME_COMM_STATUS_IND                0x57 //!< MLME Comm Status Indication
#define MAC_MLME_SET_REQ                        0x58 //!< MLME Set Request
#define MAC_MLME_SET_CNF                        0x59 //!< MLME Set Confirm
#define MAC_MLME_START_REQ                      0x5a //!< MLME Start Request
#define MAC_MLME_START_CNF                      0x5b //!< MLME Start Confirm
#define MAC_MLME_SYNC_LOSS_IND                  0x5c //!< MLME Sync Loss Indication
#define MAC_MLME_POLL_REQ                       0x5d //!< MLME Poll Request
#define MAC_MLME_POLL_CNF                       0x5e //!< MLME Poll Confirm
#define MAC_MLME_SCAN_COMPLETE                  0x5f //!< internal use, MLME Scan Complete
#define MAC_MLME_BEACON_REQ_CMD                 0x60 //!< internal use, Receive Beacon Request Command
#define MAC_MLME_DATA_REQ_CMD                   0x61 //!< internal use, Receive Data Request Command
#define MAC_MLME_ASSO_RSP_CNF                   0x62 //!< internal use, Receive Associate Response Confirm
#define MAC_MLME_POLL_IND                       0x63 //!< MLME Poll Indication for zigBee 3.0

/**
 * @brief MAC attributes Identifier
 */
enum {
    MAC_PHY_ATTR_CURRENT_CHANNEL                = 0x00,
    MAC_PHY_ATTR_MODE_CCA                       = 0x03,
    MAC_PHY_ATTR_CURRENT_PAGE                   = 0x04,

    MAC_PIB_ATTRIBUTE_START                     = 0x40, //!< START
    MAC_ATTR_ACK_WAIT_DURATION                  = 0x40, //!< attribute Identifier: macAckWaitDuration
    MAC_ATTR_ASSOCIATION_PERMIT                 = 0x41, //!< attribute Identifier: macAssociationPermit
    MAC_ATTR_AUTO_REQUEST                       = 0x42, //!< attribute Identifier: macAutoRequest
    MAC_ATTR_BATTERY_LIFE_EXT                   = 0x43, //!< attribute Identifier: macBattLifeExt
    MAC_ATTR_BATTERY_LIFE_EXT_PERIODS           = 0x44, //!< attribute Identifier: macBattLifeExtPeriods
    MAC_ATTR_BEACON_PAYLOAD                     = 0x45, //!< attribute Identifier: macBeaconPayload
    MAC_ATTR_BEACON_PAYLOAD_LENGTH              = 0x46, //!< attribute Identifier: macBeaconPayloadLength
    MAC_ATTR_BEACON_ORDER                       = 0x47, //!< attribute Identifier: macBeaconOrder
    MAC_ATTR_BEACON_TX_TIME                     = 0x48, //!< attribute Identifier: macBeaconTxTime
    MAC_ATTR_BSN                                = 0x49, //!< attribute Identifier: macBSN
    MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS       = 0x4A, //!< attribute Identifier: macCoordExtendedAddress
    MAC_ATTR_COORDINATOR_SHORT_ADDRESS          = 0x4B, //!< attribute Identifier: macCoordShortAddress
    MAC_ATTR_DSN                                = 0x4C, //!< attribute Identifier: macDSN
    MAC_ATTR_GTS_PERMIT                         = 0x4D, //!< attribute Identifier: macGTSPermit
    MAC_ATTR_MAX_CSMA_BACKOFFS                  = 0x4E, //!< attribute Identifier: macMaxCSMABackoffs
    MAC_ATTR_MIN_BE                             = 0x4F, //!< attribute Identifier: macMinBE
    MAC_ATTR_PAN_ID                             = 0x50, //!< attribute Identifier: macPANId
    MAC_ATTR_PROMISCUOUS_MODE                   = 0x51, //!< attribute Identifier: macPromiscuousMode
    MAC_ATTR_RX_ON_WHEN_IDLE                    = 0x52, //!< attribute Identifier: macRxOnWhenIdle
    MAC_ATTR_SHORT_ADDRESS                      = 0x53, //!< attribute Identifier: macShortAddress
    MAC_ATTR_SUPERFRAME_ORDER                   = 0x54, //!< attribute Identifier: macSuperframeOrder
    MAC_ATTR_TRANSACTION_PERSISTENCE_TIME       = 0x55, //!< attribute Identifier: macTransactionPersistenceTime
    MAC_ATTR_PAN_COORDASSOCIATED                = 0x56, //!< attribute Identifier: macAssociatedPANCoord
    MAC_ATTR_MAX_BE                             = 0x57, //!< attribute Identifier: macMaxBE
    MAC_ATTR_MAX_FRAME_TOTAL_WAIT_TIME          = 0x58, //!< attribute Identifier: macMaxFrameTotalWaitTime
    MAC_ATTR_MAX_FRAME_RETRIES                  = 0x59, //!< attribute Identifier: macMaxFrameRetries
    MAC_ATTR_RESPONSE_WAIT_TIME                 = 0x5A, //!< attribute Identifier: macMaxFrameRetries
    MAC_ATTR_SYNC_SYMBOL_OFFSET                 = 0x5B, //!< attribute Identifier: macSyncSymbolOffset
    MAC_ATTR_TIMESTAMP_SUPPORTED                = 0x5C, //!< attribute Identifier: macTimestampSupported
    MAC_ATTR_SECURITY_ENABLED                   = 0x5D, //!< attribute Identifier: macSecurityEnabled

    MAC_CONST_EXT_ADDR                          = 0x5E,	//!< attribute Identifier: mac ieee address, internal used only
    MAC_TX_POWER                                = 0x5F,	//!< attribute Identifier: tx power, internal used only
    //added for MAC security
    MAC_KEY_TABLE                               = 0x71, //!< A table of KeyDescriptor entries
    MAC_KEY_TABLE_ENTRIES                       = 0x72, //!< The number of entries in macKeyTable
    MAC_DEVICE_TABLE                            = 0x73, //!< A table of DeviceDescriptor entries
    MAC_DEVICE_TABLE_ENTRIES                    = 0x74, //!< The number of entries in macDeviceTable.
    MAC_SECURITY_LEVEL_TABLE                    = 0x75, //!< A table of SecurityLevelDescriptor entries
    MAC_SECURITY_LEVEL_TABLE_ENTRIES            = 0x76, //!< The number of entries in macSecurityLevelTable.
    MAC_FRAME_COUNTER                           = 0x77, //!< The outgoing frame counter for this device
    MAC_AUTO_REQUEST_SECURITY_LEVEL             = 0x78, //!< The security level used for automatic data requests
    MAC_AUTO_REQUEST_KEYID_MODE                 = 0x79, //!< The key identifier mode used for automatic data requests. This attribute is invalid if the macAutoRequestSecurityLevel attribute is set to 0x00.
    MAC_AUTO_REQUEST_KEY_SOURCE                 = 0x7A, //!< The originator of the key used for automatic data requests. This attribute is invalid if the macAutoRequestKeyIdMode element is invalid or set to 0x00.
    MAC_AUTO_REQUEST_KEY_INDEX                  = 0x7B, //!< The index of the key used for automatic data requests. This attribute is invalid if the macAutoRequestKeyIdMode attribute is invalid or set to 0x00.
    MAC_DEFAULT_KEY_SOURCE                      = 0x7C, //!< The originator of the default key used for key identifier mode 0x01
    MAC_PAN_COORD_EXTENDED_ADDRESS              = 0x7D, //!< The 64-bit address of the PAN coordinator.
    MAC_PAN_COORD_SHORT_ADDRESS                 = 0x7E, //!< The 16-bit short address assigned to the PAN coordinator. A value of 0xfffe indicates that the PAN coordinator is only using its 64-bit extended address. A value of 0xffff indicates that this value is unknown.

    MAC_PIB_ATTRIBUTE_STOP                      = 0x7E,	//!< STOP
};

/**
 * @brief MAC layer status values
 */
typedef enum {
    MAC_SUCCESS                                 = 0x00,/**< Operation successful */
    MAC_STA_PAN_AT_CAPACITY                     = 0x01,
    MAC_STA_PAN_ACCESS_DENIED                   = 0x02,

    MAC_STA_UNSUPPORTED                         = 0x18,
    MAC_STA_BAD_STATE                           = 0x19,
    MAC_STA_NO_RESOURCES                        = 0x1A,
    MAC_STA_TX_ABORTED                          = 0x1D,/**< For internal use only */
    MAC_STA_FRAME_PENDING                       = 0x20,

    MAC_STA_COUNTER_ERROR                       = 0xDB,
    MAC_STA_IMPROPER_KEY_TYPE                   = 0xDC,
    MAC_STA_IMPROPER_SECURITY_LEVEL             = 0xDD,
    MAC_STA_UNSUPPORTED_LEGACY                  = 0xDE,
    MAC_STA_UNSUPPORTED_SECURITY                = 0xDF,
    MAC_STA_BEACON_LOSS                         = 0xE0,
    MAC_STA_CHANNEL_ACCESS_FAILURE              = 0xE1,
    MAC_STA_DENIED                              = 0xE2,
    MAC_STA_DISABLE_TRX_FAILURE                 = 0xE3,
    MAC_STA_SECURITY_ERROR                      = 0xE4,
    MAC_STA_FRAME_TOO_LONG                      = 0xE5,
    MAC_STA_INVALID_GTS                         = 0xE6,
    MAC_STA_INVALID_HANDLE                      = 0xE7,
    MAC_STA_INVALID_PARAMETER                   = 0xE8,
    MAC_STA_NO_ACK                              = 0xE9,
    MAC_STA_NO_BEACON                           = 0xEA,
    MAC_STA_NO_DATA                             = 0xEB,
    MAC_STA_NO_SHORT_ADDRESS                    = 0xEC,
    MAC_STA_OUT_OF_CAP                          = 0xED,
    MAC_STA_PAN_ID_CONFLICT                     = 0xEE,
    MAC_STA_REALIGNMENT_GOT                     = 0xEF,
    MAC_STA_TRANSACTION_EXPIRED                 = 0xF0,
    MAC_STA_TRANSACTION_OVERFLOW                = 0xF1,
    MAC_STA_TX_ACTIVE                           = 0xF2,
    MAC_STA_KEY_UNAVAILABLE                     = 0xF3,
    MAC_STA_UNSUPPORTED_ATTRIBUTE               = 0xF4,
    MAC_STA_INVALID_ADDRESS                     = 0xF5,
    MAC_STA_ON_TIME_TOO_LONG                    = 0xF6,
    MAC_STA_PAST_TIME                           = 0xF7,
    MAC_STA_TRACKING_OFF                        = 0xF8,
    MAC_STA_INVALID_INDEX                       = 0xF9,
    MAC_STA_LIMIT_REACHED                       = 0xFA,
    MAC_STA_READ_ONLY                           = 0xFB,
    MAC_STA_IN_SCAN_DOING                       = 0xFC,
    MAC_STA_SUPERFRAME_OVERLAP                  = 0xFD,
    MAC_STA_AUTOACK_PENDING_ALL_ON              = 0xFE,
    MAC_STA_AUTOACK_PENDING_ALL_OFF             = 0xFF,
} mac_sts_t;

/**
 * @brief CTX status
 */
enum {
    ZB_MAC_STATE_NORMAL                         = 0,
    ZB_MAC_STATE_ED_SCAN,
    ZB_MAC_STATE_ACTIVE_SCAN,
    ZB_MAC_STATE_ORPHAN_SCAN,
    ZB_MAC_STATE_PASSIVE_SCAN,
    ZB_MAC_STATE_INDIRECT_DATA,
};

/**
 * @brief MAC CTX
 */
typedef struct {
    u8 warmStart;               /*!< The restart mode of the MAC layer. */
    u8 status;                  /*!< The status of the MAC layer. */
    u8 curChannel;              /*!< The selected channel of the MAC layer. */
    u8 beaconTriesNum;

    u8 *txRawDataBuf;

    u8 indirectData;
    u8 dev_type;
} tl_zb_mac_ctx_t;

/**
 * @brief Diagnostics for MAC
 */
typedef struct {
    u32 macRxCrcFail;
    u32 macTxCcaFail;
    u32 macRxBcast;
    u32 macTxBcast;
    u32 macRxUcast;
    u32 macTxUcast;
    u16 macTxUcastRetry;
    u16 macTxUcastFail;
    u16 phytoMACqueuelimitreached;
    u8 macTxIrqTimeoutCnt;
    u8 macTxIrqCnt;
    u8 macRxIrqCnt;
    u8 phyLengthError;
} mac_diagnostics_t;

typedef enum zb_addr_mode_e {
    ZB_ADDR_NO_ADDR                             = 0,/*!< 802.15: 0x00 = no address (addressing fields omitted, see 7.2.1.1.8). */
    ZB_ADDR_16BIT_MULTICAST                     = 1,/*!< 802.15: 0x1 = reserved.
                                                     * NWK: 0x01 = 16-bit multicast group address
                                                     * APS: 0x01 = 16-bit group address for DstAddress;
                                                     */
    ZB_ADDR_16BIT_GROUP                         = 1,
    ZB_ADDR_16BIT_DEV_OR_BROADCAST              = 2,/*!< 802.15: 0x02 = 16-bit short address.
                                                     * NWK: 0x02=16-bit network address of a device or a 16-bit broadcast address
                                                     */
    ZB_ADDR_64BIT_DEV                           = 3 /*!< 802.15: 0x03 = 64-bit extended address. */
} zb_addr_mode_t;

/**
 * @brief MAC TX options
 */
enum mac_tx_options_bits_e {
    MAC_TX_OPTION_ACKNOWLEDGED_BIT              = 1,
    MAC_TX_OPTION_GTS_CAP_BIT                   = 2,
    MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT     = 4,
    MAC_TX_OPTION_USE_CSMA_BIT                  = 8,//add for green power
};

/**
 * @brief MCPS-DATA.request
 */
typedef struct {
    u8 *pdu;
    u8 pduLen;
    u8 *msdu;
    u16 dstPanId;
    u8 msduLength;
    u8 msduHandle;
    addr_t srcAddr;
    addr_t dstAddr;
    u8 txOptions;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mcps_data_req_t;//32

/**
 * @brief MCPS-DATA.confirm
 */
typedef struct {
    u8 *msdu;
    u32 timestamp;
    u16 macDstAddr;
    u8 msduHandle;
    u8 status;
    s8 rssi;
    u8 lqi;
} zb_mcps_data_conf_t;//16

/**
 * @brief MCPS-DATA.indication
 */
typedef struct {
    u8 *msdu;
    u32 timestamp;
    u16 srcPanId;
    u16 dstPanId;
    u8 msduLength;
    u8 mpduLinkQuality;
    u8 dsn;
    s8 rssi;
    addr_t srcAddr;
    addr_t dstAddr;

    u8 dataFromTrustLink; //data from trust link
    u8 assumeNwkEncry;    //only for trust link type

#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mcps_data_ind_t;//36

/**
 * @brief MCPS-PURGE.request
 */
typedef struct {
    u8 msduHandle;
} zb_mcps_purge_req_t;

/**
 * @brief MCPS-PURGE.confirm
 */
typedef struct {
    u8 msduHandle;
    u8 status;
} zb_mcps_purge_conf_t;

typedef struct {
    u8 altPanCoord:1;
    u8 devType:1;
    u8 powerSrc:1;
    u8 rcvOnWhenIdle:1;
    u8 reserved:2;
    u8 secuCapability:1;
    u8 allocAddr:1;
} capability_info_t;

/**
 * @brief MLME-ASSOCIATE.request
 */
typedef struct {
    u8 logicalChannel;
    u8 channelPage;
    u16 coordPanId;

    addr_t coordAddress;
    capability_info_t capbilityInfo;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mlme_associate_req_t;

/**
 * @brief MLME-ASSOCIATE.indication
 */
typedef struct {
    addrExt_t devAddress;
    capability_info_t capbilityInfo;
    u8 lqi;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mlme_associate_ind_t;

/**
 * @brief MLME-ASSOCIATE.response
 */
typedef struct {
    u16 shortAddress;
    addrExt_t devAddress;
    u8 status;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mlme_associate_resp_t;

/**
 * @brief MLME-ASSOCIATE.confirm
 */
typedef struct {
    addrExt_t parentAddress;
    u16 shortAddress;
    u8 status;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mlme_associate_conf_t;

enum {
    ZB_MAC_DISASSOCIATE_REASON_RESV             = 0,
    ZB_MAC_DISASSOCIATE_REASON_COORD2DEV        = 1,
    ZB_MAC_DISASSOCIATE_REASON_DEV              = 2,
};

/**
 * @brief MLME-DISASSOCIATE.request
 */
typedef struct {
    u8 disassociateReason;
    bool txIndirect;
    u16 devPanId;
    addr_t devAddr;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mlme_disassociate_req_t;

/**
 * @brief MLME-DISASSOCIATE.indication
 */
typedef struct {
    addrExt_t devAddress;
    u8 disassociateReason;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mlme_disassociate_ind_t;

/**
 * @brief MLME-DISASSOCIATE.confirm
 */
typedef struct {
    addr_t devAddr;
    u8 status;
    u16 devPanId;
} zb_mlme_disassociate_conf_t;

/**
 * @brief Definition of PAN descriptor
 */
typedef struct {
    u32 timestamp;              //!< The time at which the beacon frame was received, in symbols
    u16 coordPanId;             //!< PAN identifier of the coordinator
    u16 superframeSpec;         //!< Super frame specification as specified in the received beacon frame
    u8 logicalChannel;          //!< The current logical channel occupied by the network
    u8 channelPage;             //!< The current channel page occupied by the network
    u8 gtsPermit;               //!< PAN coordinator is accepting GTS requests or not
    u8 linkQuality;             //!< The LQI at which the network beacon was received
    addr_t coordAddr;           //!< Coordinator address
    u8 securityFailure;         //!< Set to TRUE if there was an error in the security processing
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} mac_panDesc_t;

/**
 * @brief MLME-BEACON-NOTIFY.indication
 */
typedef struct {
    u8 *pAddrList;              //!< The list of device addresses for which the sender of the beacon has data
    u8 *psdu;                   //!< The beacon payload
    u8 bsn;                     //!< Beacon Sequence Number
    u8 pendAddrSpec;            //!< The beacon pending address specification
    u8 psduLength;              //!< Number of octets contained in the beacon payload of the beacon frame
    u8 resved;                  //!< for align-4
    mac_panDesc_t panDesc;      //!< The PAN Descriptor for the received beacon
} zb_mlme_beacon_notify_ind_t;

/**
 * @brief MLME-GTS.request
 */
typedef struct {
    u8 gts;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} mac_mlme_gts_req_t;

/**
 * @brief MLME-GTS.confirm
 */
typedef struct {
    u8 gts;
    u8 status;
} mac_mlme_gts_conf_t;

/**
 * @brief MLME-GTS.indication
 */
typedef struct {
    u16 devAddr;
    u8 gts;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} mac_mlme_gts_ind_t;

/**
 * @brief MLME-ORPHAN.indication
 */
typedef struct {
    addrExt_t orphanAddr;
} mac_mlme_orphan_ind_t;

/**
 * @brief MLME-ORPHAN.response
 */
typedef struct {
    addrExt_t orphanAddr;       /*<! The 64 bits address of the orphaned device */
    u16 shortAddr;              /*<! The 16-bit short address allocated to the
                                 *   orphaned device if it is associated with this coordinator */
    bool associated;            /*<! TRUE if the orphaned device is associated with this
                                 *   coordinator or FALSE otherwise */
} mac_mlme_orphan_resp_t;

/**
 * @brief MLME-RESET.request
 */
typedef struct {
    bool setAsDefault;
} mac_mlme_reset_req_t;

/**
 * @brief MLME-RESET.confirm
 */
typedef struct {
    u8 status;
} mac_mlme_reset_conf_t;

/**
 * @brief MLME-RX-ENABLE.request
 */
typedef struct {
    u32 rxOnTime;
    u32 rxOnDuration;
    bool deferPermit;
} mac_mlme_rx_enable_req_t;

/**
 * @brief MLME-RX-ENABLE.confirm
 */
typedef struct {
    u8 status;
} mac_mlme_rx_enable_conf_t;


enum zb_mac_scan_type_e {
    ED_SCAN                     = 0,
    ACTIVE_SCAN                 = 1,
    PASSIVE_SCAN                = 2,
    ORPHAN_SCAN                 = 3
};

/**
 * @brief MLME-SCAN.request
 */
typedef struct {
    u32 scanChannels;           //!< Indicate which channels are to be scanned
    u8 scanType;                //!< The type of scan to be performed
    u8 scanDuration;            //!< The time spent on scanning each channel
    u8 channelPage;             //!< Channel page on which to perform the scan
    u8 resved;                  //!< for align-4
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mac_mlme_scan_req_t;

/**
 * @brief MLME-SCAN.confirm
 */
typedef struct {
    u8 status;                  //!< Status of the scan request
    u8 scanType;                //!< The type of scan performed
    u8 channelPage;             //!< Channel page on which the scan was performed
    u8 resultListSize;          //!< Number of elements returned in the appropriate result lists

    u32 unscannedChannels;      //!< Channels given in the request which were not scanned

    union {                     //!< Define a union of energy measurements list and pan descriptor list
        u8 energyDetectList[MAX_ED_SCAN_RESULTS_SUPPORTED];     //!< List of energy measurements
        mac_panDesc_t panDescList[MAX_PAN_DESC_SUPPORTED];      //!< List of PAN descriptors
    } resultList;
} zb_mac_mlme_scan_conf_t;

/**
 * @brief MLME-COMM-STATUS.indication
 */
typedef struct {
    u16 panId;
    addr_t srcAddr;
    addr_t dstAddr;
    u8 status;
    bool isAssoc;//TRUE is associate, FALSE is orphan
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} zb_mlme_comm_status_ind_t;

/**
 * @brief MLME-START.request
 */
typedef struct {
    u32 startTime;              //!< Time at which to begin transmitting beacons

    u16 panId;                  //!< PAN identifier to be used by the device
    u8 logicalChannel;          //!< Logical channel on which to begin
    u8 channelPage;             //!< Channel page on which to begin

    u8 beaconOrder;             //!< Indicates how often the beacon is to be transmitted
    u8 superframeOrder;         //!< Length of the active portion of the superframe
    u8 panCoordinator;          //!< Length of the active portion of the superframe
    u8 batteryLifeExt;          //!< Indicates if the receiver of the beaconing device is disabled or not

    u8 coordRealignment;        //!< Indicates if the coordinator realignment command is to be transmitted
    u8 rsv[3];                  //!< for align(4)
#if ZB_MAC_SECURITY
    mac_sec_t coordRealignSec;  //!< Security parameters for the coordinator realignment frame
    mac_sec_t beaconSec;        //!< Security parameters for the beacon frame
#endif
} zb_mac_mlme_start_req_t;

/**
 * @brief MLME-START.confirm
 */
typedef struct {
    u8 status;
} mac_mlme_startCnf_t;

/**
 * @brief MLME-SYNC.request
 */
typedef struct {
    u8 logicalChannel;
    u8 channelPage;
    bool trackBeacon;
} mac_mlme_sync_req_t;

typedef enum {
    ZB_SYNC_LOSS_REASON_PAN_ID_CONFLICT,
    ZB_SYNC_LOSS_REASON_REALIGNMENT,
    ZB_SYNC_LOSS_REASON_BEACON_LOST
} mac_sync_loss_reason_e;

/**
 * @brief MLME-SYNC-LOSS.indication
 */
typedef struct {
    u16 panId;                  /* The PAN identifier with which the device lost
                                 * synchronization or to which it was realigned */
    u8 lossReason;              /* The reason that synchronization was lost */
    u8 logicalChannel;          /* Logical channel */
    u8 channelPage;             /* Channel page */
} zb_mlme_sync_loss_ind_t;

/**
 * @brief MLME-POLL.request
 */
typedef struct {
    tl_zb_addr_t coordAddr;
    u16 coordPanId;
    u8 coordAddrMode;
    u8 resved;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} mac_mlme_poll_req_t;

/**
 * @brief MLME-POLL.confirm
 */
typedef struct {
    u8 status;
} mac_mlme_poll_conf_t;

/**
 * @brief MLME-POLL.indication
 */
typedef struct {
    tl_zb_addr_t devAddr;       /*<! The address of the device requesting pending data */
    u8 addrMode;                /*<! 2 - 16 bit short address. 3 - 64 bit extended address */
} mac_mlme_poll_ind_t;


/**
 * @brief MAC frame types
 */
enum mac_frame_type_e {
    MAC_FRAME_BEACON            = 0,
    MAC_FRAME_DATA              = 1,
    MAC_FRAME_ACKNOWLEDGMENT    = 2,
    MAC_FRAME_COMMAND           = 3,
};

/**
 * @brief MAC frame control field
 */
typedef struct {
    u16 frameType:3;
    u16 security:1;
    u16 framePending:1;
    u16 ackReq:1;
    u16 panIdComp:1;
    u16 reserved:3;
    u16 dstAddrMode:2;
    u16 frameVer:2;
    u16 srcAddrMode:2;
} macFrameCtrl_t;

/**
 * @brief MAC header
 */
typedef struct {
    u16 frameCtrl;
    u16 dstPanId;
    u16 srcPanId;
    u8 dstAddrMode;
    u8 srcAddrMode;
    u8 panIdMode;
    u8 seqNum;
    tl_zb_addr_t dstAddr;
    tl_zb_addr_t srcAddr;
#if ZB_MAC_SECURITY
    mac_sec_t sec;
#endif
} tl_zb_mac_mhr_t;


#define BEACON_GTS_DESCROPTION_NUM_MAX  7

typedef struct {
    u8 beaconOrder:4;
    u8 superframeOrder:4;
    u8 finalCapSlot:4;
    u8 batteryLifeExtension:1;
    u8 resv:1;
    u8 panCoordinator:1;
    u8 associatePermit:1;
} zb_superframe_spec_t;

typedef struct {
    u8 descCount:3;
    u8 resv:4;
    u8 permit:1;
} zb_gts_spec_t;

typedef struct {
    u16 shortAddr;
    u8 resved;
    u8 startSlot:4;
    u8 len:4;
} zb_gts_desc_t;

typedef struct {
    zb_gts_spec_t spec;
    u8 direction;
    zb_gts_desc_t desc[BEACON_GTS_DESCROPTION_NUM_MAX];
} zb_mac_gts_t;

typedef struct {
    u8 shortPendingAddrNum:3;
    u8 resv0:1;
    u8 extPendingAddrNum:3;
    u8 resv1:1;
} zb_pending_addr_spec_t;

typedef struct {
    zb_pending_addr_spec_t spec;
    u16 addr[BEACON_GTS_DESCROPTION_NUM_MAX];
} zb_mac_pending_addr_t;

/**
 * @brief MAC Beacon
 */
typedef struct {
    zb_superframe_spec_t superFrameSpec;
    zb_mac_gts_t gts;
    zb_mac_pending_addr_t pendingAddr;
    zb_mac_beacon_payload_t beaconPayload;
} mac_beacon_t;

/**
 * @brief MAC command id
 */
enum mac_command_id {
    MAC_CMD_ASSOCIATION_REQUEST                 = 0x01,
    MAC_CMD_ASSOCIATION_RESPONSE                = 0x02,
    MAC_CMD_DISASSOCIATION_NOTIFICATION         = 0x03,
    MAC_CMD_DATA_REQUEST                        = 0x04,
    MAC_CMD_PAN_ID_CONFLICT_NOTIFICATION        = 0x05,
    MAC_CMD_ORPHAN_NOTIFICATION                 = 0x06,
    MAC_CMD_BEACON_REQUEST                      = 0x07,
    MAC_CMD_COORDINATOR_REALIGNMENT             = 0x08,
    MAC_CMD_GTS_REQUEST                         = 0x09,
};

/**
 * @brief Association request command
 */
typedef struct {
    capability_info_t cap;
} mac_assoc_req_cmd_t;

/**
 * @brief Association response command
 */
typedef struct _attribute_packed_ {
    u16 shortAddr;
    u8 status;
} mac_assoc_resp_cmd_t;

/**
 * @brief Disassociation notification command
 */
typedef struct {
    u8 reason;
} mac_disassoc_noti_cmd_t;

/**
 * @brief Coordinator realignment command
 */
typedef struct _attribute_packed_ {
    u16 panId;
    u16 coordShortAddr;
    u8 logicalChannel;
    u16 shortAddr;
    u8 channelPage;
} mac_coord_realignment_cmd_t;

/**
 * @brief GTS request command
 */
typedef struct {
    u8 gts;
} mac_gts_req_cmd_t;


void tl_zbMacMcpsDataRequestProc(void *arg);

/**
 * @brief Primitive from NWK to MAC layer
 */
#define tl_zbMacMcpsDataRequest(p)      tl_zbMacMcpsDataRequestProc(p)
#define tl_zbMacResetRequest(p)         tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_RESET_REQ, p)
#define tl_zbMacPollRequest(p)          tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_POLL_REQ, p)
#define tl_zbMacAssociateRequest(p)     tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_ASSOCIATE_REQ, p)
#define tl_zbMacDisassociateRequest(p)  tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_DISASSOCIATE_REQ, p)
#define	tl_zbMacAssociateResponse(p)    tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_ASSOCIATE_RES, p)
#define tl_zbMacOrphanResponse(p)       tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_ORPHAN_RES, p)
#define tl_zbMacScanRequest(p)          tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_SCAN_REQ, p)
#define tl_zbMacStartRequest(p)         tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_START_REQ, p)

/**
 * @brief Primitive from MAC to NWK layer
 */
#define tl_zbMacResetConfirm(p)         tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_RESET_CNF, p)
#define tl_zbMacAssociateConfirm(p)     tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_ASSOCIATE_CNF, p)
#define tl_zbMacDisassociateConfirm(p)  tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_DISASSOCIATE_CNF, p)
#define tl_zbMacAssociateIndicate(p)    tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_ASSOCIATE_IND, p)
#define tl_zbMacDisassociateIndicate(p) tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_DISASSOCIATE_IND, p)
#define tl_zbMacDataIndicate(p)         tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MCPS_DATA_IND, p)
#define tl_zbMacDataConfirm(p)          tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MCPS_DATA_CNF, p)
#define tl_zbMacPurgeConfirm(p)         tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MCPS_PURGE_CNF, p)
#define tl_zbMacPollConfirm(p)          tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_POLL_CNF, p)
#define tl_zbMacPollIndication(p)       tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_POLL_IND, p)
#define tl_zbMacBeaconNotifyIndicate(p) tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_BEACON_NOTIFY_IND, p)
#define tl_zbMacOrphanIndicate(p)       tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_ORPHAN_IND, p)
#define tl_zbMacScanConfirm(p)          tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_SCAN_CNF, p)
#define tl_zbMacCommStatusIndication(p) tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_COMM_STATUS_IND, p)
#define tl_zbMacStartConfirm(p)         tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_START_CNF, p)
#define tl_zbMacSyncLossIndication(p)   tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_SYNC_LOSS_IND, p)

typedef bool (*mac_callback)(void *p);

typedef struct {
    mac_callback macBeaconReqRcvCb;
    mac_callback macBeaconRcvCb;
    mac_callback macAssociationReqRcvCb;
} mac_appIndCb_t;

typedef struct {
    const tl_zb_mac_pib_t *pib_dft;
    tl_zb_mac_pib_t *pib;
    mac_diagnostics_t *diags;
} mac_params_t;

extern const addrExt_t g_zero_addr;
extern const addrExt_t g_invalid_addr;
extern u8 ZB_MAC_PENDING_TRANS_QUEUE_SIZE;
extern u8 ZB_MAC_EXT_EXPEIRY_CNT;
extern mac_appIndCb_t *macAppIndCb;
extern mac_diagnostics_t *g_mac_diags;
extern tl_zb_mac_ctx_t g_zbMacCtx;

#define TL_ZB_MAC_STATUS_SET(sta)       (g_zbMacCtx.status = sta)
#define TL_ZB_MAC_STATUS_GET()          (g_zbMacCtx.status)


/**
 * @brief  MAC layer parameters (default PIB, PIB, diagnostics) register
 *
 * @param  param
 *
 * @return none
 */
void mac_params_register(mac_params_t *params);

/**
 * @brief  MAC layer indication register
 *
 * @param  cb - callback
 *
 * @return none
 */
void mac_appIndCbRegister(mac_appIndCb_t *cb);

/**
 * @brief  MAC layer initialization
 *
 * @param  coldReset
 *
 * @return none
 */
void tl_zbMacInit(u8 coldReset);

/**
 * @brief  MAC layer reset
 *
 * @param  none
 *
 * @return none
 */
void tl_zbMacReset(void);

/**
 * @brief  MAC layer main task procedure
 *
 * @param  none
 *
 * @return none
 */
void tl_zbMacTaskProc(void);

#endif /* MAC_H */
