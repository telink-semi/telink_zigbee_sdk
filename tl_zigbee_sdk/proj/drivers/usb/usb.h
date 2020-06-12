/********************************************************************************************************
 * @file     usb.h
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

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

enum {
	USB_TIME_BEFORE_ALLOW_SUSPEND = (3000*1000),//  3000 ms
};

enum {
	USB_IRQ_SETUP_REQ = 0,
	USB_IRQ_DATA_REQ,
};

// telink usb report ctrl command. used mixed with USB_AUD_PLAY_PAUSE...
enum{
	USB_REPORT_NO_EVENT		= 0xf0,
	USB_REPORT_RELEASE 		= 0xff,
};

/**
 * @brief This function initializes the USB interface
 * @param   none
 * @return none
 */
void usb_init(void);

/**
 * @brief This function handles the USB related irq.
 * @param   none
 * @return none
 */
void usb_handle_irq(void);


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

