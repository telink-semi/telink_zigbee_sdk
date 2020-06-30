/********************************************************************************************************
 * @file     zcl_zllTouchLinkDiscovery.c
 *
 * @brief	 touch link discovery procedure
 *
 * @author
 * @date     June. 12, 2017
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


/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"
#include "zcl_touchlink_attr.h"
#include "zcl_zll_commissioning_internal.h"

#define ZB_PRIMARY_CHANNEL_1	11
#define ZB_PRIMARY_CHANNEL_2	15
#define ZB_PRIMARY_CHANNEL_3	20
#define ZB_PRIMARY_CHANNEL_4	25

_CODE_ZCL_  void zcl_zllTouchLinkDiscoveyStop(void);
_CODE_ZCL_  void zcl_zllTouchLinkDiscoveyStart(void);
extern zcl_zllCommission_t g_zllCommission;
extern bool scanReqProfileInterop;
u8 reset2FactoryFlag = 0;

s32 zcl_touchLinkIdentifyRequestDone(void *arg){
	u32 sta = (u32)arg;
	zcl_zllTouchLinkFinish(sta);
	return -1;
}

/*
 * @fn      zcl_zllTouchLinkIdentifyRequest
 *
 * @brief	send command of "identify request"
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static void zcl_zllTouchLinkIdentifyRequest(void *arg){
	zll_touchLinkScanInfo *peerInfo = &g_zllTouchLink.disc->scanList[g_zllTouchLink.opIdx];
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	memcpy((u8 *)&dstEp,(u8 *)&peerInfo->dstEp, sizeof(dstEp));

	zcl_zllTouchLinkIdentifyReq_t req;
	req.transId = g_zllTouchLink.transId;
	req.identifyDuration = 3;

	g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_IDENTIFY;
	zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_IDENTIFY, TRUE,
						ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, sizeof(zcl_zllTouchLinkIdentifyReq_t), (u8 *)&req);

	if(BDB_ATTR().nodeIsOnANetwork){
		if(ZB_EXTPANID_CMP(peerInfo->epanId, NWK_NIB().extPANId)){
			/* follow the bdb Spec8.7: Step 9*/
			if(peerInfo->nwkUpdateId < NWK_NIB().updateId){
				/* send network update command */
				zcl_zllTouchLinkNetworkUpdateReq_t networkUpdateCmd;
				networkUpdateCmd.transId = g_zllTouchLink.transId;
				ZB_EXTPANID_COPY(networkUpdateCmd.epanId, NWK_NIB().extPANId);
				networkUpdateCmd.nwkUpdateId = NWK_NIB().updateId;
				networkUpdateCmd.logicalChannel = MAC_IB().phyChannelCur;
				networkUpdateCmd.panId = NWK_NIB().panId;
				networkUpdateCmd.nwkAddr = NWK_NIB().nwkAddr;
				zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_NETWORK_UPDATE, TRUE,
										ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, sizeof(zcl_zllTouchLinkNetworkUpdateReq_t), (u8 *)&networkUpdateCmd);

			}

			if(peerInfo->nwkUpdateId > NWK_NIB().updateId){
				NWK_NIB().updateId = peerInfo->nwkUpdateId;
				zdo_nlmeChannelShift(peerInfo->logicalChannel);
			}
			/* channel restore */
			TL_ZB_TIMER_SCHEDULE(zcl_touchLinkIdentifyRequestDone, (void *)ZCL_ZLL_TOUCH_LINK_STA_EXIST, 5000);
			return;
		}

		if(!ss_securityModeIsDistributed()){
			zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_NOT_PERMITTED);
			return;
		}
	}

	/* Is initiator	address assignment capable?  Set bdbCommissioningStatus
		to NOT_AA_CAPABLE*/
	if(g_zllTouchLink.zllInfo.bf.addrAssign == 0){
		zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_NO_CAPACITY);
		return;
	}

	TL_SCHEDULE_TASK(zcl_zllTouchLinkNetworkStartOrJoin, NULL);
}


/*
 * @fn      zcl_zllDeviceInformationRequest
 *
 * @brief	send command of "Device Information request"
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkDeviceInformationRequest(void *arg){
	zll_touchLinkScanInfo *peerInfo = &g_zllTouchLink.disc->scanList[g_zllTouchLink.opIdx];
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	memcpy((u8 *)&dstEp,(u8 *)&peerInfo->dstEp, sizeof(dstEp));

	/* send Device Information request */
	zcl_zllTouchLinkDeviceInfoReq_t req;
	req.transId = g_zllTouchLink.transId;
	req.startIdx = (u8)((u32)arg);
	zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_DEVICE_INFORMATION, TRUE,
						ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, sizeof(zcl_zllTouchLinkDeviceInfoReq_t), (u8 *)&req);
	g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DEVICE_INFO_EXCHANGE;

}


/*
 * @fn      zcl_zllDeviceInformationRequestHandler
 *
 * @brief	the handler for receiving the command of "Device Information request"
 *
 * @param 	arg
 *
 */

_CODE_ZCL_ void zcl_zllTouchLinkDeviceInformationRequestHandler(epInfo_t *dstEp,u8 startEpIdx){
	/*
	 * send Device Information Response here
	 * */
	af_endpoint_descriptor_t *aed = af_epDescriptorGet();
	u8 MatchEpnumOnce = 0;
	u8 totalMatchEpnum = 0;
	u8 matchEpIdx[MAX_ACTIVE_EP_NUMBER] = {0};
	for(u8 i = 0; i < af_availableEpNumGet(); i++){
		if(af_clsuterIdMatched(ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, aed[i].correspond_simple_desc)){
			matchEpIdx[totalMatchEpnum++] = i;
		}
	}

	if(totalMatchEpnum > startEpIdx){
		MatchEpnumOnce = totalMatchEpnum - startEpIdx;
		MatchEpnumOnce = (MatchEpnumOnce >= 5) ? 5 : MatchEpnumOnce;
	}

	u8 len = MatchEpnumOnce * sizeof(zcl_zllDeviceInfoRec_t) + sizeof(zcl_zllTouchLinkDeviceInfoResp_t);
	zcl_zllTouchLinkDeviceInfoResp_t *resp = (zcl_zllTouchLinkDeviceInfoResp_t *)ev_buf_allocate(len);
	if(resp){
		memset(resp, 0, len);
		resp->numOfSubdevices = totalMatchEpnum;
		resp->transId = g_zllTouchLink.transId;
		resp->deviceInfoRecordCnt = MatchEpnumOnce;
		resp->startIdx = startEpIdx;
		if(MatchEpnumOnce > 0)
		{
			zcl_zllDeviceInfoRec_t *rec = resp->rec;
			for(u8 i = 0; i < MatchEpnumOnce; i++){
				u8 epIdx = matchEpIdx[startEpIdx + i];
				rec->deviceId = aed[epIdx].correspond_simple_desc->app_dev_id;
				rec->epId = aed[epIdx].ep;
				rec->groupIdCnt = 0;
				if(scanReqProfileInterop){
					rec->profileId = aed[epIdx].correspond_simple_desc->app_profile_id;
				}else{
					rec->profileId = LL_PROFILE_ID;
				}
				rec->sort = 0;
				rec->version = aed[epIdx].correspond_simple_desc->app_dev_ver;
				memcpy(rec->ieeeAddr, MAC_IB().extAddress, 8);
				rec++;
			}
		}

		zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_DEVICE_INFORMATION_RSP, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, TRUE, 0, g_zllTouchLink.seqNo++, len, (u8 *)resp);
		ev_buf_free((u8 *)resp);
	}
}


/*
 * @fn      zcl_zllDeviceInformationResponseHandler
 *
 * @brief	the handler for receiving the command of "Device Information Response"
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkDeviceInformationResponseHandler(zcl_zllTouchLinkDeviceInfoResp_t *devInfoResp){

	if(devInfoResp->startIdx + devInfoResp->deviceInfoRecordCnt >= devInfoResp->numOfSubdevices)
	{
		/* send identify request */
		TL_SCHEDULE_TASK(zcl_zllTouchLinkIdentifyRequest, NULL);
	}
	else
	{
		u32 nextIndex = devInfoResp->startIdx + devInfoResp->deviceInfoRecordCnt;
		TL_SCHEDULE_TASK(zcl_zllTouchLinkDeviceInformationRequest, (void*)nextIndex);
	}
	return;
}



/*
 * @fn      zcl_zllTouchLinkScanRequestProc
 *
 * @brief	send touch link scan response command once receive scan request
 *
 * @param 	NULL
 *
 */
_CODE_ZCL_  void zcl_zllTouchLinkScanRequestHandler(epInfo_t *srcEp, u8 seqNo){

#if (ZB_ED_ROLE)
	/* if it's under rejoin mode, exit */
	if(zb_isUnderRejoinMode()){
		return;
	}
	zb_setPollRate(0);
#endif
	/*
	 *  maybe the initiator receive the scan request, if it's a factory new device, firstly stop the scan operation
	 *
	 *  */
	zcl_zllTouchLinkDiscoveyStop();

	zcl_zllTouchLinkScanResp_t resp;
	TL_SETSTRUCTCONTENT(resp,0);
	u8 scanRespLen = sizeof(zcl_zllTouchLinkScanResp_t) - sizeof(zcl_zllSubdeviceInfo_t);

	resp.transId = g_zllTouchLink.transId;
	resp.rssiCorrection = 0;					//This value should  be pre-configured

	resp.zbInfo.bf.logicDevType = af_nodeDevTypeGet();
	resp.zbInfo.bf.rxOnWihleIdle = (af_nodeMacCapabilityGet() & MAC_CAP_RX_ON_WHEN_IDLE) ? 1 : 0;

	if(is_device_factory_new())
	{
		g_zllTouchLink.zllInfo.bf.factoryNew = 1;	//the bf is initiated in function of touchlinkinit(),maybe changed later,should be updated here.
	}
	else{
		g_zllTouchLink.zllInfo.bf.factoryNew = 0;
	}
	g_zllTouchLink.zllInfo.bf.profileInterop = 1;
	memcpy(&resp.zllInfo, &g_zllTouchLink.zllInfo, 1);

	if(!is_device_factory_new()){
		memcpy(resp.epanId, NWK_NIB().extPANId, sizeof(addrExt_t));// NIB/AIB
		resp.nwkUpdateId = NWK_NIB().updateId;  //if factory new , it is 0x00,  else nwkUpdateId attribute of originator , only have my update_id, not originators ???
		resp.panId = MAC_IB().panId;
		resp.nwkAddr = NIB_NETWORK_ADDRESS();
	}else{ //is_device_factory_new()
		resp.nwkUpdateId = 0;
		memset(resp.epanId, 0, 8);
		if(MAC_IB().panId == MAC_INVALID_PANID){
			MAC_IB().panId = ZB_RANDOM();
		}
		resp.panId = MAC_IB().panId;
	}

	resp.keyBitmask = 0;
#ifdef ZB_SECURITY
	resp.keyBitmask |= (1 << g_zllTouchLink.keyType);
#endif

	resp.respId = g_zllTouchLink.respId;
	resp.logicalChannel = MAC_IB().phyChannelCur;
	resp.totalGroupIds = 0;

	af_endpoint_descriptor_t *aed = af_epDescriptorGet();

	for(u8 i = 0; i < af_availableEpNumGet(); i++){
		if(af_clsuterIdMatched(ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, aed[i].correspond_simple_desc)){
			resp.numOfSubdevices++;
		}
	}

	/**TODO
	 *  only present if no of sub devices is 1.
	 *   Now only one end point is supported on a device, don't add this if multiple End Points are present in this device   */
	if(1 == resp.numOfSubdevices){
		for(u8 i = 0; i < af_availableEpNumGet(); i++){
			if(af_clsuterIdMatched(ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, aed[i].correspond_simple_desc)){
				resp.subDevInfo.epId = aed[i].ep;
				if(scanReqProfileInterop){
					resp.subDevInfo.profileId = aed[i].correspond_simple_desc->app_profile_id;
				}else{
					resp.subDevInfo.profileId = LL_PROFILE_ID;
				}
				resp.subDevInfo.deviceId = aed[i].correspond_simple_desc->app_dev_id;
				resp.subDevInfo.version = aed[i].correspond_simple_desc->app_dev_ver;
				resp.subDevInfo.groupIdCnt  = 0; //TODO - find group identifier count needed  for this end device.
				scanRespLen +=  sizeof(zcl_zllSubdeviceInfo_t);

				break;
			}
		}
	}

	MAC_IB().rxOnWhenIdle = 1;
	g_zllTouchLink.touchLinkChan = MAC_IB().phyChannelCur;

	/* Send Response to Originator ,should be Unicast */
	zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, srcEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_SCAN_RSP, TRUE,
						ZCL_FRAME_SERVER_CLIENT_DIR, TRUE, 0, seqNo, scanRespLen, (u8 *)&resp);

	/* start a timer which is used during the whole touch link */
	if(g_zllTouchLink.transIdLifeTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.transIdLifeTimer);
	}
	g_zllTouchLink.transIdLifeTimer = TL_ZB_TIMER_SCHEDULE(zcl_zllTouchLinkTimeout, NULL, ZB_INTER_PAN_TRANS_ID_LIFETIME);

	/* bdb set as BDB_STATE_COMMISSIONING_TOUCHLINK status */
	if(g_zllCommission.appCb->touchLinkCallback){
		g_zllCommission.appCb->touchLinkCallback(ZCL_ZLL_TOUCH_LINK_STA_TARGET_START, NULL);
	}
}



/*
 * @fn      zcl_zllTouchLinkScanResponseProc
 *
 * @brief	add scan result to scan list
 *
 * @param 	resp the target information from scan response command
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkScanResponseHandler(zcl_zllTouchLinkScanResp_t *resp, epInfo_t *dstEp){
	zcl_zllTouchLinkScanResp_t *p = resp;

	u8 idx = g_zllTouchLink.disc->targetNum;
	if(idx >= g_zllTouchLink.scanListNum - 1){
		return;// ZCL_STA_INVALID_VALUE;
	}

#ifdef ZB_SECURITY
	u16 matched_algorithms = 0;
	if((matched_algorithms = p->keyBitmask & (1 << g_zllTouchLink.keyType)) == 0){
		return;// ZCL_STA_INVALID_VALUE;
	}
#endif

	zll_touchLinkScanInfo *scanInfo =  &g_zllTouchLink.disc->scanList[idx];

	/* check if the response by the same node with different respId */
	zll_touchLinkScanInfo *list = &g_zllTouchLink.disc->scanList[0];
	u8 i = 0;
	for(i = 0; i < idx; i++){
		if(ZB_64BIT_ADDR_CMP(list->dstEp.dstAddr.extAddr, dstEp->dstAddr.extAddr)){
			if(p->respId != list->respId){
				scanInfo = list;
				break;
			}
		}
		list++;
	}

	if(i >= idx){
		g_zllTouchLink.disc->targetNum++;
	}

	ZB_EXTPANID_COPY(scanInfo->epanId, p->epanId);
	scanInfo->panId = p->panId;
	scanInfo->nwkAddr = p->nwkAddr;
	scanInfo->zbInfo.byte = p->zbInfo.byte;
	scanInfo->zllInfo.byte = p->zllInfo.byte;
	scanInfo->logicalChannel = p->logicalChannel;
	scanInfo->numOfSubdevices = p->numOfSubdevices;
	scanInfo->totalGroupIds = p->totalGroupIds;
	scanInfo->respId = p->respId;
	scanInfo->nwkUpdateId = p->nwkUpdateId;
	memcpy((u8 *)&scanInfo->dstEp, (u8 *)dstEp, sizeof(epInfo_t));
	if(scanInfo->numOfSubdevices == 1){
		memcpy(&scanInfo->devInfo, &p->subDevInfo, sizeof(zcl_zllSubdeviceInfo_t));
	}

#ifdef ZB_SECURITY
//	u8 i = 0;
	for(i = 0; i <= CERTIFICATION_KEY; i++){
		if((matched_algorithms & (1 << i)) && ((i == DEVELOPMENT_KEY) || (i == MASTER_KEY) || (i == CERTIFICATION_KEY))){
			scanInfo->keyIdx = i;
		}
	}
#endif
}


/*
 * @fn      zcl_zllTouchLinkDiscoverydone
 *
 * @brief	touch link scan finish
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static void zcl_zllTouchLinkDiscoverydone(void *arg){
	zcl_zllTouckLinkDisc_t *pDisc = g_zllTouchLink.disc;

	if(pDisc->targetNum){
		g_zllTouchLink.opIdx = 0;
		zll_touchLinkScanInfo *pScanInfo = &pDisc->scanList[g_zllTouchLink.opIdx];
		for(u32 i = 0; i < pDisc->targetNum; i++){
			if(pScanInfo->zllInfo.bf.priorityReq){
				g_zllTouchLink.opIdx = i;
				break;
			}
			pScanInfo++;
		}

		pScanInfo = &pDisc->scanList[g_zllTouchLink.opIdx];

		/* switch the channel to the target node 's */
		ZB_TRANSCEIVER_SET_CHANNEL(pScanInfo->logicalChannel);
		g_zllTouchLink.touchLinkChan = pScanInfo->logicalChannel;

		/**
		 *	If  number of sub devices is 1 , info of that sub device will be in this scan response itself
		 *	so we don't need to send Device Info Request to target device.  :)
		 *	Check the scan response and prioritize the device if asked for priority.
		 */
		g_zllTouchLink.respId = pScanInfo->respId;

		if(reset2FactoryFlag){
			zcl_zllTouchLinkResetFactoryReq(NULL);
			zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_SUCC);
			return;
		}

		if(pScanInfo->numOfSubdevices > 1)
		{
			u32 start_idx = 0;
			TL_SCHEDULE_TASK(zcl_zllTouchLinkDeviceInformationRequest, (void*)start_idx);
		}
		else
		{
			TL_SCHEDULE_TASK(zcl_zllTouchLinkIdentifyRequest, NULL);
		}
	}else{
		zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_NO_SERVER);
	}
}


/*
 * @fn      zcl_touchLinkScanStart
 *
 * @brief	touch link scan start
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static s32 zcl_touchLinkScanStart(void *arg){
	zcl_zllTouckLinkDisc_t *pDisc = g_zllTouchLink.disc;
	epInfo_t dstEpInfo;
	u32 i = 0;

	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = 0;
	dstEpInfo.dstAddr.shortAddr = MAC_SHORT_ADDR_BROADCAST;
	dstEpInfo.profileId = LL_PROFILE_ID;
	dstEpInfo.txOptions = 0;
	dstEpInfo.radius    = 0;

	if(g_zllTouchLink.IsFirstChannel){
		pDisc->scanCnt = 5;
	}
	if(g_zllTouchLink.IsFirstChannel || pDisc->scanCnt == 0){
		for(i = TL_ZB_MAC_CHANNEL_START; i < TL_ZB_MAC_CHANNEL_STOP+1; i++){
			if(pDisc->unscannedChannelMask & (1 << i)){
				pDisc->currentScannChannel = i;
				g_zllTouchLink.IsFirstChannel = 0;
				pDisc->unscannedChannelMask &= ~(1 << i);
				break;
			}
		}
	}
	if(pDisc->scanCnt > 0){
		pDisc->scanCnt--;
	}

	if(i == TL_ZB_MAC_CHANNEL_STOP+1){
		pDisc->primaryChannelScanComplete = 1;
	}

	if(!pDisc->primaryChannelScanComplete){
		/* switch channel to another one */
		ZB_TRANSCEIVER_SET_CHANNEL(pDisc->currentScannChannel);

		/* send scan request */
		zcl_zllTouchLinkScanReq_t scanReq;
		scanReq.transId = g_zllTouchLink.transId;
		scanReq.zbInfo.byte = g_zllTouchLink.zbInfo.byte;
		scanReq.zllInfo.byte = g_zllTouchLink.zllInfo.byte;
		zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId , &dstEpInfo, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_SCAN, TRUE, ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0,
							g_zllTouchLink.seqNo++, sizeof(zcl_zllTouchLinkScanReq_t), (u8 *)&scanReq);
	}else{
		if(g_zllTouchLink.vDoPrimaryScan){
			g_zllTouchLink.vDoPrimaryScan = 0;
			pDisc->unscannedChannelMask = BDB_ATTR().secondaryChannelSet;
			pDisc->primaryChannelScanComplete = 0;
			if(pDisc->unscannedChannelMask){
				return ZB_SCAN_TIME_BASE_DURATION;
			}
		}
		TL_SCHEDULE_TASK(zcl_zllTouchLinkDiscoverydone, NULL);
		g_zllTouchLink.runTimer = NULL;
		return -1;
	}

	return ZB_SCAN_TIME_BASE_DURATION;
}

/*
 * @fn      zcl_zllTouchLinkDiscoveyStart
 *
 * @brief	start a timer to process touch link discovery
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkDiscoveyStart(void){
	/* start a timer which is used during touch link discovery */
	if(g_zllTouchLink.runTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.runTimer);
	}
	g_zllTouchLink.runTimer = TL_ZB_TIMER_SCHEDULE(zcl_touchLinkScanStart, NULL, 5000);

	/* start a timer which is used during the whole touch link */
	if(g_zllTouchLink.transIdLifeTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.transIdLifeTimer);
	}
	g_zllTouchLink.transIdLifeTimer = TL_ZB_TIMER_SCHEDULE(zcl_zllTouchLinkTimeout, NULL, ZB_INTER_PAN_TRANS_ID_LIFETIME);
}


/*
 * @fn      zcl_zllTouchLinkDiscoveyStop
 *
 * @brief	stop the timer that is to process touch link discovery
 *
 * @param 	arg
 *
 */
_CODE_ZCL_  void zcl_zllTouchLinkDiscoveyStop(void){
	if(g_zllTouchLink.runTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.runTimer);
	}

	if(g_zllTouchLink.transIdLifeTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.transIdLifeTimer);
	}

	if(g_zllTouchLink.disc){
		ev_buf_free((u8 *)g_zllTouchLink.disc);
		g_zllTouchLink.disc = NULL;
	}
}

