/********************************************************************************************************
 * @file     zcl_zllTouchLinkJoinOrStart.c
 *
 * @brief	 procedure for touch link network start, join router/ED
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

_CODE_ZCL_ static void zcl_zllTouchLinNetworkStartRespCmdSend(void *arg);

/*
 * @fn      tl_zbNwkZllCommissionScanConfirm
 *
 * @brief	the callback for touch link active scan
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void tl_zbNwkZllCommissionScanConfirm(void *arg){
	u8 channel = 0xff;
	u8 panNumOnChannel[16] = {0};
	u8 nodesOnChannel = 0xff;

	u32 i = 0;
	u8  neighborNum = tl_zbAdditionNeighborTableNumGet();
	tl_zb_addition_neighbor_entry_t *pEntry = NULL;

	for(i = 0; i < neighborNum; i++){
		pEntry = tl_zbAdditionNeighborEntryGetFromIdx(i);
		if(pEntry && pEntry->logicChannel >= TL_ZB_MAC_CHANNEL_START && pEntry->logicChannel <= TL_ZB_MAC_CHANNEL_STOP){
			panNumOnChannel[pEntry->logicChannel-TL_ZB_MAC_CHANNEL_START]++;
		}
	}

	/* select channel */
	for(i = TL_ZB_MAC_CHANNEL_START ; i <= TL_ZB_MAC_CHANNEL_STOP ; i++){
		if((g_zllTouchLink.scanChanMask & (1 << i)) && panNumOnChannel[i-TL_ZB_MAC_CHANNEL_START] < nodesOnChannel){
			nodesOnChannel = panNumOnChannel[i-TL_ZB_MAC_CHANNEL_START];
			channel = i;
		}
	}

	/* select panId */
	u32 unique_pan_id = 0x00;
	u16 panId = ZB_RANDOM();
	while(!unique_pan_id){
		if(panId == MAC_INVALID_PANID){
			panId = ZB_RANDOM();
		}

		for(i = 0; i < neighborNum; i++){
			pEntry = tl_zbAdditionNeighborEntryGetFromIdx(i);
			if(pEntry->panId == panId){
				panId++;
				break;
			}
		}
		if(i == neighborNum){
			unique_pan_id = 1;
		}
	}

	extPANId_t extPanId;
	u16 random[4] = {0};
	for(u32 i = 0; i < 4; i++){
		random[i] = zb_random();
	}
	ZB_EXTPANID_COPY(extPanId, random);

	//channel = 0x0b;  //for DEBUG
	g_zllTouchLink.workingChannelBackUp = channel;
	tl_zbAdditionNeighborReset();

	/* use the old channel */
	ZB_TRANSCEIVER_SET_CHANNEL(g_zllTouchLink.touchLinkChan);

	if(g_bdbCtx.role == BDB_COMMISSIONING_ROLE_INITIATOR){
		NWK_NIB().panId = panId;
		MAC_IB().panId = panId;
		ZB_EXTPANID_COPY(NWK_NIB().extPANId, extPanId);
		g_zbMacCtx.curChannel = g_zllTouchLink.workingChannelBackUp; //channel;

		u16 nwkAddr = 0xffff;
		if(g_touchlinkAttr.freeNwkAddressRangeBegin == 0x0000){
			nwkAddr = tl_zbNwkStochasticAddrCal();
		}else{
			nwkAddr = g_touchlinkAttr.freeNwkAddressRangeBegin++;
		}
		MAC_IB().shortAddress = nwkAddr;
		NWK_NIB().nwkAddr = MAC_IB().shortAddress;

		/* network key generate */
		zb_routerStart();
	}else{
		/* network info backup */
		zcl_zllTouchLinkNetworkStartReq_t *startParams = (zcl_zllTouchLinkNetworkStartReq_t *)&g_zllTouchLink.networkStartInfo->params.networkStartCmd;
		if(ZB_EXTPANID_IS_ZERO(startParams->epanId)){
			memcpy(startParams->epanId, extPanId, 8);
		}
		if(startParams->panId == 0){
			startParams->panId = panId;
		}
		startParams->logicalChannel = g_zllTouchLink.workingChannelBackUp;//channel;

		if(g_zllTouchLink.transId != 0 && channel != 0xff){
			TL_SCHEDULE_TASK(zcl_zllTouchLinNetworkStartRespCmdSend, (void *)SUCCESS);
		}
	}
}

/* NWK-discovery */
static void touchlink_discovery_network(u32 chanMask){
	nlme_nwkDisc_req_t req;
	TL_SETSTRUCTCONTENT(req,0);
	if(0 == chanMask){
		req.scanChannels = BDB_ATTR().primaryChannelSet;
	}else{
		req.scanChannels = chanMask;
	}
	req.scanDuration = BDB_ATTR().scanDuration;

	g_zllTouchLink.scanChanMask = chanMask;
	zb_nwkDiscReq(&req, NLME_STATE_ZLL_COMMISSION);
}


/*
 * @fn      zcl_zllTouchLinkStartNetworkTimeout
 *
 * @brief	the callback of the timer for touch link start network
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static  s32 zcl_zllTouchLinkStartNetworkStartOrJoinTimeout(void *arg){
	g_zllTouchLink.runTimer = NULL;
	zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_STARTORJOIN_TIMEOUT);
	return -1;
}


/*
 * @fn      zcl_zllTouchLinkStartNetworkTimerStop
 *
 * @brief	stop the timer that is for touch link start network
 *
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ s32 zcl_zllTouchLinkStartNetworkStartOrJoinTimerStop(void){
	if(g_zllTouchLink.runTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.runTimer);
	}

	/* should cancel the transId life timer */
	if(g_zllTouchLink.transIdLifeTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.transIdLifeTimer);
	}

	return -1;
}



/*
 * @fn      zcl_zllTouchLinkNetworkStartDirectJoin
 *
 * @brief	directly join network
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkNetworkStartDirectJoin(void *arg){
	if(g_zllTouchLink.transId){
		g_zbNwkCtx.joined = 1;

#ifdef ZB_ROUTER_ROLE
		zcl_zllTouchLinkNetworkStartReq_t *pInitiator = &g_zllTouchLink.networkStartInfo->params.networkStartCmd;
#endif
		if((g_zllTouchLink.networkStartInfo->joinNetworkMode == ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_START)
			&& (g_zllTouchLink.networkStartInfo->params.networkStartCmd.initiatorNwkAddr != 0)){
#ifdef ZB_ROUTER_ROLE
			nlme_directJoin_req_t req;
			req.nwkAddr = pInitiator->initiatorNwkAddr;
			memcpy(req.deviceAddr, pInitiator->initiatorIeeeAddr , 8);
			req.capabilityInfo.devType = (g_zllTouchLink.zbInfo.bf.logicDevType & 0x02) ? 0 : 1;;
			req.capabilityInfo.rcvOnWhenIdle = g_zllTouchLink.zbInfo.bf.rxOnWihleIdle;

			if(zb_nwkDirectJoin(req) != RET_OK){
				if(g_zllTouchLink.state != ZCL_ZLL_COMMISSION_STATE_IDLE){
					zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_FAIL);
				}
				return ;
			}
#endif
		}
		zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_SUCC);
		return ;
	}else{
		if(g_zllTouchLink.state != ZCL_ZLL_COMMISSION_STATE_IDLE){
			zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_FAIL);
		}
		return ;
	}
}

/*
 * @fn      zcl_zllTouchLinkNetworkStartNwkLeave
 *
 * @brief	send network leave command if the device isn't a factory new device
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static void zcl_zllTouchLinkStartRouteRequest(void){
	zcl_zllTouchLinkNetworkStartReq_t *startParams = &g_zllTouchLink.networkStartInfo->params.networkStartCmd;


	/* store the network key */
	ss_nwkKeyStore(startParams->encryptedNwkKey);

	/* Update free network and group address range values */
	g_touchlinkAttr.freeNwkAddressRangeBegin	= startParams->freeNwkAddrRangeBegin;
	g_touchlinkAttr.freeNwkAddressRangeEnd	= startParams->freeNwkAddrRangeEnd;
	g_touchlinkAttr.freeGroupAddressRangeBegin = startParams->freeGroupIdRangeBegin;
	g_touchlinkAttr.freeGroupAddressRangeEnd	= startParams->freeGroupIdRangeEnd;

	MAC_IB().phyChannelCur = startParams->logicalChannel;
	ZB_TRANSCEIVER_SET_CHANNEL(MAC_IB().phyChannelCur);

	NIB_NETWORK_ADDRESS() = startParams->nwkAddr;
	ZB_PIB_SHORT_ADDRESS() = NIB_NETWORK_ADDRESS();
	NWK_NIB().panId = startParams->panId;
	NWK_NIB().nwkAddr = NIB_NETWORK_ADDRESS();
	MAC_IB().panId = startParams->panId;
	ZB_EXTPANID_COPY(NWK_NIB().extPANId, startParams->epanId);
	ZB_EXTPANID_COPY(aps_ib.aps_use_ext_panid, NWK_NIB().extPANId);

	/* router start */
	zb_routerStart();
}

/*
 * @fn      zcl_zllTouchLinkNetworkJoinRealjoin
 *
 * @brief	join network
 *
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static void zcl_zllTouchLinkNetworkJoinRealjoin(void){
	/* Fill NIB */
	zcl_zllTouchLinkNetworkJoinReq_t *pReq = &g_zllTouchLink.networkStartInfo->params.networkJoinCmd;

	/* store the network key */
	ss_nwkKeyStore(pReq->encryptedNwkKey);

	/* Update logical channel */
	MAC_IB().phyChannelCur = pReq->logicalChannel;
	ZB_TRANSCEIVER_SET_CHANNEL(MAC_IB().phyChannelCur);
	g_zllTouchLink.workingChannelBackUp = pReq->logicalChannel;

	/* Update extended PAN ID */
	ZB_EXTPANID_COPY(NWK_NIB().extPANId, pReq->epanId);
	ZB_EXTPANID_COPY(aps_ib.aps_use_ext_panid, NWK_NIB().extPANId);

	/* Update PAN identifier */
	MAC_IB().panId = pReq->panId;

	/* Update network address */
	NIB_NETWORK_ADDRESS() = pReq->nwkAddr;
	ZB_PIB_SHORT_ADDRESS() = NIB_NETWORK_ADDRESS();

	NWK_NIB().panId = MAC_IB().panId;
	NWK_NIB().nwkAddr = NIB_NETWORK_ADDRESS();

	/* Update NWK Update ID */
	NWK_NIB().updateId = pReq->nwkUpdateId;
	/* Update free network and group address range values */
	g_touchlinkAttr.freeNwkAddressRangeBegin = pReq->freeNwkAddrRangeBegin;
	g_touchlinkAttr.freeNwkAddressRangeEnd = pReq->freeNwkAddrRangeEnd;
	g_touchlinkAttr.freeGroupAddressRangeBegin = pReq->freeGroupIdRangeBegin;
	g_touchlinkAttr.freeGroupAddressRangeEnd = pReq->freeGroupIdRangeEnd;

	if(af_nodeDevTypeGet() == DEVICE_TYPE_END_DEVICE){
		aps_ib.aps_authenticated = 1;
		MAC_IB().rxOnWhenIdle = g_zllTouchLink.zbInfo.bf.rxOnWihleIdle;
		zb_rejoin_mode_set(REJOIN_SECURITY);
		zdo_nwk_rejoin_req(NLME_REJOIN_METHOD_REJOIN, (1 << MAC_IB().phyChannelCur));
	}else if(af_nodeDevTypeGet() == DEVICE_TYPE_ROUTER){
		/* router start */
		zb_routerStart();
	}
}

_CODE_ZCL_ bool zcl_zllTouchLinkLeaveCnfCb(void *arg){
	//nlme_leave_cnf_t *pCnf = (nlme_leave_cnf_t *)arg;

	if(!g_zllTouchLink.networkStartInfo){
		return FALSE;
	}

	if(g_zllTouchLink.networkStartInfo->joinNetworkMode == ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_START){
		zcl_zllTouchLinkStartRouteRequest();
	}else if(g_zllTouchLink.networkStartInfo->joinNetworkMode == ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_JOIN){
		zcl_zllTouchLinkNetworkJoinRealjoin();
	}

	return TRUE;
}

/*
 * @fn      zcl_zllTouchLinkNetworkStartOrJoinNwkLeave
 *
 * @brief
 *
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static s32 zcl_zllTouchLinkNetworkStartOrJoinNwkLeave(void *arg){
	/* should cancel the transId life timer */
	if(g_zllTouchLink.transIdLifeTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.transIdLifeTimer);
	}

	if(!is_device_factory_new()){
		nlme_leave_req_t leaveReq;
		leaveReq.rejoin = 0;
		leaveReq.removeChildren = 0;
		ZB_64BIT_ADDR_ZERO(leaveReq.deviceAddr);
		zb_nlmeLeaveReq(&leaveReq);
	}else{
		if(g_zllTouchLink.networkStartInfo->joinNetworkMode == ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_START){
			zcl_zllTouchLinkStartRouteRequest();
		}else if(g_zllTouchLink.networkStartInfo->joinNetworkMode == ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_JOIN){
			zcl_zllTouchLinkNetworkJoinRealjoin();
		}
	}

	return -1;
}

/*
 * @fn      zcl_zllTouchLinkRespSendForNetworkStart
 *
 * @brief	send network start response
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static void zcl_zllTouchLinNetworkStartRespCmdSend(void *arg){
	if(!g_zllTouchLink.networkStartInfo){
		return;
	}
	if(g_zllTouchLink.networkStartInfo->params.networkStartCmd.transId == 0){
		return;
	}

#ifdef ZB_ROUTER_ROLE
	u32 status = (u32)arg;
	zcl_zllTouchLinkNetworkStartReq_t *startParams = &g_zllTouchLink.networkStartInfo->params.networkStartCmd;
	u8 tcAddr[8] = { 0 };
	zcl_zllTouchLinkNetworkStartResp_t startResp = { 0 };

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	memcpy(&dstEp, &g_zllTouchLink.networkStartInfo->initiatorEpInfo, sizeof(epInfo_t));

#ifdef ZB_SECURITY
	memset(tcAddr, 0xff, 8);
	ZB_IEEE_ADDR_COPY(ss_ib.trust_center_address, tcAddr);
#endif

	startResp.transId = g_zllTouchLink.transId;
	startResp.status = (u8)status;
	startResp.nwkUpdateId = NWK_NIB().updateId;
	startResp.logicalChannel = startParams->logicalChannel;
	startResp.panId = startParams->panId;
	memcpy(startResp.epanId, startParams->epanId, 8);

	/* send network start response here */
	u8 seqNo = g_zllTouchLink.networkStartInfo->seqNo;
	u8 pktLen = sizeof(zcl_zllTouchLinkNetworkStartResp_t);
	zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_NETWORK_START_RSP, TRUE,
						ZCL_FRAME_SERVER_CLIENT_DIR, TRUE, 0, seqNo, pktLen, (u8 *)&startResp);

	/*
	 * after network start response sent, shall send network leave command if it,s a non-factory new device
	 *
	 * */

	if(status == SUCCESS){
		TL_ZB_TIMER_SCHEDULE(zcl_zllTouchLinkNetworkStartOrJoinNwkLeave, NULL, 100 * 1000);
	}else{
		zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_FAIL);
	}
#endif

}


/*
 * @fn      zcl_zllTouchLinkNetworkStartRequstProc
 *
 * @brief	start active scan for touch link network start
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkNetworkStartRequstHandler(u8 logicChannel){
	/* if disable by application */
	if(!g_zllTouchLink.startNetworkAllowed){
		TL_SCHEDULE_TASK(zcl_zllTouchLinNetworkStartRespCmdSend, (void *)FAILURE);
		return;
	}

	/* nwk_discovery ->
	 * startRespCmd send ->
	 * leave(if non-factory New device) ->
	 * router start */
	if(logicChannel >= TL_ZB_MAC_CHANNEL_START && logicChannel <= TL_ZB_MAC_CHANNEL_STOP){
		touchlink_discovery_network(1 << (logicChannel));
	}else{
		touchlink_discovery_network(ZB_ZLL_PRIMARY_CHANNELS_MASK);
	}
}



/*
 * @fn      zcl_zllTouchLinkNetworkStartResponseProc
 *
 * @brief	the handler to Network Start Response command
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ s32 zcl_zllTouchLinkNetworkStartResponseHandler(void *arg){
	zcl_zllTouchLinkNetworkStartResp_t *p = (zcl_zllTouchLinkNetworkStartResp_t *)arg;
	if(p->status == SUCCESS){
		/* security join */
		MAC_IB().rxOnWhenIdle = g_zllTouchLink.zbInfo.bf.rxOnWihleIdle;
		zb_rejoin_mode_set(REJOIN_SECURITY);
		zdo_nwk_rejoin_req(NLME_REJOIN_METHOD_REJOIN, 1<<(p->logicalChannel));
	}else{
		zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_NO_SERVER);
	}

	/*
	 * rejoin_req -> sent -> zdo_nlme_join_done -> zcl_zllTouchLinkNetworkStartDirectJoin
	 * so don't need to  call zcl_zllTouchLinkFinish() here
	 * zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_SUCC);
	 *
	 *  */
	ev_buf_free((u8 *)arg);
	return -1;
}

/*
 * @fn      zcl_zllTouchLinkStartNetworkTimerStop
 *
 * @brief	stop the timer that is for touch link start network
 *
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkNetworkJoinRequestHandler(u8 cmd){
	/*
	 * and then send join-router/join-ed command
	 */
	//zcl_zllTouchLinkNetworkJoinReq_t *joinParams = (zcl_zllTouchLinkNetworkJoinReq_t *)&g_zllTouchLink.networkStartInfo->params.networkJoinCmd;
	zcl_zllTouchLinkNetworkJoinResp_t joinResp = { 0 };
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	memcpy(&dstEp, &g_zllTouchLink.networkStartInfo->initiatorEpInfo, sizeof(epInfo_t));

	if(!g_zllTouchLink.startNetworkAllowed)	/* if disable by application */
	{
		/* send network start response here */
		joinResp.status = FAILURE;
	}
	else
	{
		joinResp.status = SUCCESS;
	}

	if(!ss_securityModeIsDistributed() && BDB_ATTR().nodeIsOnANetwork){
		joinResp.status = FAILURE;
	}

	joinResp.transId = g_zllTouchLink.transId;

	u8 seqNo = g_zllTouchLink.networkStartInfo->seqNo;
	u8 pktLen = sizeof(zcl_zllTouchLinkNetworkJoinResp_t);
	u8 respCmd = 0xff;
	if(cmd == ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER){
		respCmd = ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER_RSP;
	}else if(cmd == ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE){
		respCmd = ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE_RSP;
	}
	zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, respCmd, TRUE,
						ZCL_FRAME_SERVER_CLIENT_DIR, TRUE, 0, seqNo, pktLen, (u8 *)&joinResp);


	if(joinResp.status == FAILURE)	/* if disable by application */
	{
		zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_NOT_PERMITTED);
	}
	else
	{
		/*
		 * leave network if it's a non-factory new device
		 */
		TL_ZB_TIMER_SCHEDULE(zcl_zllTouchLinkNetworkStartOrJoinNwkLeave, NULL, 100*1000);
	}
	return;
}



/*
 * @fn      zcl_zllTouchLinkNetwokJoinResponseProc
 *
 * @brief	stop the timer that is for touch link start network
 *
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ s32 zcl_zllTouchLinkNetwokJoinResponseHandler(void *arg){
	zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_SUCC);
	return -1;
}



/*
 * @fn      rf_regProtocolSpecific
 *
 * @brief	zcl_zllTouchLinkNetworkStartOrJoin
 * 			start send start network if the device is a factory new device
 * 			or send join ed/route if the device is a non-factory new device
 *
 * @param 	arg target information form scan list
 *
 */
_CODE_ZCL_ void zcl_zllTouchLinkNetworkStartOrJoin(void *arg){
	zll_touchLinkScanInfo *peerInfo = &g_zllTouchLink.disc->scanList[g_zllTouchLink.opIdx];
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	memcpy((u8 *)&dstEp,(u8 *)&peerInfo->dstEp, sizeof(dstEp));

	ZB_TRANSCEIVER_SET_CHANNEL(g_zllTouchLink.touchLinkChan);

#ifdef ZB_SECURITY
	/* generate network key, and encrypt it */
	u8 encryptedNwkKey[CCM_KEY_SIZE] _attribute_aligned_(4) ;
	ll_get_encrypted_network_key(peerInfo->keyIdx, g_zllTouchLink.transId, g_zllTouchLink.respId, encryptedNwkKey);
#endif

	if(!BDB_ATTR().nodeIsOnANetwork){
		u8 selfForm = 0;
		if(g_zllTouchLink.zbInfo.bf.logicDevType == DEVICE_TYPE_ROUTER){
			/* start new network */
			touchlink_discovery_network(ZB_ZLL_PRIMARY_CHANNELS_MASK);
			//zcl_zllTouchLinkNetworkStartRequstHandler();
			selfForm = 1;
		}else{
			/*
			 * return error if the target is end devicec for factory new device.
			 *
			 * */
			if( peerInfo->zbInfo.bf.logicDevType == DEVICE_TYPE_END_DEVICE){
				zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_NO_SERVER);
				return;
			}else{
				/* send start network request */
			}
		}

		if(!selfForm){
			zcl_zllTouchLinkNetworkStartReq_t startReq = { 0 };
			startReq.transId = g_zllTouchLink.transId;
			memcpy(startReq.epanId, NWK_NIB().extPANId, sizeof(addrExt_t));
			startReq.panId = 0x0000;
			startReq.logicalChannel = MAC_IB().phyChannelCur;
			memcpy(startReq.initiatorIeeeAddr, NIB_IEEE_ADDRESS(), sizeof(addrExt_t));

			startReq.groupIdbegin = APS_GROUP_TABLE_SIZE + 1;
			startReq.groupIdEnd = APS_GROUP_TABLE_SIZE + peerInfo->totalGroupIds + 1;
			/*
			 *
			 * follow zll spec 8.4.3.1 Initiator procedure
			 */
			startReq.groupIdbegin = APS_GROUP_TABLE_SIZE + 1;
			startReq.groupIdEnd = APS_GROUP_TABLE_SIZE + peerInfo->totalGroupIds + 1;

			u16 nwkAddr = 0;
			if(g_touchlinkAttr.freeNwkAddressRangeBegin == 0x0000){
				nwkAddr = tl_zbNwkStochasticAddrCal();
				startReq.initiatorNwkAddr = nwkAddr;
				startReq.nwkAddr = nwkAddr + 1;
			}else{
				startReq.initiatorNwkAddr = g_touchlinkAttr.freeNwkAddressRangeBegin++;
				startReq.nwkAddr = g_touchlinkAttr.freeNwkAddressRangeBegin++;

				if(peerInfo->zllInfo.bf.addrAssign){
					startReq.freeNwkAddrRangeBegin = g_touchlinkAttr.freeNwkAddressRangeBegin++;
					g_touchlinkAttr.freeNwkAddressRangeBegin += ZB_ADDR_RANGE_ASSIGNED_FOR_TARGET;
					startReq.freeNwkAddrRangeEnd = g_touchlinkAttr.freeNwkAddressRangeBegin++;

					/* TODO - Assign Group Address Range*/
					startReq.freeGroupIdRangeBegin = 0;
					startReq.freeGroupIdRangeEnd = 0;
				}
			}
			peerInfo->nwkAddr = startReq.nwkAddr;

	#ifdef ZB_SECURITY
			startReq.keyIdx = peerInfo->keyIdx;
			memcpy(startReq.encryptedNwkKey, encryptedNwkKey, CCM_KEY_SIZE);
	#endif

			NWK_NIB().nwkAddr = startReq.initiatorNwkAddr;
			MAC_IB().shortAddress = NWK_NIB().nwkAddr;

			/* start a timer which life cycle is aplcRxWindowDuration(second)*/
			g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_START;
			zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_NETWORK_START, TRUE,
								ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, sizeof(zcl_zllTouchLinkNetworkStartReq_t), (u8 *)&startReq);
		}
	}else{
		/*
		 * non-factory new device
		 * */
		zcl_zllTouchLinkNetworkJoinReq_t joinReq = { 0 };
		joinReq.transId = g_zllTouchLink.transId;
		joinReq.nwkUpdateId = NWK_NIB().updateId;
#ifdef ZB_SECURITY
		joinReq.keyIdx = peerInfo->keyIdx;
		memcpy(joinReq.encryptedNwkKey, encryptedNwkKey, CCM_KEY_SIZE);
#endif
		memcpy(joinReq.epanId, NWK_NIB().extPANId, sizeof(addrExt_t));
		joinReq.panId = MAC_IB().panId;
		joinReq.logicalChannel = g_zllTouchLink.workingChannelBackUp;

		joinReq.groupIdBegin = APS_GROUP_TABLE_SIZE + 1;
		joinReq.groupIdEnd = APS_GROUP_TABLE_SIZE + peerInfo->totalGroupIds + 1;

		if(g_touchlinkAttr.freeNwkAddressRangeBegin == 0x0000){
			u16 nwkAddr = tl_zbNwkStochasticAddrCal();
			joinReq.nwkAddr = nwkAddr;
		}else{
			joinReq.nwkAddr = g_touchlinkAttr.freeNwkAddressRangeBegin++;
			if(peerInfo->zllInfo.bf.addrAssign){
				joinReq.freeNwkAddrRangeBegin = g_touchlinkAttr.freeNwkAddressRangeBegin++;
				g_touchlinkAttr.freeNwkAddressRangeBegin += ZB_ADDR_RANGE_ASSIGNED_FOR_TARGET;
				joinReq.freeNwkAddrRangeEnd = g_touchlinkAttr.freeNwkAddressRangeBegin++;

				/* TODO - Assign Group Address Range*/
				joinReq.freeGroupIdRangeBegin = 0;
				joinReq.freeGroupIdRangeEnd = 0;
			}
		}
		peerInfo->nwkAddr = joinReq.nwkAddr;
		u8 pldLen = sizeof(zcl_zllTouchLinkNetworkJoinReq_t);
		u8 cmd = ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER;
		g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_JOIN_ROUTE;
		if(peerInfo->zbInfo.bf.logicDevType == DEVICE_TYPE_END_DEVICE){
			cmd = ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE;
			g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_JOIN_ED;
		}

		zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, cmd, TRUE,
							ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, pldLen, (u8 *)&joinReq);

	}
	
	/*
	 *
	 * start a timer to process  the network start/join command timeout
	 *
	 * */
	if(g_zllTouchLink.runTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.runTimer);
	}
	g_zllTouchLink.runTimer = TL_ZB_TIMER_SCHEDULE(zcl_zllTouchLinkStartNetworkStartOrJoinTimeout, NULL, ZB_RX_WINDOW_DURATION);
}


s32 zcl_touchLinkDevStartIndicate(void *arg){
	/* start router confirm or rejoin confirm */

	u32 startStatus = (u32)arg;
	
	if(startStatus == SUCCESS){
		if(g_zllTouchLink.zbInfo.bf.logicDevType == DEVICE_TYPE_END_DEVICE){
			zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_SUCC);
		}else{
			if(g_bdbCtx.role == BDB_COMMISSIONING_ROLE_INITIATOR){
				TL_SCHEDULE_TASK(zcl_zllTouchLinkNetworkStartOrJoin, NULL);
			}else{
				TL_SCHEDULE_TASK(zcl_zllTouchLinkNetworkStartDirectJoin, NULL);
			}
		}
	}else{
		zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_FAIL);
	}
	return -1;
}

_CODE_ZCL_ void zcl_zllTouchLinkResetFactoryReq(void *arg){
	zll_touchLinkScanInfo *peerInfo = &g_zllTouchLink.disc->scanList[g_zllTouchLink.opIdx];
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	memcpy((u8 *)&dstEp,(u8 *)&peerInfo->dstEp, sizeof(dstEp));

	zcl_zllTouchLinkResetFactoryReq_t req;
	req.transId = g_zllTouchLink.transId;

	g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_IDENTIFY;
	zcl_sendInterPANCmd(g_zllTouchLink.devInfo.epId, &dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_RESET_TO_FACTORY_NEW, TRUE,
						ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, sizeof(zcl_zllTouchLinkResetFactoryReq_t), (u8 *)&req);
}
