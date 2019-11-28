/********************************************************************************************************
 * @file     my_usb.h
 *
 * @brief    This is the header file for TLSR8258
 *
 * @author	 junwei.lu@telink-semi.com;yafei.tu@telink-semi.com
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/

#pragma once

enum {
	MY_USB_IRQ_SETUP_REQ = 0,
	MY_USB_IRQ_DATA_REQ,
};

void my_usb_send_response(void);
void my_usb_prepare_desc_data(void);
void my_usb_handle_in_class_intf_req();
void my_usb_handle_request(unsigned char data_request);
void my_usb_handle_ctl_ep_setup();
void my_usb_handle_ctl_ep_data(void);
void my_usb_handle_ctl_ep_status();
void my_usb_handle_irq(void);
void my_usb_init(void);
void my_usb_send_response(void);

int MouseHidReport(unsigned char * p);



