/********************************************************************************************************
 * @file     usb_desc.h
 *
 * @brief    This file provides set of functions to manage the USB CDC descriptors
 *
 * @author   jian.zhang@telink-semi.com
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
 *           
 *******************************************************************************************************/
#pragma once

#include "usb_cdc.h"

// Endpoint Direction Masks
#define ENDPOINT_DIR_MASK    0x80
#define ENDPOINT_DIR_OUT     0x00
#define ENDPOINT_DIR_IN      0x80

// Endpoint/Pipe Type Masks 
#define EP_TYPE_MASK         0x03
#define EP_TYPE_CONTROL      0x00
#define EP_TYPE_ISOCHRONOUS  0x01
#define EP_TYPE_BULK         0x02
#define EP_TYPE_INTERRUPT    0x03


#define STRING_VENDOR        L"Telink"
#define STRING_PRODUCT       L"TelinkUSBDongle"
#define STRING_SERIAL        L"TLSR8269"

/** Endpoint number of the CDC device-to-host notification IN endpoint. */
#define CDC_NOTIFICATION_EPNUM         2
/** Endpoint number of the CDC device-to-host data IN endpoint. */
#define CDC_TX_EPNUM                   4 
/** Endpoint number of the CDC host-to-device data OUT endpoint. */
#define CDC_RX_EPNUM                   5
/** Size in bytes of the CDC device-to-host notification IN endpoint. */
#define CDC_NOTIFICATION_EPSIZE        8
/** Size in bytes of the CDC data IN and OUT endpoints. */
#define CDC_TXRX_EPSIZE                64

enum {
    USB_STRING_LANGUAGE = 0,
    USB_STRING_VENDOR,
    USB_STRING_PRODUCT,
    USB_STRING_SERIAL,
};

// interface id
typedef enum {
    USB_INTF_CDC_CCI,
    USB_INTF_CDC_DCI,
    USB_INTF_MAX,
} USB_INTF_ID_E;

typedef struct {
    // CDC Control Interface
    USB_CDC_Descriptor_FunctionalHeader_t    CDC_Functional_Header;
    USB_CDC_Descriptor_FunctionalACM_t       CDC_Functional_ACM;
    USB_CDC_Descriptor_FunctionalUnion_t     CDC_Functional_Union;
    USB_CDC_Descriptor_FunctionalUnion_t     CDC_Functional_CallManagement;
    USB_Descriptor_Endpoint_t                CDC_NotificationEndpoint;

    // CDC Data Interface
    USB_Descriptor_Interface_t               CDC_DCI_Interface;
    USB_Descriptor_Endpoint_t                CDC_DataOutEndpoint;
    USB_Descriptor_Endpoint_t                CDC_DataInEndpoint;
} USB_CDC_Descriptor_t;

typedef struct {
    USB_Descriptor_Configuration_Header_t Config;
    USB_Descriptor_Interface_Association_t cdc_iad;
    USB_Descriptor_Interface_t cdc_interface;
    USB_CDC_Descriptor_t cdc_descriptor;
} USB_Descriptor_Configuration_t;

unsigned char *USBDESC_LanguageGet(void);
unsigned char *USBDESC_VendorGet(void);
unsigned char *USBDESC_ProductGet(void);
unsigned char *USBDESC_SerialGet(void);
unsigned char *USBDESC_DeviceGet(void);
unsigned char *USBDESC_ConfigurationGet(void);
