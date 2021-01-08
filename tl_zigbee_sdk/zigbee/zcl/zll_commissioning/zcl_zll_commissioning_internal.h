/********************************************************************************************************
 * @file	zcl_zll_commissioning_internal.h
 *
 * @brief	This is the header file for zcl_zll_commissioning_internal
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
#ifndef ZCL_ZLL_COMMOSSIONING_INTERNAL_H
#define ZCL_ZLL_COMMOSSIONING_INTERNAL_H


/*
 * information used during touch link
 *
 * */

typedef struct{
	zcl_zllTouckLinkDisc_t 				*disc;
	zcl_zllTouckLinkNetworkStartParams	*networkStartInfo;
	ev_time_event_t 					*transIdLifeTimer;
	ev_time_event_t 					*runTimer;
	u32 				transId;
	u32 				respId;
	u32					scanChanMask;
	u8					touchLinkChan;
	zll_tlZbInfo_t  	zbInfo;
	zll_tlZllInfo_t 	zllInfo;
	u8 					seqNo;
	u8 					scanListNum;
	u8 					opIdx;
	zcl_DeviceInfo 		dstDevInfo;
	zcl_DeviceInfo 		devInfo;
	u8 					state;
	u8 					status;
	u8 					workingChannelBackUp;
	u8					lqiThreshold;
	u8					commissionTxPower;
	u8 					keyType;
	u8					vDoPrimaryScan:1;
	u8					IsFirstChannel:1;
	u8					startNetworkAllowed:1;
	u8					resved:5;
}zcl_zllTouckLink_t;


extern zcl_zllTouckLink_t g_zllTouchLink;

void ll_network_generate(u8 *nwkKey);

void ll_network_store(u8 *nwkKey);

void zcl_zllTouchLinkScanRequestHandler(epInfo_t *dstEp, u8 seqNo);

void zcl_zllTouchLinkScanResponseHandler(zcl_zllTouchLinkScanResp_t *resp, epInfo_t *dstEp);

void zcl_zllTouchLinkNetworkStartOrJoin(void *arg);

void zcl_zllTouchLinkResetFactoryReq(void *arg);

void zcl_zllTouchLinkNetworkStartRequstHandler(u8 logicChannel);

s32 zcl_zllTouchLinkNetworkStartResponseHandler(void *arg);

void zcl_zllTouchLinkNetworkStartDirectJoin(void *arg);

void zcl_zllTouchLinkNetworkJoinRequestHandler(u8 cmd);

s32 zcl_zllTouchLinkNetwokJoinResponseHandler(void *arg);

s32 zcl_zllTouchLinkTimeout(void *arg);

void zcl_zllTouchLinkFinish(u8 status);

bool zcl_zllTouchLinkLeaveCnfCb(nlme_leave_cnf_t *pLeaveCnf);

void zcl_zllTouchLinkDeviceInformationRequestHandler(epInfo_t *dstEp,u8 startEpIdx);

void zcl_zllTouchLinkDeviceInformationResponseHandler(zcl_zllTouchLinkDeviceInfoResp_t *devInfoResp);

s32 zcl_zllTouchLinkStartNetworkStartOrJoinTimerStop(void);

void ll_zllAttrClr(void);

#endif	/* ZCL_ZLL_COMMOSSIONING_INTERNAL_H */
