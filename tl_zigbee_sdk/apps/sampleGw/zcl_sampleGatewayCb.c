/********************************************************************************************************
 * @file     zcl_sampleGatewayCb.c
 *
 * @brief    call back function for zcl
 *
 * @author
 * @date     Dec. 5, 2016
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

#if (__PROJECT_TL_GW__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleGateway.h"
#include "app_ui.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif

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
static void sampleGW_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void sampleGW_zclWriteRspCmd(zclIncoming_t *pInMsg);
#endif
#ifdef ZCL_REPORT
static void sampleGW_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd);
static void sampleGW_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void sampleGW_zclReportCmd(zclIncoming_t *pInMsg);
static void sampleGW_zclCfgReadRspCmd(zclReadReportCfgRspCmd_t *pReadCfgRspCmd);
#endif
static void sampleGW_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd);

/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_time_event_t *identifyTimerEvt = NULL;
#endif


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      sampleGW_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void sampleGW_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
//	printf("sampleGW_zclProcessIncomingMsg\n");

	switch(pInHdlrMsg->hdr.cmd)
	{
#ifdef ZCL_READ
		case ZCL_CMD_READ_RSP:
			sampleGW_zclReadRspCmd(pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_WRITE
		case ZCL_CMD_WRITE_RSP:
			sampleGW_zclWriteRspCmd(pInHdlrMsg);
			break;
#endif
#ifdef ZCL_REPORT
		case ZCL_CMD_CONFIG_REPORT:
			sampleGW_zclCfgReportCmd(pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT_RSP:
			sampleGW_zclCfgReportRspCmd(pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_READ_REPORT_CFG_RSP:
			sampleGW_zclCfgReadRspCmd(pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_REPORT:
			sampleGW_zclReportCmd(pInHdlrMsg);
			break;
#endif
		case ZCL_CMD_DEFAULT_RSP:
			sampleGW_zclDfltRspCmd(pInHdlrMsg->attrCmd);
			break;
		default:
			break;
	}
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      sampleGW_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleGW_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u16 dataLen = 0;
	u8 *pBuf = array;

	*pBuf++ = pReadRspCmd->numAttr;
	for(u8 i = 0; i < pReadRspCmd->numAttr; i++){
		*pBuf++ = HI_UINT16(pReadRspCmd->attrList[i].attrID);
		*pBuf++ = LO_UINT16(pReadRspCmd->attrList[i].attrID);
		*pBuf++ = pReadRspCmd->attrList[i].status;
		if(pReadRspCmd->attrList[i].status == ZCL_STA_SUCCESS){
			*pBuf++ = pReadRspCmd->attrList[i].dataType;
			dataLen = zcl_getAttrSize(pReadRspCmd->attrList[i].dataType, pReadRspCmd->attrList[i].data);
			memcpy(pBuf, pReadRspCmd->attrList[i].data, dataLen);
			if( (pReadRspCmd->attrList[i].dataType != ZCL_DATA_TYPE_LONG_CHAR_STR) && (pReadRspCmd->attrList[i].dataType != ZCL_DATA_TYPE_LONG_OCTET_STR) &&
				(pReadRspCmd->attrList[i].dataType != ZCL_DATA_TYPE_CHAR_STR) && (pReadRspCmd->attrList[i].dataType != ZCL_DATA_TYPE_OCTET_STR) &&
				(pReadRspCmd->attrList[i].dataType != ZCL_DATA_TYPE_STRUCT) ){
					ZB_LEBESWAP(pBuf, dataLen);
			}
			pBuf += dataLen;
		}
	}

   	zbhciTx(ZBHCI_CMD_ZCL_ATTR_READ_RSP, pBuf - array, array);
#endif
}
#endif	/* ZCL_READ */

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      sampleGW_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleGW_zclWriteRspCmd(zclIncoming_t *pInMsg)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u8 *pBuf = array;
	zclWriteRspCmd_t *pWriteRsp = (zclWriteRspCmd_t *)pInMsg->attrCmd;

	*pBuf++ = pWriteRsp->numAttr;

	if(pInMsg->dataLen == 1){//the case of successful writing of all attributes
		*pBuf++ = pWriteRsp->attrList[0].status;
		*pBuf++ = 0xFF;
		*pBuf++ = 0xFF;
	}else{
		for(u8 i = 0; i < pWriteRsp->numAttr; i++){
			*pBuf++ = pWriteRsp->attrList[i].status;
			*pBuf++ = HI_UINT16(pWriteRsp->attrList[i].attrID);
			*pBuf++ = LO_UINT16(pWriteRsp->attrList[i].attrID);
		}
	}

	zbhciTx(ZBHCI_CMD_ZCL_ATTR_WRITE_RSP, pBuf - array, array);
#endif
}
#endif	/* ZCL_WRITE */

/*********************************************************************
 * @fn      sampleGW_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleGW_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd)
{
//    printf("sampleGW_zclDfltRspCmd\n");

}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      sampleGW_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleGW_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd)
{
//    printf("sampleGW_zclCfgReportCmd\n");
}

/*********************************************************************
 * @fn      sampleGW_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleGW_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
//    printf("sampleGW_zclCfgReportRspCmd\n");
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u8 *pBuf = array;

	*pBuf++ = pCfgReportRspCmd->numAttr;

	for(u8 i = 0; i < pCfgReportRspCmd->numAttr; i++){
		*pBuf++ = pCfgReportRspCmd->attrList[i].status;
		*pBuf++ = pCfgReportRspCmd->attrList[i].direction;
		*pBuf++ = HI_UINT16(pCfgReportRspCmd->attrList[i].attrID);
		*pBuf++ = LO_UINT16(pCfgReportRspCmd->attrList[i].attrID);
	}

	zbhciTx(ZBHCI_CMD_ZCL_CONFIG_REPORT_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleGW_zclReportCmd(zclIncoming_t *pInMsg)
{
//    printf("sampleGW_zclReportCmd\n");
#if ZBHCI_EN
	zclReportCmd_t *pReportCmd = (zclReportCmd_t *)pInMsg->attrCmd;

	u8 array[64];
	memset(array, 0, 64);

	u16 dataLen = 0;
	u8 *pBuf = array;

	*pBuf++ = HI_UINT16(pInMsg->msg->indInfo.src_short_addr);
	*pBuf++ = LO_UINT16(pInMsg->msg->indInfo.src_short_addr);
	*pBuf++ = pInMsg->msg->indInfo.src_ep;

	*pBuf++ = pReportCmd->numAttr;
	for(u8 i = 0; i < pReportCmd->numAttr; i++){
		*pBuf++ = HI_UINT16(pReportCmd->attrList[i].attrID);
		*pBuf++ = LO_UINT16(pReportCmd->attrList[i].attrID);
		*pBuf++ = pReportCmd->attrList[i].dataType;
		dataLen = zcl_getAttrSize(pReportCmd->attrList[i].dataType, pReportCmd->attrList[i].attrData);
		memcpy(pBuf, pReportCmd->attrList[i].attrData, dataLen);
		if( (pReportCmd->attrList[i].dataType != ZCL_DATA_TYPE_LONG_CHAR_STR) && (pReportCmd->attrList[i].dataType != ZCL_DATA_TYPE_LONG_OCTET_STR) &&
			(pReportCmd->attrList[i].dataType != ZCL_DATA_TYPE_CHAR_STR) && (pReportCmd->attrList[i].dataType != ZCL_DATA_TYPE_OCTET_STR) &&
			(pReportCmd->attrList[i].dataType != ZCL_DATA_TYPE_STRUCT) ){
				ZB_LEBESWAP(pBuf, dataLen);
		}
		pBuf += dataLen;
	}

	zbhciTx(ZBHCI_CMD_ZCL_REPORT_MSG_RCV, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclCfgReadRspCmd
 *
 * @brief   Handler for ZCL Report Configure Read command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */

static void sampleGW_zclCfgReadRspCmd(zclReadReportCfgRspCmd_t *pReadCfgRspCmd)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u16 dataLen = 0;
	u8 *pBuf = array;

	*pBuf++ = pReadCfgRspCmd->numAttr;

	for(u8 i = 0; i < pReadCfgRspCmd->numAttr; i++){
		*pBuf++ = pReadCfgRspCmd->attrList[i].status;
		*pBuf++ = pReadCfgRspCmd->attrList[i].direction;
		*pBuf++ = HI_UINT16(pReadCfgRspCmd->attrList[i].attrID);
		*pBuf++ = LO_UINT16(pReadCfgRspCmd->attrList[i].attrID);

		if(pReadCfgRspCmd->attrList[i].direction == ZCL_SEND_ATTR_REPORTS){
			*pBuf++ = pReadCfgRspCmd->attrList[i].dataType;
			*pBuf++ = HI_UINT16(pReadCfgRspCmd->attrList[i].minReportInt);
			*pBuf++ = LO_UINT16(pReadCfgRspCmd->attrList[i].minReportInt);
			*pBuf++ = HI_UINT16(pReadCfgRspCmd->attrList[i].maxReportInt);
			*pBuf++ = LO_UINT16(pReadCfgRspCmd->attrList[i].maxReportInt);

			if(zcl_analogDataType(pReadCfgRspCmd->attrList[i].dataType)){
				dataLen = zcl_getAttrSize(pReadCfgRspCmd->attrList[i].dataType, pReadCfgRspCmd->attrList[i].reportableChange);
				memcpy(pBuf, pReadCfgRspCmd->attrList[i].reportableChange, dataLen);
				pBuf += dataLen;
			}
		}else{
			*pBuf++ = HI_UINT16(pReadCfgRspCmd->attrList[i].timeoutPeriod);
			*pBuf++ = LO_UINT16(pReadCfgRspCmd->attrList[i].timeoutPeriod);
		}
	}

	zbhciTx(ZBHCI_CMD_ZCL_READ_REPORT_CFG_RSP, pBuf - array, array);
#endif
}
#endif	/* ZCL_REPORT */

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      sampleGW_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleGW_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_BASIC */

#ifdef ZCL_IDENTIFY
s32 sampleGW_zclIdentifyTimerCb(void *arg)
{
	if(g_zcl_identifyAttrs.identifyTime <= 0){
		identifyTimerEvt = NULL;
		return -1;
	}
	g_zcl_identifyAttrs.identifyTime--;
	return 0;
}

void sampleGW_zclIdentifyTimerStop(void)
{
	if(identifyTimerEvt){
		TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
	}
}

/*********************************************************************
 * @fn      sampleGW_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime
 *
 * @return  None
 */
void sampleGW_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
	g_zcl_identifyAttrs.identifyTime = identifyTime;

	if(identifyTime == 0){
		sampleGW_zclIdentifyTimerStop();
		light_blink_stop();
	}else{
		if(!identifyTimerEvt){
			light_blink_start(identifyTime, 500, 500);
			identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleGW_zclIdentifyTimerCb, NULL, 1000*1000);
		}
	}
}

/*********************************************************************
 * @fn      sampleGW_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void sampleGW_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
	u8 effectId = pTriggerEffect->effectId;
//	u8 effectVariant = pTriggerEffect->effectVariant;

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
 * @fn      sampleGW_zclIdentifyQueryRspCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   srcAddr
 * @param   cmdPayload
 *
 * @return  None
 */
static void sampleGW_zclIdentifyQueryRspCmdHandler(u16 srcAddr, zcl_identifyRspCmd_t *cmdPayload)
{
#if ZBHCI_EN
	u8 array[4];
	memset(array, 0, 4);

	u8 *pBuf = array;

	*pBuf++ = HI_UINT16(srcAddr);
	*pBuf++ = LO_UINT16(srcAddr);
	*pBuf++ = HI_UINT16(cmdPayload->timeout);
	*pBuf++ = LO_UINT16(cmdPayload->timeout);

	zbhciTx(ZBHCI_CMD_ZCL_IDENTIFY_QUERY_RSP, pBuf - array, array);
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
status_t sampleGW_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_GW_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_IDENTIFY:
					sampleGW_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
					break;
				case ZCL_CMD_TRIGGER_EFFECT:
					sampleGW_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
					break;
				default:
					break;
			}
		}else{
			if(cmdId == ZCL_CMD_IDENTIFY_QUERY_RSP){
				sampleGW_zclIdentifyQueryRspCmdHandler(pAddrInfo->srcAddr, (zcl_identifyRspCmd_t *)cmdPayload);
			}
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif	/* ZCL_IDENTIFY */

#ifdef ZCL_GROUP
/*********************************************************************
 * @fn      sampleGW_zclAddGroupRspCmdHandler
 *
 * @brief   Handler for ZCL add group response command.
 *
 * @param   pApsdeInd
 * @param   pAddGroupRsp
 *
 * @return  None
 */
static void sampleGW_zclAddGroupRspCmdHandler(zcl_addGroupRsp_t *pAddGroupRsp)
{
#if ZBHCI_EN
	u8 array[4];
	memset(array, 0, 4);

	u8 *pBuf = array;

	*pBuf++ = pAddGroupRsp->status;
	*pBuf++ = HI_UINT16(pAddGroupRsp->groupId);
	*pBuf++ = LO_UINT16(pAddGroupRsp->groupId);

	zbhciTx(ZBHCI_CMD_ZCL_GROUP_ADD_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclViewGroupRspCmdHandler
 *
 * @brief   Handler for ZCL view group response command.
 *
 * @param   pApsdeInd
 * @param   pViewGroupRsp
 *
 * @return  None
 */
static void sampleGW_zclViewGroupRspCmdHandler(zcl_viewGroupRsp_t *pViewGroupRsp)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u8 *pBuf = array;

	*pBuf++ = pViewGroupRsp->status;
	*pBuf++ = HI_UINT16(pViewGroupRsp->groupId);
	*pBuf++ = LO_UINT16(pViewGroupRsp->groupId);
	if(pViewGroupRsp->status == ZCL_STA_SUCCESS){
		*pBuf++ = pViewGroupRsp->pGroupName[0];
		if(pViewGroupRsp->pGroupName[0]){
			memcpy(pBuf, &pViewGroupRsp->pGroupName[1], pViewGroupRsp->pGroupName[0]);
			pBuf += pViewGroupRsp->pGroupName[0];
		}
	}

	zbhciTx(ZBHCI_CMD_ZCL_GROUP_VIEW_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclRemoveGroupRspCmdHandler
 *
 * @brief   Handler for ZCL remove group response command.
 *
 * @param   pApsdeInd
 * @param   pRemoveGroupRsp
 *
 * @return  None
 */
static void sampleGW_zclRemoveGroupRspCmdHandler(zcl_removeGroupRsp_t *pRemoveGroupRsp)
{
#if ZBHCI_EN
	u8 array[4];
	memset(array, 0, 4);

	u8 *pBuf = array;

	*pBuf++ = pRemoveGroupRsp->status;
	*pBuf++ = HI_UINT16(pRemoveGroupRsp->groupId);
	*pBuf++ = LO_UINT16(pRemoveGroupRsp->groupId);

	zbhciTx(ZBHCI_CMD_ZCL_GROUP_REMOVE_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclGetGroupMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get group membership response command.
 *
 * @param   pApsdeInd
 * @param   pGetGroupMembershipRsp
 *
 * @return  None
 */
static void sampleGW_zclGetGroupMembershipRspCmdHandler(zcl_getGroupMembershipRsp_t *pGetGroupMembershipRsp)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u8 *pBuf = array;

	*pBuf++ = pGetGroupMembershipRsp->capacity;
	*pBuf++ = pGetGroupMembershipRsp->groupCnt;
	for(u8 i = 0; i < pGetGroupMembershipRsp->groupCnt; i++){
		u16 groupId = BUILD_U16(pGetGroupMembershipRsp->pGroupLsit[i*2], pGetGroupMembershipRsp->pGroupLsit[(i*2)+1]);

		*pBuf++ = HI_UINT16(groupId);
		*pBuf++ = LO_UINT16(groupId);
	}

	zbhciTx(ZBHCI_CMD_ZCL_GROUP_GET_MEMBERSHIP_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_groupCb
 *
 * @brief   Handler for ZCL Group command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t sampleGW_groupCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_GW_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_GROUP_ADD_GROUP_RSP:
					sampleGW_zclAddGroupRspCmdHandler((zcl_addGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_VIEW_GROUP_RSP:
					sampleGW_zclViewGroupRspCmdHandler((zcl_viewGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_REMOVE_GROUP_RSP:
					sampleGW_zclRemoveGroupRspCmdHandler((zcl_removeGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP:
					sampleGW_zclGetGroupMembershipRspCmdHandler((zcl_getGroupMembershipRsp_t *)cmdPayload);
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
 * @fn      sampleGW_zclAddSceneRspCmdHandler
 *
 * @brief   Handler for ZCL add scene response command.
 *
 * @param   pApsdeInd
 * @param   cmdId
 * @param   pAddSceneRsp
 *
 * @return  None
 */
static void sampleGW_zclAddSceneRspCmdHandler(u8 cmdId, addSceneRsp_t *pAddSceneRsp)
{
#if ZBHCI_EN
	u8 array[4];
	memset(array, 0, 4);

	u8 *pBuf = array;

	*pBuf++ = pAddSceneRsp->status;
	*pBuf++ = HI_UINT16(pAddSceneRsp->groupId);
	*pBuf++ = LO_UINT16(pAddSceneRsp->groupId);
	*pBuf++ = pAddSceneRsp->sceneId;

	zbhciTx(ZBHCI_CMD_ZCL_SCENE_ADD_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclViewSceneRspCmdHandler
 *
 * @brief   Handler for ZCL view scene response command.
 *
 * @param   pApsdeInd
 * @param   cmdId
 * @param   pViewSceneRsp
 *
 * @return  None
 */
static void sampleGW_zclViewSceneRspCmdHandler(u8 cmdId, viewSceneRsp_t *pViewSceneRsp)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u8 *pBuf = array;

	*pBuf++ = pViewSceneRsp->status;
	*pBuf++ = HI_UINT16(pViewSceneRsp->scene.groupId);
	*pBuf++ = LO_UINT16(pViewSceneRsp->scene.groupId);
	*pBuf++ = pViewSceneRsp->scene.sceneId;
	if(pViewSceneRsp->status == ZCL_STA_SUCCESS){
		*pBuf++ = HI_UINT16(pViewSceneRsp->scene.transTime);
		*pBuf++ = LO_UINT16(pViewSceneRsp->scene.transTime);
		*pBuf++ = pViewSceneRsp->scene.sceneName[0];
		if(pViewSceneRsp->scene.sceneName[0]){
			memcpy(pBuf, &pViewSceneRsp->scene.sceneName[1], pViewSceneRsp->scene.sceneName[0]);
			pBuf += pViewSceneRsp->scene.sceneName[0];
		}
		*pBuf++ = pViewSceneRsp->scene.extFieldLen;
		if(pViewSceneRsp->scene.extFieldLen){
			memcpy(pBuf, pViewSceneRsp->scene.extField, pViewSceneRsp->scene.extFieldLen);
			pBuf += pViewSceneRsp->scene.extFieldLen;
		}
	}

	zbhciTx(ZBHCI_CMD_ZCL_SCENE_VIEW_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclRemoveSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove scene response command.
 *
 * @param   pApsdeInd
 * @param   pRemoveSceneRsp
 *
 * @return  None
 */
static void sampleGW_zclRemoveSceneRspCmdHandler(removeSceneRsp_t *pRemoveSceneRsp)
{
#if ZBHCI_EN
	u8 array[4];
	memset(array, 0, 4);

	u8 *pBuf = array;

	*pBuf++ = pRemoveSceneRsp->status;
	*pBuf++ = HI_UINT16(pRemoveSceneRsp->groupId);
	*pBuf++ = LO_UINT16(pRemoveSceneRsp->groupId);
	*pBuf++ = pRemoveSceneRsp->sceneId;

	zbhciTx(ZBHCI_CMD_ZCL_SCENE_REMOVE_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclRemoveAllSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove all scene response command.
 *
 * @param   pApsdeInd
 * @param   pRemoveAllSceneRsp
 *
 * @return  None
 */
static void sampleGW_zclRemoveAllSceneRspCmdHandler(removeAllSceneRsp_t *pRemoveAllSceneRsp)
{
#if ZBHCI_EN
	u8 array[4];
	memset(array, 0, 4);

	u8 *pBuf = array;

	*pBuf++ = pRemoveAllSceneRsp->status;
	*pBuf++ = HI_UINT16(pRemoveAllSceneRsp->groupId);
	*pBuf++ = LO_UINT16(pRemoveAllSceneRsp->groupId);

	zbhciTx(ZBHCI_CMD_ZCL_SCENE_REMOVE_ALL_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclStoreSceneRspCmdHandler
 *
 * @brief   Handler for ZCL store scene response command.
 *
 * @param   pApsdeInd
 * @param   pStoreSceneRsp
 *
 * @return  None
 */
static void sampleGW_zclStoreSceneRspCmdHandler(storeSceneRsp_t *pStoreSceneRsp)
{
#if ZBHCI_EN
	u8 array[4];
	memset(array, 0, 4);

	u8 *pBuf = array;

	*pBuf++ = pStoreSceneRsp->status;
	*pBuf++ = HI_UINT16(pStoreSceneRsp->groupId);
	*pBuf++ = LO_UINT16(pStoreSceneRsp->groupId);
	*pBuf++ = pStoreSceneRsp->sceneId;

	zbhciTx(ZBHCI_CMD_ZCL_SCENE_STORE_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_zclGetSceneMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get scene membership response command.
 *
 * @param   pApsdeInd
 * @param   pGetSceneMembershipRsp
 *
 * @return  None
 */
static void sampleGW_zclGetSceneMembershipRspCmdHandler(getSceneMemRsp_t *pGetSceneMembershipRsp)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	u8 *pBuf = array;

	*pBuf++ = pGetSceneMembershipRsp->status;
	*pBuf++ = pGetSceneMembershipRsp->capacity;
	*pBuf++ = HI_UINT16(pGetSceneMembershipRsp->groupId);
	*pBuf++ = LO_UINT16(pGetSceneMembershipRsp->groupId);
	if(pGetSceneMembershipRsp->status == ZCL_STA_SUCCESS){
		*pBuf++ = pGetSceneMembershipRsp->sceneCnt;
		for(u8 i = 0; i < pGetSceneMembershipRsp->sceneCnt; i++){
			*pBuf++ = pGetSceneMembershipRsp->sceneList[i];
		}
	}

	zbhciTx(ZBHCI_CMD_ZCL_SCENE_GET_MEMBERSHIP_RSP, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t sampleGW_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_GW_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_SCENE_ADD_SCENE_RSP:
				case ZCL_CMD_SCENE_ENHANCED_ADD_SCENE_RSP:
					sampleGW_zclAddSceneRspCmdHandler(cmdId, (addSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_VIEW_SCENE_RSP:
				case ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE_RSP:
					sampleGW_zclViewSceneRspCmdHandler(cmdId, (viewSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_REMOVE_SCENE_RSP:
					sampleGW_zclRemoveSceneRspCmdHandler((removeSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP:
					sampleGW_zclRemoveAllSceneRspCmdHandler((removeAllSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_STORE_SCENE_RSP:
					sampleGW_zclStoreSceneRspCmdHandler((storeSceneRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_GET_SCENE_MEMSHIP_RSP:
					sampleGW_zclGetSceneMembershipRspCmdHandler((getSceneMemRsp_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_SCENE */

#ifdef ZCL_DOOR_LOCK
/*********************************************************************
 * @fn      sampleGw_zclDoorLockRspCmdHandler
 *
 * @brief   Handler for ZCL door lock response command.
 *
 * @param   pApsdeInd
 * @param   cmdId
 * @param   rspState
 *
 * @return  None
 */
static void sampleGW_zclDoorLockRspCmdHandler(u8 cmdId, zcl_doorlockRsp_t *doorlockRsp)
{
#if ZBHCI_EN
	u8 array[64];
	memset(array, 0, 64);

	//u8 *pBuf = array;

#endif
}

/*********************************************************************
 * @fn      sampleGw_doorLockCb
 *
 * @brief   Handler for ZCL Door Lock command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t sampleGW_doorLockCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_GW_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_LOCK_DOOR_RESPONSE:
				case ZCL_CMD_UNLOCK_DOOR_RESPONSE:
				case ZCL_CMD_DOOR_LOCK_TOGGLE_RESPONSE:
					sampleGW_zclDoorLockRspCmdHandler(cmdId, (zcl_doorlockRsp_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_DOOR_LOCK */

#ifdef ZCL_IAS_ZONE
/*********************************************************************
 * @fn      sampleGw_iasZoneCb
 *
 * @brief   Handler for ZCL IAS Zone command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t sampleGW_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_GW_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			if(cmdId == ZCL_CMD_ZONE_STATUS_CHANGE_NOTIFICATION){
				light_blink_start(5, 250, 250);
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_IAS_ZONE */


#ifdef ZCL_POLL_CTRL
/*********************************************************************
 * @fn      sampleGW_zclPollCtrlChkInCmdHandler
 *
 * @brief   Handler for ZCL poll control request command.
 *
 * @param   pApsdeInd
 * @param   seqNum
 *
 * @return  status_t
 */
static status_t sampleGW_zclPollCtrlChkInCmdHandler(zclIncomingAddrInfo_t *pAddrInfo)
{
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
	dstEpInfo.dstEp = pAddrInfo->srcEp;
	dstEpInfo.profileId = pAddrInfo->profileId;

	zcl_chkInRsp_t checkInRsp;
	checkInRsp.startFastPolling = FALSE;
	checkInRsp.fastPollTimeout = 0;

	zcl_pollCtrl_chkInRspCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, TRUE, &checkInRsp);

	return ZCL_STA_CMD_HAS_RESP;
}

/*********************************************************************
 * @fn      sampleGw_iasZoneCb
 *
 * @brief   Handler for ZCL IAS Zone command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t sampleGW_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == SAMPLE_GW_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			if(cmdId == ZCL_CMD_CHK_IN){
				status = sampleGW_zclPollCtrlChkInCmdHandler(pAddrInfo);
			}
		}
	}

	return status;
}
#endif	/* ZCL_POLL_CTRL */

#endif  /* __PROJECT_TL_GW__ */


