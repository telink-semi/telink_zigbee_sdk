/********************************************************************************************************
 * @file     zcl_light_colorCtrl.c
 *
 * @brief	 APIs for color control cluster
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


#ifdef ZCL_LIGHT_COLOR_CONTROL
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
static void zcl_lightColorCtrl_startUpColorTempMireds(u8 endpoint);
static status_t zcl_lightColorCtrl_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_lightColorCtrl_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	u8 status = ZCL_STA_SUCCESS;

	status = zcl_registerCluster(endpoint, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, attrNum, attrTbl, zcl_lightColorCtrl_cmdHandler, cb);

	if(status == ZCL_STA_SUCCESS){
		zcl_lightColorCtrl_startUpColorTempMireds(endpoint);
	}

	return status;
}

_CODE_ZCL_ static void zcl_lightColorCtrl_startUpColorTempMireds(u8 endpoint)
{
	u16 startUpColorTempMireds = 0;
	u16 preColorTempMireds = 0;
	u16 attrLen = 0;

	if( (zcl_getAttrVal(endpoint, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_ATTRID_START_UP_COLOR_TEMPERATURE_MIREDS, &attrLen, (u8 *)&startUpColorTempMireds) == ZCL_STA_SUCCESS) &&
		(zcl_getAttrVal(endpoint, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_ATTRID_COLOR_TEMPERATURE_MIREDS, &attrLen, (u8 *)&preColorTempMireds) == ZCL_STA_SUCCESS) ){
			u16 dstColorTempMireds = 0;

			if(startUpColorTempMireds == ZCL_START_UP_COLOR_TEMPERATURE_MIREDS_TO_PREVIOUS){
				dstColorTempMireds = preColorTempMireds;
			}else{
				dstColorTempMireds = startUpColorTempMireds;
			}

			if(dstColorTempMireds != preColorTempMireds){
				zcl_setAttrVal(endpoint, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_ATTRID_COLOR_TEMPERATURE_MIREDS, (u8 *)&dstColorTempMireds);
			}
	}
}


_CODE_ZCL_ status_t zcl_lightColorCtrl_move2hue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToHueCmd_t *pMove2Hue)
{
    u8 buf[6];
    u8 *pBuf = buf;

    *pBuf++ = pMove2Hue->hue;
    *pBuf++ = pMove2Hue->direction;
    *pBuf++ = LO_UINT16(pMove2Hue->transitionTime);
    *pBuf++ = HI_UINT16(pMove2Hue->transitionTime);
    if(pMove2Hue->optPresent){
        *pBuf++ = pMove2Hue->optionsMask;
        *pBuf++ = pMove2Hue->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_moveHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveHueCmd_t *pMoveHue)
{
    u8 buf[4];
    u8 *pBuf = buf;

    *pBuf++ = pMoveHue->moveMode;
    *pBuf++ = pMoveHue->rate;
    if(pMoveHue->optPresent){
        *pBuf++ = pMoveHue->optionsMask;
        *pBuf++ = pMoveHue->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_HUE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_stepHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepHueCmd_t *pStepHue)
{
    u8 buf[5];
    u8 *pBuf = buf;

    *pBuf++ = pStepHue->stepMode;
    *pBuf++ = pStepHue->stepSize;
    *pBuf++ = pStepHue->transitionTime;
    if(pStepHue->optPresent){
        *pBuf++ = pStepHue->optionsMask;
        *pBuf++ = pStepHue->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_HUE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_move2saturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToSaturationCmd_t *pMove2Sat)
{
    u8 buf[5];
    u8 *pBuf = buf;

    *pBuf++ = pMove2Sat->saturation;
    *pBuf++ = LO_UINT16(pMove2Sat->transitionTime);
    *pBuf++ = HI_UINT16(pMove2Sat->transitionTime);
    if(pMove2Sat->optPresent){
        *pBuf++ = pMove2Sat->optionsMask;
        *pBuf++ = pMove2Sat->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_SATURATION, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_moveSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveSaturationCmd_t *pMoveSat)
{
    u8 buf[4];
    u8 *pBuf = buf;

    *pBuf++ = pMoveSat->moveMode;
    *pBuf++ = pMoveSat->rate;
    if(pMoveSat->optPresent){
        *pBuf++ = pMoveSat->optionsMask;
        *pBuf++ = pMoveSat->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_SATURATION, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_stepSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepSaturationCmd_t *pStepSat)
{
    u8 buf[5];
    u8 *pBuf = buf;

    *pBuf++ = pStepSat->stepMode;
    *pBuf++ = pStepSat->stepSize;
    *pBuf++ = pStepSat->transitionTime;
    if(pStepSat->optPresent){
        *pBuf++ = pStepSat->optionsMask;
        *pBuf++ = pStepSat->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_SATURATION, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_move2hueAndSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToHueAndSaturationCmd_t *pMove2HueAndSat)
{
    u8 buf[6];
    u8 *pBuf = buf;

    *pBuf++ = pMove2HueAndSat->hue;
    *pBuf++ = pMove2HueAndSat->saturation;
    *pBuf++ = LO_UINT16(pMove2HueAndSat->transitionTime);
    *pBuf++ = HI_UINT16(pMove2HueAndSat->transitionTime);
    if(pMove2HueAndSat->optPresent){
        *pBuf++ = pMove2HueAndSat->optionsMask;
        *pBuf++ = pMove2HueAndSat->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE_AND_SATURATION, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}
_CODE_ZCL_ status_t zcl_lightColorCtrl_move2color(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToColorCmd_t *pMove2Color)
{
    u8 buf[8];
    u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pMove2Color->colorX);
    *pBuf++ = HI_UINT16(pMove2Color->colorX);
    *pBuf++ = LO_UINT16(pMove2Color->colorY);
    *pBuf++ = HI_UINT16(pMove2Color->colorY);
    *pBuf++ = LO_UINT16(pMove2Color->transitionTime);
    *pBuf++ = HI_UINT16(pMove2Color->transitionTime);
    if(pMove2Color->optPresent){
        *pBuf++ = pMove2Color->optionsMask;
        *pBuf++ = pMove2Color->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_moveColor(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveColorCmd_t *pMoveColor)
{
    u8 buf[6];
    u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pMoveColor->rateX);
    *pBuf++ = HI_UINT16(pMoveColor->rateX);
    *pBuf++ = LO_UINT16(pMoveColor->rateY);
    *pBuf++ = HI_UINT16(pMoveColor->rateY);
    if(pMoveColor->optPresent){
        *pBuf++ = pMoveColor->optionsMask;
        *pBuf++ = pMoveColor->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_stepColor(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepColorCmd_t *pStepColor)
{
    u8 buf[8];
    u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pStepColor->stepX);
    *pBuf++ = HI_UINT16(pStepColor->stepX);
    *pBuf++ = LO_UINT16(pStepColor->stepY);
    *pBuf++ = HI_UINT16(pStepColor->stepY);
    *pBuf++ = LO_UINT16(pStepColor->transitionTime);
    *pBuf++ = HI_UINT16(pStepColor->transitionTime);
    if(pStepColor->optPresent){
        *pBuf++ = pStepColor->optionsMask;
        *pBuf++ = pStepColor->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_move2colorTemperature(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToColorTemperatureCmd_t *pMove2ColorTemp)
{
    u8 buf[6];
    u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pMove2ColorTemp->colorTemperature);
    *pBuf++ = HI_UINT16(pMove2ColorTemp->colorTemperature);
    *pBuf++ = LO_UINT16(pMove2ColorTemp->transitionTime);
    *pBuf++ = HI_UINT16(pMove2ColorTemp->transitionTime);
    if(pMove2ColorTemp->optPresent){
        *pBuf++ = pMove2ColorTemp->optionsMask;
        *pBuf++ = pMove2ColorTemp->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_enhancedMove2Hue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedMoveToHueCmd_t *pEnhancedMove2Hue)
{
    u8 buf[7];
    u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pEnhancedMove2Hue->enhancedHue);
    *pBuf++ = HI_UINT16(pEnhancedMove2Hue->enhancedHue);
    *pBuf++ = pEnhancedMove2Hue->direction;
    *pBuf++ = LO_UINT16(pEnhancedMove2Hue->transitionTime);
    *pBuf++ = HI_UINT16(pEnhancedMove2Hue->transitionTime);
    if(pEnhancedMove2Hue->optPresent){
        *pBuf++ = pEnhancedMove2Hue->optionsMask;
        *pBuf++ = pEnhancedMove2Hue->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_enhancedMoveHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedMoveHueCmd_t *pEnhancedMoveHue)
{
    u8 buf[5];
    u8 *pBuf = buf;

    *pBuf++ = pEnhancedMoveHue->moveMode;
    *pBuf++ = LO_UINT16(pEnhancedMoveHue->rate);
    *pBuf++ = HI_UINT16(pEnhancedMoveHue->rate);
    if(pEnhancedMoveHue->optPresent){
        *pBuf++ = pEnhancedMoveHue->optionsMask;
        *pBuf++ = pEnhancedMoveHue->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_HUE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_enhancedStepHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedStepHueCmd_t *pEnhancedStepHue)
{
    u8 buf[7];
    u8 *pBuf = buf;

    *pBuf++ = pEnhancedStepHue->stepMode;
    *pBuf++ = LO_UINT16(pEnhancedStepHue->stepSize);
    *pBuf++ = HI_UINT16(pEnhancedStepHue->stepSize);
    *pBuf++ = LO_UINT16(pEnhancedStepHue->transitionTime);
    *pBuf++ = HI_UINT16(pEnhancedStepHue->transitionTime);
    if(pEnhancedStepHue->optPresent){
        *pBuf++ = pEnhancedStepHue->optionsMask;
        *pBuf++ = pEnhancedStepHue->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_STEP_HUE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_enhancedMove2HueAndSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedMoveToHueAndSaturationCmd_t *pEnhancedMove2HueAndSat)
{
    u8 buf[7];
    u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pEnhancedMove2HueAndSat->enhancedHue);
    *pBuf++ = HI_UINT16(pEnhancedMove2HueAndSat->enhancedHue);
    *pBuf++ = pEnhancedMove2HueAndSat->saturation;
    *pBuf++ = LO_UINT16(pEnhancedMove2HueAndSat->transitionTime);
    *pBuf++ = HI_UINT16(pEnhancedMove2HueAndSat->transitionTime);
    if(pEnhancedMove2HueAndSat->optPresent){
        *pBuf++ = pEnhancedMove2HueAndSat->optionsMask;
        *pBuf++ = pEnhancedMove2HueAndSat->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_AND_SATURATION, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_colorLoopSet(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlColorLoopSetCmd_t *pColorLoopSet)
{
    u8 buf[9];
    u8 *pBuf = buf;

    *pBuf++ = pColorLoopSet->updateFlags.updateFlags;
    *pBuf++ = pColorLoopSet->action;
    *pBuf++ = pColorLoopSet->direction;
    *pBuf++ = LO_UINT16(pColorLoopSet->time);
    *pBuf++ = HI_UINT16(pColorLoopSet->time);
    *pBuf++ = LO_UINT16(pColorLoopSet->startHue);
    *pBuf++ = HI_UINT16(pColorLoopSet->startHue);
    if(pColorLoopSet->optPresent){
        *pBuf++ = pColorLoopSet->optionsMask;
        *pBuf++ = pColorLoopSet->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_COLOR_LOOP_SET, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_stopMoveStep(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStopMoveStepCmd_t *pStopMoveStep)
{
    u8 buf[2];
    u8 *pBuf = buf;

    if(pStopMoveStep->optPresent){
        *pBuf++ = pStopMoveStep->optionsMask;
        *pBuf++ = pStopMoveStep->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_STOP_MOVE_STEP, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_moveColorTemperature(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveColorTemperatureCmd_t *pMoveColorTemp)
{
    u8 buf[9];
    u8 *pBuf = buf;

    *pBuf++ = pMoveColorTemp->moveMode;
    *pBuf++ = LO_UINT16(pMoveColorTemp->rate);
    *pBuf++ = HI_UINT16(pMoveColorTemp->rate);
    *pBuf++ = LO_UINT16(pMoveColorTemp->colorTempMinMireds);
    *pBuf++ = HI_UINT16(pMoveColorTemp->colorTempMinMireds);
    *pBuf++ = LO_UINT16(pMoveColorTemp->colorTempMaxMireds);
    *pBuf++ = HI_UINT16(pMoveColorTemp->colorTempMaxMireds);
    if(pMoveColorTemp->optPresent){
        *pBuf++ = pMoveColorTemp->optionsMask;
        *pBuf++ = pMoveColorTemp->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_lightColorCtrl_stepColorTemperature(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepColorTemperatureCmd_t *pStepColorTemp)
{
    u8 buf[11];
    u8 *pBuf = buf;

    *pBuf++ = pStepColorTemp->stepMode;
    *pBuf++ = LO_UINT16(pStepColorTemp->stepSize);
    *pBuf++ = HI_UINT16(pStepColorTemp->stepSize);
    *pBuf++ = LO_UINT16(pStepColorTemp->transitionTime);
    *pBuf++ = HI_UINT16(pStepColorTemp->transitionTime);
    *pBuf++ = LO_UINT16(pStepColorTemp->colorTempMinMireds);
    *pBuf++ = HI_UINT16(pStepColorTemp->colorTempMinMireds);
    *pBuf++ = LO_UINT16(pStepColorTemp->colorTempMaxMireds);
    *pBuf++ = HI_UINT16(pStepColorTemp->colorTempMaxMireds);
    if(pStepColorTemp->optPresent){
        *pBuf++ = pStepColorTemp->optionsMask;
        *pBuf++ = pStepColorTemp->optionsOverride;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR_TEMPERATURE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ static status_t zcl_lightColorCtrl_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	zcl_colorCtrl_cmdPayload_t cmdPayload;
	memset((u8 *)&cmdPayload, 0, sizeof(zcl_colorCtrl_cmdPayload_t));

    switch(pInMsg->hdr.cmd)
    {
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE:
			cmdPayload.moveToHue.hue = *pData++;
			cmdPayload.moveToHue.direction  = *pData++;
			cmdPayload.moveToHue.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 6){
				cmdPayload.moveToHue.optionsMask = *pData++;
				cmdPayload.moveToHue.optionsOverride = *pData++;
				cmdPayload.moveToHue.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_HUE:
			cmdPayload.moveHue.moveMode = *pData++;
			cmdPayload.moveHue.rate  = *pData++;
			if(pInMsg->dataLen == 4){
				cmdPayload.moveHue.optionsMask = *pData++;
				cmdPayload.moveHue.optionsOverride = *pData++;
				cmdPayload.moveHue.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_HUE:
			cmdPayload.stepHue.stepMode = *pData++;
			cmdPayload.stepHue.stepSize = *pData++;
			cmdPayload.stepHue.transitionTime = *pData++;
			if(pInMsg->dataLen == 5){
				cmdPayload.stepHue.optionsMask = *pData++;
				cmdPayload.stepHue.optionsOverride = *pData++;
				cmdPayload.stepHue.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_SATURATION:
			cmdPayload.moveToSaturation.saturation = *pData++;
			cmdPayload.moveToSaturation.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 5){
				cmdPayload.moveToSaturation.optionsMask = *pData++;
				cmdPayload.moveToSaturation.optionsOverride = *pData++;
				cmdPayload.moveToSaturation.optPresent = 1;
			}
			 break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_SATURATION:
			cmdPayload.moveSaturation.moveMode = *pData++;
			cmdPayload.moveSaturation.rate = *pData++;
			if(pInMsg->dataLen == 4){
				cmdPayload.moveSaturation.optionsMask = *pData++;
				cmdPayload.moveSaturation.optionsOverride = *pData++;
				cmdPayload.moveSaturation.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_SATURATION:
			cmdPayload.stepSaturation.stepMode = *pData++;
			cmdPayload.stepSaturation.stepSize = *pData++;
			cmdPayload.stepSaturation.transitionTime = *pData++;
			if(pInMsg->dataLen == 5){
				cmdPayload.stepSaturation.optionsMask = *pData++;
				cmdPayload.stepSaturation.optionsOverride = *pData++;
				cmdPayload.stepSaturation.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE_AND_SATURATION:
			cmdPayload.moveToHueAndSaturation.hue = *pData++;
			cmdPayload.moveToHueAndSaturation.saturation = *pData++;
			cmdPayload.moveToHueAndSaturation.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 6){
				cmdPayload.moveToHueAndSaturation.optionsMask = *pData++;
				cmdPayload.moveToHueAndSaturation.optionsOverride = *pData++;
				cmdPayload.moveToHue.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR:
			cmdPayload.moveToColor.colorX = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.moveToColor.colorY = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.moveToColor.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 8){
				cmdPayload.moveToColor.optionsMask = *pData++;
				cmdPayload.moveToColor.optionsOverride = *pData++;
				cmdPayload.moveToColor.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR:
			cmdPayload.moveColor.rateX = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.moveColor.rateY = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 6){
				cmdPayload.moveColor.optionsMask = *pData++;
				cmdPayload.moveColor.optionsOverride = *pData++;
				cmdPayload.moveColor.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR:
			cmdPayload.stepColor.stepX = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.stepColor.stepY = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.stepColor.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 8){
				cmdPayload.stepColor.optionsMask = *pData++;
				cmdPayload.stepColor.optionsOverride = *pData++;
				cmdPayload.stepColor.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE:
			cmdPayload.moveToColorTemperature.colorTemperature = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.moveToColorTemperature.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 6){
				cmdPayload.moveToColorTemperature.optionsMask = *pData++;
				cmdPayload.moveToColorTemperature.optionsOverride = *pData++;
				cmdPayload.moveColor.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE:
			cmdPayload.enhancedMoveToHue.enhancedHue = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.enhancedMoveToHue.direction = *pData++;
			cmdPayload.enhancedMoveToHue.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 7){
				cmdPayload.enhancedMoveToHue.optionsMask = *pData++;
				cmdPayload.enhancedMoveToHue.optionsOverride = *pData++;
				cmdPayload.enhancedMoveToHue.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_HUE:
			cmdPayload.enhancedMoveHue.moveMode = *pData++;
			cmdPayload.enhancedMoveHue.rate = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 5){
				cmdPayload.enhancedMoveHue.optionsMask = *pData++;
				cmdPayload.enhancedMoveHue.optionsOverride = *pData++;
				cmdPayload.enhancedMoveHue.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_STEP_HUE:
			cmdPayload.enhancedStepHue.stepMode = *pData++;
			cmdPayload.enhancedStepHue.stepSize = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.enhancedStepHue.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 7){
				cmdPayload.enhancedStepHue.optionsMask = *pData++;
				cmdPayload.enhancedStepHue.optionsOverride = *pData++;
				cmdPayload.enhancedStepHue.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_AND_SATURATION:
			cmdPayload.enhancedMoveToHueAndSaturation.enhancedHue = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.enhancedMoveToHueAndSaturation.saturation = *pData++;
			cmdPayload.enhancedMoveToHueAndSaturation.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 7){
				cmdPayload.enhancedMoveToHueAndSaturation.optionsMask = *pData++;
				cmdPayload.enhancedMoveToHueAndSaturation.optionsOverride = *pData++;
				cmdPayload.enhancedMoveToHueAndSaturation.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_COLOR_LOOP_SET:
			cmdPayload.colorLoopSet.updateFlags.updateFlags = *pData++;
			cmdPayload.colorLoopSet.action = *pData++;
			cmdPayload.colorLoopSet.direction = *pData++;
			cmdPayload.colorLoopSet.time = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.colorLoopSet.startHue = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 9){
				cmdPayload.colorLoopSet.optionsMask = *pData++;
				cmdPayload.colorLoopSet.optionsOverride = *pData++;
				cmdPayload.colorLoopSet.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_STOP_MOVE_STEP:
			if(pInMsg->dataLen == 2){
				cmdPayload.stopMoveStep.optionsMask = *pData++;
				cmdPayload.stopMoveStep.optionsOverride = *pData++;
				cmdPayload.stopMoveStep.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE:
			cmdPayload.moveColorTemperature.moveMode = *pData++;
			cmdPayload.moveColorTemperature.rate = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.moveColorTemperature.colorTempMinMireds = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.moveColorTemperature.colorTempMaxMireds = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 9){
				cmdPayload.moveColorTemperature.optionsMask = *pData++;
				cmdPayload.moveColorTemperature.optionsOverride = *pData++;
				cmdPayload.moveColorTemperature.optPresent = 1;
			}
			break;
		case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR_TEMPERATURE:
			cmdPayload.stepColorTemperature.stepMode = *pData++;
			cmdPayload.stepColorTemperature.stepSize = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.stepColorTemperature.transitionTime = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.stepColorTemperature.colorTempMinMireds = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.stepColorTemperature.colorTempMaxMireds = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			if(pInMsg->dataLen == 11){
				cmdPayload.stepColorTemperature.optionsMask = *pData++;
				cmdPayload.stepColorTemperature.optionsOverride = *pData++;
				cmdPayload.stepColorTemperature.optPresent = 1;
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

_CODE_ZCL_ static status_t zcl_lightColorCtrl_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_lightColorCtrl_clientCmdHandler(pInMsg);
	}else{
		return ZCL_STA_UNSUP_CLUSTER_COMMAND;
	}
}

#endif	/* ZCL_LIGHT_COLOR_CONTROL */
