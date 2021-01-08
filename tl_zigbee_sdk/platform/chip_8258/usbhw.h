/********************************************************************************************************
 * @file	usbhw.h
 *
 * @brief	This is the header file for B85
 *
 * @author	Driver & Zigbee Group
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
#pragma once

#include "bit.h"
#include "analog.h"
#include "usbhw_i.h"
#include "gpio.h"

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
	unsigned char dat = analog_read(0x0b);
	if(en){
		dat = dat | BIT(7);
	}else{
		dat = dat & 0x7f ;
	}

	analog_write(0x0b, dat);
}

/**
 * @brief      This function serves to power on or down USB module
 * @param[in]  En - 1: power on 0: power down
 * @return     none
 */
static inline void usb_power_on(unsigned char en)
{
	if(en){
		analog_write(0x34, analog_read(0x34) & 0xfd);
	}else{
		analog_write(0x34, analog_read(0x34) | 0x02);
	}
}

/**
 * @brief      This function serves to set GPIO MUX function as DP and DM pin of USB
 * @param[in]  none.
 * @return     none.
 */
static inline void usb_set_pin_en(void)
{
	//clear DP first
	gpio_set_func(GPIO_PA6, AS_GPIO);
	gpio_set_output_en(GPIO_PA6, 1);
	gpio_write(GPIO_PA6, 0);

	gpio_set_func(GPIO_PA5, AS_USB);
	gpio_set_func(GPIO_PA6, AS_USB);
	usb_dp_pullup_en(1);
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
void usbhw_write_ep(unsigned int ep, unsigned char * data, int len);

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

