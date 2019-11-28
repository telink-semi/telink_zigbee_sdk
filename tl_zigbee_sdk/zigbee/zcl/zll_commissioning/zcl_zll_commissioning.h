/********************************************************************************************************
 * @file     zcl_zll_commissioning.h
 *
 * @brief	 APIs for ZLL zigbee light link commissioning
 *
 * @author
 * @date     June. 10, 2017
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
#pragma once

#include "tl_common.h"
#include "../../zdo/zb_af.h"

//RF power configure for touch link
#define RF_POWER_FOR_COMMISSION				RF_POWER_m0P6dBm//RF_POWER_7dBm//RF_POWER_m9P5dBm//


/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */

/** @addtogroup  ZCL_CLUSTER_ZLL_COMMISSIONING ZLL_COMMISSIONING Cluster
 *  @{
 */


/** @addtogroup zcl_zll_commissioning_constant Zll Commissioning Cluster Constants
 *  Definition for zll commissioning constants
 *  @{
 */
#define	ZB_ADDR_RANGE_ASSIGNED_FOR_TARGET			5
#define ZB_ZLL_PRIMARY_CHANNELS_MASK   				0x02108800
#define ZB_ZLL_SECONDARY_CHANNELS_MASK 				0x05EF7000
#define	ZB_ZLL_COMMISSION_UNSCANEDCHANNEL_MASK		0x07fff800


/** @} end of group zcl_zll_commissioning_constant */



/** @addtogroup zcl_zll_commissioning_cmdId Zll Commissioning Command Ids
 *  Definition for zll commissioning command ids
 * @{
 */
#define ZCL_CMD_ZLL_COMMISSIONING_SCAN                    0x00	//!< touch link command id: scan request
#define ZCL_CMD_ZLL_COMMISSIONING_DEVICE_INFORMATION      0x02	//!< touch link command id: device information request
#define ZCL_CMD_ZLL_COMMISSIONING_IDENTIFY                0x06	//!< touch link command id: identify request
#define ZCL_CMD_ZLL_COMMISSIONING_RESET_TO_FACTORY_NEW    0x07	//!< touch link command id: reset factory new
#define ZCL_CMD_ZLL_COMMISSIONING_NETWORK_START			  0x10	//!< touch link command id: network start request
#define ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER	  0x12	//!< touch link command id: network join route request
#define ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE 0x14	//!< touch link command id: network join end device request
#define ZCL_CMD_ZLL_COMMISSIONING_NETWORK_UPDATE		  0x16	//!< touch link command id: network update request

/* Utilities */
#define ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS	  0x41	//!< commissioning utility command id: get group identifies request
#define ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST		  0x42	//!< utility command id: get endpoint list request


// Server generated command
/* Touchlink */
#define ZCL_CMD_ZLL_COMMISSIONING_SCAN_RSP                	 	0x01	//!< touch link command id: scan response
#define ZCL_CMD_ZLL_COMMISSIONING_DEVICE_INFORMATION_RSP  	 	0x03	//!< touch link command id: device information response
#define ZCL_CMD_ZLL_COMMISSIONING_NETWORK_START_RSP		  	 	0x11	//!< touch link command id: network start response
#define ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER_RSP 	 	0x13	//!< touch link command id: network join route response
#define ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE_RSP 	0x15	//!< touch link command id: network join end device response

/* Utilities */
#define ZCL_CMD_ZLL_COMMISSIONING_ENDPOINT_INFORMATION		 	0x40	//!< utility command id: endpoint information
#define ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS_RSP  	0x41	//!< utility command id: get group identifies response
#define ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST_RSP      	0x42	//!< utility command id: get endpoint list response

/** @} end of group zcl_zll_commissioning_cmdId */


/** @addtogroup zcl_zll_commissioning_state Zll Commissioning State
 *  Definition for zll commissioning States
 * @{
 */
enum{
	ZCL_ZLL_COMMISSION_STATE_IDLE,							//!< touch link status: idle
	ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DISCOVERY,			//!< touch link status: discovery
	ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DEVICE_INFO_EXCHANGE,//!< touch link status: device information exchange
	ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_IDENTIFY,			//!< touch link status: identify
	ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_STARTORJOIN = 0x80,			//!< touch link status: start/join
	ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_START = 0x81,			//!< touch link status: start/join
	ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_JOIN_ED = 0x82,			//!< touch link status: start/join
	ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_JOIN_ROUTE = 0x83,			//!< touch link status: start/join
};
/** @} end of group zcl_zll_commissioning_state */



/** @addtogroup zcl_zll_touch_link_status Zll Touch Link Status
 *  Definition for Zll Touch Link Status
 * @{
 */
enum{
	ZCL_ZLL_TOUCH_LINK_STA_SUCC,					//!< touch link success
	ZCL_ZLL_TOUCH_LINK_FAIL,						//!< touch link fail
	ZCL_ZLL_TOUCH_LINK_STA_NO_SERVER,				//!< no valid target
	ZCL_ZLL_TOUCH_LINK_STA_EXIST,					//!< the target already in the network
	ZCL_ZLL_TOUCH_LINK_STA_NOT_PERMITTED,			//!< the target is not permitted
	ZCL_ZLL_TOUCH_LINK_STA_TIMEOUT,					//!< touch link timeout
	ZCL_ZLL_TOUCH_LINK_STA_STARTORJOIN_TIMEOUT,		//!< touch link start/join timeout
	ZCL_ZLL_TOUCH_LINK_STA_TARGET_START,			//!< target start
	ZCL_ZLL_TOUCH_LINK_STA_NO_CAPACITY,				//!< target start
	ZCL_ZLL_TOUCH_LINK_BUSY,						//!< during touch link
};
/** @} end of group zcl_zll_touch_link_status */


/** @addtogroup zcl_zll_touch_link_mode Zll Touch Link Mode
 *  Definition for Zll Touch Link Mode
 * @{
 */
enum{
	ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_START,	//!< network join route/ED
	ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_JOIN		//!< network start
};
/** @} end of group zcl_zll_touch_link_mode */


/** @addtogroup zll_commissing_type ZLL Commissioning Types
 *  @{
 */

/** @addtogroup zll_commissioning_touch_link_type ZLL Commissioning-TouchLink Types
 *  @{
 */
/**
 *  @brief  Definition for zigbee infomation for zll commissiong
 */
typedef struct{
	union{
		struct{
			u8 logicDevType:2;		/*!< logical device type */
			u8 rxOnWihleIdle: 1;	/*!< logical device type */
			u8 resv:5;				/*!< reserved */
		}bf;
		u8 byte;
	};
}zll_tlZbInfo_t;


/**
 *  @brief  Definition for zll infomation for zll commissiong
 */
typedef struct{
	union{
		struct{
			u8 factoryNew:1;		/*!< 1: factory new device, 0: non-factory new device*/
			u8 addrAssign:1;		/*!< Address assignment */
			u8 resv0:2;				/*!< reserved */
			u8 linkInitiator:1;		/*!< touch link initiator */
			u8 priorityReq:1;		/*!< priority request */
			u8 resv1:2;				/*!< reserved */
		}bf;
		u8 byte;
	};
}zll_tlZllInfo_t;


/**
 *  @brief  Definition for sub-device information
 */
typedef struct {
	u8 		epId;				/*!< end-point identifier */
	u16 	profileId;			/*!< profile identifier */
	u16 	deviceId;			/*!< device identifier */
	u8 		version;			/*!< version */
	u8 		groupIdCnt;			/*!< group identifies count */
}zcl_zllSubdeviceInfo_t;


/**
 *  @brief  Definition for device information
 */
typedef struct {
	addrExt_t 	deviceAddr;		/*!< device address */
	u8 			epId;			/*!< end-point identifier */
	u16 		profileId;		/*!< profile identifier */
	u16 		deviceId;		/*!< device identifier */
	u8 			version;		/*!< device identifier */
	u8 			groupIdCnt;		/*!< group identifies count */
	u8 			sort;			/*!< sort */
} zcl_DeviceInfo;


/**
 *  @brief  Definition for target information by touch link discovery
 */
typedef struct{
	u32 				respId;				/*!< response identify */
	addrExt_t 			epanId;				/*!< Extended PAN identifier */
	u16 				panId;				/*!< PAN identifier */
	u16 				nwkAddr;			/*!< network address */
	zll_tlZbInfo_t  	zbInfo;				/*!< zigbee information */
	zll_tlZllInfo_t  	zllInfo;			/*!< zll information */
	u8 					logicalChannel;		/*!< logic channel */
	u8					numOfSubdevices;	/*!< sub-devices numbers */
	epInfo_t			dstEp;				/*!< destine end point */
	u8 					totalGroupIds;		/*!< total group identifies */
	u8 					keyIdx;				/*!< key index */
	u8					nwkUpdateId;		/*!< nwkUpdateId */
	zcl_zllSubdeviceInfo_t devInfo;			/*!< sub-device information */
}zll_touchLinkScanInfo;

/**
 *  @brief  Definition for touch link discovery
 */
typedef struct{
	u32						unscannedChannelMask;		/*!< un-scanned channel bit mask */
	u8						targetNum;					/*!< target number by touch link discovery */
	bool					primaryChannelScanComplete; /*!< primary channel scan complete */
	u8						currentScannChannel;		/*!< current scan channel */
	u8						scanCnt;					/*!< scan count */
	zll_touchLinkScanInfo	scanList[1];				/*!< pointer to the target information by touch link discovery */
} zcl_zllTouckLinkDisc_t;


/**
 *  @brief  Definition for touch link scan request command format
 */
typedef struct {
	u32  transId;				/*!< Inter-PAN transaction identifier */
	zll_tlZbInfo_t  zbInfo;		/*!< zigbee information */
	zll_tlZllInfo_t zllInfo;	/*!< zll informationn */
} zcl_zllTouchLinkScanReq_t;


/**
 *  @brief  Definition for touch link scan response command format
 */
typedef struct {
	u32  			transId;			/*!< Inter-PAN transaction identifier */
	u8  			rssiCorrection;		/*!< RSSI correction */
	zll_tlZbInfo_t  zbInfo;				/*!< ZigBee information */
	zll_tlZllInfo_t zllInfo;			/*!< Zll information */
	u16 			keyBitmask;			/*!< Key bit mask */
	u32 			respId;				/*!< Response identifier */
	addrExt_t 		epanId;				/*!< Extended PAN identifier */
	u8 				nwkUpdateId;		/*!< Network update identifier */
	u8 				logicalChannel;		/*!< Logical channel */
	u16 			panId;				/*!< PAN identifier */
	u16 			nwkAddr;			/*!< Network address */
	u8 				numOfSubdevices;	/*!< Number of sub-devices */
	u8 				totalGroupIds;		/*!< Total group identifiers */
	zcl_zllSubdeviceInfo_t		subDevInfo; /*!< sub-device information, only used when numOfSubdevices is 1 */
} zcl_zllTouchLinkScanResp_t;


/**
 *  @brief  Definition for touch link network start request command format
 */
typedef struct {
	u32 		transId;				/*!< Inter-PAN transaction identifier */
	addrExt_t 	epanId;					/*!< Extended PAN identifier */
	u8 			keyIdx;					/*!< Key index */
	u8 			encryptedNwkKey[16];	/*!< Encrypted network key */
	u8 			logicalChannel;			/*!< Logical channel */
	u16 		panId;					/*!< PAN identifier */
	u16 		nwkAddr;				/*!< Network address */
	u16 		groupIdbegin;			/*!< Group identifiers begin */
	u16 		groupIdEnd;				/*!< Group identifiers end */
	u16 		freeNwkAddrRangeBegin;	/*!< Free network address range begin */
	u16 		freeNwkAddrRangeEnd;	/*!< Free network address range end */
	u16 		freeGroupIdRangeBegin;	/*!< Free group identifier range begin */
	u16 		freeGroupIdRangeEnd;	/*!< Free group identifier range end */
	addrExt_t 	initiatorIeeeAddr;		/*!< Initiator IEEE address */
	u16 		initiatorNwkAddr;		/*!< Initiator network address */
} zcl_zllTouchLinkNetworkStartReq_t;


/**
 *  @brief  Definition for touch link network start response command format
 */
typedef struct {
	u32 		transId;				/*!< Inter-PAN transaction identifier */
	u8 			status;					/*!< status */
	addrExt_t 	epanId;					/*!< Extended PAN identifier */
	u8 			nwkUpdateId;			/*!< Network update identifier */
	u8 			logicalChannel;			/*!< Logical channel */
	u16 		panId;					/*!< PAN identifier */
} zcl_zllTouchLinkNetworkStartResp_t;


/**
 *  @brief  Definition for touch link network join ED/route request command format
 */
typedef struct {
	u32 		transId;				/*!< Inter-PAN transaction identifier */
	addrExt_t 	epanId;					/*!< Extended PAN identifier */
	u8 			keyIdx;					/*!< Key index */
	u8 			encryptedNwkKey[16];	/*!< Encrypted network key */
	u8 			nwkUpdateId;			/*!< Network update identifier */
	u8 			logicalChannel;			/*!< Logical channel */
	u16 		panId;					/*!< PAN identifier */
	u16 		nwkAddr;				/*!< Network address */
	u16 		groupIdBegin;			/*!< Group identifiers begin */
	u16 		groupIdEnd;				/*!< Group identifiers end */
	u16 		freeNwkAddrRangeBegin;	/*!< Free network address range begin */
	u16 		freeNwkAddrRangeEnd;	/*!< Free network address range end */
	u16 		freeGroupIdRangeBegin;	/*!< Free group identifier range begin */
	u16 		freeGroupIdRangeEnd;	/*!< Free group identifier range end */
} zcl_zllTouchLinkNetworkJoinReq_t;


/**
 *  @brief  Definition for touch link network join ED/route response command format
 */
typedef struct {
	u32 		transId;			/*!< Inter-PAN transaction identifier */
	u8 			status;				/*!< status */
} zcl_zllTouchLinkNetworkJoinResp_t;


/**
 *  @brief  Definition for touch link network start or join parameters
 */
typedef struct{
	union{
		zcl_zllTouchLinkNetworkStartReq_t networkStartCmd;		/*!< touch link network start request */
		zcl_zllTouchLinkNetworkJoinReq_t  networkJoinCmd;		/*!< touch link network join request */
	}params;
	u8 seqNo;					/*!< network start/join  request sequence number */
	u8 joinNetworkMode;			/*!< network join mode:  start/join route/join ED */
	epInfo_t initiatorEpInfo;	/*!< touch link initiator End point information */
} zcl_zllTouckLinkNetworkStartParams;


/*
 * @brief  Definition for the format of the device information record field
 *
 * */
typedef struct {
	addrExt_t 	ieeeAddr;				/*!< IEEE address */
	u8 			epId;					/*!< Endpoint identifier */
	u16 		profileId;				/*!< Profile identifier */
	u16 		deviceId;				/*!< Device identifier */
	u8 			version;				/*!< Version */
	u8 			groupIdCnt;				/*!< Group identifier count */
	u8 			sort;					/*!< sort */
} zcl_zllDeviceInfoRec_t;

/*
 * @brief  Definition for touch link device information request command format
 *
 * */
typedef struct {
	u32 				transId;		/*!< Inter-PAN transaction identifier */
	u8 					startIdx;			/*!< start index */
} zcl_zllTouchLinkDeviceInfoReq_t;


/*
 * @brief  Definition for touch link device information response command format
 *
 * */
typedef struct {
	u32 				transId;			/*!< Inter-PAN transaction identifier */
	u8 					numOfSubdevices;	/*!< Number of sub devices */
	u8 				startIdx;			/*!< Start index */
	u8 				deviceInfoRecordCnt;/*!< Device information record count */
	zcl_zllDeviceInfoRec_t 	rec[];			/*!< Device information record */
} zcl_zllTouchLinkDeviceInfoResp_t;



/*
 * @brief  Definition for zll Touch Link identify request command format
 *
 * */
typedef struct {
	u32  transId;					/*!< Inter-PAN transaction identifier */
	u16  identifyDuration;			/*!< identify duration */
} zcl_zllTouchLinkIdentifyReq_t;


/*
 * @brief  Definition for zll Touch Link network update Request command format
 *
 * */
typedef struct {
	u32 		transId;			/*!< Inter-PAN transaction identifier */
	addrExt_t 	epanId;				/*!< Extended PAN identifier */
	u8 			nwkUpdateId;		/*!< Network update identifier */
	u8 			logicalChannel;		/*!< Logical channel */
	u16 		panId;				/*!< PAN identifier */
	u16 		nwkAddr;			/*!< Network address */
} zcl_zllTouchLinkNetworkUpdateReq_t;



typedef void (*zcl_zllTouchLinkAppCallbacks_t)(u8 status, void *arg);

/** @} end of group zll_commissioning_touch_link_type */


/** @addtogroup zll_commissioning_utility_type ZLL Commissioning-Utility Types
 *  @{
 */

/*
 * @brief  endpiont information
 *
 * */
typedef struct {
	u16 		nwkAddr;		/*!< Network address */
	u8 			epId;			/*!< end point identifier */
	u16 		profileId;		/*!< profile identifier */
	u16 		deviceId;		/*!< device identifier */
	u8 			version;		/*!< version */
} zcl_zllEndpointInfo_t;

/*
 * @brief  endpiont information (for zll commission utility) command format
 *
 * */
typedef struct {
	addrExt_t 	ieeeAddr;		/*!< IEEE address */
	zcl_zllEndpointInfo_t info;	/*!< endpiont information */
} zcl_zllUtilityEndpointInfo_t;



typedef struct {
	u16 	groupId;
	u8 		groupType;
} zcl_groupInfo;

/*
 * @brief  get group identifiers request (for zll commission utility) command format
 *
 * */
typedef struct {
	u8 	startIdx;		/*!< start index */
} zcl_zllUtilityGetGroupIdsReq_t;


/*
 * @brief  get group identifiers response (for zll commission utility) command format
 *
 * */
typedef struct {
	u8 	total;			/*!< otal number of group identifiers supported by the 3 device */
	u8 	startIdx;		/*!< start index */
	u8 	count;			/*!< the number of entries in the group information record 10 list field */
	zcl_groupInfo *groupInfoRecord;	/*!< Group information record list */
} zcl_zllUtilityGetGroupIdsResp_t;


/*
 * @brief  get endpiont list (for zll commission utility ) request command format
 *
 * */
typedef struct {
	u8 startIdx;		/*!< start index */
} zcl_zllUtilityGetEpListReq_t;


/*
 * @brief  get endpiont list (for zll commission utility ) response command format
 *
 * */
typedef struct {
	u8 total;			/*!< the total number of endpoints supported by the device */
	u8 startIdx;		/*!< start index */
	u8 count;			/*!< the number of entries in the endpoint information 11 record list field */
	zcl_zllEndpointInfo_t *epInfoRecord;		/*!< Endpoint information record list */
} zcl_zllUtilityGetEpListResp_t;

typedef void (*zcl_zllCommissionUtilityAppCallbacks_t)(u8 cmd, void *arg);

/** @} end of group zll_commissioning_utility_type */


/*
 * @brief  Definition for zll commissioning callback type for application
 *
 * */
typedef struct{
	zcl_zllTouchLinkAppCallbacks_t 			touchLinkCallback;		/*!< zll touch-link callback for application */
	zcl_zllCommissionUtilityAppCallbacks_t  utilityAppCallback;		/*!< zll utility callback for application */
}zcl_touchlinkAppCallbacks_t;


/*
 * @brief  Definition for zll commissioning type
 *
 * */
typedef struct{
	const zcl_touchlinkAppCallbacks_t *appCb;			/*!< zll commissioning callback type for application */
}zcl_zllCommission_t;


/** @} end of group zll_commissing_type */



/** @addtogroup zll_commissiing_function zll commissioning api
 *  @brief Function declaration of zll commissioning module
 *  @{
 */

 /**
  * @brief       register zll commissioning cluster.
  *
  * @param[in]   cb   - application callback during zll commissioning
  *
  * @return
  */
status_t zcl_touchlink_register(u8 endpoint, const zcl_touchlinkAppCallbacks_t *cb);


/**
 * @brief       network start event indication
 *
 * @param[in]
 *
 * @return
 */
void zcl_touchLinkDevStartIndicate(void);

/**
 * @brief       start touck link.
 *
 * @param[in]
 *
 * @return
 */
void zcl_zllTouchLinkDiscoveyStart(void);


/*
 * @brief	send command of "Get Group Identifiers request"
 *
 * @param[in] 	pDstEpInfo 	- destination end point inforamtion
 *
 * @param[in] 	startIdx	- start index
 *
 * @return
 */
void zcl_zllGetGroupIdentifiersRequest(epInfo_t* pDstEpInfo, u8 startIdx);


/*
 * @brief	send command of "Get endpoint list request"
 *
 * @param[in] 	pDstEpInfo 	- destination end point inforamtion
 *
 * @param[in] 	startIdx	- start index
 *
 * @return
 */
void zcl_zllGetEndpointListRequest(epInfo_t* pDstEpInfo, u8 startIdx);


/*
 * @brief	send the command of " end point Information"
 *
 * @param 	dstEp destination end point inforamtion
 *
 * @return
 */
void zcl_zllEndpointInformation(epInfo_t *dstEp);


/*
 * @brief	set link quality threshold for touch link, when the lqi is less than this, will be filtered
 *
 * @param 	lqi link quality
 *
 * @return
 */
void  touchlink_lqiThresholdSet(u8 lqi);


/*
 * @brief	set tx power for touch link commission, should be 0dbm
 *
 * @param 	tx power
 *
 * @return
 */
void  touchlink_commissionTxPowerSet(u8 power);

/*
 * @brief	set touch link mode
 *
 * @param 	keyType  MASTER_KEY or CERTIFICATION_KEY
 *
 * @return
 */
void  touchlink_keyModeSet(u8 keyType, u8 *key);

/*
 * @brief	touch link initiation
 *
 * @param 	none
 *
 * @return
 */
u8 zcl_touchLinkInit(void);

/*
 * @brief	touch link start
 *
 * @param 	none
 *
 * @return
 */
void zcl_touchLinkStart(void);

/** @} end of group zll_commissiing_function */

/** @} end of group ZCL_CLUSTER_ZLL_COMMISSIONING */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

