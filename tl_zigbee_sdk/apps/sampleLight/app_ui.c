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
#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void led_on(u32 pin){
	gpio_write(pin, LED_ON);
}

void led_off(u32 pin){
	gpio_write(pin, LED_OFF);
}

void led_init(void){
	led_off(LED_POWER);
	led_off(LED_PERMIT);
}

void localPermitJoinState(void){
	static bool assocPermit = 0;
	if(assocPermit != zb_getMacAssocPermit()){
		assocPermit = zb_getMacAssocPermit();
		if(assocPermit){
			led_on(LED_PERMIT);
		}else{
			led_off(LED_PERMIT);
		}
	}
}

void buttonKeepPressed(u8 btNum){
	if(btNum == VK_SW1){
		gLightCtx.state = APP_FACTORY_NEW_DOING;
		zb_factoryReset();
	}else if(btNum == VK_SW2){

	}
}

void buttonShortPressed(u8 btNum){
	if(btNum == VK_SW1){
		if(zb_isDeviceJoinedNwk()){
			gLightCtx.sta = !gLightCtx.sta;
			if(gLightCtx.sta){
				sampleLight_onoff(ZCL_ONOFF_STATUS_ON);
			}else{
				sampleLight_onoff(ZCL_ONOFF_STATUS_OFF);
			}
		}
	}else if(btNum == VK_SW2){
		/* toggle local permit Joining */
		static u8 duration = 0;
		duration = duration ? 0 : 0xff;
		zb_nlmePermitJoiningRequest(duration);
	}
}

void keyScan_keyPressedCB(kb_data_t *kbEvt){
//	u8 toNormal = 0;
	u8 keyCode = kbEvt->keycode[0];
//	static u8 lastKeyCode = 0xff;

	buttonShortPressed(keyCode);

	if(keyCode == VK_SW1){
		gLightCtx.keyPressedTime = clock_time();
		gLightCtx.state = APP_FACTORY_NEW_SET_CHECK;
	}
}


void keyScan_keyReleasedCB(u8 keyCode){
	gLightCtx.state = APP_STATE_NORMAL;
}

volatile u8 T_keyPressedNum = 0;
void app_key_handler(void){
	static u8 valid_keyCode = 0xff;

	if(gLightCtx.state == APP_FACTORY_NEW_SET_CHECK){
		if(clock_time_exceed(gLightCtx.keyPressedTime, 5*1000*1000)){
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

	gLightCtx.installCodeAvailable = TRUE;
	gLightCtx.useInstallCodeFlg = TRUE;
}


#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
