/********************************************************************************************************
 * @file     usb_desc.c
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
#include "../../../tl_common.h"
#include "drivers.h"

const USB_Descriptor_String_t language_desc = { 
    {sizeof(USB_Descriptor_Header_t) + 2, DTYPE_String},
    {LANGUAGE_ID_ENG}};

const USB_Descriptor_String_t vendor_desc = { 
    {sizeof(USB_Descriptor_Header_t) + sizeof(STRING_VENDOR) - 2, DTYPE_String}, // Header
    STRING_VENDOR};

const USB_Descriptor_String_t product_desc = { 
    {sizeof(USB_Descriptor_Header_t) + sizeof(STRING_PRODUCT) - 2, DTYPE_String}, // Header
    STRING_PRODUCT};

const USB_Descriptor_String_t serial_desc = { 
    {sizeof(USB_Descriptor_Header_t) + sizeof(STRING_SERIAL) - 2, DTYPE_String}, // Header
    STRING_SERIAL};

const USB_Descriptor_Device_t device_desc = { 
    { sizeof(USB_Descriptor_Device_t), DTYPE_Device }, // Header
    
    0x0110, // USBSpecification, USB 1.1

    0xEF, // Class
    0x02, // SubClass
    0x01, // Protocol

    8, // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
    0x248a, // VendorID
    0x8267, // Product ID
    0x0100, // .ReleaseNumber
    USB_STRING_VENDOR, // .ManufacturerStrIndex
    USB_STRING_PRODUCT, // .ProductStrIndex
    3, //.SerialNumStrIndex, iSerialNumber
    1 
};

const USB_Descriptor_Configuration_t configuration_desc = {
    { 
        {
            sizeof(USB_Descriptor_Configuration_Header_t),
            DTYPE_Configuration 
        }, // Length, type
        sizeof(USB_Descriptor_Configuration_t), // TotalLength: variable
        USB_INTF_MAX, // NumInterfaces
        1, // Configuration index
        NO_DESCRIPTOR, // Configuration String
        USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_REMOTEWAKEUP, // Attributes
        USB_CONFIG_POWER_MA(50) // MaxPower = 100mA
    },

    {
        // iad0
        {sizeof(USB_Descriptor_Interface_Association_t), DTYPE_InterfaceAssociation}, // Header
        USB_INTF_CDC_CCI, // FirstInterfaceIndex
        2, // TotalInterface
        CDC_CSCP_CDCClass, // Class
        CDC_CSCP_ACMSubclass, // Subclass
        CDC_CSCP_ATCommandProtocol, // protocol
        NO_DESCRIPTOR  // IADStrIndex
    },

    {
        // cdc_interface
        {sizeof(USB_Descriptor_Interface_t), DTYPE_Interface}, // Header
        USB_INTF_CDC_CCI, // InterfaceNumber
        0, // AlternateSetting
        1, // TotalEndpoints
        CDC_CSCP_CDCClass, // Class
        CDC_CSCP_ACMSubclass, // SubClass
        CDC_CSCP_ATCommandProtocol, // Protocol
        NO_DESCRIPTOR //InterfaceStrIndex
    },

    {
        // cdc_descriptor
        //CDC_Functional_Header
        {
            {sizeof(USB_CDC_Descriptor_FunctionalHeader_t), DTYPE_CSInterface}, // Header
            CDC_DSUBTYPE_CSInterface_Header, // Subtype
            0x0110 // CDCSpecification
        },

        // CDC_Functional_ACM =
        {
            {sizeof(USB_CDC_Descriptor_FunctionalACM_t), DTYPE_CSInterface}, // Header
            CDC_DSUBTYPE_CSInterface_ACM, // Subtype
            0x02 // Capabilities
        },

        // CDC_Functional_Union =
        {
            {sizeof(USB_CDC_Descriptor_FunctionalUnion_t), DTYPE_CSInterface}, // Header
            CDC_DSUBTYPE_CSInterface_Union, // Subtype
            USB_INTF_CDC_CCI, // MasterInterfaceNumber
            USB_INTF_CDC_DCI, // SlaveInterfaceNumber
        },

            
        // CDC_CallManagement =
        {
            {sizeof(USB_CDC_Descriptor_FunctionalUnion_t), DTYPE_CSInterface}, // Header
            CDC_DSUBTYPE_CSInterface_CallManagement, // Subtype
            0, // MasterInterfaceNumber
            USB_INTF_CDC_DCI, // SlaveInterfaceNumber
        },

        // CDC_NotificationEndpoint =
        {
            {sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint}, // Header
            (ENDPOINT_DIR_IN | CDC_NOTIFICATION_EPNUM), // EndpointAddress
            (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA), // Attributes
            CDC_NOTIFICATION_EPSIZE, // EndpointSize
            0x40 // PollingIntervalMS
        },

        // CDC_DCI_Interface =
        {
            {sizeof(USB_Descriptor_Interface_t), DTYPE_Interface}, // Header
            USB_INTF_CDC_DCI, // InterfaceNumber
            0, // AlternateSetting
            2, // TotalEndpoints
            CDC_CSCP_CDCDataClass, // Class
            CDC_CSCP_NoDataSubclass, // SubClass
            CDC_CSCP_NoDataProtocol, // Protocol
            NO_DESCRIPTOR // InterfaceStrIndex
        },

        // CDC_DataOutEndpoint =
        {
            {sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint}, // Header
            (ENDPOINT_DIR_OUT | CDC_RX_EPNUM), // EndpointAddress
            (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA), // Attributes
            CDC_TXRX_EPSIZE, // EndpointSize
            0x00 // PollingIntervalMS
        },

        // CDC_DataInEndpoint =
        {
            {sizeof(USB_Descriptor_Endpoint_t),  DTYPE_Endpoint}, // Header
            (ENDPOINT_DIR_IN | CDC_TX_EPNUM), // EndpointAddress
            (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA), // Attributes
            CDC_TXRX_EPSIZE, // EndpointSize
            0x00 // PollingIntervalMS
        },
    },
};



unsigned char *USBDESC_LanguageGet(void) 
{
    return (unsigned char*) (&language_desc);
}

unsigned char *USBDESC_VendorGet(void) 
{
    return (unsigned char*) (&vendor_desc);
}

unsigned char *USBDESC_ProductGet(void) 
{
    return (unsigned char*) (&product_desc);
}

unsigned char *USBDESC_SerialGet(void) 
{
	return (unsigned char*) (&serial_desc);
}

unsigned char *USBDESC_DeviceGet(void) 
{
	return (unsigned char*) (&device_desc);
}

unsigned char *USBDESC_ConfigurationGet(void) 
{
	return (unsigned char*) (&configuration_desc);
}

unsigned char *USBDESC_CdcGet(void) 
{
	return (unsigned char*) (&configuration_desc.cdc_descriptor);
}

unsigned char *USBDESC_CdcInfGet(void) 
{
	return (unsigned char*) (&configuration_desc.cdc_interface);
}


