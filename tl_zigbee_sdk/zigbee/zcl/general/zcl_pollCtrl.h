/********************************************************************************************************
 * @file     zcl_pollCtrl.h
 *
 * @brief	 APIs statement for pollCtrl cluster
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
 *  @brief	zcl_pollCtrl_cmdId POLL CONTROL Command Ids
 */
// Client generated command
#define ZCL_CMD_CHK_IN_RSP                              0x00
#define ZCL_CMD_FAST_POLL_STOP							0x01
#define ZCL_CMD_SET_LONG_POLL_INTERVAL					0x02
#define ZCL_CMD_SET_SHORT_POLL_INTERVAL					0x03

// Server generated command
#define ZCL_CMD_CHK_IN                        			0x00


/**
 *  @brief	zcl_pollCtrl_attrId POLL CONTROL Attribute Ids
 */
#define ZCL_ATTRID_CHK_IN_INTERVAL						0x0000
#define ZCL_ATTRID_LONG_POLL_INTERVAL					0x0001
#define ZCL_ATTRID_SHORT_POLL_INTERVAL					0x0002
#define ZCL_ATTRID_FAST_POLL_TIMEOUT					0x0003
#define ZCL_ATTRID_CHK_IN_INTERVAL_MIN					0x0004
#define ZCL_ATTRID_LONG_POLL_INTERVAL_MIN				0x0005
#define ZCL_ATTRID_FAST_POLL_TIMEOUT_MAX				0x0006

/**
 *  @brief  Check-in response payload
 */
typedef struct{
	bool startFastPolling;
	u16	 fastPollTimeout;
}zcl_chkInRsp_t;

/**
 *  @brief  Set long poll interval command payload
 */
typedef struct{
	u32	 newLongPollInterval;
}zcl_setLongPollInterval_t;

/**
 *  @brief  Set short poll interval command payload
 */
typedef struct{
	u16	 newShortPollInterval;
}zcl_setShortPollInterval_t;




#if 0
/**
 *  @brief  Definition for pollCtrl command callback function type
 */
typedef status_t (*zcl_pollCtrl_chkInRspCb_t)(apsdeDataInd_t *pApsdeInd, zcl_chkInRsp_t *pCmd);
typedef status_t (*zcl_pollCtrl_fastPollStopCb_t)(apsdeDataInd_t *pApsdeInd);
typedef status_t (*zcl_pollCtrl_setLongPollIntervalCb_t)(apsdeDataInd_t *pApsdeInd, zcl_setLongPollInterval_t *pCmd);
typedef status_t (*zcl_pollCtrl_setShortPollIntervalCb_t)(apsdeDataInd_t *pApsdeInd, zcl_setShortPollInterval_t *pCmd);
typedef status_t (*zcl_pollCtrl_chkInCb_t)(apsdeDataInd_t *pApsdeInd, u8 seqNum);

/**
 *  @brief  Structure definition for commands callback functions in pollCtrl cluster
 */
typedef struct {
	zcl_pollCtrl_chkInRspCb_t					pollCtrlChkInRspCbFunc;
	zcl_pollCtrl_fastPollStopCb_t				pollCtrlFastPollStopCbFunc;
	zcl_pollCtrl_setLongPollIntervalCb_t		pollCtrlSetLongPollIntervalCbFunc;
	zcl_pollCtrl_setShortPollIntervalCb_t		pollCtrlSetShortPollIntervalCbFunc;
	zcl_pollCtrl_chkInCb_t                     	pollCtrlChkInCbFunc;
}zcl_pollCtrl_AppCallbacks_t;


/**
 *  @brief  Structure definition for poll control attribute
 */
typedef struct{
	u32	chkInInterval;
	u32	longPollInterval;
	u32	chkInIntervalMin;
	u32	longPollIntervalMin;
	u16	shortPollInterval;
	u16	fastPollTimeout;
	u16	fastPollTimeoutMax;
}zcl_pollCtrl_attr_t;

/**
 *  @brief  External variable for pollCtrl Attribute
 */
extern zcl_pollCtrl_attr_t gZclPollCtrlAttr;

/**
 *  @brief  External variable for pollCtrl Attribute info table
 */
extern const zclAttrInfo_t pollCtrl_attrTbl[];

extern const u8 zcl_pollCtrl_attrNum;
#endif


status_t zcl_pollCtrl_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_pollCtrl_chkIn(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_pollCtrl_checkInCmd(a,b,c)	(zcl_pollCtrl_chkIn((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_pollCtrl_chkInRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_chkInRsp_t *pCheckInRsp);
#define zcl_pollCtrl_chkInRspCmd(a,b,c,d)	(zcl_pollCtrl_chkInRsp((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_pollCtrl_fastPollStop(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_pollCtrl_fastPollStopCmd(a,b,c)	(zcl_pollCtrl_fastPollStop((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_pollCtrl_setLongPollInterval(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_setLongPollInterval_t *pSetLongPollInterval);
#define zcl_pollCtrl_setLongPollIntervalCmd(a,b,c,d)	(zcl_pollCtrl_setLongPollInterval((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_pollCtrl_setShortPollInterval(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_setShortPollInterval_t *pSetShortPollInterval);
#define zcl_pollCtrl_setShortPollIntervalCmd(a,b,c,d)	(zcl_pollCtrl_setShortPollInterval((a), (b), (c), ZCL_SEQ_NUM, (d)))

