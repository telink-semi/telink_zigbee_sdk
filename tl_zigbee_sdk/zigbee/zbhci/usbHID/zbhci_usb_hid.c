/********************************************************************************************************
 * @file     zbhci_usb_hid.c
 *
 * @brief    HCI  API for USB HID
 *
 * @author
 * @date     Jan. 16, 2019
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "../../include/zb_common.h"
#include "../../../proj/drivers/usb/usbHID/my_usb.h"
#include "../../../proj/drivers/usb/usbHID/my_usbdesc.h"
#include "../../../proj/drivers/usb/usbHID/usbhw_i.h"
#include "../zbhci.h"

#if (ZBHCI_EN && ZBHCI_USB_HID)

int KeyBoardHidReport(u8 *data){
	//if(reg_usb_ep_ctrl(EDP_ID_KEYBOARD_IN) & FLD_USB_EP_BUSY)
	if(usbhw_is_ep_busy(EDP_ID_KEYBOARD_IN))
		return 0;
	reg_usb_ep_ptr(EDP_ID_KEYBOARD_IN) = 0;
	for(int i = 0; i < 8; ++i){
    	reg_usb_ep_dat(EDP_ID_KEYBOARD_IN) = ((u8*)data)[i];
    }
	reg_usb_ep_ctrl(EDP_ID_KEYBOARD_IN) = FLD_EP_DAT_ACK;
	return 1;
}

int KeysHidReport(u8 *data){
	//if(reg_usb_ep_ctrl(EDP_ID_KEYBOARD_IN) & FLD_USB_EP_BUSY)
	if(usbhw_is_ep_busy(EDP_ID_KEYS_IN))
		return 0;
	reg_usb_ep_ptr(EDP_ID_KEYS_IN) = 0;
	for(int i = 0; i < 8; ++i){
    	reg_usb_ep_dat(EDP_ID_KEYS_IN) = ((u8*)data)[i];
    }
	reg_usb_ep_ctrl(EDP_ID_KEYS_IN) = FLD_EP_DAT_ACK;
	return 1;
}


int VoiceHidReport(u8 *data){
	//if(reg_usb_ep_ctrl(EDP_ID_KEYBOARD_IN) & FLD_USB_EP_BUSY)
	if(usbhw_is_ep_busy(EDP_ID_VOICE_IN))
		return 0;
	reg_usb_ep_ptr(EDP_ID_VOICE_IN) = 0;
	for(int i = 0; i < 8; ++i){
    	reg_usb_ep_dat(EDP_ID_VOICE_IN) = ((u8*)data)[i];
    }
	reg_usb_ep_ctrl(EDP_ID_VOICE_IN) = FLD_EP_DAT_ACK;
	return 1;
}


void usb_hid_init(void){
	printf("INIT! \r\n");
	usb_dp_pullup_en (1);
	my_usb_init();
	usbhw_data_ep_ack(EDP_ID_KEYBOARD_OUT);
	usbhw_data_ep_ack(EDP_ID_KEYS_OUT);
}

void usbHidTask(void ){
	my_usb_handle_irq();
}
#endif


