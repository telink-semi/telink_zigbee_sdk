/********************************************************************************************************
 * @file     app_ui.c
 *
 * @brief    application UI handler
 *
 * @author
 * @date     Dec. 1, 2016
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
#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
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
	gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	gpio_write(pin, LED_OFF);
}

void light_on(void)
{
	led_on(LED1);
}

void light_off(void)
{
	led_off(LED1);
}

void light_init(void)
{
	led_off(LED1);

}

s32 zclLightTimerCb(void *arg)
{
	u32 interval = 0;

	if(g_switchAppCtx.sta == g_switchAppCtx.oriSta){
		g_switchAppCtx.times--;
		if(g_switchAppCtx.times <= 0){
			g_switchAppCtx.timerLedEvt = NULL;
			return -1;
		}
	}

	g_switchAppCtx.sta = !g_switchAppCtx.sta;
	if(g_switchAppCtx.sta){
		light_on();
		interval = g_switchAppCtx.ledOnTime;
	}else{
		light_off();
		interval = g_switchAppCtx.ledOffTime;
	}

	return interval * 1000;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_switchAppCtx.times = times;

	if(!g_switchAppCtx.timerLedEvt){
		if(g_switchAppCtx.oriSta){
			light_off();
			g_switchAppCtx.sta = 0;
			interval = ledOffTime;
		}else{
			light_on();
			g_switchAppCtx.sta = 1;
			interval = ledOnTime;
		}
		g_switchAppCtx.ledOnTime = ledOnTime;
		g_switchAppCtx.ledOffTime = ledOffTime;

		g_switchAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval * 1000);
	}
}

void light_blink_stop(void)
{
	if(g_switchAppCtx.timerLedEvt){
		TL_ZB_TIMER_CANCEL(&g_switchAppCtx.timerLedEvt);

		g_switchAppCtx.times = 0;
		if(g_switchAppCtx.oriSta){
			light_on();
		}else{
			light_off();
		}
	}
}

/*******************************************************************
 * @brief	Button click detect:
 * 			SW1. keep press button1 5s === factory reset
 * 			SW1. short press button1   === send level step with OnOff command (Up)
 * 			SW2. keep press button2 5s === invoke EZ-Mode
 * 			SW2. short press button2   === send level step with OnOff command (Down)
 *
 */
void buttonKeepPressed(u8 btNum){
	if(btNum == VK_SW1){
		g_switchAppCtx.state = APP_FACTORY_NEW_DOING;
		zb_factoryReset();
	}else if(btNum == VK_SW2){

	}
}

void set_detect_voltage(s32 v){
    g_switchAppCtx.Vbat = v;
}

ev_time_event_t *brc_toggleEvt = NULL;

s32 brc_toggleCb(void *arg)
{
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
	dstEpInfo.dstAddr.shortAddr = 0xfffc;
	dstEpInfo.profileId = HA_PROFILE_ID;

	zcl_onOff_toggleCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE);

	return 0;
}

void brc_toggle(void)
{
	if(!brc_toggleEvt){
		brc_toggleEvt = TL_ZB_TIMER_SCHEDULE(brc_toggleCb, NULL, 1 * 1000 * 1000);
	}else{
		TL_ZB_TIMER_CANCEL(&brc_toggleEvt);
	}
}

volatile u8 T_appOnOffHandler = 0xff;
void buttonShortPressed(u8 btNum){
	if(btNum == VK_SW1){
		if(zb_isDeviceJoinedNwk()){
#if 1
			epInfo_t dstEpInfo;
			TL_SETSTRUCTCONTENT(dstEpInfo, 0);

			dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
			dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
			dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
			dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif
			zcl_onOff_toggleCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
#else
			brc_toggle();
#endif
		}
	}else if(btNum == VK_SW2){
		if(zb_isDeviceJoinedNwk()){
			static u8 lvl = 1;
			static bool dir = 1;

			epInfo_t dstEpInfo;
			TL_SETSTRUCTCONTENT(dstEpInfo, 0);

			dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
			dstEpInfo.dstAddr.shortAddr = 0xfffc;
			dstEpInfo.profileId = HA_PROFILE_ID;

			moveToLvl_t move2Level;

			move2Level.optPresent = 0;
			move2Level.transitionTime = 0x0A;
			move2Level.level = lvl;

			zcl_level_move2levelCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move2Level);

			if(dir){
				lvl += 50;
				if(lvl >= 250){
					dir = 0;
				}
			}else{
				lvl -= 50;
				if(lvl <= 1){
					dir = 1;
				}
			}
		}
	}
}


void keyScan_keyPressedCB(kb_data_t *kbEvt){
	//u8 toNormal = 0;
	u8 keyCode = kbEvt->keycode[0];
	//static u8 lastKeyCode = 0xff;

	buttonShortPressed(keyCode);

	if(keyCode == VK_SW1){
		g_switchAppCtx.keyPressedTime = clock_time();
		g_switchAppCtx.state = APP_FACTORY_NEW_SET_CHECK;
	}
}


void keyScan_keyReleasedCB(u8 keyCode){
	g_switchAppCtx.state = APP_STATE_NORMAL;
}

volatile u8 T_keyPressedNum = 0;
void app_key_handler(void){
	static u8 valid_keyCode = 0xff;
	if(g_switchAppCtx.state == APP_FACTORY_NEW_SET_CHECK){
		if(clock_time_exceed(g_switchAppCtx.keyPressedTime, 5*1000*1000)){
			buttonKeepPressed(VK_SW1);
		}
	}
	if(kb_scan_key(0 , 1)){
		T_keyPressedNum++;
		if(kb_event.cnt){
			g_switchAppCtx.keyPressed = 1;
			keyScan_keyPressedCB(&kb_event);
			if(kb_event.cnt == 1){
				valid_keyCode = kb_event.keycode[0];
			}
		}else{
			keyScan_keyReleasedCB(valid_keyCode);
			valid_keyCode = 0xff;
			g_switchAppCtx.keyPressed = 0;
		}
	}
}


void zb_pre_install_code_load(app_linkkey_info_t *appLinkKey){
	u8 invalidInstallCode[SEC_KEY_LEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
										   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	u8 installCode[SEC_KEY_LEN];

	flash_read(CFG_PRE_INSTALL_CODE, SEC_KEY_LEN, (u8 *)installCode);

	if(!memcmp((u8 *)installCode, (u8 *)invalidInstallCode, SEC_KEY_LEN)){
		return;
	}

	u8 key[SEC_KEY_LEN];
	tl_bdbUseInstallCode(installCode, key);

	appLinkKey->tcLinkKey.keyType = SS_UNIQUE_LINK_KEY;
	memcpy(appLinkKey->tcLinkKey.key, key, SEC_KEY_LEN);

	g_switchAppCtx.installCodeAvailable = TRUE;
	g_switchAppCtx.useInstallCodeFlg = TRUE;
}

#endif  /* __PROJECT_TL_SWITCH__ */
