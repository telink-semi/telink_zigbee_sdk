/********************************************************************************************************
 * @file     usbhw.h
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
#ifndef USBHW_H
#define USBHW_H

#include "bit.h"
#include "usbhw_i.h"


enum {
	USB_EDP_PRINTER_IN 		= 8, 	// endpoint 8 is alias of enpoint 0,  becareful.  // default hw buf len = 64
	USB_EDP_MOUSE 			= 2,	// default hw buf len = 8
	USB_EDP_KEYBOARD_IN 	= 1,	// default hw buf len = 8
	USB_EDP_KEYBOARD_OUT 	= 3,	// default hw buf len = 16
	USB_EDP_AUDIO_IN 		= 4,	// default hw buf len = 64
	USB_EDP_PRINTER_OUT 	= 5,	// default hw buf len = 64
	USB_EDP_SPEAKER 		= 6,	// default hw buf len = 16
	USB_EDP_MIC 			= 7,	// default hw buf len = 16
#if(USB_GAMEPAD_ENABLE)
	USB_EDP_GAMEPAD_IN 		= 3,	// default hw buf len = 64
	USB_EDP_GAMEPAD_OUT 	= 5,	// default hw buf len = 64
#endif
	USB_EDP_MS_IN 			= USB_EDP_PRINTER_IN,	// mass storage
	USB_EDP_MS_OUT 			= USB_EDP_PRINTER_OUT,
	USB_EDP_SOMATIC_IN 		= USB_EDP_AUDIO_IN,		//  when USB_SOMATIC_ENABLE, USB_EDP_PRINTER_OUT disable
	USB_EDP_SOMATIC_OUT 	= USB_EDP_PRINTER_OUT,
    USB_EDP_CDC_IN 			= 4,
    USB_EDP_CDC_OUT 		= 5,
};

// #defined in the standard spec
enum {
	USB_HID_AUDIO       	= 2,
	USB_HID_MOUSE       	= 1,
	USB_HID_KB_MEDIA    	= 3,// media
	USB_HID_KB_SYS      	= 4,// system : power,sleep,wakeup
#if(USB_GAMEPAD_ENABLE)
	USB_HID_GAMEPAD			= 5,// somatic sensor,  may have many report ids
#endif
	USB_HID_SOMATIC			= 5,// somatic sensor,  may have many report ids
};

/**
 * @brief      This function enables or disables the internal pull-up resistor
 *             of DP pin of USB interface
 * @param[in]  En - enables or disables the internal pull-up resistor(1: enable 0: disable)
 * @return     none
 */
static inline void usb_dp_pullup_en(int en)
{
	unsigned char dat = analog_read(0x00);
	if (en) {
		dat &= ~BIT(4);
	}else {
		dat |= BIT(4);
	}

	analog_write(0x00, dat);
}

/**
 * @brief      This function disables the manual interrupt
 *             (Endpont8 is the alias of endpoint0)
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usbhw_disable_manual_interrupt(int m);

/**
 * @brief      This function enable the manual interrupt
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usbhw_enable_manual_interrupt(int m);

/**
 * @brief      This function sends a bulk of data to host via the specified endpoint
 * @param[in]  ep - the number of the endpoint
 * @param[in]  data - pointer to the data need to send
 * @param[in]  len - length in byte of the data need to send
 * @return     none
 */
void usbhw_write_ep(unsigned int ep, unsigned char* data, int len);

/**
 * @brief      This function sends two bytes data to host via the control endpoint
 *             (handy help function)
 * @param[in]  v - the two bytes data need to send
 * @return     none
 */
void usbhw_write_ctrl_ep_u16(unsigned short v);

/**
 * @brief   This function reads two bytes data from host via the control endpoint
 * @param   none
 * @return  the two bytes data read from the control endpoint
 */
unsigned short usbhw_read_ctrl_ep_u16(void);

#endif	/* USBHW_H */
