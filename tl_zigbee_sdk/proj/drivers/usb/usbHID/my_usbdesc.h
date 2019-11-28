/********************************************************************************************************
 * @file     my_usbdesc.h
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
//#include "app_config.h"
#include "drivers.h"
#define MY_STRING_VENDOR			L"Telink Semi-Conductor"
#define MY_STRING_PRODUCT			L"MY USB Mouse"
#define MY_STRING_SERIAL			L"TLSR8258"

#define MY_ID_VENDOR				0x248a			//< define Vender ID
#define MY_ID_PRODUCT				0x8000          //< define Product ID
#define MY_ID_VERSION				0x0001			//< define Version Number

#define EDP_ID_MOUSE              	1				//< endpoint:1


#define USB_KEYBOARD_POLL_INTERVAL		10		// in ms	USB_KEYBOARD_POLL_INTERVAL < KEYBOARD_SCAN_INTERVAL to ensure PC no missing key
#define USB_MOUSE_POLL_INTERVAL			4		// in ms
#define USB_SOMATIC_POLL_INTERVAL     	8		// in ms


enum {
	MY_USB_STRING_LANGUAGE = 0,
	MY_USB_STRING_VENDOR,
	MY_USB_STRING_PRODUCT,
	MY_USB_STRING_SERIAL,
};

// interface id
enum {
	USB_INTF_KEYBOARD_ID = 0,
	USB_INTF_KEYS_ID,
	USB_INTF_VOICE_ID,
	USB_INTF_MAX_ID,
};

//typedef struct {
//	USB_Descriptor_Configuration_Header_t Config;
//	USB_Descriptor_Interface_t mouse_interface;
//	USB_HID_Descriptor_HID_Mouse_t mouse_descriptor;
//} MY_USB_Descriptor_Configuration_t;


typedef struct {
	USB_Descriptor_Configuration_Header_t Config;

	USB_Descriptor_Interface_t keyboard_interface;
	USB_HID_Descriptor_HID_Keyboard_t keyboard_descriptor;

	USB_Descriptor_Interface_t keys_interface;
	USB_HID_Descriptor_HID_Keyboard_t keys_descriptor;

	USB_Descriptor_Interface_t voice_interface;
	USB_HID_Descriptor_HID_Audio_t voice_descriptor;
//	USB_Descriptor_Interface_t voice_interface;
//	USB_HID_Descriptor_HID_Keyboard_t voice_descriptor;
} MY_USB_Descriptor_Configuration_t;



#define EDP_ID_KEYBOARD_IN      0x01
#define EDP_ID_KEYBOARD_OUT     0x05
#define EDP_ID_KEYS_IN      	0x02
#define EDP_ID_KEYS_OUT     	0x06
#define EDP_ID_VOICE_IN      	0x04


u8 *my_usbdesc_get_language(void);
u8 *my_usbdesc_get_vendor(void);
u8 *my_usbdesc_get_product(void);
u8 *my_usbdesc_get_serial(void);
u8 *my_usbdesc_get_device(void);
u8 *my_usbdesc_get_configuration(void);


u8* my_usbkeyboard_get_report_desc(void);
u16 my_usbkeyboard_get_report_desc_size(void);
u8* my_usbdesc_get_keyboard(void);

u8* my_usbkeys_get_report_desc(void);
u16 my_usbkeys_get_report_desc_size(void);
u8* my_usbdesc_get_keys(void);


u8* my_usbvoice_get_report_desc(void);
u16 my_usbvoice_get_report_desc_size(void);
u8* my_usbdesc_get_voice(void);

//u8* my_usbmouse_get_report_desc(void);
//u16 my_usbmouse_get_report_desc_size(void);
//u8* my_usbdesc_get_mouse(void);
