/********************************************************************************************************
 * @file     zcl_colorCtrlCb.c
 *
 * @brief    call back function for cluster of "color Control"
 *
 * @author
 * @date     April. 23, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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

#if (__PROJECT_TL_DIMMABLE_LIGHT__)
/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"

#ifdef ZCL_LIGHT_COLOR_CONTROL

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_COLOR_CHANGE_INTERVAL		100



/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
#if COLOR_RGB_SUPPORT
	s32 stepHue256;
	u16	currentHue256;
	u16	hueRemainingTime;

	s32 stepSaturation256;
	u16 currentSaturation256;
	u16 saturationRemainingTime;
	
#elif COLOR_CCT_SUPPORT
	s32 stepColorTemp256;
	u32	currentColorTemp256;
	u16	colorTempRemainingTime;
	u16	colorTempMinMireds;
	u16	colorTempMaxMireds;
#endif
}zcl_colorInfo_t;

/**********************************************************************
 * LOCAL VARIABLES
 */
static zcl_colorInfo_t colorInfo = {
#if COLOR_RGB_SUPPORT
	.stepHue256 				= 0,
	.currentHue256				= 0,
	.hueRemainingTime			= 0,

	.stepSaturation256			= 0,
	.currentSaturation256		= 0,
	.saturationRemainingTime	= 0,
	
#elif COLOR_CCT_SUPPORT
	.stepColorTemp256			= 0,
	.currentColorTemp256		= 0,
	.colorTempRemainingTime		= 0,
	.colorTempMinMireds			= 0,
	.colorTempMaxMireds			= 0,
#endif
};

static ev_time_event_t *colorTimerEvt = NULL;
#if COLOR_RGB_SUPPORT
static ev_time_event_t *colorLoopTimerEvt = NULL;
#endif


/**********************************************************************
 * FUNCTIONS
 */
void sampleLight_updateColorMode(u8 colorMode);


/*********************************************************************
 * @fn      sampleLight_colorInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void sampleLight_colorInit(void)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

#if COLOR_RGB_SUPPORT
	pColor->colorCapabilities = ZCL_COLOR_CAPABILITIES_BIT_HUE_SATURATION;
	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;

	colorInfo.currentHue256 = (u16)(pColor->currentHue) << 8;
	colorInfo.currentSaturation256 = (u16)(pColor->currentSaturation) << 8;

	colorInfo.hueRemainingTime = 0;
	colorInfo.saturationRemainingTime = 0;

	light_applyUpdate(&pColor->currentHue, &colorInfo.currentHue256, &colorInfo.stepHue256, &colorInfo.hueRemainingTime,
						ZCL_COLOR_ATTR_HUE_MIN, ZCL_COLOR_ATTR_HUE_MAX, TRUE);

	light_applyUpdate(&pColor->currentSaturation, &colorInfo.currentSaturation256, &colorInfo.stepSaturation256, &colorInfo.saturationRemainingTime,
						ZCL_COLOR_ATTR_SATURATION_MIN, ZCL_COLOR_ATTR_SATURATION_MAX, FALSE);
#elif COLOR_CCT_SUPPORT
	pColor->colorCapabilities = ZCL_COLOR_CAPABILITIES_BIT_COLOR_TEMPERATURE;
	pColor->colorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;

	colorInfo.currentColorTemp256 = (u32)(pColor->colorTemperatureMireds) << 8;
	colorInfo.colorTempRemainingTime = 0;

	light_applyUpdate_16(&pColor->colorTemperatureMireds, &colorInfo.currentColorTemp256, &colorInfo.stepColorTemp256, &colorInfo.colorTempRemainingTime,
							pColor->colorTempPhysicalMinMireds, pColor->colorTempPhysicalMaxMireds, FALSE);
#endif
}

/*********************************************************************
 * @fn      sampleLight_updateColorMode
 *
 * @brief
 *
 * @param   ZCL_COLOR_MODE_CURRENT_HUE_SATURATION
 * 			ZCL_COLOR_MODE_CURRENT_X_Y
 * 			ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS
 * 			ZCL_ENHANCED_COLOR_MODE_CURRENT_HUE_SATURATION
 *
 * @return  None
 */
void sampleLight_updateColorMode(u8 colorMode)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	if(colorMode != pColor->colorMode){
		if(colorMode == ZCL_COLOR_MODE_CURRENT_X_Y){

		}else if(colorMode == ZCL_COLOR_MODE_CURRENT_HUE_SATURATION){

		}else if(colorMode == ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS){

		}
	}
}

/*********************************************************************
 * @fn      sampleLight_updateColor
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void sampleLight_updateColor(void)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

#if COLOR_RGB_SUPPORT
	hwLight_colorUpdate_HSV2RGB(pColor->currentHue, pColor->currentSaturation, pLevel->curLevel);
#elif COLOR_CCT_SUPPORT
	hwLight_colorUpdate_colorTemperature(pColor->colorTemperatureMireds, pLevel->curLevel);
#endif
}

/*********************************************************************
 * @fn      sampleLight_colorTimerEvtCb
 *
 * @brief
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 sampleLight_colorTimerEvtCb(void *arg)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

#if COLOR_RGB_SUPPORT
	if( (pColor->enhancedColorMode == ZCL_COLOR_MODE_CURRENT_HUE_SATURATION) ||
		(pColor->enhancedColorMode == ZCL_ENHANCED_COLOR_MODE_CURRENT_HUE_SATURATION) ){
		if(colorInfo.saturationRemainingTime){
			light_applyUpdate(&pColor->currentSaturation, &colorInfo.currentSaturation256, &colorInfo.stepSaturation256, &colorInfo.saturationRemainingTime,
									ZCL_COLOR_ATTR_SATURATION_MIN, ZCL_COLOR_ATTR_SATURATION_MAX, FALSE);
		}

		if(colorInfo.hueRemainingTime){
			light_applyUpdate(&pColor->currentHue, &colorInfo.currentHue256, &colorInfo.stepHue256, &colorInfo.hueRemainingTime,
									ZCL_COLOR_ATTR_HUE_MIN, ZCL_COLOR_ATTR_HUE_MAX, TRUE);
		}
	}
#elif COLOR_CCT_SUPPORT
	if(pColor->enhancedColorMode == ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS){
		if(colorInfo.colorTempRemainingTime){
			light_applyUpdate_16(&pColor->colorTemperatureMireds, &colorInfo.currentColorTemp256, &colorInfo.stepColorTemp256, &colorInfo.colorTempRemainingTime,
									colorInfo.colorTempMinMireds, colorInfo.colorTempMaxMireds, FALSE);
		}
	}
#endif

#if COLOR_RGB_SUPPORT
	if(colorInfo.saturationRemainingTime || colorInfo.hueRemainingTime){
#elif COLOR_CCT_SUPPORT
	if(colorInfo.colorTempRemainingTime){
#endif
		return 0;
	}else{
		colorTimerEvt = NULL;
		return -1;
	}
}

/*********************************************************************
 * @fn      sampleLight_colorTimerStop
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
static void sampleLight_colorTimerStop(void)
{
	if(colorTimerEvt){
		TL_ZB_TIMER_CANCEL(&colorTimerEvt);
	}
}

#if COLOR_RGB_SUPPORT
/*********************************************************************
 * @fn      sampleLight_colorLoopTimerEvtCb
 *
 * @brief
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 sampleLight_colorLoopTimerEvtCb(void *arg)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	if(pColor->colorLoopActive){

	}else{
		colorLoopTimerEvt = NULL;
		return -1;
	}

	return 0;
}

/*********************************************************************
 * @fn      sampleLight_colorLoopTimerStop
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
static void sampleLight_colorLoopTimerStop(void)
{
	if(colorLoopTimerEvt){
		TL_ZB_TIMER_CANCEL(&colorLoopTimerEvt);
	}
}


/*********************************************************************
 * @fn      sampleLight_moveToHueProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveToHueProcess(zcl_colorCtrlMoveToHueCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;

	colorInfo.currentHue256 = (u16)(pColor->currentHue) << 8;

	s16 hueDiff = (s16)cmd->hue - pColor->currentHue;

	switch(cmd->direction){
		case COLOR_CTRL_DIRECTION_SHORTEST_DISTANCE:
			if(hueDiff > (ZCL_COLOR_ATTR_HUE_MAX / 2)){
				hueDiff -= (ZCL_COLOR_ATTR_HUE_MAX + 1);
			}else if(hueDiff < -ZCL_COLOR_ATTR_HUE_MAX / 2){
				hueDiff += (ZCL_COLOR_ATTR_HUE_MAX + 1);
			}
			break;
		case COLOR_CTRL_DIRECTION_LONGEST_DISTANCE:
			if((hueDiff > 0) && (hueDiff < (ZCL_COLOR_ATTR_HUE_MAX / 2))){
				hueDiff -= (ZCL_COLOR_ATTR_HUE_MAX + 1);
			}else if((hueDiff < 0) && (hueDiff > -ZCL_COLOR_ATTR_HUE_MAX / 2)){
				hueDiff += (ZCL_COLOR_ATTR_HUE_MAX + 1);
			}
			break;
		case COLOR_CTRL_DIRECTION_UP:
			if(hueDiff < 0){
				hueDiff += ZCL_COLOR_ATTR_HUE_MAX;
			}
			break;
		case COLOR_CTRL_DIRECTION_DOWN:
			if(hueDiff > 0){
				hueDiff -= ZCL_COLOR_ATTR_HUE_MAX;
			}
			break;
		default:
			break;
	}

	colorInfo.hueRemainingTime = (cmd->transitionTime == 0) ? 1 : cmd->transitionTime;
	colorInfo.stepHue256 = ((s32)hueDiff) << 8;
	colorInfo.stepHue256 /= (s32)colorInfo.hueRemainingTime;

	light_applyUpdate(&pColor->currentHue, &colorInfo.currentHue256, &colorInfo.stepHue256, &colorInfo.hueRemainingTime,
							ZCL_COLOR_ATTR_HUE_MIN, ZCL_COLOR_ATTR_HUE_MAX, TRUE);

	if(colorInfo.hueRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_moveHueProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveHueProcess(zcl_colorCtrlMoveHueCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;

	colorInfo.currentHue256 = (u16)(pColor->currentHue) << 8;

	switch(cmd->moveMode){
		case COLOR_CTRL_MOVE_STOP:
			colorInfo.stepHue256 = 0;
			colorInfo.hueRemainingTime = 0;
			break;
		case COLOR_CTRL_MOVE_UP:
			colorInfo.stepHue256 = (((s32)cmd->rate) << 8) / 10;
			colorInfo.hueRemainingTime = 0xFFFF;
			break;
		case COLOR_CTRL_MOVE_DOWN:
			colorInfo.stepHue256 = ((-(s32)cmd->rate) << 8) / 10;
			colorInfo.hueRemainingTime = 0xFFFF;
			break;
		default:
			break;
	}

	light_applyUpdate(&pColor->currentHue, &colorInfo.currentHue256, &colorInfo.stepHue256, &colorInfo.hueRemainingTime,
							ZCL_COLOR_ATTR_HUE_MIN, ZCL_COLOR_ATTR_HUE_MAX, TRUE);

	if(colorInfo.hueRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_stepHueProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_stepHueProcess(zcl_colorCtrlStepHueCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;

	colorInfo.currentHue256 = (u16)(pColor->currentHue) << 8;

	colorInfo.hueRemainingTime = (cmd->transitionTime == 0) ? 1 : cmd->transitionTime;

	colorInfo.stepHue256 = (((s32)cmd->stepSize) << 8) / colorInfo.hueRemainingTime;

	switch(cmd->stepMode){
		case COLOR_CTRL_STEP_MODE_UP:
			break;
		case COLOR_CTRL_STEP_MODE_DOWN:
			colorInfo.stepHue256 = -colorInfo.stepHue256;
			break;
		default:
			break;
	}

	light_applyUpdate(&pColor->currentHue, &colorInfo.currentHue256, &colorInfo.stepHue256, &colorInfo.hueRemainingTime,
							ZCL_COLOR_ATTR_HUE_MIN, ZCL_COLOR_ATTR_HUE_MAX, TRUE);

	if(colorInfo.hueRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_moveToSaturationProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveToSaturationProcess(zcl_colorCtrlMoveToSaturationCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;

	colorInfo.currentSaturation256 = (u16)(pColor->currentSaturation) << 8;

	colorInfo.saturationRemainingTime = (cmd->transitionTime == 0) ? 1 : cmd->transitionTime;

	colorInfo.stepSaturation256 = ((s32)(cmd->saturation - pColor->currentSaturation)) << 8;
	colorInfo.stepSaturation256 /= (s32)colorInfo.saturationRemainingTime;

	light_applyUpdate(&pColor->currentSaturation, &colorInfo.currentSaturation256, &colorInfo.stepSaturation256, &colorInfo.saturationRemainingTime,
							ZCL_COLOR_ATTR_SATURATION_MIN, ZCL_COLOR_ATTR_SATURATION_MAX, FALSE);

	if(colorInfo.saturationRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_moveSaturationProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveSaturationProcess(zcl_colorCtrlMoveSaturationCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;

	colorInfo.currentSaturation256 = (u16)(pColor->currentSaturation) << 8;

	switch(cmd->moveMode){
		case COLOR_CTRL_MOVE_STOP:
			colorInfo.stepSaturation256 = 0;
			colorInfo.saturationRemainingTime = 0;
			break;
		case COLOR_CTRL_MOVE_UP:
			colorInfo.stepSaturation256 = (((s32)cmd->rate) << 8) / 10;
			colorInfo.saturationRemainingTime = 0xFFFF;
			break;
		case COLOR_CTRL_MOVE_DOWN:
			colorInfo.stepSaturation256 = ((-(s32)cmd->rate) << 8) / 10;
			colorInfo.saturationRemainingTime = 0xFFFF;
			break;
		default:
			break;
	}

	light_applyUpdate(&pColor->currentSaturation, &colorInfo.currentSaturation256, &colorInfo.stepSaturation256, &colorInfo.saturationRemainingTime,
							ZCL_COLOR_ATTR_SATURATION_MIN, ZCL_COLOR_ATTR_SATURATION_MAX, FALSE);

	if(colorInfo.saturationRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_stepSaturationProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_stepSaturationProcess(zcl_colorCtrlStepSaturationCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;

	colorInfo.currentSaturation256 = (u16)(pColor->currentSaturation) << 8;

	colorInfo.saturationRemainingTime = (cmd->transitionTime == 0) ? 1 : cmd->transitionTime;

	colorInfo.stepSaturation256 = (((s32)cmd->stepSize) << 8) / colorInfo.saturationRemainingTime;

	switch(cmd->stepMode){
		case COLOR_CTRL_STEP_MODE_UP:
			break;
		case COLOR_CTRL_STEP_MODE_DOWN:
			colorInfo.stepSaturation256 = -colorInfo.stepSaturation256;
			break;
		default:
			break;
	}

	light_applyUpdate(&pColor->currentSaturation, &colorInfo.currentSaturation256, &colorInfo.stepSaturation256, &colorInfo.saturationRemainingTime,
							ZCL_COLOR_ATTR_SATURATION_MIN, ZCL_COLOR_ATTR_SATURATION_MAX, FALSE);

	if(colorInfo.saturationRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_moveToHueAndSaturationProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveToHueAndSaturationProcess(zcl_colorCtrlMoveToHueAndSaturationCmd_t *cmd)
{
	zcl_colorCtrlMoveToHueCmd_t moveToHueCmd;
	zcl_colorCtrlMoveToSaturationCmd_t moveToSaturationCmd;

	moveToHueCmd.hue = cmd->hue;
	moveToHueCmd.direction = COLOR_CTRL_DIRECTION_SHORTEST_DISTANCE;
	moveToHueCmd.transitionTime = cmd->transitionTime;

	moveToSaturationCmd.saturation = cmd->saturation;
	moveToSaturationCmd.transitionTime = cmd->transitionTime;

	sampleLight_moveToHueProcess(&moveToHueCmd);
	sampleLight_moveToSaturationProcess(&moveToSaturationCmd);
}

/*********************************************************************
 * @fn      sampleLight_moveToColorProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveToColorProcess(zcl_colorCtrlMoveToColorCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_X_Y);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_X_Y;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_X_Y;


}

/*********************************************************************
 * @fn      sampleLight_moveColorProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveColorProcess(zcl_colorCtrlMoveColorCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_X_Y);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_X_Y;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_X_Y;


}

/*********************************************************************
 * @fn      sampleLight_stepColorProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_stepColorProcess(zcl_colorCtrlStepColorCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_X_Y);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_X_Y;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_CURRENT_X_Y;


}

/*********************************************************************
 * @fn      sampleLight_enhancedMoveToHueProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_enhancedMoveToHueProcess(zcl_colorCtrlEnhancedMoveToHueCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_ENHANCED_COLOR_MODE_CURRENT_HUE_SATURATION;

	switch(cmd->direction){
		case COLOR_CTRL_DIRECTION_SHORTEST_DISTANCE:
			break;
		case COLOR_CTRL_DIRECTION_LONGEST_DISTANCE:
			break;
		case COLOR_CTRL_DIRECTION_UP:
			break;
		case COLOR_CTRL_DIRECTION_DOWN:
			break;
		default:
			break;
	}


}

/*********************************************************************
 * @fn      sampleLight_enhancedMoveHueProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_enhancedMoveHueProcess(zcl_colorCtrlEnhancedMoveHueCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_ENHANCED_COLOR_MODE_CURRENT_HUE_SATURATION;

	switch(cmd->moveMode){
		case COLOR_CTRL_MOVE_STOP:
			break;
		case COLOR_CTRL_MOVE_UP:
			break;
		case COLOR_CTRL_MOVE_DOWN:
			break;
		default:
			break;
	}


}

/*********************************************************************
 * @fn      sampleLight_enhancedStepHueProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_enhancedStepHueProcess(zcl_colorCtrlEnhancedStepHueCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_CURRENT_HUE_SATURATION);

	pColor->colorMode = ZCL_COLOR_MODE_CURRENT_HUE_SATURATION;
	pColor->enhancedColorMode = ZCL_ENHANCED_COLOR_MODE_CURRENT_HUE_SATURATION;

	switch(cmd->stepMode){
		case COLOR_CTRL_STEP_MODE_UP:
			break;
		case COLOR_CTRL_STEP_MODE_DOWN:
			break;
		default:
			break;
	}


}

/*********************************************************************
 * @fn      sampleLight_enhancedMoveToHueAndSaturationProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_enhancedMoveToHueAndSaturationProcess(zcl_colorCtrlEnhancedMoveToHueAndSaturationCmd_t *cmd)
{
	zcl_colorCtrlEnhancedMoveToHueCmd_t enhancedMoveToHueCmd;
	zcl_colorCtrlMoveToSaturationCmd_t moveToSaturationCmd;

	enhancedMoveToHueCmd.enhancedHue = cmd->enhancedHue;
	enhancedMoveToHueCmd.direction = COLOR_CTRL_DIRECTION_SHORTEST_DISTANCE;
	enhancedMoveToHueCmd.transitionTime = cmd->transitionTime;

	moveToSaturationCmd.saturation = cmd->saturation;
	moveToSaturationCmd.transitionTime = cmd->transitionTime;

	sampleLight_enhancedMoveToHueProcess(&enhancedMoveToHueCmd);
	sampleLight_moveToSaturationProcess(&moveToSaturationCmd);
}

/*********************************************************************
 * @fn      sampleLight_colorLoopSetProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_colorLoopSetProcess(zcl_colorCtrlColorLoopSetCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	if(cmd->updateFlags.bits.direction){
		pColor->colorLoopDirection = cmd->direction;
	}

	if(cmd->updateFlags.bits.time){
		pColor->colorLoopTime = cmd->time;
	}

	if(cmd->updateFlags.bits.startHue){
		pColor->colorLoopStartEnhancedHue = cmd->startHue;
	}

	if(cmd->updateFlags.bits.action){
		switch(cmd->action){
			case COLOR_LOOP_SET_DEACTION:
				break;
			case COLOR_LOOP_SET_ACTION_FROM_COLOR_LOOP_START_ENHANCED_HUE:
				break;
			case COLOR_LOOP_SET_ACTION_FROM_ENHANCED_CURRENT_HUE:
				break;
			default:
				break;
		}
	}
}

#elif COLOR_CCT_SUPPORT

/*********************************************************************
 * @fn      sampleLight_moveToColorTemperatureProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveToColorTemperatureProcess(zcl_colorCtrlMoveToColorTemperatureCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS);

	pColor->colorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;

	colorInfo.colorTempMinMireds = pColor->colorTempPhysicalMinMireds;
	colorInfo.colorTempMaxMireds = pColor->colorTempPhysicalMaxMireds;

	colorInfo.currentColorTemp256 = (u32)(pColor->colorTemperatureMireds) << 8;

	colorInfo.colorTempRemainingTime = (cmd->transitionTime == 0) ? 1 : cmd->transitionTime;

	colorInfo.stepColorTemp256 = ((s32)(cmd->colorTemperature - pColor->colorTemperatureMireds)) << 8;
	colorInfo.stepColorTemp256 /= (s32)colorInfo.colorTempRemainingTime;

	light_applyUpdate_16(&pColor->colorTemperatureMireds, &colorInfo.currentColorTemp256, &colorInfo.stepColorTemp256, &colorInfo.colorTempRemainingTime,
							colorInfo.colorTempMinMireds, colorInfo.colorTempMaxMireds, FALSE);

	if(colorInfo.colorTempRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_moveColorTemperatureProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_moveColorTemperatureProcess(zcl_colorCtrlMoveColorTemperatureCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS);

	pColor->colorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;

	if(cmd->colorTempMinMireds){
		colorInfo.colorTempMinMireds = (cmd->colorTempMinMireds < pColor->colorTempPhysicalMinMireds) ? pColor->colorTempPhysicalMinMireds
																									  : cmd->colorTempMinMireds;
	}else{
		colorInfo.colorTempMinMireds = pColor->colorTempPhysicalMinMireds;
	}

	if(cmd->colorTempMaxMireds){
		colorInfo.colorTempMaxMireds = (cmd->colorTempMaxMireds > pColor->colorTempPhysicalMaxMireds) ? pColor->colorTempPhysicalMaxMireds
																									  : cmd->colorTempMaxMireds;
	}else{
		colorInfo.colorTempMaxMireds = pColor->colorTempPhysicalMaxMireds;
	}

	colorInfo.currentColorTemp256 = (u32)(pColor->colorTemperatureMireds) << 8;

	switch(cmd->moveMode){
		case COLOR_CTRL_MOVE_STOP:
			colorInfo.stepColorTemp256 = 0;
			colorInfo.colorTempRemainingTime = 0;
			break;
		case COLOR_CTRL_MOVE_UP:
			colorInfo.stepColorTemp256 = (((s32)cmd->rate) << 8) / 10;
			colorInfo.colorTempRemainingTime = 0xFFFF;
			break;
		case COLOR_CTRL_MOVE_DOWN:
			colorInfo.stepColorTemp256 = ((-(s32)cmd->rate) << 8) / 10;
			colorInfo.colorTempRemainingTime = 0xFFFF;
			break;
		default:
			break;
	}

	light_applyUpdate_16(&pColor->colorTemperatureMireds, &colorInfo.currentColorTemp256, &colorInfo.stepColorTemp256, &colorInfo.colorTempRemainingTime,
							colorInfo.colorTempMinMireds, colorInfo.colorTempMaxMireds, FALSE);

	if(colorInfo.colorTempRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_stepColorTemperatureProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_stepColorTemperatureProcess(zcl_colorCtrlStepColorTemperatureCmd_t *cmd)
{
	zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

	sampleLight_updateColorMode(ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS);

	pColor->colorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;
	pColor->enhancedColorMode = ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS;

	if(cmd->colorTempMinMireds){
		colorInfo.colorTempMinMireds = (cmd->colorTempMinMireds < pColor->colorTempPhysicalMinMireds) ? pColor->colorTempPhysicalMinMireds
																									  : cmd->colorTempMinMireds;
	}else{
		colorInfo.colorTempMinMireds = pColor->colorTempPhysicalMinMireds;
	}

	if(cmd->colorTempMaxMireds){
		colorInfo.colorTempMaxMireds = (cmd->colorTempMaxMireds > pColor->colorTempPhysicalMaxMireds) ? pColor->colorTempPhysicalMaxMireds
																									  : cmd->colorTempMaxMireds;
	}else{
		colorInfo.colorTempMaxMireds = pColor->colorTempPhysicalMaxMireds;
	}

	colorInfo.currentColorTemp256 = (u32)(pColor->colorTemperatureMireds) << 8;

	colorInfo.colorTempRemainingTime = (cmd->transitionTime == 0) ? 1 : cmd->transitionTime;

	colorInfo.stepColorTemp256 = (((s32)cmd->stepSize) << 8) / colorInfo.colorTempRemainingTime;

	switch(cmd->stepMode){
		case COLOR_CTRL_STEP_MODE_UP:
			break;
		case COLOR_CTRL_STEP_MODE_DOWN:
			colorInfo.stepColorTemp256 = -colorInfo.stepColorTemp256;
			break;
		default:
			break;
	}

	light_applyUpdate_16(&pColor->colorTemperatureMireds, &colorInfo.currentColorTemp256, &colorInfo.stepColorTemp256, &colorInfo.colorTempRemainingTime,
							colorInfo.colorTempMinMireds, colorInfo.colorTempMaxMireds, FALSE);

	if(colorInfo.colorTempRemainingTime){
		sampleLight_colorTimerStop();
		colorTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_colorTimerEvtCb, NULL, ZCL_COLOR_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_colorTimerStop();
	}
}

#endif

/*********************************************************************
 * @fn      sampleLight_stopMoveStepProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_stopMoveStepProcess(void)
{
	//zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

#if COLOR_RGB_SUPPORT
	colorInfo.hueRemainingTime = 0;
	colorInfo.saturationRemainingTime = 0;
#elif COLOR_CCT_SUPPORT
	colorInfo.colorTempRemainingTime = 0;
#endif

	sampleLight_colorTimerStop();
}

/*********************************************************************
 * @fn      sampleLight_colorCtrlCb
 *
 * @brief   Handler for ZCL COLOR CONTROL command. This function will set Color Control attribute first.
 *
 * @param   pAddrInfo
 * @param   cmdId - color cluster command id
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleLight_colorCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_LIGHT_ENDPOINT){
		switch(cmdId){
#if COLOR_RGB_SUPPORT
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE:
				sampleLight_moveToHueProcess((zcl_colorCtrlMoveToHueCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_HUE:
				sampleLight_moveHueProcess((zcl_colorCtrlMoveHueCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_HUE:
				sampleLight_stepHueProcess((zcl_colorCtrlStepHueCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_SATURATION:
				sampleLight_moveToSaturationProcess((zcl_colorCtrlMoveToSaturationCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_SATURATION:
				sampleLight_moveSaturationProcess((zcl_colorCtrlMoveSaturationCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_SATURATION:
				sampleLight_stepSaturationProcess((zcl_colorCtrlStepSaturationCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE_AND_SATURATION:
				sampleLight_moveToHueAndSaturationProcess((zcl_colorCtrlMoveToHueAndSaturationCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR:
				sampleLight_moveToColorProcess((zcl_colorCtrlMoveToColorCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR:
				sampleLight_moveColorProcess((zcl_colorCtrlMoveColorCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR:
				sampleLight_stepColorProcess((zcl_colorCtrlStepColorCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE:
				sampleLight_enhancedMoveToHueProcess((zcl_colorCtrlEnhancedMoveToHueCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_HUE:
				sampleLight_enhancedMoveHueProcess((zcl_colorCtrlEnhancedMoveHueCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_STEP_HUE:
				sampleLight_enhancedStepHueProcess((zcl_colorCtrlEnhancedStepHueCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_AND_SATURATION:
				sampleLight_enhancedMoveToHueAndSaturationProcess((zcl_colorCtrlEnhancedMoveToHueAndSaturationCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_COLOR_LOOP_SET:
				sampleLight_colorLoopSetProcess((zcl_colorCtrlColorLoopSetCmd_t *)cmdPayload);
				break;
#elif COLOR_CCT_SUPPORT
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE:
				sampleLight_moveToColorTemperatureProcess((zcl_colorCtrlMoveToColorTemperatureCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE:
				sampleLight_moveColorTemperatureProcess((zcl_colorCtrlMoveColorTemperatureCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR_TEMPERATURE:
				sampleLight_stepColorTemperatureProcess((zcl_colorCtrlStepColorTemperatureCmd_t *)cmdPayload);
				break;
#endif
			case ZCL_CMD_LIGHT_COLOR_CONTROL_STOP_MOVE_STEP:
				sampleLight_stopMoveStepProcess();
				break;
			default:
				break;
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif	/* ZCL_LIGHT_COLOR_CONTROL */

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
