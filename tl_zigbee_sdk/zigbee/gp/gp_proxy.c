/********************************************************************************************************
 * @file    gp_proxy.c
 *
 * @brief   This is the source file for gp_proxy
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

/**********************************************************************
 * INCLUDES
 */
#include "../common/includes/zb_common.h"
#include "../zcl/zcl_include.h"
#include "gp_base.h"
#include "gp.h"
#include "gp_proxy.h"

#if GP_SUPPORT_ENABLE
/**********************************************************************
 * GLOBAL VARIABLES
 */
gpp_ctx_t g_gppCtx = {
		.aliasConflictTimeoutEvt		= NULL,
		.commissioningWindowTimeoutEvt	= NULL,
		.gpAliasConflictAddr			= NWK_BROADCAST_RESERVED,
		.gpCommissionerAddr				= NWK_BROADCAST_RESERVED,
		.gpCommissioningWindow			= GPP_COMMISSIONING_WINDOW_DEFAULT,
		.gpCommissioningModeOpt.opts	= 0,
		.gppInCommMode 					= FALSE,
		.firstToForward					= FALSE,
};

/**********************************************************************
 * FUNCTIONS
 */
void gp_proxyInit(void)
{
	gpProxyTabInit();

	COPY_U24TOBUFFER(zclGpAttr_gppFunc, GPP_FUNCTIONALITY);
	COPY_U24TOBUFFER(zclGpAttr_gppActiveFunc, GPP_ACTIVE_FUNCTIONALITY);
}

gpSecRsp_status_t gpProxySecOperation(u8 appId, gpdId_t gpdId,
									  u8 endpoint, u32 gpdSecFrameCnt,
									  u8 gpdfSecurityLevel, u8 gpdfKeyType,
									  u8 *pKeyType, u8 *pKey)
{
	u8 secChkFail = FALSE;
	u8 epChkFail = FALSE;
	gpSecRsp_status_t status = GP_SEC_RSP_ERROR;

	//A.3.7.3.3
	gpProxyTabEntry_t *pGpProxyTabEntry = gp_getProxyTabByGpdId(appId, gpdId);

	//printf("gpProxySecOperation: proxyEntry = %x\n", pGpProxyTabEntry);

	if(pGpProxyTabEntry){
		if(pGpProxyTabEntry->options.bits.entryActive){
			if(pGpProxyTabEntry->options.bits.secUse){
				//check security level and frame counter.
				if((gpdfSecurityLevel != pGpProxyTabEntry->secOptions.bits.secLevel) ||
				   (gpdSecFrameCnt <= pGpProxyTabEntry->gpdSecFrameCnt)){
					secChkFail = TRUE;
				}
				//check security key type mapping.
				else if(gpSecKeyTypeMappingChk(pGpProxyTabEntry->secOptions.bits.secKeyType, gpdfKeyType)){
					secChkFail = TRUE;
				}
			}else if(gpdfSecurityLevel != GP_SEC_LEVEL_NO_SECURITY){
				secChkFail = TRUE;
			}

			if(secChkFail == TRUE){
				if(g_gppCtx.gppInCommMode == FALSE){
					return GP_SEC_RSP_STATUS_DROP_FRAME;
				}else{
					if(gpdfSecurityLevel == GP_SEC_LEVEL_NO_SECURITY){
						return GP_SEC_RSP_STATUS_MATCH;
					}else{
						return GP_SEC_RSP_STATUS_PASS_UNPROCESSED;
					}
				}
			}else{//security check success
				if(appId == GP_APP_ID_GPD){
					if((endpoint != pGpProxyTabEntry->endpoint) &&
					   (endpoint != 0) && (endpoint != 0xFF)){
						epChkFail = TRUE;
					}
				}

				if(epChkFail){
					status = GP_SEC_RSP_STATUS_TX_THEN_DROP;
				}else{
					status = GP_SEC_RSP_STATUS_MATCH;
				}

				if(gpdfSecurityLevel == GP_SEC_LEVEL_NO_SECURITY){
					return status;
				}

				//key recovery
				return gpKeyRecovery(gpdfKeyType, pGpProxyTabEntry->secOptions.bits.secKeyType, pGpProxyTabEntry->gpdKey, status, pKeyType, pKey);
			}
		}else{
			if(g_gppCtx.gppInCommMode == FALSE){
				//TODO: update the SearchCounter
				return GP_SEC_RSP_STATUS_DROP_FRAME;
			}
		}
	}

	//inactive or no entry
	if(gpdfKeyType == 0){//shared key
		//if GPDF is encrypted and there is no shared key, then pass unprocessed.
		if((gpdfSecurityLevel != GP_SEC_LEVEL_NO_SECURITY) &&
		    ZB_SEC_KEY_IS_NULL(zclGpAttr_gpSharedSecKey)){
			return GP_SEC_RSP_STATUS_PASS_UNPROCESSED;
		}

		memcpy(pKey, zclGpAttr_gpSharedSecKey, SEC_KEY_LEN);
		*pKeyType = (gpdfSecurityLevel == GP_SEC_LEVEL_NO_SECURITY) ? GP_SEC_KEY_TYPE_NO_KEY : zclGpAttr_gpSharedSecKeyType;

		return GP_SEC_RSP_STATUS_MATCH;
	}else{//individual key
		*pKeyType = GP_SEC_KEY_TYPE_OUT_OF_THE_BOX_GPD_KEY;//TODO: not sure

		return GP_SEC_RSP_STATUS_PASS_UNPROCESSED;
	}
}

static void gpExitCommissioningMode(void)
{
	g_gppCtx.gpCommissioningModeOpt.opts = 0;
	g_gppCtx.gppInCommMode = FALSE;
	g_gppCtx.gpCommissionerAddr = NWK_BROADCAST_RESERVED;
	g_gppCtx.gpCommissioningWindow = GPP_COMMISSIONING_WINDOW_DEFAULT;

	//A.3.2.8, release gpTxQueue
	gpTxQueueFree();
}

static void gpCommissioningWindowTimeoutStop(void)
{
	//printf("windowTimeoutStop\n");

	if(g_gppCtx.commissioningWindowTimeoutEvt){
		TL_ZB_TIMER_CANCEL(&g_gppCtx.commissioningWindowTimeoutEvt);
	}
}

static s32 gpCommissioningWindowTimeoutCb(void *arg)
{
	//printf("gpCommWindowTimeoutCb\n");

	gpExitCommissioningMode();

	//notify up layer the current commissioning mode
	if(gpAppCb && gpAppCb->gpCommissioningModeCb){
		gpAppCb->gpCommissioningModeCb(g_gppCtx.gppInCommMode);
	}

	g_gppCtx.commissioningWindowTimeoutEvt = NULL;
	return -1;
}

static status_t gp_proxyTabEntryUpdate(gpProxyTabEntry_t *pEntry, zcl_gp_pairingCmd_t *pCmd)
{
	if(pCmd->options.bits.addSink){
		if(pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST){
			if(lwSinkAddrListAdd(pEntry, pCmd->sinkIeeeAddr, pCmd->sinkNwkAddr) == FAILURE){
				return ZCL_STA_INSUFFICIENT_SPACE;
			}
		}else if(pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID){
			u16 alias = pCmd->options.bits.assignedAliasPresent ? pCmd->assignedAlias : 0xffff;
			if(sinkGroupListAdd(pEntry, pCmd->sinkGroupID, alias) == FAILURE){
				return ZCL_STA_INSUFFICIENT_SPACE;
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
				return ZCL_STA_NOT_FOUND;
			}
		}else if(pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID){
			if(sinkGroupListRemove(pEntry, pCmd->sinkGroupID) == FAILURE){
				return ZCL_STA_NOT_FOUND;
			}
		}

		if((pEntry->lwSinkCnt == 0) && (pEntry->sinkGroupCnt == 0) && !pEntry->options.bits.derivedGroupGPS){
			gp_proxyTabEntryClear(pEntry);
		}
	}

	return ZCL_STA_SUCCESS;
}

static status_t gp_pairingUpdateProxyTab(zcl_gp_pairingCmd_t *pCmd)
{
	gpProxyTabEntry_t *pEntry = gp_getProxyTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);

	if(pCmd->options.bits.removeGPD){
		if(pEntry){
			gp_proxyTabEntryClear(pEntry);
			return ZCL_STA_SUCCESS;
		}
		return ZCL_STA_NOT_FOUND;
	}

	if(pCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST){
		//not support
		return ZCL_STA_INVALID_FIELD;
	}

	if(pCmd->options.bits.secLevel == GP_SEC_LEVEL_RESERVED){
		return ZCL_STA_INVALID_FIELD;
	}

	//not found the entry
	if(!pEntry){
		if(pCmd->options.bits.addSink){
			pEntry = gp_proxyTabEntryFreeGet();
			if(!pEntry){
				return ZCL_STA_INSUFFICIENT_SPACE;
			}
		}else{
			return ZCL_STA_NOT_FOUND;
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
static status_t gpPairingCmdProcess(zcl_gp_pairingCmd_t *pCmd)
{
	status_t status = gp_pairingUpdateProxyTab(pCmd);

	//printf("gpPairingCmdRcv: sta = %x\n", status);

	if(status == ZCL_STA_SUCCESS){
		//update the proxy table
		gpProxyTabUpdate();

		if(g_gppCtx.gppInCommMode && (g_gppCtx.gpCommissioningModeOpt.bits.exitMode & EXIT_ON_FIRST_PAIRING_SUCCESS)){
			gpCommissioningWindowTimeoutStop();
			gpExitCommissioningMode();

			//notify up layer the current commissioning mode
			if(gpAppCb && gpAppCb->gpCommissioningModeCb){
				gpAppCb->gpCommissioningModeCb(g_gppCtx.gppInCommMode);
			}
		}
	}

	if( (NIB_NETWORK_ADDRESS() == gpAliasSrcAddrDerived(pCmd->options.bits.appId, pCmd->gpdId)) ||
		(NIB_NETWORK_ADDRESS() == pCmd->assignedAlias) ){
		tl_zbNwkAddrConflictStatusSend(NWK_BROADCAST_RX_ON_WHEN_IDLE, NIB_NETWORK_ADDRESS(), TRUE);

		NIB_NETWORK_ADDRESS() = g_zbMacPib.shortAddress = tl_zbNwkStochasticAddrCal();

		u16 ref = 0;
		tl_zbNwkAddrMapAdd(g_zbMacPib.shortAddress, g_zbMacPib.extAddress, &ref);
		zb_info_save(NULL);

		zb_zdoSendDevAnnance();
	}

	return status;
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
void gpProxyCommissioningModeCmdProcess(u16 srcAddr, zcl_gp_proxyCommissioningModeCmd_t *pCmd)
{
	if(g_gppCtx.gppInCommMode && (g_gppCtx.gpCommissionerAddr != srcAddr)){
		//if we are in commissioning mode and the source address is not the device that set the
		//proxy in commissioning mode, drop the request.
		return;
	}

	//printf("gppCommMode: srcAddr = %x, action = %x\n", srcAddr, pCmd->options.bits.action);

	if(pCmd->options.bits.action){
		g_gppCtx.gpCommissioningModeOpt = pCmd->options;
		g_gppCtx.gpCommissionerAddr = srcAddr;
		g_gppCtx.gppInCommMode = TRUE;

		if(pCmd->options.bits.channelPresent){
			//in the current version of the GP spec, the channel present sub-field shall always be set to 0,
			//and the channel field shall not be present. A.3.3.5.3
		}

		if(pCmd->options.bits.commWindowPresent){
			g_gppCtx.gpCommissioningWindow = pCmd->commissioningWindow;

			//start the commissioning window timeout
			gpCommissioningWindowTimeoutStop();
			if(g_gppCtx.gpCommissioningWindow){
				g_gppCtx.commissioningWindowTimeoutEvt = TL_ZB_TIMER_SCHEDULE(gpCommissioningWindowTimeoutCb,
																			  NULL,
																			  g_gppCtx.gpCommissioningWindow * 1000);
			}
		}
	}else{
		gpCommissioningWindowTimeoutStop();
		gpExitCommissioningMode();
	}

	//notify up layer the current commissioning mode
	if(gpAppCb && gpAppCb->gpCommissioningModeCb){
		gpAppCb->gpCommissioningModeCb(g_gppCtx.gppInCommMode);
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
void gpResponseCmdProcess(zcl_gp_responseCmd_t *pCmd)
{
	//printf("GPP_gpResponseCmdCb: mode = %x\n", g_gppCtx.gppInCommMode);

	gp_data_req_t gpDataReq;
	TL_SETSTRUCTCONTENT(gpDataReq, 0);

	if(pCmd->tempMasterShortAddr == NIB_NETWORK_ADDRESS()){
		g_gppCtx.firstToForward = TRUE;//TODO: not sure
		gpDataReq.action = TRUE;
	}else{
		g_gppCtx.firstToForward = FALSE;
		gpDataReq.action = FALSE;
	}

	gpDataReq.txOptions.useGpTxQueue = 1;
	gpDataReq.txOptions.txOnMatchingEndpoint = pCmd->options.bits.transmitOnEndpointMatch;
	gpDataReq.appId = pCmd->options.bits.appId;
	memcpy((u8 *)&gpDataReq.gpdId, (u8 *)&pCmd->gpdId, sizeof(gpdId_t));
	gpDataReq.endpoint = pCmd->endpoint;
	gpDataReq.gpdCmdId = pCmd->gpdCmdID;
	gpDataReq.gpdAsduLen = pCmd->payloadLen;
	gpDataReq.gpdAsdu = pCmd->pGpdCmdPayload;

	gpTranimitGPDF(&gpDataReq);

	if(gpDataReq.action){
		if(pCmd->gpdCmdID == GPDF_CMD_ID_CHANNEL_CONFIGURATION){
			gpd_channelConfigurationCmd_payload_t channel;
			TL_SETSTRUCTCONTENT(channel, 0);

			channel.channel = *(pCmd->pGpdCmdPayload);

			gpSwitchToTransmitChannel((u8)channel.bits.operationChannel, pCmd->tempMasterTxChannel);
		}
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
static void gpProxyTabReqCmdProcess(zclIncomingAddrInfo_t *pAddrInfo, zcl_gp_proxyTabReqCmd_t *pCmd)
{
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
	dstEpInfo.dstEp = pAddrInfo->srcEp;
	dstEpInfo.profileId = pAddrInfo->profileId;

	zcl_gp_proxyTabRspCmd_t proxyTabRspCmd;
	TL_SETSTRUCTCONTENT(proxyTabRspCmd, 0);

	u8 *pBuf = NULL;

	proxyTabRspCmd.totalTabEntries = gp_getProxyTabEntryTotalNum();

	if(proxyTabRspCmd.totalTabEntries == 0){
		proxyTabRspCmd.status = ZCL_STA_NOT_FOUND;
		proxyTabRspCmd.startIdx = (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_GPD_ID) ? 0xFF : pCmd->index;
		proxyTabRspCmd.entriesCnt = 0;
		proxyTabRspCmd.entriesLen = 0;

		zcl_gp_proxyTableRspCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pAddrInfo->seqNum, &proxyTabRspCmd);
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

			memset(pBuf, 0, len);

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
		proxyTabRspCmd.status = ZCL_STA_NOT_FOUND;
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
			proxyTabRspCmd.status = ZCL_STA_SUCCESS;

			pBuf = (u8 *)ev_buf_allocate(entryLen);
			if(!pBuf){
				return;
			}

			memset(pBuf, 0, entryLen);

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

	zcl_gp_proxyTableRspCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pAddrInfo->seqNum, &proxyTabRspCmd);

	if(pBuf){
		ev_buf_free(pBuf);
	}
}

status_t zcl_gpServerCmdHandler(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	switch(cmdId){
		case ZCL_CMD_GP_PAIRING:
			status = gpPairingCmdProcess((zcl_gp_pairingCmd_t *)cmdPayload);
			break;
		case ZCL_CMD_GP_PROXY_COMMISSIONING_MODE:
			gpProxyCommissioningModeCmdProcess(pAddrInfo->srcAddr, (zcl_gp_proxyCommissioningModeCmd_t *)cmdPayload);
			break;
		case ZCL_CMD_GP_RESPONSE:
			gpResponseCmdProcess((zcl_gp_responseCmd_t *)cmdPayload);
			break;
		case ZCL_CMD_GP_PROXY_TABLE_REQUEST:
			gpProxyTabReqCmdProcess(pAddrInfo, (zcl_gp_proxyTabReqCmd_t *)cmdPayload);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}


static u8 gpParseDataInd(gp_data_ind_t *pGpDataInd, zcl_gp_notificationCmd_t *pCmd)
{
	gpdId_t gpdId;
	TL_SETSTRUCTCONTENT(gpdId, 0);

	if(pGpDataInd->appId == GP_APP_ID_SRC_ID){
		gpdId.srcId = pGpDataInd->srcId;
	}else if(pGpDataInd->appId == GP_APP_ID_GPD){
		ZB_64BIT_ADDR_COPY(gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
	}

	gpProxyTabEntry_t *pGpProxyTabEntry = gp_getProxyTabByGpdId(pGpDataInd->appId, gpdId);
	if(!pGpProxyTabEntry){
		return FAILURE;
	}

#if 0
	if(pGpDataInd->appId == GP_APP_ID_GPD){
		if((pGpDataInd->endpoint != 0) && (pGpDataInd->endpoint != 0xFF) && (pGpDataInd->endpoint != pGpProxyTabEntry->endpoint)){
			return FAILURE;
		}
	}
#endif

	if(!pGpProxyTabEntry->options.bits.inRange){
		pGpProxyTabEntry->options.bits.inRange = 1;
	}

	if((pGpDataInd->status == GP_DATA_IND_STATUS_AUTH_FAILURE) || (pGpDataInd->status == GP_DATA_IND_STATUS_UNPROCESSED)){
		return FAILURE;
	}else{
		pGpProxyTabEntry->gpdSecFrameCnt = pGpDataInd->gpdfSecurityLevel ? pGpDataInd->gpdSecFrameCnt : pGpDataInd->seqNum;
	}

#if 0
	if(pGpDataInd->gpdCmdId != GPDF_CMD_ID_COMMISSIONING){
		if(gpSecFreshnessChk(pGpProxyTabEntry->secOptions.bits.secKeyType,
							 pGpProxyTabEntry->secOptions.bits.secLevel,
							 pGpDataInd->gpdfKeyType,
							 pGpDataInd->gpdfSecurityLevel)){
			if((pGpDataInd->status == GP_DATA_IND_STATUS_AUTH_FAILURE) || (pGpDataInd->status == GP_DATA_IND_STATUS_UNPROCESSED)){
				return FAILURE;
			}
		}else{
			pGpProxyTabEntry->gpdSecFrameCnt = pGpDataInd->gpdfSecurityLevel ? pGpDataInd->gpdSecFrameCnt : pGpDataInd->seqNum;
		}
	}
#endif

	if(pGpDataInd->appId == GP_APP_ID_SRC_ID){
		pCmd->options.bits.appId = GP_APP_ID_SRC_ID;
		pCmd->gpdId.srcId = pGpDataInd->srcId;
	}else if(pGpDataInd->appId == GP_APP_ID_GPD){
		pCmd->options.bits.appId = GP_APP_ID_GPD;
		ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
		pCmd->endpoint = pGpDataInd->endpoint;
	}

	pCmd->options.bits.alsoUnicast = pGpProxyTabEntry->options.bits.lightWeightUnicastGPS;
	pCmd->options.bits.alsoDerivedGroup = pGpProxyTabEntry->options.bits.derivedGroupGPS;
	pCmd->options.bits.alsoCommGroup = pGpProxyTabEntry->options.bits.commGroupGPS;
	pCmd->options.bits.secLevel = pGpProxyTabEntry->secOptions.bits.secLevel;
	pCmd->options.bits.secKeyType = pGpProxyTabEntry->secOptions.bits.secKeyType;
	pCmd->options.bits.rxAfterTx = (pGpDataInd->frameType == GP_NWK_FRAME_TYPE_DATA) ? pGpDataInd->rxAfterTx : 0;
	pCmd->options.bits.gpTxQueueFull = 1;
	pCmd->options.bits.bidirectionalCap = 0;
	pCmd->options.bits.proxyInfoPresent = 1;

	pCmd->gpdSecFrameCnt = pGpDataInd->gpdSecFrameCnt;
	pCmd->gpdCmdID = pGpDataInd->gpdCmdId;
	pCmd->payloadLen = pGpDataInd->gpdAsduLen - 1;
	if(pCmd->payloadLen){
		pCmd->pGpdCmdPayload = &(pGpDataInd->gpdAsdu[1]);
	}
	pCmd->gppShortAddr = NWK_NIB().nwkAddr;

	s8 RSSI = pGpDataInd->rssi;
	(RSSI > 8) ? RSSI = 8 : (RSSI < -109) ? RSSI = -109 : 0;
	RSSI += 110;
	RSSI /= 2;

	pCmd->gppGpdLink.bits.rssi = RSSI;

	u8 LQI = pGpDataInd->lqi ? 2 : 0;

	pCmd->gppGpdLink.bits.lqi = LQI;

	return SUCCESS;
}

static void gpNotificationCmdSend(gp_data_ind_t *pGpDataInd, zcl_gp_notificationCmd_t *pCmd)
{
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	gpProxyTabEntry_t *pEntry = gp_getProxyTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);
	if(!pEntry){
		return;
	}

	dstEpInfo.dstEp = GREEN_POWER_ENDPOINT;
	dstEpInfo.profileId = GP_PROFILE_ID;
	dstEpInfo.radius = (pEntry->groupcastRadius == 0xff) ? 0 : pEntry->groupcastRadius;

	if(pEntry->options.bits.lightWeightUnicastGPS){
		dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEpInfo.useAlias = FALSE;

		for(u8 i = 0; i < pEntry->lwSinkCnt; i++){
			dstEpInfo.dstAddr.shortAddr = pEntry->lightweightSinkAddrList[i].sinkNwkAddr;

			zcl_gp_notificationCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pCmd);
		}
	}

	if(pEntry->options.bits.derivedGroupGPS){
		dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
		dstEpInfo.dstAddr.shortAddr = gpAliasSrcAddrDerived(pEntry->options.bits.appId, pEntry->gpdId);
		dstEpInfo.useAlias = TRUE;
		dstEpInfo.aliasSrcAddr = pEntry->options.bits.assignedAlias ? pEntry->gpdAssignedAlias
																	: gpAliasSrcAddrDerived(pEntry->options.bits.appId, pEntry->gpdId);
		dstEpInfo.aliasSeqNum = pGpDataInd->seqNum;

		zcl_gp_notificationCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pCmd);
	}

	if(pEntry->options.bits.commGroupGPS){
		dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;

		dstEpInfo.useAlias = TRUE;
		dstEpInfo.aliasSeqNum = (pGpDataInd->seqNum - 9) % 256;

		for(u8 i = 0; i < pEntry->sinkGroupCnt; i++){
			dstEpInfo.dstAddr.shortAddr = pEntry->sinkGroupList[i].groupId;
			dstEpInfo.aliasSrcAddr = (pEntry->sinkGroupList[i].alias != 0xffff) ? pEntry->sinkGroupList[i].alias
																				: gpAliasSrcAddrDerived(pEntry->options.bits.appId, pEntry->gpdId);

			zcl_gp_notificationCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pCmd);
		}
	}
}

static void gppOperationalModeProcess(gp_data_ind_t *pGpDataInd)
{
	if(pGpDataInd->status == GP_DATA_IND_STATUS_AUTH_FAILURE){
		return;
	}

	//if the proxy is in operational mode, check the SrcID or GPD IEEE is 0x0, if so, drop it.
	if(((pGpDataInd->appId == GP_APP_ID_SRC_ID) && (pGpDataInd->srcId == 0)) ||
	   ((pGpDataInd->appId == GP_APP_ID_GPD) && (ZB_IEEE_ADDR_IS_ZERO(pGpDataInd->srcAddr.extAddr)))){
		return;
	}

	if((pGpDataInd->gpdCmdId == GPDF_CMD_ID_CHANNEL_REQ) ||
	   (pGpDataInd->gpdCmdId == GPDF_CMD_ID_SUCCESS) ){
		return;
	}

	zcl_gp_notificationCmd_t gpNotificationCmd;
	TL_SETSTRUCTCONTENT(gpNotificationCmd, 0);

	if(gpParseDataInd(pGpDataInd, &gpNotificationCmd) == SUCCESS){
		//update the proxy table
		gpProxyTabUpdate();

		//printf("gpNotif: cmdId = %x\n", pGpDataInd->gpdCmdId);

		if((gpNotificationCmd.gpdCmdID == GPDF_CMD_ID_COMMISSIONING) ||
		   (gpNotificationCmd.gpdCmdID == GPDF_CMD_ID_DECOMMISSIONING)){
			gpNotificationCmd.options.bits.rxAfterTx = 0;
		}

		gpNotificationCmdSend(pGpDataInd, &gpNotificationCmd);
	}
}

static u8 gpParseManteinanceInd(gp_data_ind_t *pGpDataInd, zcl_gp_commissioningNotificationCmd_t *pCmd)
{
	if(pGpDataInd->gpdCmdId != GPDF_CMD_ID_CHANNEL_REQ){
		if(pGpDataInd->appId == GP_APP_ID_SRC_ID){
			pCmd->options.bits.appId = GP_APP_ID_SRC_ID;
			pCmd->gpdId.srcId = pGpDataInd->srcId;
		}else if(pGpDataInd->appId == GP_APP_ID_GPD){
			pCmd->options.bits.appId = GP_APP_ID_GPD;
			ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
			pCmd->endpoint = pGpDataInd->endpoint;
		}
	}

	if(pGpDataInd->gpdCmdId == GPDF_CMD_ID_CHANNEL_REQ){
		pCmd->options.bits.rxAfterTx = 1;
	}else if(pGpDataInd->gpdCmdId == GPDF_CMD_ID_COMMISSIONING){
		pCmd->options.bits.rxAfterTx = pGpDataInd->rxAfterTx;
	}else{
		pCmd->options.bits.rxAfterTx = pGpDataInd->rxAfterTx;
	}

	pCmd->options.bits.secLevel = pGpDataInd->gpdfSecurityLevel;
	pCmd->options.bits.secKeyType = pGpDataInd->gpdfKeyType;

	if((pGpDataInd->status == GP_DATA_IND_STATUS_AUTH_FAILURE) ||
	   (pGpDataInd->status == GP_DATA_IND_STATUS_UNPROCESSED)){
		pCmd->options.bits.secPrcFailed = 1;
		pCmd->mic = pGpDataInd->mic;
	}else{
		pCmd->options.bits.secPrcFailed = 0;
		pCmd->mic = 0xFFFFFFFF;
	}
	pCmd->options.bits.bidirectionalCap = 0;
	pCmd->options.bits.proxyInfoPresent = 1;

	pCmd->gpdSecFrameCnt = pGpDataInd->gpdSecFrameCnt;
	pCmd->gpdCmdID = pGpDataInd->gpdCmdId;
	pCmd->payloadLen = pGpDataInd->gpdAsduLen - 1;
	if(pCmd->payloadLen){
		pCmd->pGpdCmdPayload = &(pGpDataInd->gpdAsdu[1]);
	}
	pCmd->gppShortAddr = NWK_NIB().nwkAddr;

	s8 RSSI = pGpDataInd->rssi;
	(RSSI > 8) ? RSSI = 8 : (RSSI < -109) ? RSSI = -109 : 0;
	RSSI += 110;
	RSSI /= 2;

	pCmd->gppGpdLink.bits.rssi = RSSI;

	u8 LQI = pGpDataInd->lqi ? 2 : 0;

	pCmd->gppGpdLink.bits.lqi = LQI;

	return SUCCESS;
}

static s32 gpCommissioningNotificationCmdSend(void *arg)
{
	gp_noti_t *buf = (gp_noti_t *)arg;

	zcl_gp_commissioningNotificationCmdSend(GREEN_POWER_ENDPOINT, &buf->dstEpInfo, TRUE, &buf->gpCommissioningNotificationCmd);

	epInfo_t *pDstEpInfo = (epInfo_t *)&buf->dstEpInfo;
	//TODO: need fixed if unicast.
	if(!aps_duplicate_check(pDstEpInfo->aliasSrcAddr, pDstEpInfo->aliasSeqNum)){
		gpCommissioningNotificationCmdFromLocalCb(&buf->gpCommissioningNotificationCmd);
	}

	ev_buf_free((u8 *)buf);

	return -1;
}

static void gppCommissioningModeProcess(gp_data_ind_t *pGpDataInd)
{
	if(pGpDataInd->gpdCmdId == GPDF_CMD_ID_CHANNEL_REQ){
		if(pGpDataInd->frameType == GP_NWK_FRAME_TYPE_DATA){
			return;
		}

		if(gpAppCb && gpAppCb->gpChangleChannelReqCb){
			if(gpAppCb->gpChangleChannelReqCb() == FALSE){
				//Not permit
				return;
			}
		}
	}else if((pGpDataInd->gpdCmdId == GPDF_CMD_ID_CHANNEL_CONFIGURATION) ||
			 (pGpDataInd->gpdCmdId == GPDF_CMD_ID_COMMISSIONING_REPLY)){
				return;
	}else if((pGpDataInd->gpdCmdId == GPDF_CMD_ID_COMMISSIONING) ||
			 (pGpDataInd->gpdCmdId == GPDF_CMD_ID_SUCCESS)){
				//case 5.4.1.10, step 5.
				if(pGpDataInd->autoCommissioning){
					return;
				}
	}

	//case 5.4.1.13, step 4.
	if((pGpDataInd->frameType == GP_NWK_FRAME_TYPE_DATA) &&
	   (((pGpDataInd->appId == GP_APP_ID_SRC_ID) && (pGpDataInd->srcId == 0)) ||
	   ((pGpDataInd->appId == GP_APP_ID_GPD) && (ZB_IEEE_ADDR_IS_ZERO(pGpDataInd->srcAddr.extAddr))))){
		return;
	}

	gp_noti_t *buf = (gp_noti_t *)ev_buf_allocate(sizeof(gp_noti_t));
	if(!buf){
		return;
	}
	memset((u8 *)buf, 0, sizeof(gp_noti_t));

	if(gpParseManteinanceInd(pGpDataInd, &buf->gpCommissioningNotificationCmd) == SUCCESS){
		//printf("gpCommNotif: cmdId = %x\n", pGpDataInd->gpdCmdId);

		buf->dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		buf->dstEpInfo.dstAddr.shortAddr = g_gppCtx.gpCommissioningModeOpt.bits.unicastCommunication ? g_gppCtx.gpCommissionerAddr
																									: NWK_BROADCAST_RX_ON_WHEN_IDLE;
		buf->dstEpInfo.dstEp = GREEN_POWER_ENDPOINT;
		buf->dstEpInfo.profileId = GP_PROFILE_ID;

		if(ZB_NWK_IS_ADDRESS_BROADCAST(buf->dstEpInfo.dstAddr.shortAddr)){
			buf->dstEpInfo.useAlias = TRUE;
			buf->dstEpInfo.aliasSrcAddr = gpAliasSrcAddrDerived(buf->gpCommissioningNotificationCmd.options.bits.appId,
																buf->gpCommissioningNotificationCmd.gpdId);
			buf->dstEpInfo.aliasSeqNum = (pGpDataInd->seqNum - 12) % 256;
		}else{
			buf->dstEpInfo.useAlias = FALSE;
		}

		u8 gppTunnelingDelay = gppTunnelingDelayGet(buf->gpCommissioningNotificationCmd.options.bits.rxAfterTx,
													buf->gpCommissioningNotificationCmd.gppGpdLink.bits.lqi,
													g_gppCtx.firstToForward, FALSE);
		//printf("delay = %d\n", gppTunnelingDelay);
		TL_ZB_TIMER_SCHEDULE(gpCommissioningNotificationCmdSend, buf, gppTunnelingDelay);
	}else{
		ev_buf_free((u8 *)buf);
	}
}

void gppGpDataIndProcess(gp_data_ind_t *pGpDataInd)
{
	if(!pGpDataInd){
		return;
	}

//	printf("GPP_dataInd: cmdId = %x, fc = %x, sta = %x, isCommMode = %x\n",
//		   pGpDataInd->gpdCmdId, pGpDataInd->gpdSecFrameCnt, pGpDataInd->status, g_gppCtx.gppInCommMode);

	if(g_gppCtx.gppInCommMode == FALSE){
		gppOperationalModeProcess(pGpDataInd);
	}else{
		gppCommissioningModeProcess(pGpDataInd);
	}
}

#endif	/* GP_SUPPORT_ENABLE */
