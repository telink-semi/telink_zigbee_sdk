/********************************************************************************************************
 * @file     zcl_zll_commissioning.c
 *
 * @brief	 process touch link command
 *
 * @author
 * @date     June. 11, 2017
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

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define		DEBUG_TOUCHLINK_EN				0


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL FUNCTIONS */


/**********************************************************************
 * GLOBAL VARIABLES
 */
zcl_zllTouckLink_t g_zllTouchLink = {
		.lqiThreshold = 0xa0,
		.commissionTxPower = ZB_RADIO_TX_0DBM,
		.keyType = MASTER_KEY,
};

zcl_zllCommission_t g_zllCommission;

touchlink_attr_t g_touchlinkAttrDft = { 0x0001, 0xfff7, 0x0001, 0xfeff };

zdo_touchLinkCb_t touchLinkCb = {zcl_zllTouchLinkLeaveCnfCb};

#define TOUCHLIK_INITIATOR_SET(mode) 		g_zllTouchLink.zllInfo.bf.linkInitiator = mode
#define TOUCHLIK_ADDR_ASSIGN_SET(mode) 		g_zllTouchLink.zllInfo.bf.addrAssign = mode
#define TOUCHLIK_FACTORY_NEW_SET(mode) 		g_zllTouchLink.zllInfo.bf.factoryNew = mode

/*
 * @fn      zcl_zllGetGroupIdentifiersRequest
 *
 * @brief	send Get group identifiers request
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllGetGroupIdentifiersRequest(epInfo_t* pDstEpInfo, u8 startIdx){
	u8 buf[1];
	buf[0] = startIdx;
	zcl_sendCmd(g_zllTouchLink.devInfo.epId, pDstEpInfo, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS, TRUE,
				ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, 1, buf);
}


/*
 * @fn      zcl_zllGetGroupIdentifiersRequestHandler
 *
 * @brief	the handler for receiving Get group identifiers request command
 *
 * @param 	arg
 *
 */
_CODE_ZCL_  static void zcl_zllGetGroupIdentifiersRequestHandler(u8 startIdx, epInfo_t *dstEpInfo){
	zcl_zllUtilityGetGroupIdsResp_t *pResp = NULL;
	u8 groupsNum = 0;
	u16 groupIds[APS_GROUP_TABLE_SIZE];

	aps_group_list_get(&groupsNum, groupIds);

	u8 pktLen = groupsNum * sizeof(zcl_groupInfo) + 3;
	pResp = (zcl_zllUtilityGetGroupIdsResp_t *)ev_buf_allocate(pktLen);
	if(pResp){
		pResp->total = groupsNum;
		pResp->startIdx = (startIdx >= groupsNum)? 0 : startIdx;
		pResp->count = 0;
		zcl_groupInfo *pList = (zcl_groupInfo *)&pResp->groupInfoRecord;
		for(s32 i = 0; i < groupsNum; i++){
			if(i >= startIdx){
				pList->groupId = groupIds[i];
				pList->groupType = 0x00;
				pResp->count++;
				pList++;
			}
		}
		zcl_sendCmd(g_zllTouchLink.devInfo.epId, dstEpInfo, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, FALSE, 0, g_zllTouchLink.seqNo++, pktLen, (u8 *)pResp);

		ev_buf_free((u8 *)pResp);
	}
}


/*
 * @fn      zcl_zllGetGroupIdentifiersResponseHandler
 *
 * @brief	the handler for receiving Get group identifiers response command
 *
 * @param 	arg
 *
 */
_CODE_ZCL_  static void zcl_zllGetGroupIdentifiersResponseHandler(zcl_zllUtilityGetGroupIdsResp_t *resp){
	if(g_zllCommission.appCb->utilityAppCallback){
		g_zllCommission.appCb->utilityAppCallback(ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS_RSP, resp);
	}
}


/*
 * @fn      zcl_zllGetEndpointListRequest
 *
 * @brief	send command of "Get endpoint list request"
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllGetEndpointListRequest(epInfo_t* pDstEpInfo, u8 startIdx){
	u8 ctx = startIdx;
	zcl_sendCmd(g_zllTouchLink.devInfo.epId, pDstEpInfo, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST, TRUE,
				ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, g_zllTouchLink.seqNo++, 1, &ctx);
}


/*
 * @fn      zcl_zllGetEndpointListRequestHandler
 *
 * @brief	the handler for receiving the command of "Get endpoint list request"
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static void zcl_zllGetEndpointListRequestHandler(u8 startIdx, epInfo_t *dstEpInfo){
	zcl_zllUtilityGetEpListResp_t *pResp = NULL;
	u8 avaEpNum = af_availableEpNumGet();

	u16 len = (avaEpNum * sizeof(zcl_zllEndpointInfo_t) + 3);
	pResp = (zcl_zllUtilityGetEpListResp_t *)ev_buf_allocate(len);
	if(pResp){
		pResp->total = avaEpNum;
		pResp->startIdx = (startIdx >= avaEpNum) ? 0 : startIdx;
		pResp->count = 0;
		af_endpoint_descriptor_t *aed = af_epDescriptorGet();

		zcl_zllEndpointInfo_t *pList = (zcl_zllEndpointInfo_t *)&pResp->epInfoRecord;
		for(s32 i = startIdx; i < avaEpNum; i++){
			pList->nwkAddr = NIB_NETWORK_ADDRESS();
			pList->epId = aed[i].ep;
			pList->deviceId = aed[i].correspond_simple_desc->app_dev_id;
			pList->profileId = aed[i].correspond_simple_desc->app_profile_id;
			pList->version =  aed[i].correspond_simple_desc->app_dev_ver;
			pList++;
			pResp->count++;
		}

		zcl_sendCmd(g_zllTouchLink.devInfo.epId, dstEpInfo, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST_RSP, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, FALSE, 0, g_zllTouchLink.seqNo++, len, (u8 *)pResp);

		ev_buf_free((u8 *)pResp);
	}
}


/*
 * @fn      zcl_zllGetEndpointListResponseHandler
 *
 * @brief	the handler for receiving the command of "Get endpoint list response"
 *
 * @param 	arg
 *
 */

_CODE_ZCL_ static void zcl_zllGetEndpointListResponseHandler(zcl_zllUtilityGetEpListResp_t *resp){
	if(g_zllCommission.appCb->utilityAppCallback){
		g_zllCommission.appCb->utilityAppCallback(ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST_RSP, resp);
	}
}


/*
 * @fn      zcl_zllEndpointInformation
 *
 * @brief	send the command of " endpoint Information"
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void zcl_zllEndpointInformation(epInfo_t *dstEp){
	zcl_zllUtilityEndpointInfo_t epInfoCmd;
	af_endpoint_descriptor_t *aed = af_epDescriptorGet();
	memcpy(epInfoCmd.ieeeAddr, NIB_IEEE_ADDRESS(), sizeof(addrExt_t));
	epInfoCmd.info.nwkAddr = NIB_NETWORK_ADDRESS();
	epInfoCmd.info.epId = aed[0].ep;
	epInfoCmd.info.deviceId = aed[0].correspond_simple_desc->app_dev_id;
	epInfoCmd.info.profileId = aed[0].correspond_simple_desc->app_profile_id;
	epInfoCmd.info.version = aed[0].correspond_simple_desc->app_dev_ver;

	zcl_sendCmd(aed[0].ep, dstEp, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, ZCL_CMD_ZLL_COMMISSIONING_ENDPOINT_INFORMATION, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, FALSE, 0, g_zllTouchLink.seqNo++, sizeof(zcl_zllUtilityEndpointInfo_t), (u8 *)&epInfoCmd);
}

/*
 * @fn      zcl_zllEndpointInformation
 *
 * @brief	the handler for receiving the command of "endpoint Information"
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ static void zcl_zllEndpointInformationHandler(zcl_zllUtilityEndpointInfo_t *epInfo){
	if(g_zllCommission.appCb->utilityAppCallback){
		g_zllCommission.appCb->utilityAppCallback(ZCL_CMD_ZLL_COMMISSIONING_ENDPOINT_INFORMATION, epInfo);
	}
}


/*
 * @fn      zcl_zllTouchLinkFinish
 *
 * @brief	force the touch link to finish
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ void	zcl_zllTouchLinkFinish(u8 status){
	g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_IDLE;
	g_zllTouchLink.status = status;

	ZB_TRANSCEIVER_SET_CHANNEL(g_zllTouchLink.workingChannelBackUp);
	MAC_IB().rxOnWhenIdle = g_zllTouchLink.zbInfo.bf.rxOnWihleIdle;  //must restore the rxOnWhenIdle for ED

	rf_setTxPower(g_zb_txPowerSet);
#ifdef ZB_ED_ROLE
	if(status == ZCL_ZLL_TOUCH_LINK_STA_SUCC || status == ZCL_ZLL_TOUCH_LINK_STA_EXIST){
		zb_setPollRate(POLL_RATE);
	}
#endif
	g_zllTouchLink.transId = 0;
	epInfo_t peerEpInfo;
	TL_SETSTRUCTCONTENT(peerEpInfo,0);

	if(g_zllTouchLink.networkStartInfo){
		memcpy(&peerEpInfo, &g_zllTouchLink.networkStartInfo->initiatorEpInfo, sizeof(epInfo_t));
		peerEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		if(g_zllTouchLink.networkStartInfo->joinNetworkMode == ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_START){
			zcl_zllTouchLinkNetworkStartReq_t *pStartReq = &g_zllTouchLink.networkStartInfo->params.networkStartCmd;
			peerEpInfo.dstAddr.shortAddr = pStartReq->nwkAddr;
		}else{
			zcl_zllTouchLinkNetworkJoinReq_t *pJoinReq = &g_zllTouchLink.networkStartInfo->params.networkJoinCmd;
			peerEpInfo.dstAddr.shortAddr = pJoinReq->nwkAddr;
		}

		ev_buf_free((u8 *)g_zllTouchLink.networkStartInfo);
		g_zllTouchLink.networkStartInfo = NULL;
	}

	if(g_zllTouchLink.disc){
		zll_touchLinkScanInfo *peerInfo = &g_zllTouchLink.disc->scanList[g_zllTouchLink.opIdx];
		peerEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		peerEpInfo.dstAddr.shortAddr = peerInfo->nwkAddr;
		peerEpInfo.dstEp = peerInfo->devInfo.epId;
		peerEpInfo.profileId = peerInfo->devInfo.profileId;
		peerEpInfo.radius = 0;
		peerEpInfo.txOptions = 0;
		ev_buf_free((u8 *)g_zllTouchLink.disc);
		g_zllTouchLink.disc = NULL;
	}

	if(g_zllTouchLink.transIdLifeTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.transIdLifeTimer);
	}

	if(g_zllTouchLink.runTimer){
		TL_ZB_TIMER_CANCEL(&g_zllTouchLink.runTimer);
	}

	if(g_zllCommission.appCb->touchLinkCallback){

		if(peerEpInfo.dstAddrMode == 0){
			g_zllCommission.appCb->touchLinkCallback(status, NULL);
		}else{
			g_zllCommission.appCb->touchLinkCallback(status, &peerEpInfo);
		}
	}
	TOUCHLIK_INITIATOR_SET(0);
	//TOUCHLIK_ADDR_ASSIGN_SET(0);
	TOUCHLIK_FACTORY_NEW_SET(is_device_factory_new());
}

/*
 * @fn      zcl_zllTouchLinkTimeout
 *
 * @brief	ZB_INTER_PAN_TRANS_ID_LIFETIME callback
 *
 * @param 	arg
 *
 */
_CODE_ZCL_ s32 zcl_zllTouchLinkTimeout(void *arg){
	g_zllTouchLink.transIdLifeTimer = NULL;
	zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_TIMEOUT);
	return -1;
}

/*
 * @fn      zcl_zllTouchLinkClientCmdHandler
 *
 * @brief   the handler to command from client
 *
 * @param 	pInMsg incomming message
 *
 */
_CODE_ZCL_ static u8 zcl_touchLinkClientCmdHandler(zclIncoming_t *pInMsg){
	u8 status = ZCL_STA_SUCCESS;
	u8 cmd = pInMsg->hdr.cmd;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	epInfo_t srcEpInfo;

	TL_SETSTRUCTCONTENT(srcEpInfo, 0);

	srcEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
	memcpy(srcEpInfo.dstAddr.extAddr, pApsdeInd->indInfo.src_ext_addr, 8);
	srcEpInfo.dstEp = pApsdeInd->indInfo.src_ep;
	srcEpInfo.profileId = pApsdeInd->indInfo.profile_id;
	srcEpInfo.radius = 0;
	srcEpInfo.txOptions = 0;

	switch (cmd) {
		case ZCL_CMD_ZLL_COMMISSIONING_SCAN:
		{
			/*
			 *
			 * If this is a sleeping End Device ,Enable receiver while Inter-PAN transaction ID is valid (aplcInterPANTransIdLifetime)
			 * it can disable receiver before  aplcInterPANTransIdLifetime if transaction is completed and end device has joined the network.
			 * Start Timer here for interval aplcInterPANTransIdLifetime. When expired remove zcl_inter_PAN_transaction_ID.
			 * Validate zcl_inter_PAN_transaction_ID on every received commands (check if 0)
			 * check rcvd requests rssi correction for satisfactory manufacturer specific limit. if not drop request.
			 *
			 */
			if(pApsdeInd->indInfo.lqi < g_zllTouchLink.lqiThreshold){
				break;
			}

			zcl_zllTouchLinkScanReq_t scanReq;
			memcpy((u8 *)&scanReq, pInMsg->pData,sizeof(scanReq));
			if(scanReq.transId == g_zllTouchLink.transId || (g_zllTouchLink.state & ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_STARTORJOIN) == ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_STARTORJOIN){
				break;
			}else{
				if(!is_device_factory_new() && g_zllTouchLink.state == ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DISCOVERY){
					break;
				}
				if(scanReq.zllInfo.bf.linkInitiator != 1)
				{
					break;
				}
			}

			g_zllTouchLink.respId = ZB_RANDOM() << 16;
			g_zllTouchLink.respId += ZB_RANDOM();
			g_zllTouchLink.transId = scanReq.transId;
			g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DISCOVERY;

			zcl_zllTouchLinkScanRequestHandler(&srcEpInfo, pInMsg->hdr.seqNum);
			break;
		}


		case ZCL_CMD_ZLL_COMMISSIONING_NETWORK_START:
		{
			/*
			 * store some info for sending network response command and joining network
			 * */
			g_zllTouchLink.networkStartInfo = (zcl_zllTouckLinkNetworkStartParams *)ev_buf_allocate(sizeof(zcl_zllTouckLinkNetworkStartParams));
			if(g_zllTouchLink.networkStartInfo){
				memset((u8 *)g_zllTouchLink.networkStartInfo, 0, sizeof(zcl_zllTouckLinkNetworkStartParams));
				zcl_zllTouchLinkNetworkStartReq_t *pReq = &g_zllTouchLink.networkStartInfo->params.networkStartCmd;
				memcpy(pReq, (u8 *)pInMsg->pData, sizeof(zcl_zllTouchLinkNetworkStartReq_t));

				if(pReq->transId == g_zllTouchLink.transId){
#ifdef ZB_SECURITY
					u8 encryptedNwkKey[CCM_KEY_SIZE] _attribute_aligned_(4);
					memcpy(encryptedNwkKey, pReq->encryptedNwkKey, CCM_KEY_SIZE);
					ll_decrypt_network_key(pReq->keyIdx, encryptedNwkKey, g_zllTouchLink.transId, g_zllTouchLink.respId);
					memcpy(pReq->encryptedNwkKey, encryptedNwkKey, CCM_KEY_SIZE);
#endif
					g_zllTouchLink.networkStartInfo->seqNo = pInMsg->hdr.seqNum;
					memcpy(&g_zllTouchLink.networkStartInfo->initiatorEpInfo, &srcEpInfo, sizeof(epInfo_t));
					g_zllTouchLink.networkStartInfo->joinNetworkMode = ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_START;
					zcl_zllTouchLinkNetworkStartRequstHandler();
				}else{
					ev_buf_free((u8 *)g_zllTouchLink.networkStartInfo);
					g_zllTouchLink.networkStartInfo = NULL;
				}
			}
			break;
		}



#ifdef ZB_ROUTER_ROLE
		case ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER:
		{
#endif
#ifdef ZB_ED_ROLE
		case ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE:
		{
#endif
			/*
			 * store some info for sending network response command and joining network
			 * */
			g_zllTouchLink.networkStartInfo = (zcl_zllTouckLinkNetworkStartParams *)ev_buf_allocate(sizeof(zcl_zllTouckLinkNetworkStartParams));
			if(g_zllTouchLink.networkStartInfo){
				zcl_zllTouchLinkNetworkJoinReq_t *pReq = &g_zllTouchLink.networkStartInfo->params.networkJoinCmd;
				memcpy(pReq, (u8 *)pInMsg->pData, sizeof(zcl_zllTouchLinkNetworkJoinReq_t));

				if(pReq->transId == g_zllTouchLink.transId){
#ifdef ZB_SECURITY
					u8 encryptedNwkKey[CCM_KEY_SIZE] _attribute_aligned_(4);
					memcpy(encryptedNwkKey, pReq->encryptedNwkKey, CCM_KEY_SIZE);
					ll_decrypt_network_key(pReq->keyIdx, encryptedNwkKey, g_zllTouchLink.transId, g_zllTouchLink.respId);
					memcpy(pReq->encryptedNwkKey, encryptedNwkKey, CCM_KEY_SIZE);
#endif
					g_zllTouchLink.networkStartInfo->seqNo = pInMsg->hdr.seqNum;
					memcpy(&g_zllTouchLink.networkStartInfo->initiatorEpInfo, &srcEpInfo, sizeof(epInfo_t));
					g_zllTouchLink.networkStartInfo->joinNetworkMode = ZCL_ZLL_COMMISSIONING_TOUCH_LICK_MODE_JOIN;
					zcl_zllTouchLinkNetworkJoinRequestHandler(cmd);
				}else{
					ev_buf_free((u8 *)g_zllTouchLink.networkStartInfo);
					g_zllTouchLink.networkStartInfo = NULL;
				}
			}
			break;
		}


		case ZCL_CMD_ZLL_COMMISSIONING_RESET_TO_FACTORY_NEW:
		{
			u32 transId = 0xFFFFFFFF;
			memcpy(&transId, pInMsg->pData, sizeof(u32));
			if(transId == g_zllTouchLink.transId){
				if(!is_device_factory_new()){
					if(ss_securityModeIsDistributed()){
						nlme_leave_req_t leaveReq;
						leaveReq.rejoin = 0;
						leaveReq.removeChildren = 0;
						memset(leaveReq.deviceAddr,0,sizeof(addrExt_t));
						zb_nlmeLeaveReq(&leaveReq);

#ifdef ZLL_DEVICE_INITIATOR_CAPABLE
						g_touchlinkAttr.freeNwkAddressRangeBegin = 0x0001;
						g_touchlinkAttr.freeNwkAddressRangeEnd = 0xfff7;
						g_touchlinkAttr.freeGroupAddressRangeBegin = 0x0001;
						g_touchlinkAttr.freeGroupAddressRangeEnd = 0xfeff;
#endif
					}else{
						zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_EXIST);
					}
				}
			}
			break;
		}


		case ZCL_CMD_ZLL_COMMISSIONING_IDENTIFY:
		{
			zcl_zllTouchLinkIdentifyReq_t req;
			memcpy(&req, (u8 *)pInMsg->pData, sizeof(zcl_zllTouchLinkIdentifyReq_t));
			if(req.transId == g_zllTouchLink.transId){
				u16 identifyTime = 0;

				if(req.identifyDuration == 0){
					identifyTime = 0;
				}else if (req.identifyDuration >= 0x0001 && req.identifyDuration <= NWK_BROADCAST_RESERVED){
					identifyTime = req.identifyDuration;
				}else if(req.identifyDuration == 0xffff ){
					identifyTime = DEFAULT_IDENTIFY_DURATION;
				}

				zcl_identify_commissioningIdentify(pApsdeInd, identifyTime);
			}
			break;
		}

		case ZCL_CMD_ZLL_COMMISSIONING_NETWORK_UPDATE:
		{
			zcl_zllTouchLinkNetworkUpdateReq_t req;
			memcpy(&req, (u8 *)pInMsg->pData, sizeof(zcl_zllTouchLinkNetworkUpdateReq_t));
			if(req.transId == g_zllTouchLink.transId){
				if(ZB_EXTPANID_CMP(req.epanId, NWK_NIB().extPANId) && req.panId == MAC_IB().panId){
					if(req.nwkUpdateId > NWK_NIB().updateId){
						NWK_NIB().updateId = req.nwkUpdateId;
						zdo_nlmeChannelShift(req.logicalChannel);
					}
				}
			}
			zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_STA_EXIST);
			break;
		}

		case ZCL_CMD_ZLL_COMMISSIONING_DEVICE_INFORMATION:
		{
			zcl_zllTouchLinkDeviceInfoReq_t req;
			memcpy(&req, (u8 *)pInMsg->pData, sizeof(zcl_zllTouchLinkDeviceInfoReq_t));
			if(req.transId == g_zllTouchLink.transId){
				zcl_zllTouchLinkDeviceInformationRequestHandler(&srcEpInfo,req.startIdx);
			}
			break;
		}

		/* utility: ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS/ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST */
		case ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS:
		{
			u8 startIdx = *(u8 *)pInMsg->pData;
			srcEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			srcEpInfo.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
			zcl_zllGetGroupIdentifiersRequestHandler(startIdx, &srcEpInfo);
			break;
		}

		case ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST:
		{
			u8 startIdx = *(u8 *)pInMsg->pData;
			srcEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			srcEpInfo.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
			zcl_zllGetEndpointListRequestHandler(startIdx, &srcEpInfo);
			break;
		}

		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}


/*
 * @fn      zcl_zllTouchLinkServerCmdHandler
 *
 * @brief   the handler to command from server
 *
 * @param 	pInMsg incomming message
 *
 */
_CODE_ZCL_ static u8 zcl_zllCommissionServerCmdHandler(zclIncoming_t *pInMsg){
	u8 status = ZCL_STA_SUCCESS;
	u8 cmd = pInMsg->hdr.cmd;

	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	epInfo_t dstEp;

	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
	memcpy(dstEp.dstAddr.extAddr, pApsdeInd->indInfo.src_ext_addr, 8);
	dstEp.dstEp = pApsdeInd->indInfo.src_ep;
	dstEp.profileId = pApsdeInd->indInfo.profile_id;
	dstEp.radius = 0;
	dstEp.txOptions = 0;


	if(g_zllTouchLink.state > ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DISCOVERY){
		zll_touchLinkScanInfo *peerInfo = &g_zllTouchLink.disc->scanList[g_zllTouchLink.opIdx];
		if(!ZB_64BIT_ADDR_CMP(peerInfo->dstEp.dstAddr.extAddr, dstEp.dstAddr.extAddr)){
			return ZCL_STA_FAILURE;
		}
	}

	switch (cmd) {
		case ZCL_CMD_ZLL_COMMISSIONING_SCAN_RSP:
		{
			zcl_zllTouchLinkScanResp_t resp;
			memcpy((u8 *)&resp, pInMsg->pData, sizeof(zcl_zllTouchLinkScanResp_t));
			if(g_zllTouchLink.state != ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DISCOVERY || resp.transId != g_zllTouchLink.transId || !g_zllTouchLink.disc){
				status =  ZCL_STA_INVALID_VALUE;
				break;
			}
			zcl_zllTouchLinkScanResponseHandler(&resp, &dstEp);
			break;
		}


		case ZCL_CMD_ZLL_COMMISSIONING_NETWORK_START_RSP:
		{
			if(g_zllTouchLink.state != ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_START){
				break;
			}

			zcl_zllTouchLinkNetworkStartResp_t *nscr = (zcl_zllTouchLinkNetworkStartResp_t *)ev_buf_allocate(sizeof(zcl_zllTouchLinkNetworkStartResp_t));
			if(!nscr ){
				break;
			}
			memcpy((u8 *)nscr, pInMsg->pData, sizeof(zcl_zllTouchLinkNetworkStartResp_t));
			if(nscr->transId == g_zllTouchLink.transId){
				if(is_device_factory_new() && nscr->status == SUCCESS){
					memcpy(NWK_NIB().extPANId, nscr->epanId, 8);
					memcpy(aps_ib.aps_use_ext_panid, NWK_NIB().extPANId, 8);
					MAC_IB().panId = nscr->panId;
					MAC_IB().phyChannelCur = nscr->logicalChannel;
					g_zllTouchLink.workingChannelBackUp = nscr->logicalChannel;
				}
				zcl_zllTouchLinkStartNetworkStartOrJoinTimerStop();
				TL_ZB_TIMER_SCHEDULE(zcl_zllTouchLinkNetworkStartResponseHandler, nscr, ZB_MIN_STARTUP_DELAY_TIME + 200 * 1000);
			}else{
				ev_buf_free((u8 *)nscr);
			}
			break;
		}


		case ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER_RSP:
		case ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE_RSP:
		{
			if(cmd == ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_ROUTER_RSP && g_zllTouchLink.state != ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_JOIN_ROUTE){
				break;
			}
			if(cmd == ZCL_CMD_ZLL_COMMISSIONING_NETWORK_JOIN_END_DEVICE_RSP && g_zllTouchLink.state != ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_JOIN_ED){
				break;
			}

			zcl_zllTouchLinkNetworkJoinResp_t resp ;
			memcpy((u8 *)&resp, pInMsg->pData, sizeof(zcl_zllTouchLinkNetworkJoinResp_t));
			if(resp.transId == g_zllTouchLink.transId && resp.status == SUCCESS){
				zcl_zllTouchLinkStartNetworkStartOrJoinTimerStop();
				TL_ZB_TIMER_SCHEDULE(zcl_zllTouchLinkNetwokJoinResponseHandler, NULL, ZB_MIN_STARTUP_DELAY_TIME);
			}
			else
			{
				zcl_zllTouchLinkStartNetworkStartOrJoinTimerStop();
				zcl_zllTouchLinkFinish(ZCL_ZLL_TOUCH_LINK_FAIL);
			}
			break;
		}

		case ZCL_CMD_ZLL_COMMISSIONING_DEVICE_INFORMATION_RSP:
		{
			zcl_zllTouchLinkDeviceInfoResp_t resp;
			memcpy((u8 *)&resp, pInMsg->pData, sizeof(zcl_zllTouchLinkDeviceInfoResp_t));
			if(resp.transId == g_zllTouchLink.transId && g_zllTouchLink.state == ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DEVICE_INFO_EXCHANGE){

				if(resp.numOfSubdevices == 1){
					zcl_zllDeviceInfoRec_t rec;
					memcpy((u8 *)&rec, pInMsg->pData+sizeof(zcl_zllTouchLinkDeviceInfoResp_t), sizeof(zcl_zllDeviceInfoRec_t));
				}
				zcl_zllTouchLinkDeviceInformationResponseHandler(&resp);
			}
			break;
		}

		/* utilities  */
		case ZCL_CMD_ZLL_COMMISSIONING_ENDPOINT_INFORMATION:
		{
			zcl_zllUtilityEndpointInfo_t epInfo;
			memcpy((u8 *)&epInfo, pInMsg->pData, sizeof(zcl_zllUtilityEndpointInfo_t));
			zcl_zllEndpointInformationHandler(&epInfo);
			break;
		}

		case ZCL_CMD_ZLL_COMMISSIONING_GET_GROUP_IDENTIFIERS_RSP:
		{
			zcl_zllUtilityGetGroupIdsResp_t resp;
			memcpy((u8 *)&resp, pInMsg->pData, sizeof(zcl_zllUtilityGetGroupIdsResp_t));

			u16 pldLen = resp.count * sizeof(zcl_groupInfo);
			zcl_groupInfo *groupList = (zcl_groupInfo *)ev_buf_allocate(pldLen);
			if(groupList){
				resp.groupInfoRecord = groupList;
				memcpy((u8 *)groupList, pInMsg->pData+3, pldLen);
			}
			zcl_zllGetGroupIdentifiersResponseHandler(&resp);
			if(groupList){
				ev_buf_free((u8 *)groupList);
			}
			break;
		}

		case ZCL_CMD_ZLL_COMMISSIONING_GET_ENDPOINT_LIST_RSP:
		{
			zcl_zllUtilityGetEpListResp_t epListResp;
			memcpy((u8 *)&epListResp, pInMsg->pData, sizeof(zcl_zllUtilityEndpointInfo_t));
			epListResp.epInfoRecord = NULL;

			u16 pldLen = epListResp.count * sizeof(zcl_zllEndpointInfo_t);
			zcl_zllEndpointInfo_t *epList = (zcl_zllEndpointInfo_t *)ev_buf_allocate(pldLen);
			if(epList){
				epListResp.epInfoRecord = epList;
				memcpy((u8 *)epList, pInMsg->pData+3, pldLen);
			}
			zcl_zllGetEndpointListResponseHandler(&epListResp);
			if(epList){
				ev_buf_free((u8 *)epList);
			}
			break;
		}

		default:
			status = ZCL_STA_FAILURE;
			break;
	}

	return status;
}

_CODE_ZCL_ u8 zcl_touchLinkInit(void){
	if(zb_isDeviceFactoryNew()){
		memcpy(&g_touchlinkAttr, &g_touchlinkAttrDft, sizeof(touchlink_attr_t));
	}

	zdo_touchLinkCbRegister(&touchLinkCb);

	g_zllTouchLink.zbInfo.byte = 0;
	g_zllTouchLink.zbInfo.bf.rxOnWihleIdle = (af_nodeMacCapabilityGet() & MAC_CAP_RX_ON_WHEN_IDLE) ? 1 : 0;
	g_zllTouchLink.zbInfo.bf.logicDevType = af_nodeDevTypeGet();
	g_zllTouchLink.startNetworkAllowed = 1;
	g_zllTouchLink.zllInfo.byte = 0;
//	g_zllTouchLink.zllInfo.bf.addrAssign = 1;
//
//	if(zb_isDeviceFactoryNew()){  //new device, set priority as 1
//		g_zllTouchLink.zllInfo.bf.priorityReq = 1;
//		g_zllTouchLink.zllInfo.bf.factoryNew = 1;
//	}
	g_zllTouchLink.scanListNum = (ZB_BUF_SIZE - OFFSETOF(zcl_zllTouckLinkDisc_t, scanList))/sizeof(zll_touchLinkScanInfo);

	af_endpoint_descriptor_t *aed = af_epDescriptorGet();

	for(u8 i = 0; i < af_availableEpNumGet(); i++){
		if(af_clsuterIdMatched(ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, aed[i].correspond_simple_desc)){
			g_zllTouchLink.devInfo.epId = aed[i].ep;
			g_zllTouchLink.devInfo.profileId = aed[i].correspond_simple_desc->app_profile_id;
			g_zllTouchLink.devInfo.deviceId = aed[i].correspond_simple_desc->app_dev_id;
			g_zllTouchLink.devInfo.version = aed[i].correspond_simple_desc->app_dev_ver;

			return ZCL_STA_SUCCESS;
		}
	}

	return ZCL_STA_FAILURE;
}


/*
 * @fn      zcl_touchLinkStart
 *
 * @brief   start touch link
 *
 * @param
 *
 */

_CODE_ZCL_ void zcl_touchLinkStart(void){
	DEBUG(DEBUG_TOUCHLINK_EN,"ZLL trigger touch link operation, current state %d\n",g_zllTouchLink.state);
	if(g_zllTouchLink.state != ZCL_ZLL_COMMISSION_STATE_IDLE){
		if(g_zllCommission.appCb->touchLinkCallback){
			g_zllCommission.appCb->touchLinkCallback(ZCL_ZLL_TOUCH_LINK_BUSY, NULL);
		}
		return;
	}

	g_zllTouchLink.vDoPrimaryScan = 1;
	g_zllTouchLink.IsFirstChannel = 1;
	u32 scanChanMask = 0;
	if(BDB_ATTR().primaryChannelSet == 0){
		g_zllTouchLink.vDoPrimaryScan = 0;
		scanChanMask = BDB_ATTR().secondaryChannelSet;
	}else{
		scanChanMask = BDB_ATTR().primaryChannelSet;
	}
	if(scanChanMask == 0){
		if(g_zllCommission.appCb->touchLinkCallback){
			g_zllCommission.appCb->touchLinkCallback(ZCL_ZLL_TOUCH_LINK_STA_NO_CAPACITY, NULL);
		}
		return;
	}

	if(g_zllTouchLink.disc){
		ev_buf_free((u8 *)g_zllTouchLink.disc);
	}
	g_zllTouchLink.disc = (zcl_zllTouckLinkDisc_t *)ev_buf_allocate(sizeof(zcl_zllTouckLinkDisc_t));
	if(!g_zllTouchLink.disc){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_BUFFER_OVERFLOWN);
		return;
	}

	/* obtain the information for touch link */
	zcl_zllTouckLinkDisc_t *pDisc = g_zllTouchLink.disc;
	pDisc->targetNum = 0;
	pDisc->primaryChannelScanComplete = false;
	pDisc->currentScannChannel	= 0;
	pDisc->scanCnt = 0;
	pDisc->unscannedChannelMask = scanChanMask;

	while(g_zllTouchLink.transId == 0){
		g_zllTouchLink.transId = ZB_RANDOM() | (ZB_RANDOM() << 16);
	}

	g_zllTouchLink.zllInfo.bf.linkInitiator = 1;
	g_zllTouchLink.zllInfo.bf.addrAssign = 1;

	/* restore the setting */
	g_zllTouchLink.networkStartInfo = NULL;
	g_zllTouchLink.seqNo = 0;
	g_zllTouchLink.state = ZCL_ZLL_COMMISSION_STATE_TOUCHLINK_DISCOVERY;

	if(is_device_factory_new()){
		g_zllTouchLink.zllInfo.bf.factoryNew = 1;
		MAC_IB().panId = zb_random();
	}
	else
	{
		g_zllTouchLink.zllInfo.bf.factoryNew = 0;
	}

	rf_setTxPower(g_zllTouchLink.commissionTxPower);

	//zdo_syn_parent_pause();
	zb_setPollRate(0);

	/* scan start */
	/* rxOnWhile should be set as 1, so as to receive the scan response.
	 * store the channel to use after scanning  */
	MAC_IB().rxOnWhenIdle = 1;
	g_zllTouchLink.workingChannelBackUp = MAC_IB().phyChannelCur;

	zcl_zllTouchLinkDiscoveyStart();
}


_CODE_ZCL_ void  touchlink_lqiThresholdSet(u8 lqi){
	g_zllTouchLink.lqiThreshold = lqi;
}


_CODE_ZCL_ void  touchlink_keyModeSet(u8 keyType, u8 *key){
	g_zllTouchLink.keyType = keyType;
	ss_ib.touchLinkKey = key;
}

_CODE_ZCL_ void  touchlink_commissionTxPowerSet(u8 power){
	g_zllTouchLink.commissionTxPower = power;
}

/*
*
*
*/
_CODE_ZCL_ u8 zcl_touchLinkCmdHandler(zclIncoming_t *pInMsg){
	if (pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR) {
		return zcl_touchLinkClientCmdHandler(pInMsg);
	} else {
		return zcl_zllCommissionServerCmdHandler(pInMsg);
	}
}


/*
	*  zcl_touchlink_register
	*
*/
_CODE_ZCL_ status_t zcl_touchlink_register(u8 endpoint, const zcl_touchlinkAppCallbacks_t *cb){
	g_zllCommission.appCb = cb;
	zcl_touchLinkInit();
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_TOUCHLINK_COMMISSIONING, 0, NULL, zcl_touchLinkCmdHandler, NULL);
}


u8 deviceInfoRsp = 0;

