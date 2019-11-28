/********************************************************************************************************
 * @file     zcl_greenPower.h
 *
 * @brief	 APIs for green power cluster
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
#include "../zcl.h"


#define DEFAULT_TC_LINK_KEY             { 0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c,\
                                          0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x30, 0x39 }

#define GP_LINK_KEY												DEFAULT_TC_LINK_KEY
#define GP_CLUSTER_REVISION										0x0001

/**
 *  @brief	List of GP infrastructure devices
 */
#define GP_DEVICE_ID_PROXY										0x0060
#define GP_DEVICE_ID_PROXY_BASIC								0x0061
#define GP_DEVICE_ID_TARGET_PLUS								0x0062
#define GP_DEVICE_ID_TARGET										0x0063
#define GP_DEVICE_ID_COMMISSIONING_TOOL							0x0064
#define GP_DEVICE_ID_COMBO										0x0065
#define GP_DEVICE_ID_COMBO_BASIC								0x0066

/**
 *  @brief	zcl_gp_attrId Attribute Ids
 */
//Server attributes
#define ZCL_ATTRID_GP_GPS_MAX_SINK_TABLE_ENTRIES				0x0000
#define ZCL_ATTRID_GP_SINK_TABLE								0x0001
#define ZCL_ATTRID_GP_GPS_COMMUNICATION_MODE					0x0002
#define ZCL_ATTRID_GP_GPS_COMMISSIONING_EXIT_MODE				0x0003
#define ZCL_ATTRID_GP_GPS_COMMISSIONING_WINDOW					0x0004
#define ZCL_ATTRID_GP_GPS_SECURITY_LEVEL						0x0005
#define ZCL_ATTRID_GP_GPS_FUNCTIONALITY							0x0006
#define ZCL_ATTRID_GP_GPS_ACTIVE_FUNCTIONALITY					0x0007
//Client attributes
#define ZCL_ATTRID_GP_GPP_MAX_PROXY_TABLE_ENTRIES				0x0010
#define ZCL_ATTRID_GP_PROXY_TABLE								0x0011
#define ZCL_ATTRID_GP_GPP_NOTIFICATION_RETRY_NUMBER				0x0012
#define ZCL_ATTRID_GP_GPP_NOTIFICATION_RETRY_TIMER				0x0013
#define ZCL_ATTRID_GP_GPP_MAX_SEARCH_COUNTER					0x0014
#define ZCL_ATTRID_GP_GPP_BLOCKED_GPD_ID						0x0015
#define ZCL_ATTRID_GP_GPP_FUNCTIONALITY							0x0016
#define ZCL_ATTRID_GP_GPP_ACTIVE_FUNCTIONALITY					0x0017
//Attributes shared by client and server
#define ZCL_ATTRID_GP_SHARED_SECURITY_KEY_TYPE					0x0020
#define ZCL_ATTRID_GP_SHARED_SECURITY_KEY						0x0021
#define ZCL_ATTRID_GP_LINK_KEY									0x0022
#define ZCL_ATTRID_GP_CLUSTER_REVISION							0xfffd

/**
 *  @brief	zcl_gp_cmdId Command Ids
 */
//Server side commands received or client side commands generated
#define ZCL_CMD_GP_NOTIFICATION                              	0x00
#define ZCL_CMD_GP_PAIRING_SEARCH								0x01
#define ZCL_CMD_GP_TUNNELING_STOP								0x03
#define ZCL_CMD_GP_COMMISSIONING_NOTIFICATION					0x04
#define ZCL_CMD_GP_SINK_COMMISSIONING_MODE						0x05
#define ZCL_CMD_GP_TRANSLATION_TABLE_UPDATE_COMMAND				0x07
#define ZCL_CMD_GP_TRANSLATION_TABLE_REQUEST					0x08
#define ZCL_CMD_GP_PAIRING_CONFIGURATION						0x09
#define ZCL_CMD_GP_SINK_TABLE_REQUEST							0x0a
#define ZCL_CMD_GP_PROXY_TABLE_RESPONSE							0x0b
//Server side commands generated or client side commands received
#define ZCL_CMD_GP_NOTIFICATION_RESPONSE						0x00
#define ZCL_CMD_GP_PAIRING										0x01
#define ZCL_CMD_GP_PROXY_COMMISSIONING_MODE						0x02
#define ZCL_CMD_GP_RESPONSE										0x06
#define ZCL_CMD_GP_TRANSLATION_TABLE_RESPONSE					0x08
#define ZCL_CMD_GP_SINK_TABLE_RESPONSE							0x0a
#define ZCL_CMD_GP_PROXY_TABLE_REQUEST							0x0b


//GPD Command IDs, A.4.1
#define GP_COMMISSIONING_COMMAND_ID                      		0xE0//Data frame
#define GP_DECOMMISSIONING_COMMAND_ID                    		0xE1//Data frame
#define GP_SUCCESS_COMMAND_ID                            		0xE2//Data frame
#define GP_CHANNEL_REQ_COMMAND_ID                        		0xE3//Maintenance frame
#define GP_COMMISSIONING_REPLY									0xF0
#define GP_CHANNEL_CONFIGURATION_COMMAND_ID						0xF3


#define ZCL_GP_MAX_SINK_TABLE_ATTR_LEN							45
#define ZCL_GP_MAX_PROXY_TABLE_ATTR_LEN							75

/**
 *  @brief	The GP Security options
 */
typedef union
{
	u8	opts;
	struct
	{
		u8	secLevel:2;
		u8	secKeyType:3;//0x00 - 0x07
		u8	reserved:3;
	}bits;
}gpSecOpt_t;

typedef struct
{
	u16	groupId;
	u16	alias;
}gpSinkGroupListItem_t;


#define SINK_GROUP_LIST_NUM						2

typedef union
{
	u16	opts;
	struct
	{
		u16	appId:3;
		u16	commuicationMode:2;
		u16	seqNumCapabilities:1;
		u16	rxOnCapability:1;
		u16	fixedLocation:1;
		u16	assignedAlias:1;
		u16	securityUse:1;
		u16	reserved:6;
	}bits;
}gpSinkEntryOpt_t;

/**
 *  @brief	Sink table entry
 */
typedef struct
{
	gpdId_t	gpdId;						//8-bytes
	u32	gpdSecFrameCnt;
	gpSinkEntryOpt_t	options;
	u16	gpdAssignedAlias;				//8-bytes
	gpSinkGroupListItem_t	groupList[SINK_GROUP_LIST_NUM];	//8-bytes

	u8	endpoint;
	u8	deviceId;
	u8	groupcastRadius;
	gpSecOpt_t	secOptions;				//4-bytes

	u8	gpdKey[16];						//16-bytes

	u8	used:1;
	u8	sinkGroupListCnt:2;
	u8	reserved:5;
}gpSinkTabEntry_t;	//45-bytes

/**
 *  @brief	gpsCommunicationMode attribute
 */
typedef enum
{
	GPS_COMM_MODE_FULL_UNICAST,
	GPS_COMM_MODE_GROUP_DGROUPID,
	GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID,
	GPS_COMM_MODE_LIGHTWEIGHT_UNICAST,
}gpsCommMode_t;

/**
 *  @brief	gpsCommunicationExitMode attribute
 */
typedef union
{
	u8	mode;
	struct
	{
		u8	onCommWindowExpiration:1;
		u8	onFirstPairingSuccess:1;
		u8	onGpProxyCommMode_exit:1;
		u8	reserved:5;
	}bits;
}gpsCommExitMode_t;

/**
 *  @brief	gpsSecurityLevel attribute
 */
typedef union
{
	u8	level;
	struct
	{
		u8	minimalGpdSecLevel:2;
		u8	protectionWithGpLinkKey:1;
		u8	involveTC:1;
		u8	reserved:4;
	}bits;
}gpsSecLevel_t;


typedef union
{
	u16	opts;
	struct
	{
		u16	appId:3;
		u16	entryActive:1;
		u16	entryValid:1;
		u16	seqNumCap:1;
		u16	lightWeightUnicastGPS:1;
		u16	derivedGroupGPS:1;
		u16	commGroupGPS:1;
		u16	firstToForward:1;
		u16	inRange:1;
		u16	gpdFixed:1;
		u16	hasAllUnicastRoutes:1;
		u16	assignedAlias:1;
		u16	secUse:1;
		u16	optsExtension:1;
	}bits;
}gpProxyEntryOpt_t;

typedef struct
{
	addrExt_t	sinkIeeeAddr;
	u16			sinkNwkAddr;
}gpLwSinkAddrListItem_t;

typedef struct
{
	u16	fullUnicastGPS:1;
	u16	reserved:15;
}gpExtendedOpt_t;

/**
 *  @brief	Proxy table entry
 */
#define PROXY_LIGHTWEIGHT_SINK_ADDR_LIST_NUM		2
#define PROXY_SINK_GROUP_LIST_NUM					2
#define PROXY_FULL_UNICAST_SINK_ADDR_LIST_NUM		2

typedef struct
{
	gpdId_t	gpdId;							//8-bytes

	gpProxyEntryOpt_t	options;
	u16					gpdAssignedAlias;
	gpSecOpt_t			secOptions;
	u8					endpoint;
	u8					groupcastRadius;
	u8					searchCnt;			//8-bytes

	u8					gpdKey[16];			//16-bytes

	u32					gpdSecFrameCnt;		//4-bytes

	gpLwSinkAddrListItem_t	lightweightSinkAddrList[PROXY_LIGHTWEIGHT_SINK_ADDR_LIST_NUM];	//20-bytes
	gpSinkGroupListItem_t	sinkGroupList[PROXY_SINK_GROUP_LIST_NUM];						//8-bytes

	u8					used:1;
	u8					lwSinkCnt:2;
	u8					sinkGroupCnt:2;
	u8					reserved:3;
}gpProxyTabEntry_t;	//75-bytes

/**
 *  @brief	gppBiockedGPDID attribute
 */
typedef struct
{
	u8	appId:3;
	u8	reserved:5;
}gppBlockedGpdIdOpt_t;

typedef struct
{
	gpdId_t	gpdId;
	gppBlockedGpdIdOpt_t	options;
	u8	endpoint;
	u8	seqNum;
	u8	searchCnt;
}gppBlockedGpdId_t;




/******************************************************************
 ******************** Format of command ***************************
 *****************************************************************/
typedef union
{
	u8 link;
	struct
	{
		u8	rssi:6;
		u8	lqi:2;
	}bits;
}gpGppGpdLink_t;

typedef union
{
	u16 opts;
	struct
	{
		u16 appId:3;
		u16	alsoUnicast:1;
		u16 alsoDerivedGroup:1;
		u16	alsoCommGroup:1;
		u16	secLevel:2;
		u16	secKeyType:3;
		u16	rxAfterTx:1;
		u16	gpTxQueueFull:1;
		u16	bidirectionalCap:1;
		u16	proxyInfoPresent:1;
		u16 reserved:1;
	}bits;
}gpNotiOpt_t;

/**
 *  @brief	Format of the GP Notification command
 */
typedef struct
{
	gpdId_t	gpdId;
	u8	*pGpdCmdPayload;
	u32	gpdSecFrameCnt;
	gpNotiOpt_t options;
	u16	gppShortAddr;
	u8	endpoint;
	u8	gpdCmdID;
	u8	payloadLen;
	gpGppGpdLink_t	gppGpdLink;
}zcl_gp_notificationCmd_t;


typedef union
{
	u24 opts;
	struct
	{
		u24	appId:3;
		u24	addSink:1;
		u24	removeGPD:1;
		u24	commMode:2;
		u24	gpdFixed:1;
		u24	gpdMacSeqNumCap:1;
		u24	secLevel:2;
		u24	secKeyType:3;
		u24	gpdSecFrameCntPresent:1;
		u24	gpdSecKeyPresent:1;
		u24	assignedAliasPresent:1;
		u24	groupcastRadiusPresent:1;
		u24	reserved:6;
	}bits;
}gpPairingOpt_t;

/**
 *  @brief	Format of the GP Pairing command
 */
typedef struct
{
	gpdId_t	gpdId;
	addrExt_t	sinkIeeeAddr;
	gpPairingOpt_t options;
	u32			gpdSecFrameCnt;
	u8			gpdKey[16];
	u16			sinkNwkAddr;
	u16			sinkGroupID;
	u16			assignedAlias;
	u8			endpoint;
	u8			deviceID;
	u8			groupcastRadius;
}zcl_gp_pairingCmd_t;


enum
{
	EXIT_ON_FIRST_PAIRING_SUCCESS 			= BIT(0),
	EXIT_ON_GP_PROXY_COMMISSIONING_MODE		= BIT(1),
};

typedef union
{
	u8	opts;
	struct
	{
		u8	action:1;
		u8	commWindowPresent:1;
		u8	exitMode:2;
		u8	channelPresent:1;
		u8	unicastCommunication:1;
		u8	reserved:2;
	}bits;
}gpProxyCommModeOpt_t;

/**
 *  @brief	Format of the GP Proxy Commissioning Mode command
 */
typedef struct
{
	gpProxyCommModeOpt_t options;
	u8	channel;
	u16 commissioningWindow;
}zcl_gp_proxyCommissioningModeCmd_t;


typedef union
{
	u16 opts;
	struct
	{
		u16 appId:3;
		u16	rxAfterTx:1;
		u16	secLevel:2;
		u16	secKeyType:3;
		u16 secPrcFailed:1;
		u16	bidirectionalCap:1;
		u16	proxyInfoPresent:1;
		u16 reserved:4;
	}bits;
}gpCommNotiOpt_t;

/**
 *  @brief	Format of the GP Commissioning Notification command
 */
typedef struct
{
	gpdId_t	gpdId;
	u8	*pGpdCmdPayload;
	u32	gpdSecFrameCnt;
	u32 mic;
	gpCommNotiOpt_t options;
	u16	gppShortAddr;
	u8	endpoint;
	u8	gpdCmdID;
	u8	payloadLen;
	gpGppGpdLink_t	gppGpdLink;
}zcl_gp_commissioningNotificationCmd_t;


typedef union
{
	u8	opts;
	struct
	{
		u8	appId:3;
		u8	transmitOnEndpointMatch:1;
		u8	reserved:4;
	}bits;
}gpResponseOpt_t;

/**
 *  @brief	Format of the GP Response command
 */
typedef struct
{
	gpdId_t	gpdId;
	u8	*pGpdCmdPayload;
	u16 tempMasterShortAddr;
	gpResponseOpt_t options;
	u8	tempMasterTxChannel;
	u8	endpoint;
	u8	gpdCmdID;
	u8	payloadLen;
}zcl_gp_responseCmd_t;


enum
{
	ACTION_ADD_TRANS_TABLE_ENTRY,
	ACTION_REPLACE_TRANS_TABLE_ENTRY,
	ACTION_REMOVE_TRANS_TABLE_ENTRY,
};

typedef union
{
	u16	opts;
	struct
	{
		u16	appId:3;
		u16 action:2;
		u16 numOfTrans:3;
		u16	additionalInfoBlockPresent:1;
		u16	reserved:7;
	}bits;
}gpTransTabUpdateOpt_t;

typedef struct
{
	u16 profile;
	u16 cluster;
	u8	index;
	u8	gpdCmdId;
	u8	endpoint;
	u8	zbCmdId;
	u8	zbCmdPayloadLen;
	u8	*pZbPayloadAndAdditionalInfo;
}gpTranslation_t;

/**
 *  @brief	Format of the GP Translation Table Update command
 */
typedef struct
{
	gpdId_t	gpdId;
	u8  *pBuf;//buffer point of translation
	gpTransTabUpdateOpt_t options;
	u8  endpoint;
	u8  bufLen;//length of translation buffer
}zcl_gp_transTabUpdateCmd_t;


/**
 *  @brief	Format of the GP Translation Table Request command
 */
typedef struct
{
	u8	startIdx;
}zcl_gp_transTabReqCmd_t;


typedef union
{
	u8	opts;
	struct
	{
		u8	appId:3;
		u8	additionalInfoBlockPresent:1;
		u8	reserved:4;
	}bits;
}gpTransTabRspOpt_t;

typedef struct
{
	gpdId_t	gpdId;
	u16	profile;
	u16 cluster;
	u8	gpdEndpoint;
	u8	gpdCmdId;
	u8	endpoint;
	u8	zbCmdId;
	u8	zbCmdPayloadLen;
	u8	*pZbPayloadAndAdditionalInfo;
}gpTransTabList_t;

/**
 *  @brief	Format of the GP Translation Table Response command
 */
typedef struct
{
	u8  *pBuf;//buffer point of translation table list
	u8	bufLen;//length of translation table list buffer
	u8	status;
	gpTransTabRspOpt_t options;
	u8	totalNumOfEntries;
	u8	startIdx;
	u8	entriesCnt;
}zcl_gp_transTabRspCmd_t;


/**
 *  @brief	Action sub-field of the actions field
 */
enum
{
	NO_ACTION,
	EXTEND_SINK_TABLE_ENTRY,
	REPLACE_SINK_TABLE_ENTRY,
	REMOVE_A_PAIRING,
	REMOVE_GPD,
	APPLICATION_DESCRIPTION,
};

typedef union
{
	u8	acts;
	struct
	{
		u8	action:3;
		u8	sendGpPairing:1;
		u8  reserved:4;
	}bits;
}gpPairingCfgActions_t;

typedef union
{
	u16	opts;
	struct
	{
		u16	appId:3;
		u16	commMode:2;
		u16	seqNumCap:1;
		u16 rxOnCap:1;
		u16 fixedLocation:1;
		u16 assignedAlias:1;
		u16 secUse:1;
		u16 appInfoPresent:1;
		u16 reserved:5;
	}bits;
}gpPairingCfgOpt_t;

typedef union
{
	u8	info;
	struct
	{
		u8	manuIdPresent:1;
		u8	modelIdPresent:1;
		u8	gpdCmdsPresent:1;
		u8	clusterListPresent:1;
		u8	switchInfoPresent:1;
		u8	gpdAppDescCmdFollows:1;
		u8  reserved:2;
	}bits;
}gpAppInfo_t;

/**
 *  @brief	Format of the GP Pairing Configuration command
 */
typedef struct
{
	u8	*pGroupList;
	u8	*pPairedEndpoints;
	u8	*pGpdCmdList;
	u8	*pClusterList;
	u8	*pSwitchInfo;
	u8	*pReportDescriptor;

	u32	gpdSecFrameCnt;
	u8	gpdSecKey[16];
	gpdId_t	gpdId;
	gpPairingCfgOpt_t options;
	u16 gpdAssignedAlias;
	u16	manufacturerId;
	u16	modelId;
	gpPairingCfgActions_t actions;
	gpAppInfo_t appInfo;
	u8	endpoint;
	u8	deviceId;
	u8	groupcastRadius;
	u8	secOptions;
	u8	numOfPairedEndpoints;
	u8	numOfGpdCmds;
	u8	totalNumOfReports;
	u8	numOfReports;
}zcl_gp_pairingConfigurationCmd_t;//70 bytes


/**
 *  @brief	Request type sub-field of the options
 */
enum
{
	REQUEST_TABLE_ENTRIES_BY_GPD_ID,
	REQUEST_TABLE_ENTRIES_BY_INDEX,
};

typedef union
{
	u8	opts;
	struct
	{
		u8	appId:3;
		u8	reqType:2;
		u8	reserved:3;
	}bits;
}gpSinkProxyTabReqOpt_t;

/**
 *  @brief	Format of the GP Sink Table Request command
 */
typedef struct
{
	gpdId_t	gpdId;
	gpSinkProxyTabReqOpt_t options;
	u8	endpoint;
	u8	index;
}zcl_gp_sinkTabReqCmd_t;

/**
 *  @brief	Format of the GP Sink Table Response command
 */
typedef struct
{
	u8	*sinkTabEntry;//buffer pointer of sink table entries field
	u8	entriesLen;//buffer length of sink table entries
	u8	status;
	u8	totalTabEntries;
	u8	startIdx;
	u8	entriesCnt;
}zcl_gp_sinkTabRspCmd_t;

/**
 *  @brief	Format of the GP Proxy Table Request command
 */
typedef struct
{
	gpdId_t	gpdId;
	gpSinkProxyTabReqOpt_t options;
	u8	endpoint;
	u8	index;
}zcl_gp_proxyTabReqCmd_t;

/**
 *  @brief	Format of the GP Proxy Table Response command
 */
typedef struct
{
	u8	*proxyTabEntry;
	u8	entriesLen;
	u8	status;
	u8	totalTabEntries;
	u8	startIdx;
	u8	entriesCnt;
}zcl_gp_proxyTabRspCmd_t;


typedef union
{
	u8	opts;
	struct
	{
		u8	action:1;
		u8	involveGpmInSec:1;
		u8	involveGpmInPairing:1;
		u8	involveProxies:1;
		u8	reserved:4;
	}bits;
}gpSinkCommModeOpt_t;

/**
 *  @brief	Format of the GP Sink Commissioning Mode command
 */
typedef struct
{
	u16 gpmAddrForSec;
	u16 gpmAddrForPairing;
	gpSinkCommModeOpt_t options;
	u8	sinkEndpoint;
}zcl_gp_sinkCommissioningModeCmd_t;

#if 0
typedef void (*zcl_gp_PairingCb_t)(apsdeDataInd_t *pInd, zcl_gp_pairingCmd_t *pCmd);
typedef void (*zcl_gp_proxyTabReqCb_t)(apsdeDataInd_t *pInd, zcl_gp_proxyTabReqCmd_t *pCmd);
typedef void (*zcl_gp_proxyCommModeCb_t)(apsdeDataInd_t *pInd, zcl_gp_proxyCommissioningModeCmd_t *pCmd);
typedef void (*zcl_gp_rspCb_t)(apsdeDataInd_t *pInd, zcl_gp_responseCmd_t *pCmd);

/**
 *  @brief  Structure definition for commands callBack functions in GP
 */
typedef struct
{
	zcl_gp_PairingCb_t                     	gpPairingCbFunc;
	zcl_gp_proxyTabReqCb_t					gpProxtTabReqCbFunc;
	zcl_gp_proxyCommModeCb_t				gpProxyCommModeCbFunc;
	zcl_gp_rspCb_t							gpRspCbFunc;
}zcl_gp_AppCallbacks_t;

/**
 *  @brief  External variable for GP Attribute
 */
extern const u8 zclGpAttr_gppMaxProxyTabEntries;
extern const u16 zclGpAttr_clusterRev;
extern u8 zclGpAttr_proxyTabEntry[];
#if 0
extern u8 zclGpAttr_gppNotificationRetryNum;
extern u8 zclGpAttr_gppNotificationTimer;
extern u8 zclGpAttr_gppMaxSearchCnt;
extern u8 *pZclGpAttr_gppBlockedGPDID;
#endif
extern u8 zclGpAttr_gppFunc[];
extern u8 zclGpAttr_gppActiveFunc[];

extern u8 zclGpAttr_gpSharedSecKeyType;
extern u8 zclGpAttr_gpSharedSecKey[];
extern u8 zclGpAttr_gpLinkKey[];



/**
 *  @brief  External variable for GP Attribute info table
 */
extern const zclAttrInfo_t gp_attrTbl[];

extern const u8 zcl_gp_attrNum;
#endif

status_t zcl_gp_register(u8 endpoint, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_gp_notificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_notificationCmd_t *pCmd);
#define zcl_gp_notificationCmdSend(a,b,c,d)	(zcl_gp_notificationCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_commissioningNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_commissioningNotificationCmd_t *pCmd);
#define zcl_gp_commissioningNotificationCmdSend(a,b,c,d)	(zcl_gp_commissioningNotificationCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_sinkTabReqCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_sinkTabReqCmd_t *pCmd);
#define zcl_gp_sinkTabReqCmdSend(a,b,c,d)	(zcl_gp_sinkTabReqCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_proxyTableRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_proxyTabRspCmd_t *pCmd);
#define zcl_gp_proxyTableRspCmdSend(a,b,c,d)	(zcl_gp_proxyTableRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_pairingCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_pairingCmd_t *pCmd);
#define zcl_gp_pairingCmdSend(a,b,c,d)	(zcl_gp_pairingCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_proxyCommissioningModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_proxyCommissioningModeCmd_t *pCmd);
#define zcl_gp_proxyCommissioningModeCmdSend(a,b,c,d)	(zcl_gp_proxyCommissioningModeCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_responseCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_responseCmd_t *pCmd);
#define zcl_gp_responseCmdSend(a,b,c,d)		(zcl_gp_responseCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_sinkTableRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_sinkTabRspCmd_t *pCmd);
#define zcl_gp_sinkTableRspCmdSend(a,b,c,d)		(zcl_gp_sinkTableRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_proxyTableReqCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_proxyTabReqCmd_t *pCmd);
#define zcl_gp_proxyTableReqCmdSend(a,b,c,d)	(zcl_gp_proxyTableReqCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_gp_sinkCommissioningModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_sinkCommissioningModeCmd_t *pCmd);
#define zcl_gp_sinkCommissioningModeCmdSend(a,b,c,d)	(zcl_gp_sinkCommissioningModeCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))
