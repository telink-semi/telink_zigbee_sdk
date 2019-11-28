/********************************************************************************************************
 * @file     usb_cdc.h
 *
 * @brief    This file provides set of functions to manage the USB cdc device
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

#include "usb_stdDescriptors.h"
/**
 *  @brief  Define USB CDC subclass 
 */
enum CDC_Descriptor_ClassSubclassProtocol_t {
    CDC_CSCP_CDCClass               = 0x02,
    CDC_CSCP_NoSpecificSubclass     = 0x00,
    CDC_CSCP_ACMSubclass            = 0x02,
    CDC_CSCP_ATCommandProtocol      = 0x01,
    CDC_CSCP_NoSpecificProtocol     = 0x00,
    CDC_CSCP_VendorSpecificProtocol = 0xFF,
    CDC_CSCP_CDCDataClass           = 0x0A,
    CDC_CSCP_NoDataSubclass         = 0x00,
    CDC_CSCP_NoDataProtocol         = 0x00,
};

/**
 *  @brief  Define USB CDC class-specific request 
 */
enum {
    CDC_REQ_SendEncapsulatedCommand = 0x00,
    CDC_REQ_GetEncapsulatedResponse = 0x01,
    CDC_REQ_SetLineEncoding         = 0x20,
    CDC_REQ_GetLineEncoding         = 0x21,
    CDC_REQ_SetControlLineState     = 0x22,
    CDC_REQ_SendBreak               = 0x23,
};

enum {
    CDC_NOTIF_SerialState = 0x20,
};

/**
 *  @brief  Define USB CDC class-specific interface type 
 */
enum {
    CDC_DSUBTYPE_CSInterface_Header           = 0x00,
    CDC_DSUBTYPE_CSInterface_CallManagement   = 0x01,
    CDC_DSUBTYPE_CSInterface_ACM              = 0x02,
    CDC_DSUBTYPE_CSInterface_DirectLine       = 0x03,
    CDC_DSUBTYPE_CSInterface_TelephoneRinger  = 0x04,
    CDC_DSUBTYPE_CSInterface_TelephoneCall    = 0x05,
    CDC_DSUBTYPE_CSInterface_Union            = 0x06,
    CDC_DSUBTYPE_CSInterface_CountrySelection = 0x07,
    CDC_DSUBTYPE_CSInterface_TelephoneOpModes = 0x08,
    CDC_DSUBTYPE_CSInterface_USBTerminal      = 0x09,
    CDC_DSUBTYPE_CSInterface_NetworkChannel   = 0x0A,
    CDC_DSUBTYPE_CSInterface_ProtocolUnit     = 0x0B,
    CDC_DSUBTYPE_CSInterface_ExtensionUnit    = 0x0C,
    CDC_DSUBTYPE_CSInterface_MultiChannel     = 0x0D,
    CDC_DSUBTYPE_CSInterface_CAPI             = 0x0E,
    CDC_DSUBTYPE_CSInterface_Ethernet         = 0x0F,
    CDC_DSUBTYPE_CSInterface_ATM              = 0x10,
};

enum{
    CDC_LINEENCODING_OneStopBit          = 0,
    CDC_LINEENCODING_OneAndAHalfStopBits = 1,
    CDC_LINEENCODING_TwoStopBits         = 2,
};

enum{
    CDC_PARITY_None  = 0,
    CDC_PARITY_Odd   = 1,
    CDC_PARITY_Even  = 2,
    CDC_PARITY_Mark  = 3,
    CDC_PARITY_Space = 4,
};

/**
 *  @brief  Define USB CDC class-specific descriptor function header
 */
typedef struct{
    USB_Descriptor_Header_t    Header;
    unsigned char              Subtype;
    unsigned short             CDCSpecification;
} USB_CDC_Descriptor_FunctionalHeader_t;

typedef struct
{
    unsigned char    bFunctionLength;
    unsigned char    bDescriptorType;
    unsigned char    bDescriptorSubType;
    unsigned short   bcdCDC;
} USB_CDC_StdDescriptor_FunctionalHeader_t;

/**
 *  @brief  Define USB CDC class-specific descriptor of ACM subclass
 */
typedef struct{
    USB_Descriptor_Header_t    Header;
    unsigned char              Subtype;
    unsigned char              Capabilities;
} USB_CDC_Descriptor_FunctionalACM_t;

typedef struct{
    unsigned char    bFunctionLength;
    unsigned char    bDescriptorType;
    unsigned char    bDescriptorSubType;
    unsigned char    bmCapabilities;
} USB_CDC_StdDescriptor_FunctionalACM_t;


typedef struct{
    USB_Descriptor_Header_t    Header;
    unsigned char              Subtype;
    unsigned char              MasterInterfaceNumber;
    unsigned char              SlaveInterfaceNumber;
} USB_CDC_Descriptor_FunctionalUnion_t;


typedef struct{
    unsigned char    bFunctionLength;
    unsigned char    bDescriptorType;
    unsigned char    bDescriptorSubType;
    unsigned char    bMasterInterface;
    unsigned char    bSlaveInterface0;
} USB_CDC_StdDescriptor_FunctionalUnion_t;


typedef struct{
    unsigned int    BaudRateBPS;
    unsigned char   CharFormat;
    unsigned char   ParityType;
    unsigned char   DataBits;
} CDC_LineEncoding_t;

typedef void (*cdc_handlerFn_t)(unsigned char *pData);

typedef struct {
    unsigned char len;
    unsigned char data[1];
} USBCDC_TxBuf_t;


typedef enum usbcdc_sts_e {
    USB_BUSY = 1,
    USB_MULTIBLOCK,
} USBCDC_Sts_t;

/**
 *  @brief  Define USB CDC device information
 */
typedef struct{
    struct{
        unsigned char       ControlInterfaceNumber;
        unsigned char       DataINEndpointNumber;
        unsigned short      DataINEndpointSize;
        unsigned char       DataINEndpointDoubleBank;

        unsigned char       DataOUTEndpointNumber;
        unsigned short      DataOUTEndpointSize;
        unsigned char       DataOUTEndpointDoubleBank;

        unsigned char       NotificationEndpointNumber;
        unsigned short      NotificationEndpointSize;
        unsigned char       NotificationEndpointDoubleBank;
    } Config;

    struct{
        struct{
            unsigned short      HostToDevice;
            unsigned short      DeviceToHost;
        } ControlLineStates;
        CDC_LineEncoding_t LineEncoding;
    } State;

} USB_ClassInfo_CDC_Device_t;

/**
 * @brief This function handles the USB CDC class-specific requests from host
 * @param[in]   bRequest the request type
 * @param[in]   wValue word-sized field that varies according to request
 * @param[in]   wIndex word-sized field that varies according to request
 *              typically used to pass an index or offset
 * @param[in]   wLength number of bytes to transfer if there is a Data stage
 * @return none
 */
extern void USBCDC_ControlRequestProcess(unsigned char bRequest, unsigned short wValue, unsigned short wIndex, unsigned short wLength);

/**
 * @brief This function sends data to host as the USB CDC device
 * @param[in]   buf pointer to the data need to send
 * @param[in]   len length in byte of the data need to send
 * @return none
 */
USBCDC_Sts_t USBCDC_DataSend(unsigned char* buf, unsigned char len);

/**
 * @brief This function sends bulk data to host as the USB CDC device
 * @param[in]   none
 * @return the length of the data send out
 */
unsigned char USBCDC_BulkDataSend(void);

/**
 * @brief This function determines whether the USB interface is available
 * @param   none
 * @return 1: the USB interface is available 0: the USB interface is busy
 */
extern unsigned char USBCDC_IsAvailable(void);

/**
 * @brief This function handles the received data
 * @param   none
 * @return none
 */
void USBCDC_DataRecv(void);

/**
 * @brief This function initializes the USB CDC device
 * @param   none
 * @return none
 */
void USBCDC_Init(void);

/**
 * @brief This function sets the USB CDC tx and rx callback function
 * @param[in]   rxFunc rx callback function
 * @param[in]   txCb tx callback function
 * @return none
 */
void USBCDC_CBSet(cdc_handlerFn_t rxFunc, cdc_handlerFn_t txCb);

/**
 * @brief This function sets the USB CDC rx buffer
 * @param[in]   buf pointer to the rx buffer
 * @return none
 */
void USBCDC_RxBufSet(unsigned char *buf);

/**
 * @brief This function used to send one byte to host
 * @param   none
 * @return 1: send out successfully 0: the USB interface is busy
 */
extern unsigned char usbWriteByte(u8 byte);

/**
 * @brief This function used to check whether the Tx of the control endpoint is busy
 * @param   none
 * @return 1: the control endpoint is busy 0: the control endpoint is idle
 */
extern unsigned char USBCDC_TxBusy(void);
