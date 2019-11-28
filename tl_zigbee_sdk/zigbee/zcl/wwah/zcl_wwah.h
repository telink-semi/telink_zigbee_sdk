/********************************************************************************************************
 * @file     zcl_wwah.h
 *
 * @brief	 APIs statement for WWAH cluster
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

/**
 *  @brief	attributes of WWAH cluster
 */
#define ZCL_ATTRID_WWAH_VERSION							0x0000
#define ZCL_ATTRID_WWAH_HUB_ADDRESS						0x0001
#define ZCL_ATTRID_WWAH_FROM_HUB_ONLY					0x0002
#define ZCL_ATTRID_WWAH_DISABLE_OTA_DOWNGRADES			0x0003
#define ZCL_ATTRID_WWAH_DISABLE_TC_REJOINS				0x0004
#define ZCL_ATTRID_WWAH_NWK_RETRY_COUNT					0x0005
#define ZCL_ATTRID_WWAH_MAC_RETRY_COUNT					0x0006
#define ZCL_ATTRID_WWAH_ROUTER_CHECK_IN_ENABLED			0x0007
#define ZCL_ATTRID_WWAH_ROUTING_CAPABILITY				0x0008
#define ZCL_ATTRID_WWAH_PARENTING_CAPABILITY			0x0009
#define ZCL_ATTRID_WWAH_APP_EVT_RETRY_ENABLED			0x000A
#define ZCL_ATTRID_WWAH_APP_EVT_RETRY_QUEUE_SIZE		0x000B
#define ZCL_ATTRID_WWAH_REJOIN_ENABLED					0x000C
#define ZCL_ATTRID_WWAH_MAC_POLL_CCA_WAIT_TIME			0x000D

/**
 *  @brief	zcl_ota_cmdId WWAH Command Ids
 */
//Received
#define ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH			0x00
#define ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH			0x01
#define ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY			0x02
#define ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY			0x03
#define ZCL_CMD_WWAH_ENABLE_APP_EVT_RETRY_ALGORITHM		0x04
#define ZCL_CMD_WWAH_DISABLE_APP_EVT_RETRY_ALGORITHM	0x05
#define ZCL_CMD_WWAH_REQUEST_TIME						0x06
#define ZCL_CMD_WWAH_ENABLE_REJOIN_ALGORITHM			0x07
#define ZCL_CMD_WWAH_DISABLE_REJOIN_ALGORITHM			0x08
#define ZCL_CMD_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD		0x09
#define ZCL_CMD_WWAH_CLEAR_BINDING_TABLE				0x0A
#define ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS	0x0B
#define ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS	0x0C
//Generated
#define ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY_RSP		0x00
#define ZCL_CMD_WWAH_POWERING_OFF_NOTIFICATION			0x01
#define ZCL_CMD_WWAH_POWERING_ON_NOTIFICATION			0x02
#define ZCL_CMD_WWAH_SHORT_ADDRESS_CHANGE				0x03



typedef struct{
	u8 numOfClustersToExempt;
	u16 clusterID[];
}wwah_enableApsLinkKeyAuth_t;

typedef wwah_enableApsLinkKeyAuth_t wwah_disableApsLinkKeyAuth_t;

typedef struct{
	u16 clusterID;
}wwah_apsLinkKeyAuthQuery_t;

typedef struct{
	u32 maxBackoffTimeInSeconds;
	u8 firstBackoffTimeInSeconds;
	u8 backoffSequenceCommonRatio;
	u8 maxReDeliveryAttepts;
}wwah_enableAppEvtRetryAlgorithm_t;

typedef struct{
	u16 fastRejoinTimeoutInSeconds;
	u16 durationBetweenEachRejoinInSeconds;
}wwah_enableRejoinAlgorithm_t;

typedef struct{
	u8 enrollmentMode;
}wwah_setIasZoneEnrollmentMethod_t;

typedef struct{
	u16 checkInInterval;
}wwah_enablePeriodicRouterCheckIns_t;

typedef struct{
	u16 clusterID;
	bool status;
}wwah_apsLinkKeyAuthQueryRsp_t;

#if 0
/**
 *  @brief  Definition for wwah command callback function type
 */
typedef status_t (*zcl_wwah_enableApsLinkKeyAuthCb_t)( apsdeDataInd_t *pInd, wwah_enableApsLinkKeyAuth_t *pEnableApsLinkKeyAuth );
typedef status_t (*zcl_wwah_disableApsLinkKeyAuthCb_t)( apsdeDataInd_t *pInd, wwah_disableApsLinkKeyAuth_t *pDisableApsLinkKeyAuth );
typedef status_t (*zcl_wwah_apsLinkKeyAuthQueryCb_t)( apsdeDataInd_t *pInd, wwah_apsLinkKeyAuthQuery_t *pApsLinkKeyAuthQuery );
typedef status_t (*zcl_wwah_requestNewApsLinkKeyCb_t)( apsdeDataInd_t *pInd);
typedef status_t (*zcl_wwah_enableAppEvtRetryAlgorithmCb_t)( apsdeDataInd_t *pInd, wwah_enableAppEvtRetryAlgorithm_t *pEnableAppEvtRetryAlgorithm );
typedef status_t (*zcl_wwah_disableAppEvtRetryAlgorithmCb_t)( apsdeDataInd_t *pInd);
typedef status_t (*zcl_wwah_requestTimeCb_t)( apsdeDataInd_t *pInd);
typedef status_t (*zcl_wwah_enableRejoinAlgorithmCb_t)( apsdeDataInd_t *pInd, wwah_enableRejoinAlgorithm_t *pEnableRejoinAlgorithm );
typedef status_t (*zcl_wwah_disableRejoinAlgorithmCb_t)( apsdeDataInd_t *pInd);
typedef status_t (*zcl_wwah_setIasZoneEnrollmentMethodCb_t)( apsdeDataInd_t *pInd, wwah_setIasZoneEnrollmentMethod_t *pSetIasZoneEnrollmentMethod );
typedef status_t (*zcl_wwah_clearBindingTableCb_t)( apsdeDataInd_t *pInd);
typedef status_t (*zcl_wwah_enablePeriodicRouterCheckInsCb_t)( apsdeDataInd_t *pInd, wwah_enablePeriodicRouterCheckIns_t *pEnablePeriodicRouterCheckIns );
typedef status_t (*zcl_wwah_disablePeriodicRouterCheckInsCb_t)( apsdeDataInd_t *pInd);

/**
 *  @brief  Structure definition for commands callback functions in WWAH cluster
 */
typedef struct {
	zcl_wwah_enableApsLinkKeyAuthCb_t			enableApsLinkKeyAuthCbFunc;
	zcl_wwah_disableApsLinkKeyAuthCb_t			disableApsLinkKeyAuthCbFunc;
	zcl_wwah_apsLinkKeyAuthQueryCb_t			apsLinkKeyAuthQueryCbFunc;
	zcl_wwah_requestNewApsLinkKeyCb_t			requestNewApsLinkKeyCbFunc;
	zcl_wwah_enableAppEvtRetryAlgorithmCb_t		enableAppEvtRetryAlgorithmCbFunc;
	zcl_wwah_disableAppEvtRetryAlgorithmCb_t	disableAppEvtRetryAlgorithmCbFunc;
	zcl_wwah_requestTimeCb_t					requestTimeCbFunc;
	zcl_wwah_enableRejoinAlgorithmCb_t			enableRejoinAlgorithmCbFunc;
	zcl_wwah_disableRejoinAlgorithmCb_t			disableRejoinAlgorithmCbFunc;
	zcl_wwah_setIasZoneEnrollmentMethodCb_t		setIasZoneEnrollmentMethodCbFunc;
	zcl_wwah_clearBindingTableCb_t				clearBindingTableCbFunc;
	zcl_wwah_enablePeriodicRouterCheckInsCb_t	enablePeriodicRouterCheckInsCbFunc;
	zcl_wwah_disablePeriodicRouterCheckInsCb_t	disablePeriodicRouterCheckInsCbFunc;
}zcl_wwah_AppCallbacks_t;

/**
 *  @brief  External variable for WWAH Attribute
 */
extern u8 zcl_attr_wwah_version;
extern addrExt_t zcl_attr_wwah_hubAddress;
extern bool zcl_attr_wwah_otaFromHubOnly;
extern bool zcl_attr_wwah_disableOTADowngrades;
extern bool zcl_attr_wwah_disableTCRejoins;
extern u8 zcl_attr_wwah_nwkRetryCount;
extern u8 zcl_attr_wwah_macRetryCount;
extern bool zcl_attr_wwah_routerCheckInEnabled;
extern bool zcl_attr_wwah_routingCapability;
extern bool zcl_attr_wwah_parentingCapability;
extern bool zcl_attr_wwah_appEvtRetryEnabled;
extern u8 zcl_attr_wwah_appEvtRetryQueueSize;
extern bool zcl_attr_wwah_rejoinEnabled;
extern u8 zcl_attr_wwah_macPollCCAWaitTime;

/**
 *  @brief  External variable for WWAH Attribute info table
 */
extern const zclAttrInfo_t wwah_attrTbl[];
extern const u8 zcl_wwah_attrNum;
#endif

status_t zcl_wwah_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_wwah_enableApsLinkKeyAuthSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enableApsLinkKeyAuth_t *pReq);
#define zcl_wwah_enableApsLinkKeyAuthCmdSend(a,b,c,d)	(zcl_wwah_enableApsLinkKeyAuthSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disableApsLinkKeyAuthSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_disableApsLinkKeyAuth_t *pReq);
#define zcl_wwah_disableApsLinkKeyAuthCmdSend(a,b,c,d)	(zcl_wwah_disableApsLinkKeyAuthSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_apsLinkKeyAuthQuerySend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_apsLinkKeyAuthQuery_t *pReq);
#define zcl_wwah_apsLinkKeyAuthQueryCmdSend(a,b,c,d)	(zcl_wwah_apsLinkKeyAuthQuerySend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_requestNewApsLinkKeySend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_requestNewApsLinkKeyCmdSend(a,b,c)	(zcl_wwah_requestNewApsLinkKeySend((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableAppEvtRetryAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enableAppEvtRetryAlgorithm_t *pReq);
#define zcl_wwah_enableAppEvtRetryAlgorithmCmdSend(a,b,c,d)	(zcl_wwah_enableAppEvtRetryAlgorithmSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disableAppEvtRetryAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableAppEvtRetryAlgorithmCmdSend(a,b,c)	(zcl_wwah_disableAppEvtRetryAlgorithmSend((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_requestTimeSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_requestTimeCmdSend(a,b,c)	(zcl_wwah_requestTimeSend((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableRejoinAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enableRejoinAlgorithm_t *pReq);
#define zcl_wwah_enableRejoinAlgorithmCmdSend(a,b,c,d)	(zcl_wwah_enableRejoinAlgorithmSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disableRejoinAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableRejoinAlgorithmCmdSend(a,b,c)	(zcl_wwah_disableRejoinAlgorithmSend((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_setIasZoneEnrollmentMethodSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_setIasZoneEnrollmentMethod_t *pReq);
#define zcl_wwah_setIasZoneEnrollmentMethodCmdSend(a,b,c,d)	(zcl_wwah_setIasZoneEnrollmentMethodSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_clearBindingTableSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_clearBindingTableCmdSend(a,b,c)	(zcl_wwah_clearBindingTableSend((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enablePeriodicRouterCheckInsSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enablePeriodicRouterCheckIns_t *pReq);
#define zcl_wwah_enablePeriodicRouterCheckInsCmdSend(a,b,c,d)	(zcl_wwah_enablePeriodicRouterCheckInsSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disablePeriodicRouterCheckInsSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disablePeriodicRouterCheckInsCmdSend(a,b,c)	(zcl_wwah_disablePeriodicRouterCheckInsSend((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_apsLinkKeyAuthQueryRspSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_apsLinkKeyAuthQueryRsp_t *pRsp);
#define zcl_wwah_apsLinkKeyAuthQueryRspCmdSend(a,b,c,d)	(zcl_wwah_apsLinkKeyAuthQueryRspSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_poweringOffNotificationSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 reason);
#define zcl_wwah_poweringOffNotificationCmdSend(a,b,c,d)	(zcl_wwah_poweringOffNotificationSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_poweringOnNotificationSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 reason);
#define zcl_wwah_poweringOnNotificationCmdSend(a,b,c,d)	(zcl_wwah_poweringOnNotificationSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_shortAddrChangeSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_shortAddrChangeCmdSend(a,b,c)	(zcl_wwah_shortAddrChangeSend((a), (b), (c), ZCL_SEQ_NUM))
