/********************************************************************************************************
 * @file     zcl_onoff.c
 *
 * @brief	 APIs for onoff cluster
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


#ifdef ZCL_ON_OFF
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
static void zcl_onOff_startUpOnOff(u8 endpoint);
static status_t zcl_onOff_cmdHandler(zclIncoming_t *pInMsg);

_CODE_ZCL_ status_t zcl_onOff_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	u8 status = ZCL_STA_SUCCESS;

    status = zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF, attrNum, attrTbl, zcl_onOff_cmdHandler, cb);

    if(status == ZCL_STA_SUCCESS){
    	zcl_onOff_startUpOnOff(endpoint);
    }

    return status;
}

_CODE_ZCL_ static void zcl_onOff_startUpOnOff(u8 endpoint)
{
	u8 startUpOnOff = 0;
	bool preOnOff = 0;
	u16 attrLen = 0;

	if( (zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_START_UP_ONOFF, &attrLen, (u8 *)&startUpOnOff) == ZCL_STA_SUCCESS) &&
		(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF, &attrLen, (u8 *)&preOnOff) == ZCL_STA_SUCCESS) ){
			bool dstOnOff = 0;

			if(startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF){
				dstOnOff = ZCL_ONOFF_STATUS_OFF;
			}else if(startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON){
				dstOnOff = ZCL_ONOFF_STATUS_ON;
			}else if(startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TOGGLE){
				dstOnOff = (preOnOff == ZCL_ONOFF_STATUS_ON) ? ZCL_ONOFF_STATUS_OFF : ZCL_ONOFF_STATUS_ON;
			}else if(startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TO_PREVIOUS){
				dstOnOff = preOnOff;
			}

			if(dstOnOff != preOnOff){
				zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF, (u8 *)&dstOnOff);
			}
	}
}


_CODE_ZCL_ status_t zcl_onOff_on(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ON_OFF, ZCL_CMD_ONOFF_ON, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_onOff_off(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ON_OFF, ZCL_CMD_ONOFF_OFF, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_onOff_toggle(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ON_OFF, ZCL_CMD_ONOFF_TOGGLE, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_onOff_offWithEffect(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_onoff_offWithEffectCmd_t *pOffWithEffect)
{
	u8 buf[2];
	
	buf[0] = pOffWithEffect->effectId;
	buf[1] = pOffWithEffect->effectVariant;
	
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ON_OFF, ZCL_CMD_OFF_WITH_EFFECT, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_onOff_onWithRecallGlobalScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ON_OFF, ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_onOff_onWithTimedOff(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_onoff_onWithTimeOffCmd_t *pOnWithTimedOff)
{
	u8 buf[5];

	buf[0] = pOnWithTimedOff->onOffCtrl.onOffCtrl;
    buf[1] = LO_UINT16(pOnWithTimedOff->onTime);
    buf[2] = HI_UINT16(pOnWithTimedOff->onTime);
    buf[3] = LO_UINT16(pOnWithTimedOff->offWaitTime);
    buf[4] = HI_UINT16(pOnWithTimedOff->offWaitTime);
	
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ON_OFF, ZCL_CMD_ON_WITH_TIMED_OFF, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 5, buf);
}


_CODE_ZCL_ static status_t zcl_onOff_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	zcl_onoff_cmdPayload_t cmdPayload;
	memset((u8 *)&cmdPayload, 0, sizeof(zcl_onoff_cmdPayload_t));

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_ONOFF_OFF:
		case ZCL_CMD_ONOFF_ON:
		case ZCL_CMD_ONOFF_TOGGLE:
		case ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE:

			break;
		case ZCL_CMD_OFF_WITH_EFFECT:
			cmdPayload.offWithEffect.effectId = pData[0];
			cmdPayload.offWithEffect.effectVariant = pData[1];
			break;
		case ZCL_CMD_ON_WITH_TIMED_OFF:
			cmdPayload.onWithTimeOff.onOffCtrl.onOffCtrl = *pData++;
			cmdPayload.onWithTimeOff.onTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.onWithTimeOff.offWaitTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
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

_CODE_ZCL_ static status_t zcl_onOff_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_onOff_clientCmdHandler(pInMsg);
	}else{
		return ZCL_STA_UNSUP_CLUSTER_COMMAND;
	}
}

#endif  /* ZCL_ON_OFF */







