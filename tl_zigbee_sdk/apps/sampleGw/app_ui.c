/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void led_on(u32 pin)
{
	drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	drv_gpio_write(pin, LED_OFF);
}

void light_on(void)
{
	led_on(LED_POWER);
}

void light_off(void)
{
	led_off(LED_POWER);
}

void light_init(void)
{
	led_on(LED_POWER);
}

s32 zclLightTimerCb(void *arg)
{
	u32 interval = 0;

	if(g_appGwCtx.sta == g_appGwCtx.oriSta){
		g_appGwCtx.times--;
		if(g_appGwCtx.times <= 0){
			g_appGwCtx.timerLedEvt = NULL;
			return -1;
		}
	}

	g_appGwCtx.sta = !g_appGwCtx.sta;
	if(g_appGwCtx.sta){
		light_on();
		interval = g_appGwCtx.ledOnTime;
	}else{
		light_off();
		interval = g_appGwCtx.ledOffTime;
	}

	return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_appGwCtx.times = times;

	if(!g_appGwCtx.timerLedEvt){
		if(g_appGwCtx.oriSta){
			light_off();
			g_appGwCtx.sta = 0;
			interval = ledOffTime;
		}else{
			light_on();
			g_appGwCtx.sta = 1;
			interval = ledOnTime;
		}
		g_appGwCtx.ledOnTime = ledOnTime;
		g_appGwCtx.ledOffTime = ledOffTime;

		g_appGwCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
	}
}

void light_blink_stop(void)
{
	if(g_appGwCtx.timerLedEvt){
		TL_ZB_TIMER_CANCEL(&g_appGwCtx.timerLedEvt);

		g_appGwCtx.times = 0;
		if(g_appGwCtx.oriSta){
			light_on();
		}else{
			light_off();
		}
	}
}


void buttonKeepPressed(u8 btNum){
	if(btNum == VK_SW1){
		//zb_factoryReset();
	}else if(btNum == VK_SW2){

	}
}

ev_timer_event_t *brc_toggleEvt = NULL;
s32 brc_toggleCb(void *arg)
{
	static bool toggle = 0;

	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = SAMPLE_GW_ENDPOINT;
	dstEpInfo.dstAddr.shortAddr = 0xffff;
	dstEpInfo.profileId = HA_PROFILE_ID;
	dstEpInfo.txOptions = 0;
	dstEpInfo.radius = 0;

	//zcl_onOff_toggleCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
	toggle = ~toggle;
	if(toggle){
		zcl_onOff_onCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
	}else{
		zcl_onOff_offCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
	}

	return 0;
}

void brc_toggle()
{
	if(!brc_toggleEvt){
		brc_toggleEvt = TL_ZB_TIMER_SCHEDULE(brc_toggleCb, NULL, 1000);
	}else{
		TL_ZB_TIMER_CANCEL(&brc_toggleEvt);
	}
}

void buttonShortPressed(u8 btNum){
	if(btNum == VK_SW1){
		if(zb_isDeviceJoinedNwk()){
#if POLL_CTRL_SUPPORT
			sampleGW_zclFastPollStopCmdSend();
#else
#if 0
			epInfo_t dstEpInfo;
			TL_SETSTRUCTCONTENT(dstEpInfo, 0);

			dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			dstEpInfo.dstEp = SAMPLE_GW_ENDPOINT;
			dstEpInfo.dstAddr.shortAddr = 0xffff;
			dstEpInfo.profileId = HA_PROFILE_ID;
			dstEpInfo.txOptions = 0;
			dstEpInfo.radius = 0;

			zcl_onOff_toggleCmd(SAMPLE_GW_ENDPOINT, &dstEpInfo, FALSE);
#else
			brc_toggle();
#endif
#endif
		}
	}else if(btNum == VK_SW2){
		if(zb_isDeviceJoinedNwk()){
			/* toggle local permit Joining */
			static u8 duration = 0;
			duration = duration ? 0 : 0xff;
			zb_nlmePermitJoiningRequest(duration);
		}
	}
}

void keyScan_keyPressedCB(kb_data_t *kbEvt){
//	u8 toNormal = 0;
	u8 keyCode = kbEvt->keycode[0];
//	static u8 lastKeyCode = 0xff;

	buttonShortPressed(keyCode);

	if(keyCode == VK_SW1){
		g_appGwCtx.keyPressedTime = clock_time();
		g_appGwCtx.state = APP_FACTORY_NEW_SET_CHECK;
	}
}


void keyScan_keyReleasedCB(u8 keyCode){
	g_appGwCtx.state = APP_STATE_NORMAL;
}

volatile u8 T_keyPressedNum = 0;
void app_key_handler(void){
	static u8 valid_keyCode = 0xff;

	if(g_appGwCtx.state == APP_FACTORY_NEW_SET_CHECK){
		if(clock_time_exceed(g_appGwCtx.keyPressedTime, 5*1000*1000)){
			buttonKeepPressed(VK_SW1);
		}
	}

	if(kb_scan_key(0 , 1)){
		T_keyPressedNum++;
		if(kb_event.cnt){
			keyScan_keyPressedCB(&kb_event);
			if(kb_event.cnt == 1){
				valid_keyCode = kb_event.keycode[0];
			}
		}else{
			keyScan_keyReleasedCB(valid_keyCode);
			valid_keyCode = 0xff;
		}
	}
}

#endif  /* __PROJECT_TL_GW__ */
