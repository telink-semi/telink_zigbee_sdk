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
#define ZCL_ATTRID_WWAH_DISABLE_OTA_DOWNGRADES					0x0002
#define ZCL_ATTRID_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED		0x0003
#define ZCL_ATTRID_WWAH_NWK_RETRY_COUNT							0x0004
#define ZCL_ATTRID_WWAH_MAC_RETRY_COUNT							0x0005
#define ZCL_ATTRID_WWAH_ROUTER_CHECK_IN_ENABLED					0x0006
#define ZCL_ATTRID_WWAH_TOUCHLINK_INTERPAN_ENABLED				0x0007
#define ZCL_ATTRID_WWAH_PARENT_CLASSIFICATION_ENABLED			0x0008
#define ZCL_ATTRID_WWAH_APP_EVT_RETRY_ENABLED					0x0009
#define ZCL_ATTRID_WWAH_APP_EVT_RETRY_QUEUE_SIZE				0x000A
#define ZCL_ATTRID_WWAH_REJOIN_ENABLED							0x000B
#define ZCL_ATTRID_WWAH_MAC_POLL_FAILURE_WAIT_TIME				0x000C
#define ZCL_ATTRID_WWAH_CONFIGURATION_MODE_ENABLED				0x000D
#define ZCL_ATTRID_WWAH_CURRENT_DEBUG_REPORT_ID					0x000E
#define ZCL_ATTRID_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED	0x000F
#define ZCL_ATTRID_WWAH_BAD_PARENT_RECOVERY_ENABLED				0x0010
#define ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_CHANNEL				0x0011
#define ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_PANID				0x0012
#define ZCL_ATTRID_WWAH_OTA_MAX_OFFLINE_DURATION				0x0013

/**
 *  @brief	zcl_ota_cmdId WWAH Command Ids
 */
//Server side commands received or client side commands generated
#define ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH					0x00
#define ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH					0x01
#define ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY					0x02
#define ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY					0x03
#define ZCL_CMD_WWAH_ENABLE_APP_EVT_RETRY_ALGORITHM				0x04
#define ZCL_CMD_WWAH_DISABLE_APP_EVT_RETRY_ALGORITHM			0x05
#define ZCL_CMD_WWAH_REQUEST_TIME								0x06
#define ZCL_CMD_WWAH_ENABLE_REJOIN_ALGORITHM					0x07
#define ZCL_CMD_WWAH_DISABLE_REJOIN_ALGORITHM					0x08
#define ZCL_CMD_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD				0x09
#define ZCL_CMD_WWAH_CLEAR_BINDING_TABLE						0x0A
#define ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS			0x0B
#define ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS			0x0C
#define ZCL_CMD_WWAH_SET_MAC_POLL_FAILURE_WAIT_TIME				0x0D
#define ZCL_CMD_WWAH_SET_PENDING_NWK_UPDATE						0x0E
#define ZCL_CMD_WWAH_REQUIRE_APS_ACKS_ON_UNICASTS				0x0F
#define ZCL_CMD_WWAH_REMOVE_APS_ACKS_ON_UNICASTS_REQUIREMENT	0x10
#define ZCL_CMD_WWAH_APS_ACK_REQUIREMENT_QUERY					0x11
#define ZCL_CMD_WWAH_DEBUG_REPORT_QUERY							0x12
#define ZCL_CMD_WWAH_SURVEY_BEACONS								0x13
#define ZCL_CMD_WWAH_DISABLE_OTA_DOWNGRADES						0x14
#define ZCL_CMD_WWAH_DISABLE_MGMT_LEAVE_WITHOUT_REJOIN			0x15
#define ZCL_CMD_WWAH_DISABLE_TOUCHLINK_INTERPAN_MSG_SUPPORT		0x16
#define ZCL_CMD_WWAH_ENABLE_PARENT_CLASSIFICATION				0x17
#define ZCL_CMD_WWAH_DISABLE_PARENT_CLASSIFICATION				0x18
#define ZCL_CMD_WWAH_ENABLE_TC_SECURITY_ON_NWK_KEY_ROTATION		0x19
#define ZCL_CMD_WWAH_ENABLE_BAD_PARENT_RECOVERY					0x1A
#define ZCL_CMD_WWAH_DISABLE_BAD_PARENT_RECOVERY				0x1B
#define ZCL_CMD_WWAH_ENABLE_CONFIGURATION_MODE					0x1C
#define ZCL_CMD_WWAH_DISABLE_CONFIGURATION_MODE					0x1D
#define ZCL_CMD_WWAH_USE_TRUST_CENTER_FOR_CLUSTER				0x1E
#define ZCL_CMD_WWAH_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY		0x1F
//Server side commands generated or client side commands received
#define ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY_RSP				0x00
#define ZCL_CMD_WWAH_POWERING_OFF_NOTIFICATION					0x01
#define ZCL_CMD_WWAH_POWERING_ON_NOTIFICATION					0x02
#define ZCL_CMD_WWAH_SHORT_ADDRESS_CHANGE						0x03
#define ZCL_CMD_WWAH_APS_ACK_REQUIREMENT_QUERY_RSP				0x04
#define ZCL_CMD_WWAH_POWER_DESCRIPTOR_CHANGE					0x05
#define ZCL_CMD_WWAH_NEW_DEBUG_REPORT_NOTIFICATION				0x06
#define ZCL_CMD_WWAH_DEBUG_REPORT_QUERY_RSP						0x07
#define ZCL_CMD_WWAH_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY_RSP	0x08
#define ZCL_CMD_WWAH_SURVEY_BEACONS_RSP							0x09


typedef struct{
	u16 *pClusterID;
	u8  numOfClustersToExempt;
}clustersToExempt_t;

typedef struct{
	u16 clusterID;
}zcl_wwah_apsLinkKeyAuthQueryCmd_t;

typedef struct{
	u32 maxBackoffTimeInSeconds;
	u8  firstBackoffTimeInSeconds;
	u8  backoffSequenceCommonRatio;
	u8  maxReDeliveryAttepts;
}zcl_wwah_enableAppEvtRetryAlgorithmCmd_t;

typedef struct{
	u16 fastRejoinTimeoutInSeconds;
	u16 durationBetweenEachRejoinInSeconds;
	u16 fastRejoinFirstBackoffInSeconds;
	u16 maxBackoffTimeInSeconds;
	u16 maxBackoffIterations;
}zcl_wwah_enableRejoinAlgorithmCmd_t;

typedef enum{
	TRIP_TO_PAIR,
	AUTO_ENROLL_RSP,
	AUTO_ENROLL_REQ,
}enrollmentMode_t;

typedef struct{
	enrollmentMode_t enrollmentMode;
}zcl_wwah_setIasZoneEnrollmentMethodCmd_t;

typedef struct{
	u16 checkInInterval;
}zcl_wwah_enablePeriodicRouterCheckInsCmd_t;

typedef struct{
	u8 waitTime;
}zcl_wwah_setMacPollFailureWaitTimeCmd_t;

typedef struct{
	u16 panID;
	u8	channel;
}zcl_wwah_setPendingNwkUpdateCmd_t;

typedef struct{
	u8	debugReportID;
}zcl_wwah_debugReportQueryCmd_t;

typedef struct{
	bool standardBeacons;
}zcl_wwah_surveyBeaconsCmd_t;

typedef struct{
	u16 *pClusterID;
	u8	numOfClusters;
}zcl_wwah_useTrustCenterForClusterCmd_t;

typedef struct{
	u16  clusterID;
	bool status;
}zcl_wwah_apsLinkKeyAuthQueryRspCmd_t;

typedef enum{
	UNKNOWN,
	BATTERY,
	BROWNOUT,
	WATCHDOG,
	RESET_PIN,
	MEMORY_OR_HW_FAULT,
	SOFTWARE_EXCEPTION,
	OTA_OR_BOOTLOAD_SUCCESS,
	SOFTWARE_RESET,
	POWER_BUTTON,
	TEMPERATURE,
	BOOTLOAD_FAILURE,
}powerNotiReason_t;

typedef struct{
	u8	*pManuPowerNotiReason;
	u16 manuID;
	powerNotiReason_t powerNotiReason;
	u8	manuPowerNotiReasonLen;
}zcl_wwah_poweringNotificationCmd_t;

typedef struct{
	addrExt_t deviceEUI64;
	u16 deviceShort;
}zcl_wwah_shortAddrChangeCmd_t;

typedef struct{
	u8 currentPowerMode:4;
	u8 availablePowerSrcs:4;
	u8 currentPowerSrc:4;
	u8 currentPowerSrcLevel:4;
}zcl_wwah_powerDescChangeCmd_t;

typedef struct{
	u32 sizeOfReport;
	u8  dbgReportID;
}zcl_wwah_newDbgReportNotiCmd_t;

typedef struct{
	u8 *pReportData;
	u8 dbgReportID;
	u8 reportDataLen;
}zcl_wwah_dbgReportQueryRspCmd_t;

typedef struct{
	u16 *pClusterID;
	u8  numOfClusters;
}zcl_wwah_tcForClusterServerQueryRspCmd_t;

typedef struct{
	u16 deviceShort;
	s8	rssi;
	u8	classificationMask;
}beaconSurvey_t;

typedef struct{
	beaconSurvey_t *pBeacon;
	u8  numOfBeacons;
}zcl_wwah_surveyBeaconsRspCmd_t;

#if 0
/**
 *  @brief  External variable for WWAH Attribute
 */
extern bool zcl_attr_wwah_disableOTADowngrades;
extern bool zcl_attr_wwah_mgmtLeaveWithoutRejoinEnabled;
extern u8 zcl_attr_wwah_nwkRetryCount;
extern u8 zcl_attr_wwah_macRetryCount;
extern bool zcl_attr_wwah_routerCheckInEnabled;
extern bool zcl_attr_wwah_touchlinkInterpanEnabled;
extern bool zcl_attr_wwah_parentClassificationEnabled;
extern bool zcl_attr_wwah_appEvtRetryEnabled;
extern u8 zcl_attr_wwah_appEvtRetryQueueSize;
extern bool zcl_attr_wwah_rejoinEnabled;
extern u8 zcl_attr_wwah_macPollFailureWaitTime;
extern bool zcl_attr_wwah_configurationModeEnabled;
extern u8 zcl_attr_wwah_currentDebugReportID;
extern bool zcl_attr_wwah_tcSecurityOnNwkKeyRotationEnabled;
extern bool zcl_attr_wwah_badParentRecoveryEnabled;
extern u8 zcl_attr_wwah_pendingNwkUpdateChannel;
extern u16 zcl_attr_wwah_pendingNwkUpdatePanID;
extern u16 zcl_attr_wwah_otaMaxOfflineDuration;

/**
 *  @brief  External variable for WWAH Attribute info table
 */
extern const zclAttrInfo_t wwah_attrTbl[];
extern const u8 zcl_wwah_attrNum;
#endif

status_t zcl_wwah_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_wwah_enableApsLinkKeyAuthCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd);
#define zcl_wwah_enableApsLinkKeyAuthCmdSend(a,b,c,d)	(zcl_wwah_enableApsLinkKeyAuthCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disableApsLinkKeyAuthCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd);
#define zcl_wwah_disableApsLinkKeyAuthCmdSend(a,b,c,d)	(zcl_wwah_disableApsLinkKeyAuthCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_apsLinkKeyAuthQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_apsLinkKeyAuthQueryCmd_t *pCmd);
#define zcl_wwah_apsLinkKeyAuthQueryCmdSend(a,b,c,d)	(zcl_wwah_apsLinkKeyAuthQueryCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_requestNewApsLinkKeyCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_requestNewApsLinkKeyCmdSend(a,b,c)	(zcl_wwah_requestNewApsLinkKeyCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableAppEvtRetryAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_enableAppEvtRetryAlgorithmCmd_t *pCmd);
#define zcl_wwah_enableAppEvtRetryAlgorithmCmdSend(a,b,c,d)	(zcl_wwah_enableAppEvtRetryAlgorithmCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disableAppEvtRetryAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableAppEvtRetryAlgorithmCmdSend(a,b,c)	(zcl_wwah_disableAppEvtRetryAlgorithmCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_requestTimeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_requestTimeCmdSend(a,b,c)	(zcl_wwah_requestTimeCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableRejoinAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_enableRejoinAlgorithmCmd_t *pCmd);
#define zcl_wwah_enableRejoinAlgorithmCmdSend(a,b,c,d)	(zcl_wwah_enableRejoinAlgorithmCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disableRejoinAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableRejoinAlgorithmCmdSend(a,b,c)	(zcl_wwah_disableRejoinAlgorithmCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_setIasZoneEnrollmentMethodCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_setIasZoneEnrollmentMethodCmd_t *pCmd);
#define zcl_wwah_setIasZoneEnrollmentMethodCmdSend(a,b,c,d)	(zcl_wwah_setIasZoneEnrollmentMethodCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_clearBindingTableCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_clearBindingTableCmdSend(a,b,c)	(zcl_wwah_clearBindingTableCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enablePeriodicRouterCheckInsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_enablePeriodicRouterCheckInsCmd_t *pCmd);
#define zcl_wwah_enablePeriodicRouterCheckInsCmdSend(a,b,c,d)	(zcl_wwah_enablePeriodicRouterCheckInsCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disablePeriodicRouterCheckInsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disablePeriodicRouterCheckInsCmdSend(a,b,c)	(zcl_wwah_disablePeriodicRouterCheckInsCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_setMacPollFailureWaitTimeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_setMacPollFailureWaitTimeCmd_t *pCmd);
#define zcl_wwah_setMacPollFailureWaitTimeCmdSend(a,b,c,d)	(zcl_wwah_setMacPollFailureWaitTimeCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_setPendingNwkUpdateCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_setPendingNwkUpdateCmd_t *pCmd);
#define zcl_wwah_setPendingNwkUpdateCmdSend(a,b,c,d)	(zcl_wwah_setPendingNwkUpdateCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_requireApsAcksOnUnicastsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd);
#define zcl_wwah_requireApsAcksOnUnicastsCmdSend(a,b,c,d)	(zcl_wwah_requireApsAcksOnUnicastsCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_removeApsAcksOnUnicastsRequirementCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_removeApsAcksOnUnicastsRequirementCmdSend(a,b,c)	(zcl_wwah_removeApsAcksOnUnicastsRequirementCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_apsAckRequirementQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_apsAckRequirementQueryCmdSend(a,b,c)	(zcl_wwah_apsAckRequirementQueryCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_debugReportQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_debugReportQueryCmd_t *pCmd);
#define zcl_wwah_debugReportQueryCmdSend(a,b,c,d)	(zcl_wwah_debugReportQueryCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_surveyBeaconsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_surveyBeaconsCmd_t *pCmd);
#define zcl_wwah_surveyBeaconsCmdSend(a,b,c,d)	(zcl_wwah_surveyBeaconsCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_disableOtaDowngradesCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableOtaDowngradesCmdSend(a,b,c)	(zcl_wwah_disableOtaDowngradesCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_disableMgmtLeaveWithoutRejoinCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableMgmtLeaveWithoutRejoinCmdSend(a,b,c)	(zcl_wwah_disableMgmtLeaveWithoutRejoinCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_disableTouchlinkInterpanMsgCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableTouchlinkInterpanMsgCmdSend(a,b,c)	(zcl_wwah_disableTouchlinkInterpanMsgCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableParentClassificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_enableParentClassificationCmdSend(a,b,c)	(zcl_wwah_enableParentClassificationCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_disableParentClassificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableParentClassificationCmdSend(a,b,c)	(zcl_wwah_disableParentClassificationCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableTcSecurityOnNwkKeyRotationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_enableTcSecurityOnNwkKeyRotationCmdSend(a,b,c)	(zcl_wwah_enableTcSecurityOnNwkKeyRotationCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableBadParentRecoveryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_enableBadParentRecoveryCmdSend(a,b,c)	(zcl_wwah_enableBadParentRecoveryCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_disableBadParentRecoveryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableBadParentRecoveryCmdSend(a,b,c)	(zcl_wwah_disableBadParentRecoveryCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_enableConfigurationModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_enableConfigurationModeCmdSend(a,b,c)	(zcl_wwah_enableConfigurationModeCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_disableConfigurationModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_disableConfigurationModeCmdSend(a,b,c)	(zcl_wwah_disableConfigurationModeCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_useTrustCenterForClusterCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_useTrustCenterForClusterCmd_t *pCmd);
#define zcl_wwah_useTrustCenterForClusterCmdSend(a,b,c,d)	(zcl_wwah_useTrustCenterForClusterCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_trustCenterForClusterServerQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_wwah_trustCenterForClusterServerQueryCmdSend(a,b,c)	(zcl_wwah_trustCenterForClusterServerQueryCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_wwah_apsLinkKeyAuthQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_apsLinkKeyAuthQueryRspCmd_t *pCmd);
#define zcl_wwah_apsLinkKeyAuthQueryRspCmdSend(a,b,c,d)	(zcl_wwah_apsLinkKeyAuthQueryRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_poweringOffNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_poweringNotificationCmd_t *pCmd);
#define zcl_wwah_poweringOffNotificationCmdSend(a,b,c,d)	(zcl_wwah_poweringOffNotificationCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_poweringOnNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_poweringNotificationCmd_t *pCmd);
#define zcl_wwah_poweringOnNotificationCmdSend(a,b,c,d)	(zcl_wwah_poweringOnNotificationCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_shortAddrChangeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_shortAddrChangeCmd_t *pCmd);
#define zcl_wwah_shortAddrChangeCmdSend(a,b,c,d)	(zcl_wwah_shortAddrChangeCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_apsAckRequirementQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd);
#define zcl_wwah_apsAckRequirementQueryRspCmdSend(a,b,c,d)	(zcl_wwah_apsAckRequirementQueryRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_powerDescriptorChangeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_powerDescChangeCmd_t *pCmd);
#define zcl_wwah_powerDescriptorChangeCmdSend(a,b,c,d)	(zcl_wwah_powerDescriptorChangeCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_newDebugReportNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_newDbgReportNotiCmd_t *pCmd);
#define zcl_wwah_newDebugReportNotificationCmdSend(a,b,c,d)	(zcl_wwah_newDebugReportNotificationCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_debugReportQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_dbgReportQueryRspCmd_t *pCmd);
#define zcl_wwah_debugReportQueryRspCmdSend(a,b,c,d)	(zcl_wwah_debugReportQueryRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_trustCenterForClusterServerQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_tcForClusterServerQueryRspCmd_t *pCmd);
#define zcl_wwah_trustCenterForClusterServerQueryRspCmdSend(a,b,c,d)	(zcl_wwah_trustCenterForClusterServerQueryRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_wwah_surveyBeaconsRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_surveyBeaconsRspCmd_t *pCmd);
#define zcl_wwah_surveyBeaconsRspCmdSend(a,b,c,d)	(zcl_wwah_surveyBeaconsRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))
