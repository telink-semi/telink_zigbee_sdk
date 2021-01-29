/********************************************************************************************************
 * @file	zcl_onoff.c
 *
 * @brief	This is the source file for zcl_onoff
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

_CODE_ZCL_ status_t zcl_onOff_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	u8 status = ZCL_STA_SUCCESS;

    status = zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF, manuCode, attrNum, attrTbl, zcl_onOff_cmdHandler, cb);

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







