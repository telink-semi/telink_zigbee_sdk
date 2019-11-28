/********************************************************************************************************
 * @file     zcl_level.c
 *
 * @brief	 APIs for level cluster
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


/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


#ifdef ZCL_LEVEL_CTRL
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
static void zcl_level_startUpCurrentLevel(u8 endpoint);
static status_t zcl_level_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_level_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	u8 status = ZCL_STA_SUCCESS;

    status = zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_LEVEL_CONTROL, attrNum, attrTbl, zcl_level_cmdHandler, cb);

    if(status == ZCL_STA_SUCCESS){
    	zcl_level_startUpCurrentLevel(endpoint);
    }

    return status;
}

_CODE_ZCL_ static void zcl_level_startUpCurrentLevel(u8 endpoint)
{
	u8 startUpCurrentLevel = 0;
	u8 preLevel = 0;
	u16 attrLen = 0;

	if( (zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_LEVEL_CONTROL, ZCL_ATTRID_LEVLE_START_UP_CURRENT_LEVEL, &attrLen, (u8 *)&startUpCurrentLevel) == ZCL_STA_SUCCESS) &&
		(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_LEVEL_CONTROL, ZCL_ATTRID_LEVEL_CURRENT_LEVEL, &attrLen, (u8 *)&preLevel) == ZCL_STA_SUCCESS) ){
			u8 dstLevel = 0;

			if(startUpCurrentLevel == ZCL_START_UP_CURRENT_LEVEL_TO_MIN){
				dstLevel = ZCL_LEVEL_ATTR_MIN_LEVEL;
			}else if(startUpCurrentLevel == ZCL_START_UP_CURRENT_LEVEL_TO_PREVIOUS){
				dstLevel = preLevel;
			}else{
				dstLevel = startUpCurrentLevel;
			}

			if(dstLevel != preLevel){
				zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_LEVEL_CONTROL, ZCL_ATTRID_LEVEL_CURRENT_LEVEL, (u8 *)&dstLevel);
			}
	}
}


_CODE_ZCL_ status_t zcl_level_move2level(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, moveToLvl_t *pMove2Level)
{
	u8 buf[5];
	u8 *pBuf = buf;

	*pBuf++ = pMove2Level->level;
	*pBuf++ = LO_UINT16(pMove2Level->transitionTime);
	*pBuf++ = HI_UINT16(pMove2Level->transitionTime);
	if(pMove2Level->optPresent){
		*pBuf++ = pMove2Level->optionsMask;
		*pBuf++ = pMove2Level->optionsOverride;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_LEVEL_CONTROL, cmdId, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_level_move(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, move_t *pMove)
{
	u8 buf[4];
	u8 *pBuf = buf;

	*pBuf++ = pMove->moveMode;
	*pBuf++ = pMove->rate;
	if(pMove->optPresent){
		*pBuf++ = pMove->optionsMask;
		*pBuf++ = pMove->optionsOverride;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_LEVEL_CONTROL, cmdId, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_level_step(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, step_t *pStep)
{
	u8 buf[6];
	u8 *pBuf = buf;

	*pBuf++ = pStep->stepMode;
	*pBuf++ = pStep->stepSize;
	*pBuf++ = LO_UINT16(pStep->transitionTime);
	*pBuf++ = HI_UINT16(pStep->transitionTime);
	if(pStep->optPresent){
		*pBuf++ = pStep->optionsMask;
		*pBuf++ = pStep->optionsOverride;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_LEVEL_CONTROL, cmdId, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_level_stop(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, stop_t *pStop)
{
	u8 buf[2];
	u8 *pBuf = buf;

	if(pStop->optPresent){
		*pBuf++ = pStop->optionsMask;
		*pBuf++ = pStop->optionsOverride;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_LEVEL_CONTROL, cmdId, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}


_CODE_ZCL_ static status_t zcl_level_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	zcl_level_cmdPayload_t cmdPayload;
	memset((u8 *)&cmdPayload, 0, sizeof(zcl_level_cmdPayload_t));

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_LEVEL_MOVE_TO_LEVEL:
		case ZCL_CMD_LEVEL_MOVE_TO_LEVEL_WITH_ON_OFF:
			cmdPayload.moveToLevel.level = *pData++;
			cmdPayload.moveToLevel.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 5){
				cmdPayload.moveToLevel.optionsMask = *pData++;
				cmdPayload.moveToLevel.optionsOverride = *pData++;
				cmdPayload.moveToLevel.optPresent = 1;
			}
			break;
		case ZCL_CMD_LEVEL_MOVE:
		case ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF:
			cmdPayload.move.moveMode = *pData++;
			cmdPayload.move.rate = *pData++;
			if(pInMsg->dataLen == 4){
				cmdPayload.move.optionsMask = *pData++;
				cmdPayload.move.optionsOverride = *pData++;
				cmdPayload.move.optPresent = 1;
			}
			break;
		case ZCL_CMD_LEVEL_STEP:
		case ZCL_CMD_LEVEL_STEP_WITH_ON_OFF:
			cmdPayload.step.stepMode = *pData++;
			cmdPayload.step.stepSize = *pData++;
			cmdPayload.step.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 6){
				cmdPayload.step.optionsMask = *pData++;
				cmdPayload.step.optionsOverride = *pData++;
				cmdPayload.step.optPresent = 1;
			}
			break;
		case ZCL_CMD_LEVEL_STOP:
		case ZCL_CMD_LEVEL_STOP_WITH_ON_OFF:
			if(pInMsg->dataLen == 2){
				cmdPayload.stop.optionsMask = *pData++;
				cmdPayload.stop.optionsOverride = *pData++;
				cmdPayload.stop.optPresent = 1;
			}
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	if(status == ZCL_STA_SUCCESS){
		if(pInMsg->clusterAppCb){
			zclIncomingAddrInfo_t addrInfo;
			addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
			addrInfo.profileId = pApsdeInd->indInfo.profile_id;
			addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
			addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
			addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
			addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

			pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmdPayload);

#ifdef ZCL_SCENE
			u16 attrLen = 0;
			u8 sceneValid = 0;
			if(zcl_getAttrVal(pInMsg->msg->indInfo.dst_ep, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_VALID, &attrLen, (u8 *)&sceneValid) == ZCL_STA_SUCCESS){
				sceneValid = 0;
				zcl_setAttrVal(pInMsg->msg->indInfo.dst_ep, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_VALID, (u8 *)&sceneValid);
			}
#endif
		}
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_level_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_level_clientCmdHandler(pInMsg);
	}else{
		return ZCL_STA_UNSUP_CLUSTER_COMMAND;
	}
}

#endif  /* ZCL_LEVEL_CTRL */
