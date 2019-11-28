/********************************************************************************************************
 * @file     zcl_levelCb.c
 *
 * @brief    call back function for cluster of "LEVEL"
 *
 * @author
 * @date     Dec. 7, 2016
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

#if (__PROJECT_TL_DIMMABLE_LIGHT__)
/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"

#ifdef ZCL_LEVEL_CTRL

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_LEVEL_CHANGE_INTERVAL		100

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	s32 stepLevel256;
	u16	currentLevel256;
	u8	withOnOff;
}zcl_levelInfo_t;

/**********************************************************************
 * LOCAL VARIABLES
 */
static zcl_levelInfo_t levelInfo = {
	.stepLevel256 		= 0,
	.currentLevel256	= 0,
	.withOnOff			= 0,
};

static ev_time_event_t *levelTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      sampleLight_levelInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void sampleLight_levelInit(void)
{
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	pLevel->remainingTime = 0;

	levelInfo.currentLevel256 = (u16)(pLevel->curLevel) << 8;

	light_applyUpdate(&pLevel->curLevel, &levelInfo.currentLevel256, &levelInfo.stepLevel256, &pLevel->remainingTime,
					ZCL_LEVEL_ATTR_MIN_LEVEL, ZCL_LEVEL_ATTR_MAX_LEVEL, FALSE);
}

/*********************************************************************
 * @fn      sampleLight_updateLevel
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void sampleLight_updateLevel(void)
{
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	hwLight_levelUpdate(pLevel->curLevel);
}

/*********************************************************************
 * @fn      sampleLight_levelTimerEvtCb
 *
 * @brief   timer event to process the level command
 *
 * @param	arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 sampleLight_levelTimerEvtCb(void * arg)
{
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	if(pLevel->remainingTime){
		light_applyUpdate(&pLevel->curLevel, &levelInfo.currentLevel256, &levelInfo.stepLevel256, &pLevel->remainingTime,
								ZCL_LEVEL_ATTR_MIN_LEVEL, ZCL_LEVEL_ATTR_MAX_LEVEL, FALSE);
	}

	if(levelInfo.withOnOff){
		if(pLevel->curLevel == ZCL_LEVEL_ATTR_MIN_LEVEL){
			sampleLight_onoff(ZCL_CMD_ONOFF_OFF);
		}
	}

	if(pLevel->remainingTime){
		return 0;
	}else{
		levelTimerEvt = NULL;
		return -1;
	}
}

/*********************************************************************
 * @fn      sampleLight_LevelTimerStop
 *
 * @brief   force to stop the level timer
 *
 * @param
 *
 * @return
 */
static void sampleLight_LevelTimerStop(void)
{
	if(levelTimerEvt){
		TL_ZB_TIMER_CANCEL(&levelTimerEvt);
	}
}

/*********************************************************************
 * @fn      sampleLight_moveToLevelProcess
 *
 * @brief
 *
 * @param	cmdId
 * @param	cmd
 *
 * @return	None
 */
static void sampleLight_moveToLevelProcess(u8 cmdId, moveToLvl_t *cmd)
{
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	pLevel->remainingTime = ((cmd->transitionTime == 0) || (cmd->transitionTime == 0xFFFF)) ? 1 : cmd->transitionTime;

	levelInfo.withOnOff = (cmdId == ZCL_CMD_LEVEL_MOVE_TO_LEVEL_WITH_ON_OFF) ? TRUE : FALSE;
	levelInfo.currentLevel256 = (u16)(pLevel->curLevel) << 8;
	levelInfo.stepLevel256 = ((s32)(cmd->level - pLevel->curLevel)) << 8;
	levelInfo.stepLevel256 /= (s32)pLevel->remainingTime;

	light_applyUpdate(&pLevel->curLevel, &levelInfo.currentLevel256, &levelInfo.stepLevel256, &pLevel->remainingTime,
							ZCL_LEVEL_ATTR_MIN_LEVEL, ZCL_LEVEL_ATTR_MAX_LEVEL, FALSE);

	if(levelInfo.withOnOff){
		if(levelInfo.stepLevel256 > 0){
			sampleLight_onoff(ZCL_CMD_ONOFF_ON);
		}else if(pLevel->curLevel == ZCL_LEVEL_ATTR_MIN_LEVEL){
			sampleLight_onoff(ZCL_CMD_ONOFF_OFF);
		}
	}

	if(pLevel->remainingTime){
		sampleLight_LevelTimerStop();
		levelTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_LevelTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_moveProcess
 *
 * @brief
 *
 * @param	cmdId
 * @param	cmd
 *
 * @return	None
 */
static void sampleLight_moveProcess(u8 cmdId, move_t *cmd)
{
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	levelInfo.withOnOff = (cmdId == ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF) ? TRUE : FALSE;
	levelInfo.currentLevel256 = (u16)(pLevel->curLevel) << 8;
	levelInfo.stepLevel256 = (((s32)cmd->rate) << 8) / 10;

	if(cmd->moveMode == LEVEL_MOVE_UP){
		if(levelInfo.withOnOff){
			sampleLight_onoff(ZCL_CMD_ONOFF_ON);
		}
	}else{
		levelInfo.stepLevel256 = -levelInfo.stepLevel256;
	}

	pLevel->remainingTime = 0xFFFF;

	light_applyUpdate(&pLevel->curLevel, &levelInfo.currentLevel256, &levelInfo.stepLevel256, &pLevel->remainingTime,
							ZCL_LEVEL_ATTR_MIN_LEVEL, ZCL_LEVEL_ATTR_MAX_LEVEL, FALSE);

	if(levelInfo.withOnOff){
		if(pLevel->curLevel == ZCL_LEVEL_ATTR_MIN_LEVEL){
			sampleLight_onoff(ZCL_CMD_ONOFF_OFF);
		}
	}

	if(pLevel->remainingTime){
		sampleLight_LevelTimerStop();
		levelTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_LevelTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_stepProcess
 *
 * @brief
 *
 * @param	cmdId
 * @param	cmd
 *
 * @return	None
 */
static void sampleLight_stepProcess(u8 cmdId, step_t *cmd)
{
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	pLevel->remainingTime = ((cmd->transitionTime == 0) || (cmd->transitionTime == 0xFFFF)) ? 1 : cmd->transitionTime;

	levelInfo.withOnOff = (cmdId == ZCL_CMD_LEVEL_STEP_WITH_ON_OFF) ? TRUE : FALSE;
	levelInfo.currentLevel256 = (u16)(pLevel->curLevel) << 8;
	levelInfo.stepLevel256 = (((s32)cmd->stepSize) << 8) / pLevel->remainingTime;

	if(cmd->stepMode == LEVEL_STEP_UP){
		if(levelInfo.withOnOff){
			sampleLight_onoff(ZCL_CMD_ONOFF_ON);
		}
	}else{
		levelInfo.stepLevel256 = -levelInfo.stepLevel256;
	}

	light_applyUpdate(&pLevel->curLevel, &levelInfo.currentLevel256, &levelInfo.stepLevel256, &pLevel->remainingTime,
							ZCL_LEVEL_ATTR_MIN_LEVEL, ZCL_LEVEL_ATTR_MAX_LEVEL, FALSE);

	if(levelInfo.withOnOff){
		if(pLevel->curLevel == ZCL_LEVEL_ATTR_MIN_LEVEL){
			sampleLight_onoff(ZCL_CMD_ONOFF_OFF);
		}
	}

	if(pLevel->remainingTime){
		sampleLight_LevelTimerStop();
		levelTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL * 1000);
	}else{
		sampleLight_LevelTimerStop();
	}
}

/*********************************************************************
 * @fn      sampleLight_stopProcess
 *
 * @brief
 *
 * @param	cmdId
 * @param	cmd
 *
 * @return	None
 */
static void sampleLight_stopProcess(u8 cmdId, stop_t *cmd)
{
	zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

	sampleLight_LevelTimerStop();
	pLevel->remainingTime = 0;

	levelInfo.currentLevel256 = ((s32)pLevel->curLevel) << 8;
}

/*********************************************************************
 * @fn      sampleLight_levelCb
 *
 * @brief   Handler for ZCL LEVEL command. This function will set LEVEL attribute first.
 *
 * @param	pAddrInfo
 * @param   cmd - level cluster command id
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleLight_levelCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_LIGHT_ENDPOINT){
		switch(cmdId){
			case ZCL_CMD_LEVEL_MOVE_TO_LEVEL:
			case ZCL_CMD_LEVEL_MOVE_TO_LEVEL_WITH_ON_OFF:
				sampleLight_moveToLevelProcess(cmdId, (moveToLvl_t *)cmdPayload);
				break;
			case ZCL_CMD_LEVEL_MOVE:
			case ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF:
				sampleLight_moveProcess(cmdId, (move_t *)cmdPayload);
				break;
			case ZCL_CMD_LEVEL_STEP:
			case ZCL_CMD_LEVEL_STEP_WITH_ON_OFF:
				sampleLight_stepProcess(cmdId, (step_t *)cmdPayload);
				break;
			case ZCL_CMD_LEVEL_STOP:
			case ZCL_CMD_LEVEL_STOP_WITH_ON_OFF:
				sampleLight_stopProcess(cmdId, (stop_t *)cmdPayload);
				break;
			default:
				break;
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif	/* ZCL_LEVEL_CTRL */

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
