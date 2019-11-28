/********************************************************************************************************
 * @file     app_ui.h
 *
 * @brief    application UI header
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
	APP_FACTORY_NEW_SET_CHECK,
	APP_FACTORY_NEW_DOING,
};


/**********************************************************************
 * FUNCTIONS
 */
void led_init(void);
void localPermitJoinState(void);
void app_key_handler(void);
void zb_pre_install_code_load(bdb_linkKey_info_t *bdbLinkKey, app_linkkey_info_t *appLinkKey);

#endif	/* _APP_UI_H_ */
