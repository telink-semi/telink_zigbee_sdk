/********************************************************************************************************
 * @file     zcl_zll_commissioning_internal.h
 *
 * @brief	 APIs for ZLL zigbee light link internal commissioning
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
#include "zcl_zll_commissioning.h"
#include "../../zdo/zb_af.h"

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

bool zcl_zllTouchLinkLeaveCnfCb(void *arg);

void zcl_zllTouchLinkDeviceInformationRequestHandler(epInfo_t *dstEp,u8 startEpIdx);

void zcl_zllTouchLinkDeviceInformationResponseHandler(zcl_zllTouchLinkDeviceInfoResp_t *devInfoResp);

s32 zcl_zllTouchLinkStartNetworkStartOrJoinTimerStop(void);

void ll_zllAttrClr(void);
