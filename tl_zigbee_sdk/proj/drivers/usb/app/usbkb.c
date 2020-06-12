/********************************************************************************************************
 * @file     usbkb.c
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "tl_common.h"

#if (MODULE_KEYBOARD_ENABLE)
#include "../usbstd/usbstd.h"
#include "../usbdesc.h"
#include "../usbhw.h"
#include "usbkb.h"


u8 usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode){
	if(usbhw_is_ep_busy(USB_EDP_KEYBOARD_IN))
		return 0;

	reg_usb_ep_ptr(USB_EDP_KEYBOARD_IN) = 0;

	// please refer to keyboard_report_desc
	reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = ctrl_key;
	reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = 0;//resv
	foreach(i, KEYBOARD_REPORT_KEY_MAX){
	    reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = keycode[i];
	}
	reg_usb_ep_ctrl(USB_EDP_KEYBOARD_IN) = FLD_EP_DAT_ACK;		// ACK
	return 1;
}


#endif
