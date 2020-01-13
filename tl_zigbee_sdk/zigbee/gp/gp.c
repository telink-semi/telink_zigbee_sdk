/********************************************************************************************************
 * @file     gp.c
 *
 * @brief	 Coordinator and Router should support GPPB.
 *
 * @author
 * @date     Dec. 1, 2016
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
#include "../zcl/zcl_include.h"
#include "gp.h"



#if GP_SUPPORT_ENABLE
/**********************************************************************
 * GLOBAL VARIABLES
 */
const u8 g_gpSharedKey[SEC_KEY_LEN] = GP_SHARED_KEY;








/**********************************************************************
 * LOCAL VARIABLES
 */
/**
 *  @brief Callback for user Application
 */
gp_appCb_t *gpAppCb = NULL;


/**********************************************************************
 * FUNCTIONS
 */
/**
 *  @brief Local functions
 */

static void gpCommissioningWindowTimeoutStop(void);

/*********************************************************************
 * @fn      gp_init
 *
 * @brief   This function initialize the Green Power EndPoint information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void gp_init(void)
{
	/* Initialize GP Stub */
	gpStubInit();

	/* Register GP EndPoint descriptor */
	af_endpointRegister(GREEN_POWER_ENDPOINT, (af_simple_descriptor_t *)&gp_simpleDesc, zcl_rx_handler, NULL);

	/* Register GP ZCL command callBacks */
	zcl_register(GREEN_POWER_ENDPOINT, GP_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_gpClusterList);

	gp_ProxyTabInit();

	COPY_U24TOBUFFER(zclGpAttr_gppFunc, GPP_FUNCTIONALITY);
	COPY_U24TOBUFFER(zclGpAttr_gppActiveFunc, GPP_ACTIVE_FUNCTIONALITY);

	zclGpAttr_gpSharedSecKeyType = GP_SEC_KEY_TYPE_GPD_GROUP_KEY;
	memcpy(zclGpAttr_gpSharedSecKey, g_gpSharedKey, SEC_KEY_LEN);
}

/*********************************************************************
 * @fn      gp_registerAppCb
 *
 * @brief   This function for application layer to register GP callBacks.
 *
 * @param   cb
 *
 * @return  None
 */
void gp_registerAppCb(gp_appCb_t *cb)
{
	gpAppCb = cb;
}









u16 gpComWindowCnt = 0;
static void gpCommissioningWindowTimeoutStop(void)
{
	//printf("windowTimeoutStop\n");

	if(g_gpCtx.commissioningWindowTimeoutEvt){
		gpComWindowCnt = 0;
		TL_ZB_TIMER_CANCEL(&g_gpCtx.commissioningWindowTimeoutEvt);
	}
}

static s32 gpCommissioningWindowTimeoutCb(void *arg)
{
	u16 timeout = (u16)((u32)arg);

	if(++gpComWindowCnt >= timeout){
		gpComWindowCnt = 0;

		//printf("gpCommWindowTimeoutCb\n");

		gpExitCommissioningMode();

		//notify up layer the current commissioning mode
		if(gpAppCb && gpAppCb->gpCommissioningModeCb){
			gpAppCb->gpCommissioningModeCb(g_gpCtx.gpInCommMode);
		}

		g_gpCtx.commissioningWindowTimeoutEvt = NULL;
		return -1;
	}

	return 0;
}

u8 gp_proxyTabEntryUpdate(gpProxyTabEntry_t *pEntry, zcl_gp_pairingCmd_t *pCmd)
{
	if(pCmd->options.bits.addSink){
		if(pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST){
			if(lwSinkAddrListAdd(pEntry, pCmd->sinkIeeeAddr, pCmd->sinkNwkAddr) == FAILURE){
				return FAILURE;
			}
		}else if(pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID){
			u16 alias = pCmd->options.bits.assignedAliasPresent ? pCmd->assignedAlias : gpAliasSrcAddrDerived(pCmd->options.bits.appId, pCmd->gpdId);
			if(sinkGroupListAdd(pEntry, pCmd->sinkGroupID, alias) == FAILURE){
				return FAILURE;
			}
		}else if(pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID){
			pEntry->options.bits.assignedAlias = pCmd->options.bits.assignedAliasPresent;
			pEntry->gpdAssignedAlias = pCmd->assignedAlias;
		}

		if(!pEntry->used){
			pEntry->used = 1;
			g_gpProxyTab.gpProxyTabNum++;
		}

		pEntry->options.bits.appId = pCmd->options.bits.appId;
		pEntry->options.bits.entryActive = TRUE;
		pEntry->options.bits.entryValid = TRUE;
		pEntry->options.bits.seqNumCap |= pCmd->options.bits.gpdMacSeqNumCap;
		pEntry->options.bits.lightWeightUnicastGPS |= (pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST) ? TRUE : FALSE;
		pEntry->options.bits.derivedGroupGPS |= (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID) ? TRUE : FALSE;
		pEntry->options.bits.commGroupGPS |= (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) ? TRUE : FALSE;
		pEntry->options.bits.firstToForward = FALSE;
		pEntry->options.bits.inRange = FALSE;
		pEntry->options.bits.gpdFixed |= pCmd->options.bits.gpdFixed ? TRUE : FALSE;
		pEntry->options.bits.hasAllUnicastRoutes |= (pCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST) ? TRUE : FALSE;
		//pEntry->options.bits.assignedAlias = pCmd->options.bits.assignedAliasPresent;
		pEntry->options.bits.secUse = (pCmd->options.bits.secLevel > 1) ? TRUE : FALSE;
		pEntry->options.bits.optsExtension = FALSE;
		if(pEntry->options.bits.appId == GP_APP_ID_SRC_ID){
			pEntry->gpdId.srcId = pCmd->gpdId.srcId;
		}else if(pEntry->options.bits.appId == GP_APP_ID_GPD){
			ZB_64BIT_ADDR_COPY(pEntry->gpdId.gpdIeeeAddr, pCmd->gpdId.gpdIeeeAddr);
		}
		pEntry->endpoint = pCmd->endpoint;
		//pEntry->gpdAssignedAlias = pCmd->assignedAlias;
		pEntry->secOptions.bits.secLevel = pCmd->options.bits.secLevel;
		pEntry->secOptions.bits.secKeyType = pCmd->options.bits.secKeyType;
		pEntry->gpdSecFrameCnt = pCmd->gpdSecFrameCnt;
		memcpy(pEntry->gpdKey, pCmd->gpdKey, SEC_KEY_LEN);
		pEntry->groupcastRadius = pCmd->options.bits.groupcastRadiusPresent ? pCmd->groupcastRadius : pEntry->groupcastRadius;
		pEntry->searchCnt = 0;
	}else{
		if(pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST){
			if(lwSinkAddrListRemove(pEntry, pCmd->sinkIeeeAddr, pCmd->sinkNwkAddr) == FAILURE){
				return FAILURE;
			}
		}else if(pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID){
			if(sinkGroupListRemove(pEntry, pCmd->sinkGroupID) == FAILURE){
				return FAILURE;
			}
		}

		if((pEntry->lwSinkCnt == 0) && (pEntry->sinkGroupCnt == 0) && !pEntry->options.bits.derivedGroupGPS){
			gp_proxyTabEntryClear(pEntry);
		}
	}

	return SUCCESS;
}

static u8 gp_pairingUpdateProxyTab(zcl_gp_pairingCmd_t *pCmd)
{
	gpProxyTabEntry_t *pEntry = gp_getProxyTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);

	if(pCmd->options.bits.removeGPD){
		if(pEntry){
			gp_proxyTabEntryClear(pEntry);
			return SUCCESS;
		}
		return FAILURE;
	}

	if(pCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST){
		//not support
		return FAILURE;
	}

	if(pCmd->options.bits.secLevel == GP_SEC_LEVEL_RESERVED){
		return FAILURE;
	}

	//not found the entry
	if(!pEntry){
		if(pCmd->options.bits.addSink){
			pEntry = gp_proxyTabEntryFreeGet();
			if(!pEntry){
				return FAILURE;
			}
		}else{
			return FAILURE;
		}
	}

	return gp_proxyTabEntryUpdate(pEntry, pCmd);
}

/*********************************************************************
 * @fn      gpPairingCmdProcess
 *
 * @brief	Handler for ZCL GP Pairing command.
 *
 * @param   cmd
 *
 * @return  None
 */
static void gpPairingCmdProcess(zcl_gp_pairingCmd_t *pCmd)
{
	if(gp_pairingUpdateProxyTab(pCmd) == SUCCESS){
		//update the proxy table
		//printf("updated\n");
		gpProxyTabUpdate();

		if(g_gpCtx.gpCommissioningModeOpt.bits.exitMode & EXIT_ON_FIRST_PAIRING_SUCCESS){
			gpCommissioningWindowTimeoutStop();
			gpExitCommissioningMode();

			//notify up layer the current commissioning mode
			if(gpAppCb && gpAppCb->gpCommissioningModeCb){
				gpAppCb->gpCommissioningModeCb(g_gpCtx.gpInCommMode);
			}
		}
	}else{
		//printf("failed\n");
	}

	if( (NIB_NETWORK_ADDRESS() == gpAliasSrcAddrDerived(pCmd->options.bits.appId, pCmd->gpdId)) ||
		(NIB_NETWORK_ADDRESS() == pCmd->assignedAlias) ){
		tl_zbNwkAddrConflictStatusSend(NWK_BROADCAST_RX_ON_WHEN_IDLE, NIB_NETWORK_ADDRESS(), TRUE);

		NIB_NETWORK_ADDRESS() = g_zbMacPib.shortAddress = tl_zbNwkStochasticAddrCal();

		zb_zdoSendDevAnnance();
	}
}

/*********************************************************************
 * @fn      gpProxyTabReqCmdProcess
 *
 * @brief	Handler for ZCL GP Proxy Table Request command.
 *
 * @param   cmd
 *
 * @return  None
 */
static void gpProxyTabReqCmdProcess(u16 srcAddr, zcl_gp_proxyTabReqCmd_t *pCmd)
{
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstAddr.shortAddr = srcAddr;
	dstEpInfo.dstEp = GREEN_POWER_ENDPOINT;
	dstEpInfo.profileId = GP_PROFILE_ID;

	zcl_gp_proxyTabRspCmd_t proxyTabRspCmd;
	TL_SETSTRUCTCONTENT(proxyTabRspCmd, 0);

	u8 *pBuf = NULL;

	proxyTabRspCmd.totalTabEntries = gp_getProxyTabEntryTotalNum();

	if(proxyTabRspCmd.totalTabEntries == 0){
		proxyTabRspCmd.status = ZCL_STA_SUCCESS;
		proxyTabRspCmd.startIdx = (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_GPD_ID) ? 0xFF : pCmd->index;
		proxyTabRspCmd.entriesCnt = 0;
		proxyTabRspCmd.entriesLen = 0;

		zcl_gp_proxyTableRspCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, &proxyTabRspCmd);
		return;
	}

	if(pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_GPD_ID){
		gpProxyTabEntry_t *pEntry = gp_getProxyTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);
		if(pEntry){
			u8 len = gp_getProxyTabEntryLen(pEntry);
			pBuf = (u8 *)ev_buf_allocate(len);
			if(!pBuf){
				return;
			}

			gp_buildProxyTabEntryFormat(pEntry, pBuf);

			proxyTabRspCmd.status = ZCL_STA_SUCCESS;
			proxyTabRspCmd.startIdx = 0xFF;
			proxyTabRspCmd.entriesCnt = 1;
			proxyTabRspCmd.entriesLen = len;
			proxyTabRspCmd.proxyTabEntry = pBuf;
		}else{
			proxyTabRspCmd.status = ZCL_STA_NOT_FOUND;
			proxyTabRspCmd.startIdx = 0xFF;
			proxyTabRspCmd.entriesCnt = 0;
			proxyTabRspCmd.entriesLen = 0;
		}
	}else if(pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX){
		proxyTabRspCmd.status = ZCL_STA_SUCCESS;
		proxyTabRspCmd.startIdx = pCmd->index;

		u8 entryLen = 0;
		u8 maxEntryLen = 0;
		u8 activeEntryCnt = 0;

		for(u8 i = 0; i < zclGpAttr_gppMaxProxyTabEntries; i++){
			maxEntryLen = entryLen;

			if(g_gpProxyTab.gpProxyTab[i].used){
				activeEntryCnt++;

				if(pCmd->index <= activeEntryCnt - 1){
					entryLen += gp_getProxyTabEntryLen(&g_gpProxyTab.gpProxyTab[i]);

					if(entryLen >= ZCL_GP_MAX_PROXY_TABLE_ATTR_LEN){
						entryLen = maxEntryLen;
						break;
					}else{
						proxyTabRspCmd.entriesCnt++;
					}
				}
			}
		}

		activeEntryCnt = 0;
		proxyTabRspCmd.entriesLen = entryLen;

		u8 entriesCnt = proxyTabRspCmd.entriesCnt;

		if(entryLen){
			pBuf = (u8 *)ev_buf_allocate(entryLen);
			if(!pBuf){
				return;
			}

			u8 *ptr = pBuf;

			for(u8 i = 0; i < zclGpAttr_gppMaxProxyTabEntries; i++){
				if(g_gpProxyTab.gpProxyTab[i].used){
					activeEntryCnt++;

					if(pCmd->index <= activeEntryCnt - 1){
						if(entriesCnt){
							ptr += gp_buildProxyTabEntryFormat(&g_gpProxyTab.gpProxyTab[i], ptr);
							entriesCnt--;
						}else{
							break;
						}
					}
				}
			}

			proxyTabRspCmd.proxyTabEntry = pBuf;
		}
	}

	zcl_gp_proxyTableRspCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, &proxyTabRspCmd);

	if(pBuf){
		ev_buf_free(pBuf);
	}
}

/*********************************************************************
 * @fn      gpProxyCommissioningModeCmdProcess
 *
 * @brief	Handler for ZCL GP Proxy Commissioning Mode command.
 *
 * @param   cmd
 *
 * @return  None
 */
static void gpProxyCommissioningModeCmdProcess(u16 srcAddr, zcl_gp_proxyCommissioningModeCmd_t *pCmd)
{
	if(g_gpCtx.gpInCommMode && (g_gpCtx.gpCommissionerAddr != srcAddr)){
		//if we are in commissioning mode and the source address is not the device that set the
		//proxy in commissioning mode, drop the request.
		return;
	}

	//printf("gpProxyCommModeCmdCb\n");

	if(pCmd->options.bits.action){
		g_gpCtx.gpCommissioningModeOpt = pCmd->options;
		g_gpCtx.gpCommissionerAddr = srcAddr;
		g_gpCtx.gpInCommMode = TRUE;

		if(pCmd->options.bits.commWindowPresent){
			g_gpCtx.gpCommissioningWindow = pCmd->commissioningWindow;
		}

		if(pCmd->options.bits.channelPresent){
			//in the current version of the GP spec, the channel present sub-field shall always be set to 0,
			//and the channel field shall not be present. A.3.3.5.3
		}

		//start the commissioning window timeout
		if(!g_gpCtx.commissioningWindowTimeoutEvt){
			gpComWindowCnt = 0;
			g_gpCtx.commissioningWindowTimeoutEvt = TL_ZB_TIMER_SCHEDULE(gpCommissioningWindowTimeoutCb, (void *)((u32)g_gpCtx.gpCommissioningWindow), 1 * 1000 * 1000);
		}else{
			gpComWindowCnt = 0;
		}
	}else{
		gpCommissioningWindowTimeoutStop();
		gpExitCommissioningMode();
	}

	//notify up layer the current commissioning mode
	if(gpAppCb && gpAppCb->gpCommissioningModeCb){
		gpAppCb->gpCommissioningModeCb(g_gpCtx.gpInCommMode);
	}
}

/*********************************************************************
 * @fn      gpResponseCmdProcess
 *
 * @brief	Handler for ZCL GP Response command.
 *
 * @param   cmd
 *
 * @return  None
 */
static void gpResponseCmdProcess(zcl_gp_responseCmd_t *pCmd)
{
	//printf("gpResponseCmdCb\n");

	if(!g_gpCtx.gpInCommMode){
		return;
	}

	if(pCmd->tempMasterShortAddr == NWK_NIB().nwkAddr){
		g_gpCtx.firstToForward = TRUE;//TODO: not sure
	}else{
		g_gpCtx.firstToForward = FALSE;
		return;
	}

	gp_data_req_t gpDataReq;
	TL_SETSTRUCTCONTENT(gpDataReq, 0);

	gpDataReq.action = TRUE;
	if(pCmd->gpdCmdID == GP_CHANNEL_CONFIGURATION_COMMAND_ID){
		gpDataReq.gpepHandle = GP_HANDLE_CHANNEL_CONFIGURATION;
	}else{
		gpDataReq.gpepHandle = dGpStubHandleGet();
	}
	gpDataReq.txOptions.useGpTxQueue = 1;
	gpDataReq.txOptions.txOnMatchingEndpoint = pCmd->options.bits.transmitOnEndpointMatch;
	gpDataReq.appId = pCmd->options.bits.appId;
	memcpy((u8 *)&gpDataReq.gpdId, (u8 *)&pCmd->gpdId, sizeof(gpdId_t));
	gpDataReq.endpoint = pCmd->endpoint;
	gpDataReq.gpdCmdId = pCmd->gpdCmdID;
	gpDataReq.gpdAsduLen = pCmd->payloadLen;
	gpDataReq.gpdAsdu = pCmd->pGpdCmdPayload;

	if(gpDataReq.action){
		if(pCmd->gpdCmdID == GP_CHANNEL_CONFIGURATION_COMMAND_ID){
			gpd_channelConfigurationCmd_payload_t channel;
			TL_SETSTRUCTCONTENT(channel, 0);

			channel.channel = *(pCmd->pGpdCmdPayload);

			gpSwitchToTransmitChannel(channel, pCmd->tempMasterTxChannel);
		}
	}

	gpTranimitGPDF(&gpDataReq);
}

/*********************************************************************
 * @fn      gpResponseCmdProcess
 *
 * @brief	Handler for ZCL Green Power command.
 *
 * @param   cmd
 *
 * @return  None
 */
status_t zcl_gpCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == GREEN_POWER_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_GP_PAIRING:
					gpPairingCmdProcess((zcl_gp_pairingCmd_t *)cmdPayload);
					break;
				case ZCL_CMD_GP_PROXY_COMMISSIONING_MODE:
					gpProxyCommissioningModeCmdProcess(pAddrInfo->srcAddr, (zcl_gp_proxyCommissioningModeCmd_t *)cmdPayload);
					break;
				case ZCL_CMD_GP_RESPONSE:
					gpResponseCmdProcess((zcl_gp_responseCmd_t *)cmdPayload);
					break;
				case ZCL_CMD_GP_PROXY_TABLE_REQUEST:
					gpProxyTabReqCmdProcess(pAddrInfo->srcAddr, (zcl_gp_proxyTabReqCmd_t *)cmdPayload);
					break;
				default:
					status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
					break;
			}
		}else{
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
		}
	}

	return status;
}

#endif /* GP_SUPPORT_ENABLE */
