/********************************************************************************************************
 * @file    zcl_zll_commissioning_internal.h
 *
 * @brief   This is the header file for zcl_zll_commissioning_internal
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

#ifndef ZCL_ZLL_COMMOSSIONING_INTERNAL_H
#define ZCL_ZLL_COMMOSSIONING_INTERNAL_H


/*
 * information used during touch link
 *
 * */

typedef struct{
	zcl_zllTouckLinkDisc_t 				*disc;
	zcl_zllTouckLinkNetworkStartParams	*networkStartInfo;
	ev_timer_event_t 					*transIdLifeTimer;
	ev_timer_event_t 					*runTimer;
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

void zcl_zllTouchLinkScanResponseHandler(zcl_zllTouchLinkScanResp_t *resp, epInfo_t *dstEp, u8 lqi);

void zcl_zllTouchLinkNetworkStartOrJoin(void *arg);

void zcl_zllTouchLinkResetFactoryReq(void *arg);

void zcl_zllTouchLinkNetworkStartRequestHandler(u8 logicChannel);

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

void tl_zbNwkZllCommissionScanConfirm(void);

#endif	/* ZCL_ZLL_COMMOSSIONING_INTERNAL_H */
