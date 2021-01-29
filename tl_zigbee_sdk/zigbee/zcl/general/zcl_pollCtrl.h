/********************************************************************************************************
 * @file	zcl_pollCtrl.h
 *
 * @brief	This is the header file for zcl_pollCtrl
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef ZCL_POLL_CTRL_H
#define ZCL_POLL_CTRL_H



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


status_t zcl_pollCtrl_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

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

#endif	/* ZCL_POLL_CTRL_H */
