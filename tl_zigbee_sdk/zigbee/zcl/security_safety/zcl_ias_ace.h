/********************************************************************************************************
 * @file     zcl_ias_ace.h
 *
 * @brief	 APIs statement for Ancillary Control Equipment of the IAS system.
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

/*********************************************************************
 * CONSTANTS
 */
/* Command ID */
// Client generated command
#define ZCL_CMD_ARM                                 0x00
#define ZCL_CMD_BYPASS                              0x01
#define ZCL_CMD_EMERGENCY                           0x02
#define ZCL_CMD_FIRE                                0x03
#define ZCL_CMD_PANIC                               0x04
#define ZCL_CMD_GET_ZONE_ID_MAP                     0x05
#define ZCL_CMD_GET_ZONE_INFORMATION                0x06
#define ZCL_CMD_GET_PANEL_STATUS                    0x07
#define ZCL_CMD_GET_BYPASSED_ZONE_LIST              0x08
#define ZCL_CMD_GET_ZONE_STATUS                     0x09

// Server generated command
#define ZCL_CMD_ARM_RESPONSE                        0x00
#define ZCL_CMD_GET_ZONE_ID_MAP_RESPONSE            0x01
#define ZCL_CMD_GET_ZONE_INFORMATION_RESPONSE       0x02
#define ZCL_CMD_ZONE_STATUS_CHANGED                 0x03
#define ZCL_CMD_PANEL_STATUS_CHANGED                0x04
#define ZCL_CMD_GET_PANEL_STATUS_RESPONSE           0x05
#define ZCL_CMD_SET_BYPASSED_ZONE_LIST              0x06
#define ZCL_CMD_BYPASS_RESPONSE                     0x07
#define ZCL_CMD_GET_ZONE_STATUS_RESPONSE            0x08


/*********************************************************************
 * TYPES
 */
/**
 *  @brief  Structure definition for arm command
 */
typedef struct {
    UTF8String_t code;
    u8 armMode;
    u8 zoneId;
} arm_t;

/**
 *  @brief  Structure definition for bypass command
 */
typedef struct {
	u8 *zoneIds;
    UTF8String_t code;
    u8 numOfZones;
} bypass_t;

/**
 *  @brief  Structure definition for get zone status command
 */
typedef struct {
	u16 zoneStatusMask;
	u8 startingZoneId;
    u8 maxNumOfZoneIds;
    bool zoneStatusMaskFlag;
} getZoneStatus_t;

/**
 *  @brief  Structure definition for get zone information response command
 */
typedef struct {
	UTF8String_t zoneLabel;
	u8  zoneId;
    u16 zoneType;
    extAddr_t ieeeAddr;
} getZoneInfoRsp_t;

/**
 *  @brief  Structure definition for zone status changed command
 */
typedef struct {
	UTF8String_t zoneLabel;
	u8  zoneId;
    u16 zoneStatus;
    u8	audibleNotification;
} zoneStatusChanged_t;

/**
 *  @brief  Structure definition for panel status changed command
 */
typedef struct {
	u8 panelStatus;
    u8 secondsRemaining;
    u8 audibleNotification;
    u8 alarmStatus;
} panelStatusChanged_t;

/**
 *  @brief  Structure definition for get panel status response command
 */
typedef struct {
	u8 panelStatus;
    u8 secondsRemaining;
    u8 audibleNotification;
    u8 alarmStatus;
} getPanelStatusRsp_t;

/**
 *  @brief  Structure definition for set bypassed zone list command
 */
typedef struct {
	u8 *zoneIds;
	u8 numOfZones;
} setBypassedZoneList_t;

/**
 *  @brief  Structure definition for bypass response command
 */
typedef struct {
	u8 *zoneIds;
	u8 numOfZones;
} bypassRsp_t;

/**
 *  @brief  Structure definition for get zone status response command
 */
typedef struct {
	u8 *zoneStatusInfo;
	u8 zoneStatusComplete;
    u8 numOfZones;
} getZoneStatusRsp_t;

#if 0
/* Command Callback Function Type */
typedef u8 (*zcl_armCb_t)(apsdeDataInd_t *pApsdeInd, arm_t *pArm);
typedef void (*zcl_bypassCb_t)(apsdeDataInd_t *pApsdeInd, bypass_t *pBypass);
typedef void (*zcl_emergencyCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_fireCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_panicCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_getZoneIdMapCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_getZoneInformationCb_t)(apsdeDataInd_t *pApsdeInd, u8 zoneId);
typedef void (*zcl_getPanelStatusCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_getBypassZoneListCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_getZoneStatusCb_t)(apsdeDataInd_t *pApsdeInd, getZoneStatus_t *pGetZoneStatus);
typedef void (*zcl_armRspCb_t)(apsdeDataInd_t *pApsdeInd, u8 armNotification);
typedef void (*zcl_getZoneIdMapRspCb_t)(apsdeDataInd_t *pApsdeInd, u8 *pZoneIdMapRsp);
typedef void (*zcl_getZoneInformationRspCb_t)(apsdeDataInd_t *pApsdeInd, getZoneInfoRsp_t *pGetZoneInfoRsp);
typedef void (*zcl_zoneStatusChangedCb_t)(apsdeDataInd_t *pApsdeInd, zoneStatusChanged_t *pZoneStatusChanged);
typedef void (*zcl_panelStatusChangedCb_t)(apsdeDataInd_t *pApsdeInd, panelStatusChanged_t *pPanelStatusChanged);
typedef void (*zcl_getPanelStatusRspCb_t)(apsdeDataInd_t *pApsdeInd, getPanelStatusRsp_t *pGetPanelStatusRsp);
typedef void (*zcl_setBypassedZoneListCb_t)(apsdeDataInd_t *pApsdeInd, setBypassedZoneList_t *pSetBypassedZoneList);
typedef void (*zcl_bypassRspCb_t)(apsdeDataInd_t *pApsdeInd, bypassRsp_t *pBypassRsp);
typedef void (*zcl_getZoneStatusRspCb_t)(apsdeDataInd_t *pApsdeInd, getZoneStatusRsp_t *pGetZoneStatusRsp);

/**
 *  @brief  Structure definition for commands callback functions in ias ace cluster
 */
typedef struct {
	zcl_armCb_t                    	armCbFunc;
    zcl_bypassCb_t                 	bypassCbFunc;
    zcl_emergencyCb_t              	emergencyCbFunc;
    zcl_fireCb_t                   	fireCbFunc;
    zcl_panicCb_t                  	panicCbFunc;
    zcl_getZoneIdMapCb_t			getZoneIdMapCbFunc;
    zcl_getZoneInformationCb_t		getZoneInformationCbFunc;
    zcl_getPanelStatusCb_t			getPanelStatusCbFunc;
    zcl_getBypassZoneListCb_t		getBypassZoneListCbFunc;
    zcl_getZoneStatusCb_t			getZoneStatusCbFunc;
    zcl_armRspCb_t					armRspCbFunc;
    zcl_getZoneIdMapRspCb_t			getZoneIdMapRspCbFunc;
    zcl_getZoneInformationRspCb_t	getZoneInformationRspCbFunc;
    zcl_zoneStatusChangedCb_t		zoneStatusChangedCbFunc;
    zcl_panelStatusChangedCb_t		panelStatusChangedCbFunc;
    zcl_getPanelStatusRspCb_t		getPanelStatusRspCbFunc;
    zcl_setBypassedZoneListCb_t		setBypassedZoneListCbFunc;
    zcl_bypassRspCb_t				bypassRspCbFunc;
    zcl_getZoneStatusRspCb_t		getZoneStatusRspCbFunc;
} zcl_iasAce_AppCallbacks_t;
#endif

status_t zcl_iasAce_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_iasAce_zoneStatusChanged(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zoneStatusChanged_t *pZoneStatusChanged);

status_t zcl_iasAce_panelStatusChanged(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, panelStatusChanged_t *pPanelStatusChanged);

status_t zcl_iasAce_getPanelStatusRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getPanelStatusRsp_t *pGetPanelStatusRsp);

status_t zcl_iasAce_setBypassedZoneList(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, setBypassedZoneList_t *pSetBypassedZoneList);

status_t zcl_iasAce_bypassResp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, bypassRsp_t *pBypassRsp);

status_t zcl_iasAce_getZoneStatusResp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getZoneStatusRsp_t *pGetZoneStatusRsp);

status_t zcl_iasAce_arm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, arm_t *pArm);
#define zcl_iasAce_armCmd(a,b,c,d)	(zcl_iasAce_arm((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_iasAce_bypass(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, bypass_t *pBypass);
#define zcl_iasAce_bypassCmd(a,b,c,d)	(zcl_iasAce_bypass((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_iasAce_emergency(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_iasAce_emergencyCmd(a,b,c)	(zcl_iasAce_emergency((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_iasAce_fire(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_iasAce_fireCmd(a,b,c)	(zcl_iasAce_fire((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_iasAce_panic(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_iasAce_panicCmd(a,b,c)	(zcl_iasAce_panic((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_iasAce_getZoneIdMap(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_iasAce_getZoneIdMapCmd(a,b,c)	(zcl_iasAce_getZoneIdMap((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_iasAce_getZoneInfo(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 ZoneId);
#define zcl_iasAce_getZoneInfoCmd(a,b,c,d)	(zcl_iasAce_getZoneInfo((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_iasAce_getPanelStatus(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_iasAce_getPanelStatusCmd(a,b,c)	(zcl_iasAce_getPanelStatus((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_iasAce_getBypassedZoneList(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_iasAce_getBypassedZoneListCmd(a,b,c)	(zcl_iasAce_getBypassedZoneList((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_iasAce_getZoneStatus(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getZoneStatus_t *pGetZoneStatus);
#define zcl_iasAce_getZoneStatusCmd(a,b,c,d)	(zcl_iasAce_getZoneStatus((a), (b), (c), ZCL_SEQ_NUM, (d)))
