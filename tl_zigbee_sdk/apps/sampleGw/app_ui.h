/********************************************************************************************************
 * @file	app_ui.h
 *
 * @brief	This is the header file for app_ui
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
#ifndef _APP_UI_H_
#define _APP_UI_H_


/**********************************************************************
 * CONSTANT
 */
#define LED_ON						1
#define LED_OFF						0

#define LINK_KEY_TYPE_INVALID   	0xff

/**********************************************************************
 * TYPEDEFS
 */
enum{
	APP_STATE_NORMAL,
	APP_FACTORY_NEW_SET_CHECK
};


/**********************************************************************
 * FUNCTIONS
 */
void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime);
void light_blink_stop(void);

void light_init(void);
void light_on(void);
void light_off(void);

void led_on(u32 pin);
void led_off(u32 pin);

void app_key_handler(void);

/**
 * @brief      store pre-install code into NV
 *
 * @param[in]   ieeeAdrr:  the ieee address of the device using unique link key join
 *
 * @param[in]   pInstallCode: the pointer of install code
 *
 * @return      None
 */
void zb_pre_install_code_store(addrExt_t ieeeAdrr, u8 *pInstallCode);

#endif	/* _APP_UI_H_ */
