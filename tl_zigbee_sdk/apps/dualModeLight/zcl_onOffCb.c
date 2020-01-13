/********************************************************************************************************
 * @file     zcl_onOffCb.c
 *
 * @brief    call back function for cluster of "ONOFF"
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

#if (__PROJECT_DUAL_MODE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_ONOFF_TIMER_INTERVAL		100    //the timer interval to change the offWaitTime/onTime attribute of the ONOFF cluster

/**********************************************************************
 * LOCAL VARIABLES
 */
static ev_time_event_t *onWithTimedOffTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      sampleLight_onOffInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void sampleLight_onOffInit(void)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	sampleLight_onoff(pOnOff->onOff);
}

/*********************************************************************
 * @fn      sampleLight_updateOnOff
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void sampleLight_updateOnOff(void)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	hwLight_onOffUpdate(pOnOff->onOff);
}

/*********************************************************************
 * @fn      sampleLight_onoff
 *
 * @brief
 *
 * @param   ZCL_CMD_ONOFF_ON / ZCL_ONOFF_STATUS_OFF
 *
 * @return  None
 */
void sampleLight_onoff(u8 cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if(cmd == ZCL_CMD_ONOFF_ON){
		pOnOff->globalSceneControl = TRUE;

		pOnOff->onOff = ZCL_ONOFF_STATUS_ON;
		if(pOnOff->onTime == 0){
			pOnOff->offWaitTime = 0;
		}
	}else if(cmd == ZCL_CMD_ONOFF_OFF){
		pOnOff->onOff = ZCL_ONOFF_STATUS_OFF;
		pOnOff->onTime = 0;
	}else{
		if(pOnOff->onOff == ZCL_ONOFF_STATUS_OFF){
			pOnOff->globalSceneControl = TRUE;

			pOnOff->onOff = ZCL_ONOFF_STATUS_ON;
			if(pOnOff->onTime == 0){
				pOnOff->offWaitTime = 0;
			}
		}else{
			pOnOff->onOff = ZCL_ONOFF_STATUS_OFF;
			pOnOff->onTime = 0;
		}
	}

	light_fresh();

#ifdef ZCL_SCENE
	zcl_sceneAttr_t *pScene = zcl_sceneAttrGet();
	pScene->sceneValid = 0;
#endif
}

/*********************************************************************
 * @fn      sampleLight_OnWithTimedOffTimerCb
 *
 * @brief   timer event to process the ON_WITH_TIMED_OFF command
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 sampleLight_OnWithTimedOffTimerCb(void *arg)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_ON) && pOnOff->onTime){
		pOnOff->onTime--;
		if(pOnOff->onTime <= 0){
			pOnOff->offWaitTime = 0;
			sampleLight_onoff(ZCL_CMD_ONOFF_OFF);
		}
	}

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_OFF) && pOnOff->offWaitTime){
		pOnOff->offWaitTime--;
		if(pOnOff->offWaitTime <= 0){
			onWithTimedOffTimerEvt = NULL;
			return -1;
		}
	}

	if(pOnOff->onTime || pOnOff->offWaitTime){
		return 0;
	}else{
		onWithTimedOffTimerEvt = NULL;
		return -1;
	}
}

/*********************************************************************
 * @fn      sampleLight_OnWithTimedOffTimerStop
 *
 * @brief   force to stop the OnWithTimedOff timer
 *
 * @param
 *
 * @return
 */
static void sampleLight_OnWithTimedOffTimerStop(void)
{
	if(onWithTimedOffTimerEvt){
		TL_ZB_TIMER_CANCEL(&onWithTimedOffTimerEvt);
	}
}

/*********************************************************************
 * @fn      sampleLight_onoff_onWithTimedOffProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_onoff_onWithTimedOffProcess(zcl_onoff_onWithTimeOffCmd_t *cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if(cmd->onOffCtrl.bits.acceptOnlyWhenOn && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		return;
	}

	if(pOnOff->offWaitTime && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		pOnOff->offWaitTime = min2(pOnOff->offWaitTime, cmd->offWaitTime);
	}else{
		pOnOff->onTime = max2(pOnOff->onTime, cmd->onTime);
		pOnOff->offWaitTime = cmd->offWaitTime;
		sampleLight_onoff(ZCL_CMD_ONOFF_ON);
	}

	if((pOnOff->onTime < 0xFFFF) && (pOnOff->offWaitTime < 0xFFFF)){
		if(pOnOff->onTime || pOnOff->offWaitTime){
			sampleLight_OnWithTimedOffTimerStop();
			onWithTimedOffTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_OnWithTimedOffTimerCb, NULL, ZCL_ONOFF_TIMER_INTERVAL * 1000);
		}
	}
}

/*********************************************************************
 * @fn      sampleLight_onoff_offWithEffectProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_onoff_offWithEffectProcess(zcl_onoff_offWithEffectCmd_t *cmd)
{
	//TODO: FIXED ME

	sampleLight_onoff(ZCL_CMD_ONOFF_OFF);
}

/*********************************************************************
 * @fn      sampleLight_onoff_onWithRecallGlobalSceneProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleLight_onoff_onWithRecallGlobalSceneProcess(void)
{

}

/*********************************************************************
 * @fn      sampleLight_onOffCb
 *
 * @brief   Handler for ZCL ONOFF command. This function will set ONOFF attribute first.
 *
 * @param	pAddrInfo
 * @param   cmdId - onoff cluster command id
 * @param	cmdPayload
 *
 * @return  status_t
 */
status_t sampleLight_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

	if(pAddrInfo->dstEp == SAMPLE_LIGHT_ENDPOINT){
		switch(cmdId){
			case ZCL_CMD_ONOFF_ON:
			case ZCL_CMD_ONOFF_OFF:
			case ZCL_CMD_ONOFF_TOGGLE:
				sampleLight_onoff(cmdId);
				break;
			case ZCL_CMD_OFF_WITH_EFFECT:
				if(pOnOff->globalSceneControl == TRUE){
					/* TODO: store its settings in its global scene */

					pOnOff->globalSceneControl = FALSE;
				}
				sampleLight_onoff_offWithEffectProcess((zcl_onoff_offWithEffectCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE:
				if(pOnOff->globalSceneControl == FALSE){
					sampleLight_onoff_onWithRecallGlobalSceneProcess();
					pOnOff->globalSceneControl = TRUE;
				}
				break;
			case ZCL_CMD_ON_WITH_TIMED_OFF:
				sampleLight_onoff_onWithTimedOffProcess((zcl_onoff_onWithTimeOffCmd_t *)cmdPayload);
				break;
			default:
				break;
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif  /* __PROJECT_DUAL_MODE_LIGHT__ */
