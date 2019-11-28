/********************************************************************************************************
 * @file     tl_zb_mac.h
 *
 * @brief    mac layer header file
 *
 * @author
 * @date     Oct. 11, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef TL_ZB_MAC_H__
#define TL_ZB_MAC_H__

#include "tl_common.h"
#include "../../common/includes/zb_buffer.h"

/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  TELINK_ZIGBEE_MAC telink mac
 *  @{
 */

#define MAX_PHY_FRM_SIZE   127

#define ZB_TURN_OFF_ORDER  15

#define ZB_MAC_SHORT_ADDR_NOT_ALLOCATED 0xFFFE


/**
 *  @brief Broadcast PAN ID
 */
#define MAC_SHORT_ADDR_MAX 			0xFFF7
#define MAC_PAN_ID_BROADCAST        0xFFFF
#define MAC_ADDR_USE_EXT            0xFFFE  //!< Short address value indicating extended address is used
#define MAC_SHORT_ADDR_BROADCAST    0xFFFF  //!< Broadcast short address
#define MAC_SHORT_ADDR_NONE         0xFFFF  //!< Short address when there is no short address


#define TL_ZB_MAC_CHANNEL_START			11

#define TL_ZB_MAC_CHANNEL_NUM			16

#define TL_ZB_MAC_CHANNEL_STOP			(TL_ZB_MAC_CHANNEL_START + TL_ZB_MAC_CHANNEL_NUM - 1)

/**
 *  @brief Key source length definition in Mac security
 */
#define MAC_KEY_SOURCE_MAX_LEN          8

/**
 *  @brief Define the max number of ED scan results supported
 */
#define MAX_ED_SCAN_RESULTS_SUPPORTED   16

/**
 *  @brief Define the max number of PAN descriptors supported
 */
#define MAX_PAN_DESC_SUPPORTED          6

enum mac_frame_version_e{
	MAC_FRAME_IEEE_802_15_4_2003 = 0,
	MAC_FRAME_IEEE_802_15_4      = 1
};

#define MAC_FCF_ACK_REQ_BIT    0x20
#define MAC_FCF_INTRA_PAN_BIT  0x40
#define MAC_FCF_DST_ADDR_BIT   0x0c
#define MAC_FCF_SRC_ADDR_BIT   0xc0
#define MAC_FCF_FRAME_TYPE     0x07


typedef enum zb_addr_mode_e
{
  ZB_ADDR_NO_ADDR = 0,          /*!< 802.15: 0x00 = no address (addressing fields omitted, see 7.2.1.1.8). */
  ZB_ADDR_16BIT_MULTICAST = 1,  /*!< 802.15: 0x1 = reserved.
                                     NWK:    0x01 = 16-bit multicast group address
                                     APS:    0x01 = 16-bit group address for DstAddress;
                                 */
#define ZB_ADDR_16BIT_GROUP  ZB_ADDR_16BIT_MULTICAST

  ZB_ADDR_16BIT_DEV_OR_BROADCAST = 2, /*!< 802.15: 0x02 = 16-bit short address.
                                           NWK:    0x02=16-bit network address of a device or a 16-bit broadcast address
                                      */

  ZB_ADDR_64BIT_DEV = 3         /*!< 802.15: 0x03 = 64-bit extended address.  */
}zb_addr_mode_t;

/**
   Device types
 */
enum{
  MAC_DEV_UNDEFINED,
  MAC_DEV_FFD,
  MAC_DEV_RFD
};

/**
   MAC options bits
 */
enum mac_tx_options_bits_e
{
  MAC_TX_OPTION_ACKNOWLEDGED_BIT          = 1,
  MAC_TX_OPTION_GTS_CAP_BIT               = 2,
  MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT = 4,
  MAC_TX_OPTION_USE_CSMA_BIT			  = 8,//add for green power
};

/**
   MAC frame types
 */
enum mac_frame_type_e
{
  MAC_FRAME_BEACON         = 0,
  MAC_FRAME_DATA           = 1,
  MAC_FRAME_ACKNOWLEDGMENT = 2,
  MAC_FRAME_COMMAND        = 3,
  MAC_FRAME_RESERVED1      = 4,
  MAC_FRAME_RESERVED2      = 5,
  MAC_FRAME_RESERVED3      = 6,
  MAC_FRAME_RESERVED4      = 7
};


/** @addtogroup zb_mac_attribute_id Zigbee Mac attributes Identifier
 *  Definition for zigbee Mac attributes Identifier
 *  @{
 */
enum{
	MAC_PHY_ATTR_CURRENT_CHANNEL			= 		 0x00,
	MAC_PHY_ATTR_MODE_CCA				= 		 0x03,
	MAC_PHY_ATTR_CURRENT_PAGE			=        0x04,

	MAC_PIB_ATTRIBUTE_START			= 		0x40,	//!<
	MAC_ATTR_ACK_WAIT_DURATION      =       0x40,	//!< attribute Identifier: macAckWaitDuration
	MAC_ATTR_ASSOCIATION_PERMIT     =       0x41,	//!< attribute Identifier: macAssociationPermit
	MAC_ATTR_AUTO_REQUEST           =       0x42,	//!< attribute Identifier: macAutoRequest
	MAC_ATTR_BATTERY_LIFE_EXT          =    0x43,	//!< attribute Identifier: macBattLifeExt
	MAC_ATTR_BATTERY_LIFE_EXT_PERIODS  =    0x44,	//!< attribute Identifier: macBattLifeExtPeriods
	MAC_ATTR_BEACON_PAYLOAD         =       0x45,	//!< attribute Identifier: macBeaconPayload
	MAC_ATTR_BEACON_PAYLOAD_LENGTH  =       0x46,	//!< attribute Identifier: macBeaconPayloadLength
	MAC_ATTR_BEACON_ORDER           =       0x47,	//!< attribute Identifier: macBeaconOrder
	MAC_ATTR_BEACON_TX_TIME         =       0x48,	//!< attribute Identifier: macBeaconTxTime
	MAC_ATTR_BSN                    =       0x49,	//!< attribute Identifier: macBSN
	MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS = 0x4A,	//!< attribute Identifier: macCoordExtendedAddress
	MAC_ATTR_OORDINATOR_SHORT_ADDRESS    =  0x4B,	//!< attribute Identifier: macCoordShortAddress
	MAC_ATTR_DSN                    =       0x4C,	//!< attribute Identifier: macDSN
	MAC_ATTR_GTS_PERMIT             =       0x4D,	//!< attribute Identifier: macGTSPermit
	MAC_ATTR_MAX_CSMA_BACKOFFS      =       0x4E,	//!< attribute Identifier: macMaxCSMABackoffs
	MAC_ATTR_MIN_BE                 =       0x4F,	//!< attribute Identifier: macMinBE
	MAC_ATTR_PAN_ID                 =       0x50,	//!< attribute Identifier: macPANId
	MAC_ATTR_PROMISCUOUS_MODE       =       0x51,	//!< attribute Identifier: macPromiscuousMode
	MAC_ATTR_RX_ON_WHEN_IDLE        =       0x52,	//!< attribute Identifier: macRxOnWhenIdle
	MAC_ATTR_SHORT_ADDRESS          =       0x53,	//!< attribute Identifier: macShortAddress
	MAC_ATTR_SUPERFRAME_ORDER       =       0x54,	//!< attribute Identifier: macSuperframeOrder
	MAC_ATTR_TRANSACTION_PERSISTENCE_TIME = 0x55,	//!< attribute Identifier: macTransactionPersistenceTime
	MAC_ATTR_PAN_COORDASSOCIATED   =        0x56,	//!< attribute Identifier: macAssociatedPANCoord
	MAC_ATTR_MAX_BE                 =       0x57,	//!< attribute Identifier: macMaxBE
	MAC_ATTR_MAX_FRAME_TOTAL_WAIT_TIME    = 0x58,	//!< attribute Identifier: macMaxFrameTotalWaitTime
	MAC_ATTR_MAX_FRAME_RETRIES      =       0x59,	//!< attribute Identifier: macMaxFrameRetries
	MAC_ATTR_RESPONSE_WAIT_TIME     =       0x5A,	//!< attribute Identifier: macMaxFrameRetries
	MAC_ATTR_SYNC_SYMBOL_OFFSET     =       0x5B,	//!< attribute Identifier: macSyncSymbolOffset
	MAC_ATTR_TIMESTAMP_SUPPORTED    =       0x5C,	//!< attribute Identifier: macTimestampSupported
	MAC_ATTR_SECURITY_ENABLED       =        0x5D,	//!< attribute Identifier: macSecurityEnabled

	MAC_CONST_EXT_ADDR			=       0x5E,		//!< attribute Identifier: mac ieee address, internal used only
	MAC_TX_POWER				=		0x5F,		//!< attribute Identifier: tx power, internal used only
	MAC_PIB_ATTRIBUTE_STOP		=       0x5F    ,	//!< attribute Identifier: autoRequest
};

/** @} end of group zb_mac_attribute_id */



/* MAC security */
#ifdef ZB_MAC_SECURITY
#define MAC_SECUR_LEV5_KEYID1_AUX_HDR_SIZE 6
#else
#define MAC_SECUR_LEV5_KEYID1_AUX_HDR_SIZE 0
#endif

#define ZB_MAC_SECURITY_LEVEL 5
#define ZB_MAC_KEY_ID_MODE    1

/**
   MAC command frame id
 */
enum mac_command_frame_id
{
  MAC_CMD_ASSOCIATION_REQUEST = 1,
  MAC_CMD_ASSOCIATION_RESPONSE,
  MAC_CMD_DISASSOCIATION_NOTIFICATION,
  MAC_CMD_DATA_REQUEST,
  MAC_CMD_PAN_ID_CONFLICT_NOTIFICATION,
  MAC_CMD_ORPHAN_NOTIFICATION,
  MAC_CMD_BEACON_REQUEST,          /* 7 */
  MAC_CMD_COORDINATOR_REALIGNMENT,
  MAC_CMD_GTS_REQUEST,

  MAC_CMD_TELINK_BASE = 0xA0,
  MAC_CMD_TELINK_OTA,
};

enum{
	ZB_MAC_STATE_NORMAL = 0,
	ZB_MAC_STATE_ED_SCAN,
	ZB_MAC_STATE_ACTIVE_SCAN,
	ZB_MAC_STATE_ORPHAN_SCAN,
	ZB_MAC_STATE_PASSIVE_SCAN,
	ZB_MAC_STATE_INDIRECT_DATA,
};


typedef enum {
	MAC_SUCCESS                 = 0x00,                /**< Operation successful */
    MAC_STA_PAN_AT_CAPACITY         = 0x1,
    MAC_STA_PAN_ACCESS_DENIED       = 0x2,
	MAC_STA_AUTOACK_PENDING_ALL_ON  = 0xFE,
    MAC_STA_AUTOACK_PENDING_ALL_OFF = 0xFF,
    MAC_STA_BEACON_LOSS             = 0xE0,
    MAC_STA_CHANNEL_ACCESS_FAILURE  = 0xE1,
    MAC_STA_COUNTER_ERROR           = 0xDB,
    MAC_STA_DENIED                  = 0xE2,
    MAC_STA_DISABLE_TRX_FAILURE     = 0xE3,
    MAC_STA_FRAME_TOO_LONG          = 0xE5,
    MAC_STA_IMPROPER_KEY_TYPE       = 0xDC,
    MAC_STA_IMPROPER_SECURITY_LEVEL = 0xDD,
    MAC_STA_INVALID_ADDRESS         = 0xF5,
    MAC_STA_INVALID_GTS             = 0xE6,
    MAC_STA_INVALID_HANDLE          = 0xE7,
    MAC_STA_INVALID_INDEX           = 0xF9,
    MAC_STA_INVALID_PARAMETER       = 0xE8,
    MAC_STA_LIMIT_REACHED           = 0xFA,
    MAC_STA_NO_ACK                  = 0xE9,
    MAC_STA_NO_BEACON               = 0xEA,
    MAC_STA_NO_DATA                 = 0xEB,
    MAC_STA_NO_SHORT_ADDRESS        = 0xEC,
    MAC_STA_ON_TIME_TOO_LONG        = 0xF6,
    MAC_STA_OUT_OF_CAP              = 0xED,
    MAC_STA_PAN_ID_CONFLICT         = 0xEE,
    MAC_STA_PAST_TIME               = 0xF7,
    MAC_STA_READ_ONLY               = 0xFB,
    MAC_STA_REALIGNMENT_GOT         = 0xEF,
    MAC_STA_IN_SCAN_DOING		    = 0xFC,
    MAC_STA_SECURITY_ERROR          = 0xE4,
    MAC_STA_SUPERFRAME_OVERLAP      = 0xFD,
    MAC_STA_TRACKING_OFF            = 0xF8,
    MAC_STA_TRANSACTION_EXPIRED     = 0xF0,
    MAC_STA_TRANSACTION_OVERFLOW    = 0xF1,
    MAC_STA_TX_ACTIVE               = 0xF2,
    MAC_STA_KEY_UNAVAILABLE         = 0xF3,
    MAC_STA_UNSUPPORTED_ATTRIBUTE   = 0xF4,
    MAC_STA_UNSUPPORTED_LEGACY      = 0xDE,
    MAC_STA_UNSUPPORTED_SECURITY    = 0xDF,
    MAC_STA_UNSUPPORTED             = 0x18,
    MAC_STA_BAD_STATE               = 0x19,
    MAC_STA_NO_RESOURCES            = 0x1A,

    MAC_STA_FRAME_PENDING			= 0x20,

    MAC_TX_ABORTED              = 0x1D,                /**< For internal use only */

} mac_sts_t;

/**
   Defines scan duration
 */
#define ZB_MAX_SCAN_DURATION_VALUE 200




/**
   Enum defines types of possible scan operations

   @param ED_SCAN
   @param ACTIVE_SCAN
   @param PASSIVE_SCAN
   @param ORPHAN_SCAN
*/
enum zb_mac_scan_type_e
{
  ED_SCAN      = 0,
  ACTIVE_SCAN  = 1,
  PASSIVE_SCAN = 2,
  ORPHAN_SCAN  = 3
};


enum{
	ZB_MAC_DISASSOCIATE_REASON_RESV = 0,
	ZB_MAC_DISASSOCIATE_REASON_COORD2DEV = 1,
	ZB_MAC_DISASSOCIATE_REASON_DEV = 2,
};

/** @addtogroup mac_primitive_id MAC primitive ID
 * Primitive ID of MAC request/response/indication/confirmation
 * @{
 */
#define MAC_MCPS_DATA_REQ                    0x40      //!< MCPS Data Request
#define MAC_MCPS_DATA_CNF                    0x41      //!< MCPS Data Confirm
#define MAC_MCPS_DATA_IND                    0x42      //!< MCPS Data Indication
#define MAC_MCPS_PURGE_REQ                   0x43      //!< MCPS Purge request
#define MAC_MCPS_PURGE_CNF                   0x44      //!< MCPS Purge Confirm
#define MAC_MLME_ASSOCIATE_REQ               0x45      //!< MLME Associate Request
#define MAC_MLME_ASSOCIATE_CNF               0x46      //!< MLME Associate Confirm
#define MAC_MLME_ASSOCIATE_IND               0x47      //!< MLME Associate Indication
#define MAC_MLME_ASSOCIATE_RES               0x48      //!< MLME Associate Response
#define MAC_MLME_DISASSOCIATE_REQ            0x49      //!< MLME Disassociate Request
#define MAC_MLME_DISASSOCIATE_CNF            0x4a      //!< MLME Disassociate Confirm
#define MAC_MLME_DISASSOCIATE_IND            0x4b      //!< MLME Disassociate Indication
#define MAC_MLME_BEACON_NOTIFY_IND           0x4c      //!< MLME Beacon Notify Indication
#define MAC_MLME_GET_REQ                     0x4d      //!< MLME Get Request
#define MAC_MLME_GET_CNF                     0x4e      //!< MLME Get Confirm
#define MAC_MLME_ORPHAN_IND                  0x4f      //!< MLME Orphan Indication
#define MAC_MLME_ORPHAN_RES                  0x50      //!< MLME Orphan Response
#define MAC_MLME_RESET_REQ                   0x51      //!< MLME Reset Request
#define MAC_MLME_RESET_CNF                   0x52      //!< MLME Reset Confirm
#define MAC_MLME_RX_ENABLE_REQ               0x53      //!< MLME RX Enable Request
#define MAC_MLME_RX_ENABLE_CNF               0x54      //!< MLME RX Enable Confirm
#define MAC_MLME_SCAN_REQ                    0x55      //!< MLME Scan Request
#define MAC_MLME_SCAN_CNF                    0x56      //!< MLME Scan Confirm
#define MAC_MLME_COMM_STATUS_IND             0x57      //!< MLME Comm Status Indication
#define MAC_MLME_SET_REQ                     0x58      //!< MLME Set Request
#define MAC_MLME_SET_CNF                     0x59      //!< MLME Set Confirm
#define MAC_MLME_START_REQ                   0x5a      //!< MLME Start Request
#define MAC_MLME_START_CNF                   0x5b      //!< MLME Start Confirm
#define MAC_MLME_SYNC_LOSS_IND               0x5c      //!< MLME Sync Loss Indication
#define MAC_MLME_POLL_REQ                    0x5d      //!< MLME Poll Request
#define MAC_MLME_POLL_CNF                    0x5e      //!< MLME Poll Confirm

#define MAC_MLME_SCAN_COMPLETE               0x5f      //!< internal use, MLME Scan Complete
#define MAC_MLME_BEACON_REQ_CMD              0x60      //!< internal use, Receive Beacon Request Command
#define MAC_MLME_DATA_REQ_CMD                0x61      //!< internal use, Receive Data Request Command
#define MAC_MLME_ASSO_RSP_CNF                0x62      //!< internal use, Receive Associate Response Confirm

#define MAC_MLME_POLL_IND					 0x63	   //!< MLME Poll Indication for zigBee 3.0



typedef struct{
	u8	altPanCoord: 	1;
	u8	devType: 		1;
	u8	powerSrc: 		1;
	u8	rcvOnWhenIdle: 	1;
	u8  reserved:		2;
	u8	secuCapability: 1;
	u8	allocAddr: 		1;
}capability_info_t;


/**
 *  @brief  Definition for Common security type
 */
typedef struct
{
	u8   keySource[MAC_KEY_SOURCE_MAX_LEN];  //!< Key source
	u8   securityLevel;                      //!< Security level
	u8   keyIdMode;                          //!< Key identifier mode
	u8   keyIndex;                           //!< Key index
} mac_sec_t;

/**
 *  @brief  Definition of PAN descriptor type
 */
typedef struct
{
	u32        timestamp;              //!< The time at which the beacon frame was received, in symbols

	u16        coordPanId;             //!< PAN identifier of the coordinator
	u16        superframeSpec;         //!< Super frame specification as specified in the received beacon frame

	addr_t     coordAddr;              //!< Coordinator address
	mac_sec_t  sec;                    //!< Security parameters
	u8         logicalChannel;         //!< The current logical channel occupied by the network

    u8         channelPage;            //!< The current channel page occupied by the network
    u8         gtsPermit;              //!< PAN coordinator is accepting GTS requests or not
    u8         linkQuality;            //!< The LQI at which the network beacon was received

    u8         securityFailure;        //!< Set to TRUE if there was an error in the security processing
    u8		   rsv[3];
}mac_panDesc_t;

/** @defgroup mac_mcsp_data_primitive MAC-MCSP:DATA primitive Types
 *  @{
 */
typedef struct{
	addr_t 	srcAddr;
	u16		dstPanId;
	addr_t 	dstAddr;
	u8 		msduLength;
	u8 		*msdu;
	u8 		msduHandle;
	u8 		txOptions;
	u8 		securityLevel;
	u8 		keyIdMode;
	u64 	keySource;
	u8 		keyIndex;
}zb_mscp_data_req_t;

/** @defgroup mac_mcsp_data_primitive MAC-MCSP:confirm primitive Types
 *  @{
 */
typedef struct{
	u32 	timestamp;
	u8      *msdu;
	u8 		msduHandle;
	u8 		status;
	u16		macDstAddr;
}zb_mscp_data_conf_t;

/** @defgroup mac_mcsp_data_primitive MAC-MCSP:indication primitive Types
 *  @{
 */
typedef struct{
	u32		timestamp;
	u8		*msdu;
	u64		KeySource;
	u16 	srcPanId;
	u16		dstPanId;
	addr_t	srcAddr;
	u8		msduLength;
	addr_t	dstAddr;
	u8		mpduLinkQuality;
	u8		dsn;
	u8		securityLevel;
	u8		keyIdMode;
	u8		keyIndex;
}zb_mscp_data_ind_t;

/** @} end of group mac_mcsp_data_primitive */


/** @defgroup mac_mcsp_purge_primitive MAC-MCSP:PURGE primitive Types
 *  @{
 */
typedef struct{
	u8 		msduHandle;		/*!< The handle of the MSDU requested to be purge from the transaction queue. */
}zb_mscp_purge_req_t;

typedef struct{
	u8 		msduHandle;		/*!< The handle of the MSDU requested to be purge from the transaction queue. */
	u8 		status;			/*!< SUCCESS or INVALID_HANDLE*/
}zb_mscp_purge_conf_t;
/** @} end of group mac_mcsp_purge_primitive */


/** @defgroup mac_mlme_associate_primitive MAC-MLME:ASSOCIATE primitive Types
 *  @{
 */
typedef struct{
	u8			logicalChannel;
	u8			channelPage;
	u16			coordPanId;

	addr_t      coordAddress;
	capability_info_t capbilityInfo;
	mac_sec_t sec;
}zb_mlme_associate_req_t;



typedef struct{
	addrExt_t				devAddress;
	capability_info_t 		capbilityInfo;
	u8						lqi;
	mac_sec_t sec;
}zb_mlme_associate_ind_t;


typedef struct{
	u16			shortAddress;
	addrExt_t	devAddress;
	u8 			status;
	mac_sec_t 	sec;
}zb_mlme_associate_resp_t;

typedef struct{
	addrExt_t		parentAddress;
	u16				shortAddress;
	u8 				status;
	mac_sec_t 		sec;
}zb_mlme_associate_conf_t;
/** @} end of group mac_mcsp_purge_primitive */


/** @defgroup mac_mlme_disassociate_primitive MAC-MLME:DISASSOCIATE primitive Types
 *  @{
 */


typedef struct{
	u16		devPanId;
	addr_t  devAddr;
	//		devAddrMode;
	//addrExt_t	devAddress;
	u8		disassociateReason;
	bool	txIndirect;
	mac_sec_t sec;
}zb_mlme_disassociate_req_t;

typedef struct{
	addrExt_t	devAddress;
	u8			disassociateReason;
	mac_sec_t sec;
}zb_mlme_disassociate_ind_t;

typedef struct{
	u16		devPanId;
	addr_t  devAddr;
	//u8		devAddrMode;
	//addrExt_t	devAddress;
	u8 		status;
}zb_mlme_disassociate_conf_t;
/** @} end of group mac_mcsp_purge_primitive */



/**
 *  @brief  Define MLME beacon notify indication type
 */
typedef struct{
	mac_panDesc_t   panDesc;             //!< The PAN Descriptor for the received beacon
	u8              *pAddrList;          //!< The list of device addresses for which the sender of the beacon has data
	u8              *psdu;             //!< The beacon payload
	u8              bsn;                 //!< Beacon Sequence Number
	u8              pendAddrSpec;        //!< The beacon pending address specification
	u8              psduLength;          //!< Number of octets contained in the beacon payload of the beacon frame
	u8				resved;				 //!< for align-4
}zb_mlme_beacon_notify_ind_t;


/**
   MLME-COMM-STATUS.indication data structure

   Not all fields included.
   Currently used after MLME-ASSOCIATE.response only.
   When will be used for MLME-ORPHAN.response, maybe, will add some fields.
 */
typedef struct{
	u16 panId;
	addr_t srcAddr;
	addr_t dstAddr;
	u8  status;
	mac_sec_t sec;
}zb_mlme_comm_status_ind_t;


/**
 *  @brief  Define MLME Request type
 */
typedef struct{
	bool            setAsDefault;               //!< set as default: 0 or 1
}mac_mlme_reset_req_t;

/**
 *  @brief  Define MLME Confirm type
 */
typedef struct{
	u8            status;               //!< The status of the reset @ref mac_status_id
}mac_mlme_reset_conf_t;


/**
 *  @brief  Define MLME Rx-On enable request type
 */
typedef struct
{
	u8              primitive;           //!< Primitive id of MLME Rx-On Enable Request @ref mac_primitive_id
	u32             rxOnTime;            //!< The Number of symbols measured before the receiver is to be enabled
	mac_sec_t       sec;                 //!< The security parameters for this message
}mac_mlme_rxOnEnableReq_t;

/**
 *  @brief  Define MLME Rx-On enable confirm type
 */
typedef struct
{
	u8              primitive;            //!< Primitive id of MLME Rx-on Enable Confirm @ref mac_primitive_id
	u8              status;               //!< The status of the data request @ref mac_status_id
}mac_mlme_rxOnEnableCnf_t;

/**
 *  @brief  Define MLME scan request type
 */
typedef struct
{
    u32       scanChannels;            //!< Indicate which channels are to be scanned
    u8        scanType;                //!< The type of scan to be performed @ref mac_scan_type
    u8        scanDuration;            //!< The time spent on scanning each channel
    u8        channelPage;             //!< Channel page on which to perform the scan
    mac_sec_t sec;                     //!< The security parameters for orphan scan
}zb_mac_mlme_scan_req_t;

/**
 *  @brief  Define MLME scan confirm type
 */
typedef struct
{
    u8        status;                   //!< Status of the scan request @ref mac_status_id
    u8        scanType;                 //!< The type of scan performed @ref mac_scan_type
    u8        channelPage;              //!< Channel page on which the scan was performed
    u8        resultListSize;           //!< Number of elements returned in the appropriate result lists


    u32       unscannedChannels;        //!< Channels given in the request which were not scanned

    union                               //!< Define a union of energy measurements list and pan descriptor list
    {
        u8 energyDetectList[MAX_ED_SCAN_RESULTS_SUPPORTED]; //!< List of energy measurements
        mac_panDesc_t panDescList[MAX_PAN_DESC_SUPPORTED];  //!< List of PAN descriptors
     } resultList;

}zb_mac_mlme_scan_conf_t;



/**
 *  @brief  Define MLME start request type
 */
typedef struct
{
	u32             startTime;           //!< Time at which to begin transmitting beacons

	u16             panId;  			 //!< PAN identifier to be used by the device
	u8              logicalChannel;      //!< Logical channel on which to begin
	u8              channelPage;         //!< Channel page on which to begin

	u8              beaconOrder;         //!< Indicates how often the beacon is to be transmitted
	u8              superframeOrder;     //!< Length of the active portion of the superframe
	u8              panCoordinator;      //!< Length of the active portion of the superframe
	u8              batteryLifeExt;      //!< Indicates if the receiver of the beaconing device is disabled or not

	mac_sec_t       coordRealignSec;     //!< Security parameters for the coordinator realignment frame
	u8              coordRealignment;    //!< Indicates if the coordinator realignment command is to be transmitted

	mac_sec_t       beaconSec;           //!< Security parameters for the beacon frame
}zb_mac_mlme_start_req_t;


/**
 *  @brief  Define MLME start confirm type
 */
typedef struct{
	u8              status;              //!< Result of the attempt to start using an updated superframe configuration @ref mac_status_id
} mac_mlme_startCnf_t;


/**
 *  @brief  Define MLME poll request type
 */
typedef struct{
	u16  coordPanId;
	tl_zb_addr_t coordAddr;
	u8   coordAddrMode;
} mac_mlme_poll_req_t;

/**
 *  @brief  Define MLME poll confirm type
 */
typedef struct{
	u8              status;              //!< Result of the attempt to start using an updated superframe configuration @ref mac_status_id
} mac_mlme_poll_conf_t;

/**
 *  @brief  Define MLME poll indication type
 */
typedef struct{
	u8				addrMode;	/*<! 2 - 16 bit short address. 3 - 64 bit extended address */
	tl_zb_addr_t	devAddr;	/*<! The address of the device requesting pending data */
} mac_mlme_poll_ind_t;

/**
   Parameters for orphan indication
*/
typedef struct{
  addrExt_t orphanAddr; /*<! The 64 bits address of the orphaned device */
}mac_mlme_orphan_ind_t;



/**
   Parameters for orphan response
*/
typedef struct{
  addrExt_t orphanAddr; /*<! The 64 bits address of the orphaned device */
  u16 shortAddr; /*<! The 16-bit short address allocated to the
                           * orphaned device if it is associated with this coordinator */
  bool associated; /*<! TRUE if the orphaned device is associated with this
                          coordinator or FALSE otherwise */
}mac_mlme_orphan_resp_t;



/**
   Sync loss reasons

   The reason that synchronization was lost.
 */
typedef enum
{
  ZB_SYNC_LOSS_REASON_PAN_ID_CONFLICT,
  ZB_SYNC_LOSS_REASON_REALIGNMENT,
  ZB_SYNC_LOSS_REASON_BEACON_LOST
}zb_sync_loss_reason_t;

/**
   Parameters for sync loss indication
*/
typedef struct{
	u16 panId; 						/* The PAN identifier with which the device lost
					   	   	   	   	   * synchronization or to which it was realigned */
	zb_sync_loss_reason_t reason; 	/* Lost syncronisation reason */
	u8 logicalChannel; 				/* Logical channel */
	u8 channelPage;					/* Channel page */
}zb_mlme_sync_loss_ind_t;


typedef struct{
	u8 srcAddrMode;
	u8 dstAddrMode;
	tl_zb_addr_t srcAddr;
	tl_zb_addr_t dstAddr;
	u8 cbType;
}zb_mlme_data_req_cmd_t;


typedef struct{
	u32 timestamp;
	u8 *psdu;
	u16 srcPanId;
	addr_t srcAddr;
	u8 len;
	u8 lqi;
	u8 curChannel;
}zb_mac_raw_frame_t;


#define ZB_MAC_GTS_DESCROPTION_NUM_MAX			7

typedef struct{
	 u8 beaconOrder:4;
	 u8 superframeOrder:4;
	 u8 finalCapSlot:4;
	 u8 batteryLifeExtension:1;
	 u8 resv:1;
	 u8 panCoordinator:1;
	 u8 associatePermit:1;
}zb_superframe_spec_t;

typedef struct{
	u8 descCount:3;
	u8 resv:4;
	u8 permit:1;
}zb_gts_spec_t;

typedef struct{
	u8 shortAddr[2];
	u8 startSlot:4;
	u8 len:4;
}zb_gts_desc_t;

typedef struct{
	zb_gts_spec_t spec;
	u8 direction;
	zb_gts_desc_t desc[ZB_MAC_GTS_DESCROPTION_NUM_MAX];
}zb_mac_gts_t;

typedef struct{
	u8 shortPendingAddrNum:3;
	u8 resv0:1;
	u8 extPendingAddrNum:3;
	u8 resv1:1;
}zb_pending_addr_spec_t;

typedef struct{
	zb_pending_addr_spec_t spec;
	u8 direction;
	zb_gts_desc_t desc[ZB_MAC_GTS_DESCROPTION_NUM_MAX];
}zb_mac_pending_addr_t;


/*
 * beacon payload
 * */
typedef struct  zb_mac_beacon_payload_s{
	u8 protocol_id;
	u8 stack_profile:4;
	u8 protocol_version:4;

	u8 reserved:2;
	u8 router_capacity:1;
	u8 device_depth:4;
	u8 end_device_capacity:1;

	extPANId_t extended_panid;
	u8 txoffset[3];
	u8 nwk_update_id;
} zb_mac_beacon_payload_t;

typedef struct{
	u8 cmdId;
}zb_rfpkt_beacon_req_t;

typedef struct{
	zb_superframe_spec_t superFrameSpec;
	zb_mac_gts_t	gts;
	zb_mac_pending_addr_t	pendingAddr;
	zb_mac_beacon_payload_t beaconPayload;
}zb_rfpkt_beacon_t;


/*
 * status for processing the indirect data
 */
enum{
	ZB_INDRECT_IDLE,
	ZB_INDRECT_ASSOCIATE = 1,
	ZB_INDRECT_POLL,

	ZB_INDRECT_ASSOCIATE_RESPONSE,
	ZB_INDRECT_MCPS_DATA,
	ZB_INDRECT_DISASSOCIATE_NOTIFY
};

#define ZB_MAC_PENDING_TRANS_QUEUE_NUM	6

/* According to 802.15.4, should be 0. */
#define ZB_MAC_INTERNAL_EXPIRY_CNT		0

/**
   define RF transmit callback function
*/
typedef void(* tl_zb_max_tx_conf_cb)(void *arg, u8 status);
typedef struct{
	void *arg;
	tl_zb_max_tx_conf_cb cb;
}tl_zb_max_tx_conf_t;

/**
   Parameters for storing data in a pending transaction
*/
typedef struct
{
	void *data;
	ev_time_event_t pendingTranstimer;
	addr_t dstAddr;
	u8 type:7;
 	u8 send:1;
}zb_mac_pending_trans_t;

typedef struct mac_indirPendingList_t{
	struct mac_indirPendingList_t *next;
	void 	*data;
	addr_t	dstAddr;
	u8		expiry;
	u8		extExpiryCnt;
	u8		send;
}mac_indirPendingList_t;

extern u8 ZB_MAC_PENDING_TRANS_QUEUE_SIZE;

extern u8 ZB_MAC_EXT_EXPEIRY_CNT;


/**
   Parameters for storing data in a pending queue
*/
typedef struct zb_mac_pending_data_s
{
  zb_buf_t *pending_data;
  zb_addr_mode_t dst_addr_mode;
  tl_zb_addr_t dst_addr;
}zb_mac_pending_data_t;

/**
   Parameters for MAC layer
*/
typedef struct{
	u8 warmStart;							/*!< The restart mode of the MAC layer. */
	u8 status;								/*!< The status of the MAC layer. */
	u8 curChannel;							/*!< The selected channel of the MAC layer. */
	u8 beaconTriesNum;
	struct{
		ev_time_event_t *timer; 	/* for original */;
	}indirectData;

	u8 *txRawDataBuf;
}tl_zb_mac_ctx_t;


/**
   the struct for parsing MAC header
*/
typedef struct
{
  u16 		frameCtrl;
  u8 		dstAddrMode;
  u8 		srcAddrMode;
  u8		panIdMode;
  u8 		seqNum;
  u16 		dstPanId;
  tl_zb_addr_t dstAddr;
  u16 		srcPanId;
  tl_zb_addr_t srcAddr;

#ifdef ZB_MAC_SECURITY
  u8      secLevel;
  u8      keyIdMode;
  u8      keySrc[8];
  u8      keyIdx;
  u32     framCounter;
#endif

}tl_zb_mac_mhr_t;

/**
   the format of the beacon packet
*/
typedef struct{
	u8  srcAddrMode;
	u8  secEnable;
  	u8  framePending;
  	u8  ble;
  	u8  beaconOrder;
    u16 superframeOrder;
}tl_zbBeaconFrame_t;



/**
   Parameters for sending data request
*/
typedef enum
{
  MAC_ASS_CONFIRM_CALLBACK = 0,
  MAC_POLL_REQUEST_CALLBACK = 1
}zb_callback_type_t;


typedef struct{
	u16 shortAddr;
	addrExt_t ieeeAddr;
}access_addr_t;


#define MAGIC_ROUND_TO_4(a) (((a) + ((4)-1)) & -(4))


extern tl_zb_mac_ctx_t g_zbMacCtx;

#define TL_ZB_MAC_STATUS_SET(sta) 			g_zbMacCtx.status = sta
#define TL_ZB_MAC_STATUS_GET() 				g_zbMacCtx.status


u8 tl_zbMacHdrParse(tl_zb_mac_mhr_t *mHhr, u8 *ptr);

u8 tl_zbMacHdrSize(u16 frameCtrl);

u8 *tl_zbMacHdrBuilder(u8 *pBuf, tl_zb_mac_mhr_t *pMacHdr);

void tl_zbMacMcpsDataRequestProc(void *arg);

/* PIB access macros */

/**
   Get mac short address
 */
#define ZB_PIB_SHORT_ADDRESS()  g_zbMacPib.shortAddress
/**
   Get mac extended address
 */
#define ZB_PIB_EXTENDED_ADDRESS() g_zbMacPib.extAddress

/**
   Get mac rx on when idle
 */
#define ZB_PIB_RX_ON_WHEN_IDLE()  g_zbMacPib.rxOnWhenIdle

/**
   Get mac ack wait duration
 */
#define ZB_PIB_ACK_WAIT_DURATION()  g_zbMacPib.ackWaitDuration

/**
   Get mac DSN
 */
#define ZB_MAC_DSN() g_zbMacPib.seqNum
/**
   Get mac pan BSN
 */
#define ZB_MAC_BSN() g_zbMacPib.beaconSeqNum
/**
   Increment mac pan DSN
 */
#define ZB_INC_MAC_DSN() (g_zbMacPib.seqNum++)
/**
   Increment mac pan BSN
 */
#define ZB_INC_MAC_BSN() (g_zbMacPib.beaconSeqNum++)
/**
   Get mac beacon payload
 */
#define ZB_PIB_BEACON_PAYLOAD() g_zbMacPib.beaconPayload

#define ZB_MAC_DEVICE_TYPE_SET(type) (g_zbMacCtx.devType = (type))

/*
 * MAC layer initialization
 *
 * @param None
 *
 * @return None
 *
 * */
void tl_zbMacInit(u8 coldReset);

void tl_macPibSave2Flash();
/*
 * MAC layer main task procedure
 *
 * @param None
 *
 * @return None
 *
 * */
void  tl_zbMacTaskProc(void);

/*
 * NWK layer to MAC layer primitive
 * */

/*
 * MLME-MCPS-DATA.request primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-MCPS-DATA.request primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacMcpsDataRequst(p)		tl_zbMacMcpsDataRequestProc(p)


/*
 * MLME-RESET.request primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-RESET.request primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacResetRequst(p)			tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_RESET_REQ, p)


/*
 * MLME-POLL.request primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-POLL.request primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacPollRequst(p)			tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_POLL_REQ, p)


/*
 * MLME-ASSOCIATE.request primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-ASSOCIATE.request primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacAssociateRequest(p)		tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_ASSOCIATE_REQ, p)

/*
 * MLME-ASSOCIATE.response primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-ASSOCIATE.response primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define	tl_zbMacAssociateResponse(p)	tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_ASSOCIATE_RES, p)

/*
 * MLME-ORPHAN.response primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-ORPHAN.response primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacOrphanResponse(p)		tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_ORPHAN_RES, p)

/*
 * MLME-SCAN.request primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-SCAN.request primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacScanRequest(p)			tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_SCAN_REQ, p)


/*
 * MLME-START.request primitive from upper layer to MAC layer
 *
 * @param p the pointer to the MLME-START.request primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacStartRequest(p)			tl_zbPrimitivePost(TL_Q_NWK2MAC, MAC_MLME_START_REQ, p)


/*
 * MLME-SET.request to set MAC pib attribute from upper layer to MAC layer
 *
 * @param attribute MAC PIB attribut MAC PIB attributee id(Table 86 ?a MAC PIB attributes)
 *
 * @param value the pointer value of the attribute
 *
 * @param len the length of the attribute vlaue
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
u8 tl_zbMacAttrSet(u8 attribute, u8 *value, u8 len);

/*
* MLME-GET.request to get MAC pib attribute from upper layer to MAC layer
*
* @param attribute MAC PIB attribut MAC PIB attributee id(Table 86 ?a MAC PIB attributes)
*
* @param value the pointer to the value of the attribute
*
* @param len the length of the attribute vlaue
*
* @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
*
* */
u8 tl_zbMacAttrGet(u8 attribute, u8* value, u8* len);

void generateIEEEAddr(void);

/*
 * MAC layer to NWK layer primitive
 * */

 /*
  * MLME-RESET.confirm primitive from MAC layer to upper layer
  *
  * @param p the pointer to the MLME-RESET.confirm primitive and msdu
  *
  * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
  *
  * */
#define tl_zbMacResetConfirm(p)			tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_RESET_CNF, p)

/*
* MLME-ASSOCIATE.confirm primitive from MAC layer to upper layer
*
* @param p the pointer to the MLME-ASSOCIATE.confirm primitive and msdu
*
* @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
*
* */
#define tl_zbMacAssociateConfirm(p)		tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_ASSOCIATE_CNF, p)


 /*
 * MLME-DISASSOCIATE.confirm primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-DISASSOCIATE.confirm primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
 #define tl_zbMacDisassociateConfirm(p)		tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_DISASSOCIATE_CNF, p)

/*
* MLME-ASSOCIATE.indiccation primitive from MAC layer to upper layer
*
* @param p the pointer to the MLME-ASSOCIATE.indiccation primitive and msdu
*
* @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
*
* */
#define tl_zbMacAssociateIndicate(p)	tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_ASSOCIATE_IND, p)

 /*
 * MLME-DISASSOCIATE.indiccation primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-DISASSOCIATE.indiccation primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
 #define tl_zbMacDisassociateIndicate(p)	tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_DISASSOCIATE_IND, p)

 /*
 * MLDE-MCPS-DATA.indiccation primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLDE-MCPS-DATA.indiccation primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacDataIndicate(p)			tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MCPS_DATA_IND, p)

 /*
 * MLDE-MCPS-DATA.confirm primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLDE-MCPS-DATA.confirm primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacDataConfirm(p)			tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MCPS_DATA_CNF, p)

 /*
 * MLDE-MCPS-DATA.confirm primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLDE-MCPS-PURGE.confirm primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacPurgeConfirm(p)			tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MCPS_PURGE_CNF, p)


 /*
 * MLME-POLL.confirm primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-POLL.confirm primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacPollConfirm(p)			tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_POLL_CNF, p)


 /*
 * MLME-POLL.indication primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-POLL.indication primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacPollIndication(p)		tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_POLL_IND, p)

 /*
 * MLME-BEACON-NOTIFY.indication primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-BEACON-NOTIFY.indication primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacBeaconNotifyIndicate(p)	tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_BEACON_NOTIFY_IND, p)


 /*
 * MLME-ORPHAN.indication primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-ORPHAN.indication primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacOrphanIndicate(p)		tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_ORPHAN_IND, p)


 /*
 * MLME-SCAN.confirm primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-SCAN.confirm primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacScanConfirm(p)			tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_SCAN_CNF, p)

 /*
 * MLME-COMM-STATUS.indication primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-COMM-STATUS.indication primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacCommStatusIndication(p)	tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_COMM_STATUS_IND, p)

 /*
 * MLME-START.confirm primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-START.confirm primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacStartConfirm(p)			tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_START_CNF, p)

 /*
 * MLME-SYNC-LOSS.indicaion primitive from MAC layer to upper layer
 *
 * @param p the pointer to the MLME-START.confirm primitive and msdu
 *
 * @return ZB_RET_OVERFLOW if failure, ZB_RET_OK else
 *
 * */
#define tl_zbMacSyncLossIndication(p)	tl_zbPrimitivePost(TL_Q_MAC2NWK, MAC_MLME_SYNC_LOSS_IND, p)

 void tl_zbMacMcpsDataRequestSendConfirm(void *arg, u8 status);

 /*
 * @brief initialize the rf transceiver and state
 *
 * @param
 *
 * @return
 *
 * */
 void mac_trxInit(void);

 /*
 * @brief reset the rf transceiver and state
 *
 * @param
 *
 * @return
 *
 * */
 void mac_trxReset(void);

#define ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME (1 /*LQI */)

typedef u8 zb_mac_capability_info_t;

typedef 	mac_sts_t zb_mac_status_t;
#define		LOGICCHANNEL_TO_PHYSICAL(p)					(((p)-10)*5)

void tl_zbMacChannelSet(u8 chan);
#define ZB_TRANSCEIVER_SET_CHANNEL(chan) 		tl_zbMacChannelSet(chan);
#define	TL_COMMISSIONSET_CHANNEL(chan)			RF_TrxStateSet(RF_TrxStateGet(),LOGICCHANNEL_TO_PHYSICAL(chan));

/** @} end of group TELINK_ZIGBEE_MAC */

typedef bool (*mac_callback)(void *p);

typedef struct{
	mac_callback	macBeaconReqRcvCb;
	mac_callback	macBeaconRcvCb;
	mac_callback	macAssociationReqRcvCb;
}mac_appIndCb_t;

extern mac_appIndCb_t *macAppIndCb;

void mac_appIndCbRegister(mac_appIndCb_t *cb);

/** @} end of group TELINK_ZIGBEE_STACK */

#endif
