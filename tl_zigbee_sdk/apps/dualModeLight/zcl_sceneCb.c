/********************************************************************************************************
 * @file     zcl_sceneCb.c
 *
 * @brief    call back function for cluster of "SCENE"
 *
 * @author
 * @date     Dec. 16, 2016
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
#if (__PROJECT_DUAL_MODE_LIGHT__)


/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"


/*********************************************************************
 * @fn      sampleLight_sceneRecallReqHandler
 *
 * @brief   Handler for ZCL scene recall command. This function will recall scene.
 *
 * @param	pApsdeInd
 * @param	pScene
 *
 * @return  None
 */
static void sampleLight_sceneRecallReqHandler(zclIncomingAddrInfo_t *pAddrInfo, zcl_sceneEntry_t *pScene)
{
	u8 extLen = 0;

#ifdef ZCL_ON_OFF
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	pOnOff->onOff = pScene->extField[extLen+3];
	extLen += 4;
#endif

#ifdef ZCL_LEVEL_CTRL
	u8 level = pScene->extField[extLen+3];
	extLen += 4;
#endif

#ifdef ZCL_LIGHT_COLOR_CONTROL
#if COLOR_RGB_SUPPORT
	u8 hue = pScene->extField[extLen+3];
	u8 saturation = pScene->extField[extLen+4];
	extLen += 5;
#elif COLOR_CCT_SUPPORT
	u16 colorTemperatureMireds = BUILD_U16(pScene->extField[extLen+3], pScene->extField[extLen+4]);
	extLen += 5;
#endif
#endif

#ifdef ZCL_LEVEL_CTRL
	moveToLvl_t moveToLevel;
	moveToLevel.level = level;
	moveToLevel.transitionTime = pScene->transTime;
	moveToLevel.optPresent = 0;

	sampleLight_levelCb(pAddrInfo, ZCL_CMD_LEVEL_MOVE_TO_LEVEL, &moveToLevel);
#endif

#ifdef ZCL_LIGHT_COLOR_CONTROL
#if COLOR_RGB_SUPPORT
	zcl_colorCtrlMoveToHueAndSaturationCmd_t move2HueAndSat;
	move2HueAndSat.hue = hue;
	move2HueAndSat.saturation = saturation;
	move2HueAndSat.transitionTime = pScene->transTime;
	move2HueAndSat.optPresent = 0;

	sampleLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE_AND_SATURATION, &move2HueAndSat);
#elif COLOR_CCT_SUPPORT
	zcl_colorCtrlMoveToColorTemperatureCmd_t move2ColorTemp;
	move2ColorTemp.colorTemperature = colorTemperatureMireds;
	move2ColorTemp.transitionTime = pScene->transTime;
	move2ColorTemp.optPresent = 0;

	sampleLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE, &move2ColorTemp);
#endif
#endif
}

/*********************************************************************
 * @fn      sampleLight_sceneStoreReqHandler
 *
 * @brief   Handler for ZCL scene store command. This function will set scene attribute first.
 *
 * @param   pApsdeInd
 * @param   pScene
 *
 * @return  None
 */
static void sampleLight_sceneStoreReqHandler(zcl_sceneEntry_t *pScene)
{
	/* receive Store Scene Request command, get the latest Scene info to save */
	u8 extLen = 0;

#ifdef ZCL_ON_OFF
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
	pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
	pScene->extField[extLen++] = 1;
	pScene->extField[extLen++] = pOnOff->onOff;
#endif

#ifdef ZCL_LEVEL_CTRL
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_GEN_LEVEL_CONTROL);
	pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_GEN_LEVEL_CONTROL);
	pScene->extField[extLen++] = 1;
	pScene->extField[extLen++] = pLevel->curLevel;
#endif

#ifdef ZCL_LIGHT_COLOR_CONTROL
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL);
	pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL);
#if COLOR_RGB_SUPPORT
	pScene->extField[extLen++] = 2;
	pScene->extField[extLen++] = pColor->currentHue;
	pScene->extField[extLen++] = pColor->currentSaturation;
#elif COLOR_CCT_SUPPORT
	pScene->extField[extLen++] = 2;
	pScene->extField[extLen++] = LO_UINT16(pColor->colorTemperatureMireds);
	pScene->extField[extLen++] = HI_UINT16(pColor->colorTemperatureMireds);
#endif
#endif

	pScene->extFieldLen = extLen;
}

/*********************************************************************
 * @fn      sampleLight_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleLight_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_LIGHT_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_SCENE_STORE_SCENE:
					sampleLight_sceneStoreReqHandler((zcl_sceneEntry_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_RECALL_SCENE:
					sampleLight_sceneRecallReqHandler(pAddrInfo, (zcl_sceneEntry_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif  /* __PROJECT_DUAL_MODE_LIGHT__ */
