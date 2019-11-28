/********************************************************************************************************
 * @file     my_usbdesc.c
 *
 * @brief    This is the source file for TLSR8258
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

#include "my_usbdesc.h"

// request parameters
/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
static const USB_Descriptor_String_t language_desc = { {
		sizeof(USB_Descriptor_Header_t) + 2, DTYPE_String
	},
	{ LANGUAGE_ID_ENG }
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const USB_Descriptor_String_t vendor_desc = { {
		sizeof(USB_Descriptor_Header_t)
		+ sizeof(MY_STRING_VENDOR) - 2, DTYPE_String
	}, // Header
	MY_STRING_VENDOR
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const USB_Descriptor_String_t product_desc = { {
		sizeof(USB_Descriptor_Header_t) + sizeof(MY_STRING_PRODUCT) - 2,
		DTYPE_String
	}, // Header
	MY_STRING_PRODUCT
};

/** Serial number string. This is a Unicode string containing the device's unique serial number, expressed as a
 *  series of uppercase hexadecimal digits.
 */
static const USB_Descriptor_String_t serial_desc = { {
		sizeof(USB_Descriptor_Header_t)
		+ sizeof(MY_STRING_SERIAL) - 2, DTYPE_String
	}, // Header
	MY_STRING_SERIAL
};

static const USB_Descriptor_HIDReport_Datatype_t my_mouse_report_desc[] = {

	0x05, 0x01, // USAGE_PAGE (Generic Desktop)

	0x09, 0x02, // USAGE (Mouse)

	0xa1, 0x01, // COLLECTION (Application)

	0x09, 0x01, // USAGE (Pointer)

	0xa1, 0x00, // COLLECTION (Physical)

	0x05, 0x09, // USAGE_PAGE (Button)

	0x19, 0x01, // USAGE_MINIMUM (Button 1)

	0x29, 0x03, // USAGE_MAXIMUM (Button 3)

	0x15, 0x00, // LOGICAL_MINIMUM (0)

	0x25, 0x01, // LOGICAL_MAXIMUM (1)

	0x95, 0x03, // REPORT_COUNT (3)

	0x75, 0x01, // REPORT_SIZE (1)

	0x81, 0x02, // INPUT (Data,Var,Abs)

	0x95, 0x01, // REPORT_COUNT (1)

	0x75, 0x05, // REPORT_SIZE (5)

	0x81, 0x03, // INPUT (Cnst,Var,Abs)

	0x05, 0x01, // USAGE_PAGE (Generic Desktop)

	0x09, 0x30, // USAGE (X)

	0x09, 0x31, // USAGE (Y)

	0x09, 0x38, // USAGE (Wheel)

	0x15, 0x81, // LOGICAL_MINIMUM (-127)

	0x25, 0x7f, // LOGICAL_MAXIMUM (127)

	0x75, 0x08, // REPORT_SIZE (8)

	0x95, 0x03, // REPORT_COUNT (3)

	0x81, 0x06, // INPUT (Data,Var,Rel)

	0xc0,       // END_COLLECTION
	0xc0        // END_COLLECTION
};



static const USB_Descriptor_HIDReport_Datatype_t my_keyboard_report_desc[] = {
		0x05, 0x01,  // Usage Pg (Generic Desktop)
		0x09, 0x06,  // Usage (Keyboard)
		0xA1, 0x01,  // Collection: (Application)
		0x85, 0x01,  // Report Id (1)
		0x05, 0x07,  //   Usage Pg (Key Codes)
		0x19, 0xE0,  //   Usage Min (224)
		0x29, 0xE7,  //   Usage Max (231)
		0x15, 0x00,  //   Log Min (0)
		0x25, 0x01,  //   Log Max (1)
		0x75, 0x01,  //   Report Size (1)
		0x95, 0x08,  //   Report Count (8)
		0x81, 0x02,  //   Input: (Data, Variable, Absolute)
		0x95, 0x01,  //   Report Count (1)
		0x75, 0x08,  //   Report Size (8)
		0x81, 0x03,  //   Input: (Constant, Variable, Absolute)
		0x95, 0x06,  //   Report Count (6)
		0x75, 0x08,  //   Report Size (8)
		0x15, 0x00,  //   Log Min (0)
		0x26, 0xFF, 0x00,  // LOGICAL_MAXIMUM (255)
		0x05, 0x07,  //   Usage Pg (Key Codes)
		0x19, 0x00,  //   Usage Min (0)
		0x29, 0xFF,  //   Usage Max (255)
		0x81, 0x00,  //   Input: (Data, Array)
		0xC0,        // End Collection
};


static const USB_Descriptor_HIDReport_Datatype_t my_keys_report_desc[] = {
		0x05, 0x0c,             // USAGE_PAGE (Consumer)
		0x09, 0x01,             // USAGE (Consumer Control)
		0xa1, 0x01,             // COLLECTION (Application)
		0x85, 0x03,             // REPORT_ID (2)
		0x95, 0x04,	// REPORT_COUNT(4)
		0x75, 0x10,	// REPORT SIZE (16)0x15, 0x00,             // LOGICAL_MINIMUM (0)
		0x26, 0xFF, 0x03,       // LOGICAL_MAXIMUM (0x3FF)
		0x19, 0x00,	// USAGE_MINIMUM (0)
		0x2A, 0xFF, 0x03,	// USAGE_MAXIMUM (0x3FF)
		0x81, 0x00,             // INPUT (Data,Ary,Abs)
		0xC0,                    // END_COLLECTION (Application)

};

static const USB_Descriptor_HIDReport_Datatype_t my_voice_report_desc[] = {
		0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
		0x09, 0x01,        // Usage (0x01)
		0xA1, 0x01,        // Collection (Application)
		0xA1, 0x02,        //   Collection (Logical)
		0x85, 0x07,        //     Report ID (7)
		0x09, 0x14,        //     Usage (0x14)
		0x15, 0x80,        //     Logical Minimum (-128)
		0x25, 0x7F,        //     Logical Maximum (127)
		0x75, 0x08,        //     Report Size (8)
		0x95, 0x14,        //     Report Count (20)
		0x81, 0x22,        //     Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
		0xC0,              //   End Collection
		0xC0,              // End Collection
		0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
		0x09, 0x01,        // Usage (0x01)
		0xA1, 0x01,        // Collection (Application)
		0xA1, 0x02,        //   Collection (Logical)
		0x85, 0x08,        //     Report ID (8)
		0x09, 0x14,        //     Usage (0x14)
		0x15, 0x80,        //     Logical Minimum (-128)
		0x25, 0x7F,        //     Logical Maximum (127)
		0x75, 0x08,        //     Report Size (8)
		0x95, 0x14,        //     Report Count (20)
		0x91, 0x22,        //     Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
		0xC0,              //   End Collection
		0xC0,              // End Collection
		0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
		0x09, 0x04,        // Usage (0x04)
		0xA1, 0x01,        // Collection (Application)
		0xA1, 0x02,        //   Collection (Logical)
		0x85, 0x1F,        //     Report ID (31)
		0x09, 0x14,        //     Usage (0x14)
		0x15, 0x80,        //     Logical Minimum (-128)
		0x25, 0x7F,        //     Logical Maximum (127)
		0x75, 0x08,        //     Report Size (8)
		0x95, 0x14,        //     Report Count (20)
		0x81, 0x22,        //     Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
		0xC0,              //   End Collection
		0xC0,              // End Collection
		0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
		0x09, 0x03,        // Usage (0x03)
		0xA1, 0x01,        // Collection (Application)
		0xA1, 0x02,        //   Collection (Logical)
		0x85, 0x1E,        //     Report ID (30)
		0x09, 0x01,        //     Usage (0x01)
		0x75, 0x08,        //     Report Size (8)
		0x95, 0x08,        //     Report Count (8)
		0xB1, 0x02,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0xC0,              //   End Collection
		0xC0,              // End Collection

};

//static const
static USB_Descriptor_Device_t device_desc = { {
		sizeof(USB_Descriptor_Device_t), DTYPE_Device
	}, // Header
	0x0110, // USBSpecification, USB 1.1
	USB_CSCP_NoDeviceClass, // Class
	USB_CSCP_NoDeviceSubclass, // SubClass
	USB_CSCP_NoDeviceProtocol, // Protocol
	8, // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
	MY_ID_VENDOR,  // VendorID
	MY_ID_PRODUCT, // ProductID
	MY_ID_VERSION, // .ReleaseNumber
	MY_USB_STRING_VENDOR, 	// .ManufacturerStrIndex
	MY_USB_STRING_PRODUCT,  // .ProductStrIndex
	MY_USB_STRING_SERIAL,   // .SerialNumStrIndex
	1   //.bNumConfigurations字段。该设备所具有的配置数。
};



//static USB_Descriptor_Device_t device_desc = { {
//		sizeof(USB_Descriptor_Device_t), DTYPE_Device
//	}, 													// Header				:0x12,0x01
//	0x0110, 											// USBSpecification		:0x10,0x01  (USB 1.1)
//	USB_CSCP_NoDeviceClass, 							// Class				:0x00
//	USB_CSCP_NoDeviceSubclass, 							// SubClass 			:0x00
//	USB_CSCP_NoDeviceProtocol, 							// Protocol 			:0x00
//	8, 													// Endpoint0Size		:0x08 (Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64)
//	MY_ID_VENDOR,  										// VendorID				:0x248a
//	MY_ID_PRODUCT, 										// ProductID			:0x8000
//	MY_ID_VERSION, 										// Device Version		:0x0001
//	MY_USB_STRING_VENDOR, 								// ManufacturerStrIndex	:0x01
//	MY_USB_STRING_PRODUCT, 								// ProductStrIndex		:0x02
//	0, 													// SerialNumStrIndex	:0x00
//	1   												// bNumConfigurations	:0x01
//};

void my_load_custom_vid(u16 my_id)
{
	device_desc.ReleaseNumber = my_id;
}


//static const MY_USB_Descriptor_Configuration_t
//configuration_desc = {
//	/* 1.configuration Descriptor*/
//	{
//		{sizeof(USB_Descriptor_Configuration_Header_t), DTYPE_Configuration}, 		// bLength:0x09, bDescriptorType:0x02
//		sizeof(MY_USB_Descriptor_Configuration_t), 									// wTotalLength:	0x0022
//		USB_INTF_MAX_ID, 															// bNumInterfaces:	0x01
//		1, 																			// bConfiguration:	0x01
//		NO_DESCRIPTOR, 																// iConfigurationz:	0x01
//		USB_CONFIG_ATTR_RESERVED, 													// bmAttributes:	0x80
//		USB_CONFIG_POWER_MA(100) 													// bMaxPower:		0x32 -> 100mA
//	},
//	/* 2.Interface Descriptor(Mouse) */
//	{
//		{sizeof(USB_Descriptor_Interface_t), DTYPE_Interface},						// bLength:0x09, bDescriptorType:0x04
//	    USB_INTF_MOUSE_ID, 															// bInterfaceNumber :	0x00
//		0, 																			// bAlternateSetting:	0x00
//		1, 																			// bNumEndpoints	:	0x01
//		HID_CSCP_HIDClass,															// bInterfaceClass	:	0x03 (HID Class:0x03)
//		HID_CSCP_BootSubclass,														// bInterfaceSubClass:	0x01 (ONLY SUPPORT BIOS SubClass IN HID 1.1 PROTOCOL)
//		HID_CSCP_MouseBootProtocol,													// bInterfaceProtocol:	0x02(Mouse:0x02,keyboard:0x01 in BIOS SubClass)
//		NO_DESCRIPTOR																// iConfiguration:		0x00(None)
//	},
//	/* 3.HID Descriptor(Mouse) */
//	{
//		{
//			{sizeof(USB_HID_Descriptor_HID_t), HID_DTYPE_HID},						// bLength:0x09, bDescriptorType:0x21 (HID Descriptor Code:0x21)
//			0x0110, 																// bcdHID:0x10,0x01(HID 1.1 Protocol)
//			USB_HID_COUNTRY_US,														// bCountyCode:0x21
//			1, 																		// bNumDescriptors:0x01
//			HID_DTYPE_Report,														// bDescriptorType:0x22
//			{sizeof(my_mouse_report_desc), 0x00}, 									// bDescriptorLength:0x0034
//		},
//		/* 4.EndPoint Descriptor(Mouse)*/
//		{
//			{sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint},					// bLength:0x07, bDescriptorType:0x05
//			ENDPOINT_DIR_IN | EDP_ID_MOUSE,											// bEndpointAddress:	0x81
//			EP_TYPE_INTERRUPT,  													// bmAttributes:		0x03(Transmit Type:Interrupt Transmit)
//			0x0008,																	// wMaxPacketSize:		0x0008
//			USB_MOUSE_POLL_INTERVAL 											    // bInterval:			0x04
//		}
//	},
//};
static const MY_USB_Descriptor_Configuration_t
configuration_desc = {
	{   {sizeof(USB_Descriptor_Configuration_Header_t), DTYPE_Configuration}, // Length, type
		sizeof(MY_USB_Descriptor_Configuration_t), // TotalLength: variable
		USB_INTF_MAX_ID, // TotalInterfaces 该配置支持的接口数 （目前只有一个鼠标接口 total interfacesNum = 1）
		1, // ConfigurationNumber (通常一个USB设备可以支持多个配置，ConfigurationNumber就是每个配置的标识）
		NO_DESCRIPTOR, // Configuration String
		USB_CONFIG_ATTR_RESERVED, // Attributes
		USB_CONFIG_POWER_MA(100) // MaxPower = 100mA
	},

	// keyboard_interface
	{	{	sizeof(USB_Descriptor_Interface_t), DTYPE_Interface},
		USB_INTF_KEYBOARD_ID, // InterfaceNumber 该接口的编号（从0开始，目前只有一个键盘接口 indexNum = 0）
		0, // AlternateSetting
		2, // bNumEndpoints
		HID_CSCP_HIDClass, HID_CSCP_BootSubclass,
		HID_CSCP_KeyboardBootProtocol, NO_DESCRIPTOR
	},
	{
		// keyboard_hid（键盘HID描述符）
		{	{	sizeof(USB_HID_Descriptor_HID_t), HID_DTYPE_HID}, 0x0111, // HIDSpec
			USB_HID_COUNTRY_US, 1, // TotalReportDescriptors
			HID_DTYPE_Report, {sizeof(my_keyboard_report_desc)&0xFF, (sizeof(my_keyboard_report_desc)>>8)&0xFF}, // HIDReportLength
		},
		// keyboard_in_endpoint
		{	{	sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint},
			ENDPOINT_DIR_IN | EDP_ID_KEYBOARD_IN,
			EP_TYPE_INTERRUPT, 0x0008, // EndpointSize
			USB_KEYBOARD_POLL_INTERVAL // PollingInterval0MS
		},
		// keyboard_out_endpoint
		{	{	sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint},
			ENDPOINT_DIR_OUT | EDP_ID_KEYBOARD_OUT,
			EP_TYPE_INTERRUPT, 0x0008, // EndpointSize
			USB_KEYBOARD_POLL_INTERVAL // PollingInterval0MS
		}
	},

	// keyboard_interface
	{	{	sizeof(USB_Descriptor_Interface_t), DTYPE_Interface},
			USB_INTF_KEYS_ID, // InterfaceNumber 该接口的编号（从0开始，目前只有一个键盘接口 indexNum = 0）
		0, // AlternateSetting
		2, // bNumEndpoints
		HID_CSCP_HIDClass, HID_CSCP_BootSubclass,
		HID_CSCP_KeyboardBootProtocol, NO_DESCRIPTOR
	},
	{
		// keyboard_hid（键盘HID描述符）
		{	{	sizeof(USB_HID_Descriptor_HID_t), HID_DTYPE_HID}, 0x0111, // HIDSpec
			USB_HID_COUNTRY_US, 1, // TotalReportDescriptors
			HID_DTYPE_Report, {sizeof(my_keys_report_desc)&0xFF, (sizeof(my_keys_report_desc)>>8)&0xFF}, // HIDReportLength
		},
		// keyboard_in_endpoint
		{	{	sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint},
			ENDPOINT_DIR_IN | EDP_ID_KEYS_IN,
			EP_TYPE_INTERRUPT, 0x0008, // EndpointSize
			USB_KEYBOARD_POLL_INTERVAL // PollingInterval0MS
		},
		// keyboard_out_endpoint
		{	{	sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint},
			ENDPOINT_DIR_OUT | EDP_ID_KEYS_OUT,
			EP_TYPE_INTERRUPT, 0x0008, // EndpointSize
			USB_KEYBOARD_POLL_INTERVAL // PollingInterval0MS
		}
	},

	// voice_interface
	{	{	sizeof(USB_Descriptor_Interface_t), DTYPE_Interface},
			USB_INTF_VOICE_ID, // InterfaceNumber 该接口的编号（从0开始，目前只有一个键盘接口 indexNum = 0）
		0, // AlternateSetting
		1, // bNumEndpoints
		HID_CSCP_HIDClass, HID_CSCP_BootSubclass,
		HID_CSCP_KeyboardBootProtocol, NO_DESCRIPTOR
	},
	{
		// keyboard_hid（键盘HID描述符）
		{	{	sizeof(USB_HID_Descriptor_HID_t), HID_DTYPE_HID}, 0x0111, // HIDSpec
			USB_HID_COUNTRY_US, 1, // TotalReportDescriptors
			HID_DTYPE_Report, {sizeof(my_voice_report_desc)&0xFF, (sizeof(my_voice_report_desc)>>8)&0xFF}, // HIDReportLength
		},
		// voice_in_endpoint
		{	{	sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint},
			ENDPOINT_DIR_IN | EDP_ID_VOICE_IN,
			EP_TYPE_INTERRUPT, 0x0008, // EndpointSize
			USB_KEYBOARD_POLL_INTERVAL // PollingInterval0MS
		},

	},


};

u8 *my_usbdesc_get_language(void) {
	return (u8 *)(&language_desc);
}

u8 *my_usbdesc_get_vendor(void) {
	return (u8 *)(&vendor_desc);
}

u8 *my_usbdesc_get_product(void) {
	return (u8 *)(&product_desc);
}
u8 *my_usbdesc_get_serial(void) {
	return (u8 *)(&serial_desc);
}

u8 *my_usbdesc_get_device(void) {
	return (u8 *)(&device_desc);
}

u8 *my_usbdesc_get_configuration(void) {
	return (u8 *)(&configuration_desc);
}


//keyboard
u8* my_usbkeyboard_get_report_desc(void) {
	return (u8*) (my_keyboard_report_desc);
}

u16 my_usbkeyboard_get_report_desc_size(void) {
	return sizeof(my_keyboard_report_desc);
}

u8* my_usbdesc_get_keyboard(void) {
	return (u8*) (&configuration_desc.keyboard_descriptor);
}

//keys
u8* my_usbkeys_get_report_desc(void) {
	return (u8*) (my_keys_report_desc);
}

u16 my_usbkeys_get_report_desc_size(void) {
	return sizeof(my_keys_report_desc);
}

u8* my_usbdesc_get_keys(void) {
	return (u8*) (&configuration_desc.keys_descriptor);
}

//voice
u8* my_usbvoice_get_report_desc(void) {
	return (u8*) (my_voice_report_desc);
}

u16 my_usbvoice_get_report_desc_size(void) {
	return sizeof(my_voice_report_desc);
}

u8* my_usbdesc_get_voice(void) {
	return (u8*) (&configuration_desc.voice_descriptor);
}



