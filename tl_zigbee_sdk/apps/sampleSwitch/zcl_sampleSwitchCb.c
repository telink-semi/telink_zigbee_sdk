/********************************************************************************************************
 * @file    zcl_sampleSwitchCb.c
 *
 * @brief   This is the source file for zcl_sampleSwitchCb
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "app_ui.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#ifdef ZCL_READ
static void sampleSwitch_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void sampleSwitch_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd);
static void sampleSwitch_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
#endif
#ifdef ZCL_REPORT
static void sampleSwitch_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd);
static void sampleSwitch_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void sampleSwitch_zclReportCmd(u16 clusterId, zclReportCmd_t *pReportCmd);
#endif
static void sampleSwitch_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd);


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      sampleSwitch_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void sampleSwitch_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
	//printf("sampleSwitch_zclProcessIncomingMsg\n");

	u16 cluster = pInHdlrMsg->msg->indInfo.cluster_id;
	switch(pInHdlrMsg->hdr.cmd)
	{
#ifdef ZCL_READ
		case ZCL_CMD_READ_RSP:
			sampleSwitch_zclReadRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_WRITE
		case ZCL_CMD_WRITE_RSP:
			sampleSwitch_zclWriteRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_WRITE:
			sampleSwitch_zclWriteReqCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_REPORT
		case ZCL_CMD_CONFIG_REPORT:
			sampleSwitch_zclCfgReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT_RSP:
			sampleSwitch_zclCfgReportRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_REPORT:
			sampleSwitch_zclReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
		case ZCL_CMD_DEFAULT_RSP:
			sampleSwitch_zclDfltRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		default:
			break;
	}
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      sampleSwitch_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSwitch_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd)
{
    //printf("sampleSwitch_zclReadRspCmd\n");

}
#endif	/* ZCL_READ */

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      sampleSwitch_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSwitch_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd)
{
    //printf("sampleSwitch_zclWriteRspCmd\n");

}

/*********************************************************************
 * @fn      sampleSwitch_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSwitch_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{
#ifdef ZCL_POLL_CTRL
	u8 numAttr = pWriteReqCmd->numAttr;
	zclWriteRec_t *attr = pWriteReqCmd->attrList;

	if(clusterId == ZCL_CLUSTER_GEN_POLL_CONTROL){
		for(s32 i = 0; i < numAttr; i++){
			if(attr[i].attrID == ZCL_ATTRID_CHK_IN_INTERVAL){
				sampleSwitch_zclCheckInStart();
				return;
			}
		}
	}
#endif
}
#endif	/* ZCL_WRITE */


/*********************************************************************
 * @fn      sampleSwitch_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSwitch_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd)
{
    //printf("sampleSwitch_zclDfltRspCmd\n");

}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      sampleSwitch_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSwitch_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd)
{
    //printf("sampleSwitch_zclCfgReportCmd\n");

}

/*********************************************************************
 * @fn      sampleSwitch_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSwitch_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
    //printf("sampleSwitch_zclCfgReportRspCmd\n");

}

/*********************************************************************
 * @fn      sampleSwitch_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSwitch_zclReportCmd(u16 clusterId, zclReportCmd_t *pReportCmd)
{
    //printf("sampleSwitch_zclReportCmd\n");

}
#endif	/* ZCL_REPORT */

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      sampleSwitch_zclBasicResetCmdHandler
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSwitch_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_BASIC */

#ifdef ZCL_IDENTIFY
s32 sampleSwitch_zclIdentifyTimerCb(void *arg)
{
	if(g_zcl_identifyAttrs.identifyTime <= 0){
		identifyTimerEvt = NULL;
		return -1;
	}
	g_zcl_identifyAttrs.identifyTime--;
	return 0;
}

void sampleSwitch_zclIdentifyTimerStop(void)
{
	if(identifyTimerEvt){
		TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
	}
}

/*********************************************************************
 * @fn      sampleSwitch_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void sampleSwitch_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
	g_zcl_identifyAttrs.identifyTime = identifyTime;

	if(identifyTime == 0){
		sampleSwitch_zclIdentifyTimerStop();
		light_blink_stop();
	}else{
		if(!identifyTimerEvt){
			light_blink_start(identifyTime, 500, 500);
			identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSwitch_zclIdentifyTimerCb, NULL, 1000);
		}
	}
}

/*********************************************************************
 * @fn      sampleSwitch_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void sampleSwitch_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
	u8 effectId = pTriggerEffect->effectId;
	//u8 effectVariant = pTriggerEffect->effectVariant;

	switch(effectId){
		case IDENTIFY_EFFECT_BLINK:
			light_blink_start(1, 500, 500);
			break;
		case IDENTIFY_EFFECT_BREATHE:
			light_blink_start(15, 300, 700);
			break;
		case IDENTIFY_EFFECT_OKAY:
			light_blink_start(2, 250, 250);
			break;
		case IDENTIFY_EFFECT_CHANNEL_CHANGE:
			light_blink_start(1, 500, 7500);
			break;
		case IDENTIFY_EFFECT_FINISH_EFFECT:
			light_blink_start(1, 300, 700);
			break;
		case IDENTIFY_EFFECT_STOP_EFFECT:
			light_blink_stop();
			break;
		default:
			break;
	}
}

/*********************************************************************
 * @fn      sampleSwitch_zclIdentifyQueryRspCmdHandler
 *
 * @brief   Handler for ZCL Identify Query response command.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyRsp
 *
 * @return  None
 */
static void sampleSwitch_zclIdentifyQueryRspCmdHandler(u8 endpoint, u16 srcAddr, zcl_identifyRspCmd_t *identifyRsp)
{
#if FIND_AND_BIND_SUPPORT
	if(identifyRsp->timeout){
		findBindDst_t dstInfo;
		dstInfo.addr = srcAddr;
		dstInfo.endpoint = endpoint;

		bdb_addIdentifyActiveEpForFB(dstInfo);
	}
#endif
}

/*********************************************************************
 * @fn      sampleLight_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSwitch_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_SWITCH_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_IDENTIFY:
					sampleSwitch_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
					break;
				case ZCL_CMD_TRIGGER_EFFECT:
					sampleSwitch_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
					break;
				default:
					break;
			}
		}else{
			if(cmdId == ZCL_CMD_IDENTIFY_QUERY_RSP){
				sampleSwitch_zclIdentifyQueryRspCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, (zcl_identifyRspCmd_t *)cmdPayload);
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_IDENTIFY */

#ifdef ZCL_GROUP
/*********************************************************************
 * @fn      sampleSwitch_zclAddGroupRspCmdHandler
 *
 * @brief   Handler for ZCL add group response command.
 *
 * @param   pAddGroupRsp
 *
 * @return  None
 */
static void sampleSwitch_zclAddGroupRspCmdHandler(zcl_addGroupRsp_t *pAddGroupRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclViewGroupRspCmdHandler
 *
 * @brief   Handler for ZCL view group response command.
 *
 * @param   pViewGroupRsp
 *
 * @return  None
 */
static void sampleSwitch_zclViewGroupRspCmdHandler(zcl_viewGroupRsp_t *pViewGroupRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclRemoveGroupRspCmdHandler
 *
 * @brief   Handler for ZCL remove group response command.
 *
 * @param   pRemoveGroupRsp
 *
 * @return  None
 */
static void sampleSwitch_zclRemoveGroupRspCmdHandler(zcl_removeGroupRsp_t *pRemoveGroupRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclGetGroupMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get group membership response command.
 *
 * @param   pGetGroupMembershipRsp
 *
 * @return  None
 */
static void sampleSwitch_zclGetGroupMembershipRspCmdHandler(zcl_getGroupMembershipRsp_t *pGetGroupMembershipRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_groupCb
 *
 * @brief   Handler for ZCL Group command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSwitch_groupCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_SWITCH_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_GROUP_ADD_GROUP_RSP:
					sampleSwitch_zclAddGroupRspCmdHandler((zcl_addGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_VIEW_GROUP_RSP:
					sampleSwitch_zclViewGroupRspCmdHandler((zcl_viewGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_REMOVE_GROUP_RSP:
					sampleSwitch_zclRemoveGroupRspCmdHandler((zcl_removeGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP:
					sampleSwitch_zclGetGroupMembershipRspCmdHandler((zcl_getGroupMembershipRsp_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_GROUP */

#ifdef ZCL_SCENE
/*********************************************************************
 * @fn      sampleSwitch_zclAddSceneRspCmdHandler
 *
 * @brief   Handler for ZCL add scene response command.
 *
 * @param   cmdId
 * @param   pAddSceneRsp
 *
 * @return  None
 */
static void sampleSwitch_zclAddSceneRspCmdHandler(u8 cmdId, addSceneRsp_t *pAddSceneRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclViewSceneRspCmdHandler
 *
 * @brief   Handler for ZCL view scene response command.
 *
 * @param   cmdId
 * @param   pViewSceneRsp
 *
 * @return  None
 */
static void sampleSwitch_zclViewSceneRspCmdHandler(u8 cmdId, viewSceneRsp_t *pViewSceneRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclRemoveSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove scene response command.
 *
 * @param   pRemoveSceneRsp
 *
 * @return  None
 */
static void sampleSwitch_zclRemoveSceneRspCmdHandler(removeSceneRsp_t *pRemoveSceneRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclRemoveAllSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove all scene response command.
 *
 * @param   pRemoveAllSceneRsp
 *
 * @return  None
 */
static void sampleSwitch_zclRemoveAllSceneRspCmdHandler(removeAllSceneRsp_t *pRemoveAllSceneRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclStoreSceneRspCmdHandler
 *
 * @brief   Handler for ZCL store scene response command.
 *
 * @param   pStoreSceneRsp
 *
 * @return  None
 */
static void sampleSwitch_zclStoreSceneRspCmdHandler(storeSceneRsp_t *pStoreSceneRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_zclGetSceneMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get scene membership response command.
 *
 * @param   pGetSceneMembershipRsp
 *
 * @return  None
 */
static void sampleSwitch_zclGetSceneMembershipRspCmdHandler(getSceneMemRsp_t *pGetSceneMembershipRsp)
{

}

/*********************************************************************
 * @fn      sampleSwitch_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSwitch_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_SWITCH_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_SCENE_ADD_SCENE_RSP:
				case ZCL_CMD_SCENE_ENHANCED_ADD_SCENE_RSP:
					sampleSwitch_zclAddSceneRspCmdHandler(cmdId, (addSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_VIEW_SCENE_RSP:
				case ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE_RSP:
					sampleSwitch_zclViewSceneRspCmdHandler(cmdId, (viewSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_REMOVE_SCENE_RSP:
					sampleSwitch_zclRemoveSceneRspCmdHandler((removeSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP:
					sampleSwitch_zclRemoveAllSceneRspCmdHandler((removeAllSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_STORE_SCENE_RSP:
					sampleSwitch_zclStoreSceneRspCmdHandler((storeSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_GET_SCENE_MEMSHIP_RSP:
					sampleSwitch_zclGetSceneMembershipRspCmdHandler((getSceneMemRsp_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_SCENE */

#ifdef ZCL_POLL_CTRL
static ev_timer_event_t *zclFastPollTimeoutTimerEvt = NULL;
static ev_timer_event_t *zclCheckInTimerEvt = NULL;
static bool isFastPollMode = FALSE;

void sampleSwitch_zclCheckInCmdSend(void)
{
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
	dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
	dstEpInfo.profileId = HA_PROFILE_ID;

	zcl_pollCtrl_checkInCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, TRUE);
}

s32 sampleSwitch_zclCheckInTimerCb(void *arg)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if(!pPollCtrlAttr->chkInInterval){
		zclCheckInTimerEvt = NULL;
		return -1;
	}

	sampleSwitch_zclCheckInCmdSend();

	return 0;
}

void sampleSwitch_zclCheckInStart(void)
{
	if(zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, SAMPLE_SWITCH_ENDPOINT)){
		zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

		if(!zclCheckInTimerEvt){
			zclCheckInTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSwitch_zclCheckInTimerCb, NULL, pPollCtrlAttr->chkInInterval * POLL_RATE_QUARTERSECONDS);
			
			if(pPollCtrlAttr->chkInInterval){
				sampleSwitch_zclCheckInCmdSend();
			}
		}
	}
}

void sampleSwitch_zclSetFastPollMode(bool fastPollMode)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	isFastPollMode = fastPollMode;
	u32 pollRate = fastPollMode ? pPollCtrlAttr->shortPollInterval : pPollCtrlAttr->longPollInterval;

	zb_setPollRate(pollRate  * POLL_RATE_QUARTERSECONDS);
}

s32 sampleSwitch_zclFastPollTimeoutCb(void *arg)
{
	sampleSwitch_zclSetFastPollMode(FALSE);

	zclFastPollTimeoutTimerEvt = NULL;
	return -1;
}

static status_t sampleSwitch_zclPollCtrlChkInRspCmdHandler(zcl_chkInRsp_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if(pCmd->startFastPolling){
		u16 fastPollTimeoutCnt = 0;

		if(pCmd->fastPollTimeout){
			if(pCmd->fastPollTimeout > pPollCtrlAttr->fastPollTimeoutMax){
				return ZCL_STA_INVALID_FIELD;
			}

			fastPollTimeoutCnt = pCmd->fastPollTimeout;
		}else{
			fastPollTimeoutCnt = pPollCtrlAttr->fastPollTimeout;
		}

		if(fastPollTimeoutCnt){
			sampleSwitch_zclSetFastPollMode(TRUE);
			
			if(zclFastPollTimeoutTimerEvt){
				TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
			}
			zclFastPollTimeoutTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSwitch_zclFastPollTimeoutCb, NULL, fastPollTimeoutCnt * POLL_RATE_QUARTERSECONDS);
		}
	}else{
		//continue in normal operation and not required to go into fast poll mode.
	}

	return ZCL_STA_SUCCESS;
}

static status_t sampleSwitch_zclPollCtrlFastPollStopCmdHandler(void)
{
	if(!isFastPollMode){
		return ZCL_STA_ACTION_DENIED;
	}else{
		if(zclFastPollTimeoutTimerEvt){
			TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
		}
		sampleSwitch_zclSetFastPollMode(FALSE);
	}

	return ZCL_STA_SUCCESS;
}

static status_t sampleSwitch_zclPollCtrlSetLongPollIntervalCmdHandler(zcl_setLongPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newLongPollInterval >= 0x04) && (pCmd->newLongPollInterval <= 0x6E0000)
		&& (pCmd->newLongPollInterval <= pPollCtrlAttr->chkInInterval) && (pCmd->newLongPollInterval >= pPollCtrlAttr->shortPollInterval)){
		pPollCtrlAttr->longPollInterval = pCmd->newLongPollInterval;
		zb_setPollRate(pCmd->newLongPollInterval * POLL_RATE_QUARTERSECONDS);
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

static status_t sampleSwitch_zclPollCtrlSetShortPollIntervalCmdHandler(zcl_setShortPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newShortPollInterval >= 0x01) && (pCmd->newShortPollInterval <= 0xff)
		&& (pCmd->newShortPollInterval <= pPollCtrlAttr->longPollInterval)){
		pPollCtrlAttr->shortPollInterval = pCmd->newShortPollInterval;
		zb_setPollRate(pCmd->newShortPollInterval * POLL_RATE_QUARTERSECONDS);
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      sampleSwitch_pollCtrlCb
 *
 * @brief   Handler for ZCL Poll Control command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSwitch_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == SAMPLE_SWITCH_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_CHK_IN_RSP:
					status = sampleSwitch_zclPollCtrlChkInRspCmdHandler((zcl_chkInRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_FAST_POLL_STOP:
					status = sampleSwitch_zclPollCtrlFastPollStopCmdHandler();
					break;
				case ZCL_CMD_SET_LONG_POLL_INTERVAL:
					status = sampleSwitch_zclPollCtrlSetLongPollIntervalCmdHandler((zcl_setLongPollInterval_t *)cmdPayload);
					break;
				case ZCL_CMD_SET_SHORT_POLL_INTERVAL:
					status = sampleSwitch_zclPollCtrlSetShortPollIntervalCmdHandler((zcl_setShortPollInterval_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return status;
}
#endif	/* ZCL_POLL_CTRL */


#endif  /* __PROJECT_TL_SWITCH__ */
