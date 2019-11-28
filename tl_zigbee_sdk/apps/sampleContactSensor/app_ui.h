/********************************************************************************************************
 * @file     app_ui.h
 *
 * @brief    Application UI header
 *
 * @author
 * @date     Jan. 4, 2018
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
#ifndef _APP_UI_H_
#define _APP_UI_H_

/**********************************************************************
 * CONSTANT
 */
#define LED_ON						1
#define LED_OFF						0

#define KEY_PRESSED_CHECK_TIMES		6
/**********************************************************************
 * TYPEDEFS
 */
enum{
	APP_STATE_NORMAL,
	APP_FACTORY_NEW_SET_CHECK,
	APP_FACTORY_NEW_DOING
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

#endif	/* _APP_UI_H_ */
