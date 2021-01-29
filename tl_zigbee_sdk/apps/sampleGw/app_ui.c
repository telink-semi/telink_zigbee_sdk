/********************************************************************************************************
 * @file	app_ui.c
 *
 * @brief	This is the source file for app_ui
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
