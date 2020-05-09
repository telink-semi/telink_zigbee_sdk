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
	gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	gpio_write(pin, LED_OFF);
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

	return interval * 1000;
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

		g_appGwCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval * 1000);
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

ev_time_event_t *brc_toggleEvt = NULL;
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
		brc_toggleEvt = TL_ZB_TIMER_SCHEDULE(brc_toggleCb, NULL, 1 * 1000 * 1000);
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
			duration =  duration ? 0 : 0xff;
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

void zb_pre_install_code_store(addrExt_t ieeeAdrr, u8 *pInstallCode){
	if(!pInstallCode ||
		ZB_IS_64BIT_ADDR_INVAILD(ieeeAdrr) ||
		ZB_IS_64BIT_ADDR_ZERO(ieeeAdrr)){
		return;
	}

	u8 key[SEC_KEY_LEN];
	tl_bdbUseInstallCode(pInstallCode, key);

	/* config unique link key for ZC */
	ss_dev_pair_set_t keyPair;
	memcpy(keyPair.device_address, ieeeAdrr, 8);
	memcpy(keyPair.linkKey, key, SEC_KEY_LEN);
	keyPair.incomingFrmaeCounter = keyPair.outgoingFrameCounter = 0;
	keyPair.apsLinkKeyType = SS_UNIQUE_LINK_KEY;
	keyPair.keyAttr = SS_UNVERIFIED_KEY;
	ss_devKeyPairSave(&keyPair);
}

#endif  /* __PROJECT_TL_GW__ */
