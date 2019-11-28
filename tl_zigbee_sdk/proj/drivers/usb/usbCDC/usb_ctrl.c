/********************************************************************************************************
 * @file     usb_ctrl.c
 *
 * @brief    This file provides set of functions to manage the USB interface
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

static USB_Request_Header_t control_request;
static unsigned char * g_response = 0;
static unsigned short g_response_len = 0;
static int g_stall = 0;
unsigned char g_rate = 0; //default 0 for all report
extern void USBCDC_ControlRequestProcesss(unsigned char bRequest, unsigned short wValue, unsigned short wIndex, unsigned short wLength);

static void USB_ResponseSend(void)
{
    unsigned short n;
    if (g_response_len < 8) {
        n = g_response_len;
    } 
    else {
        n = 8;
    }
    g_response_len -= n;

    USBHW_CtrlEpPtrReset();
    while (n-- > 0) {
        USBHW_CtrlEpDataWrite(*g_response);
        ++g_response;
    }
}

static void USB_DescDataPrepare(void) 
{
    unsigned char value_l = (control_request.wValue) & 0xff;
    unsigned char value_h = (control_request.wValue >> 8) & 0xff;

    g_response = 0;
    g_response_len = 0;

    switch (value_h) {
    case DTYPE_Device:
        g_response = USBDESC_DeviceGet();
        g_response_len = sizeof(USB_Descriptor_Device_t);
        break;

    case DTYPE_Configuration:
        g_response = USBDESC_ConfigurationGet();
        g_response_len = sizeof(USB_Descriptor_Configuration_t);
        break;

    case DTYPE_String:
        if (USB_STRING_LANGUAGE == value_l) {
            g_response = USBDESC_LanguageGet();
            g_response_len = sizeof(LANGUAGE_ID_ENG);
        } 
        else if (USB_STRING_VENDOR == value_l) {
            g_response = USBDESC_VendorGet();
            g_response_len = sizeof(STRING_VENDOR);
        } 
        else if (USB_STRING_PRODUCT == value_l) {
            g_response = USBDESC_ProductGet();
            g_response_len = sizeof(STRING_PRODUCT);
        } 
        else if (USB_STRING_SERIAL == value_l) {
            g_response = USBDESC_SerialGet();
            g_response_len = sizeof(STRING_SERIAL);
        } 
        else {
            g_stall = 1;
        }
        break;

    default:
        g_stall = 1;
        break;
    }

    if (control_request.wLength < g_response_len) {
        g_response_len = control_request.wLength;
    }

    return;
}


static void USB_OutClassIntfReqHandle(int data_request) 
{
    unsigned char property = control_request.bRequest;
//    unsigned char value_l = (control_request.wValue) & 0xff;
//    unsigned char value_h = (control_request.wValue >> 8) & 0xff;

    switch (property) {
    case CDC_REQ_SetControlLineState:
    case CDC_REQ_SetLineEncoding:
        USBCDC_ControlRequestProcesss(control_request.bRequest, control_request.wValue, control_request.wIndex, control_request.wLength);
        break;
    default:
        g_stall = 1;
        break;
    }
}

static void USB_InClassIntfReqHandle() 
{
    unsigned char property = control_request.bRequest;

    switch (property) {
    case 0x00:
        USBHW_CtrlEpDataWrite(0x00);
        break;
    case CDC_REQ_GetLineEncoding:
        USBCDC_ControlRequestProcesss(control_request.bRequest, control_request.wValue, control_request.wIndex, control_request.wLength);
        break;
    default:
        g_stall = 1;
        break;
    }
}

static void USB_StdIntfReqHandle() 
{}

static void USB_OutClassEndpReqHandle(int data_request) 
{}

static void USB_InClassEndpReqHandle() 
{}

static void USB_SetIntfHandle()
{}

static void USB_RequestHandle(unsigned char data_request) 
{
    unsigned char bmRequestType = control_request.bmRequestType;
    unsigned char bRequest = control_request.bRequest;

    USBHW_CtrlEpPtrReset();
    switch (bmRequestType) {
    case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE):
        if (REQ_GetDescriptor == bRequest) {
            if (USB_IRQ_SETUP_REQ == data_request) {
                USB_DescDataPrepare();
            }
            USB_ResponseSend();
        }
        break;

    case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_INTERFACE):
        if (REQ_GetDescriptor == bRequest) {
            if (USB_IRQ_SETUP_REQ == data_request) {
                USB_StdIntfReqHandle();
            }
            USB_ResponseSend();
        }
        break;

    case (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE):
        USB_OutClassIntfReqHandle(data_request);
        break;

    case (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_ENDPOINT):
        USB_OutClassEndpReqHandle(data_request);
        break;

    case (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE):
        USB_InClassIntfReqHandle();
        break;

    case (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_ENDPOINT):
        USB_InClassEndpReqHandle();
        break;

    case (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_INTERFACE):
        if (REQ_SetInterface == bRequest) {
            USB_SetIntfHandle();
        }
        break;

    default:
        g_stall = 1;
        break;
    }

    return;
}

static void USB_CtlEpSetupHandle() 
{
    USBHW_CtrlEpPtrReset();
    control_request.bmRequestType = USBHW_CtrlEpDataRead();
    control_request.bRequest = USBHW_CtrlEpDataRead();
    control_request.wValue = USBHW_CtrlEpU16Read();
    control_request.wIndex = USBHW_CtrlEpU16Read();
    control_request.wLength = USBHW_CtrlEpU16Read();
    g_stall = 0;
    USB_RequestHandle(USB_IRQ_SETUP_REQ);
    if (g_stall) {
        USBHW_CtrlEpCtrlWrite(FLD_EP_DAT_STALL);
    }
    else {
        USBHW_CtrlEpCtrlWrite(FLD_EP_DAT_ACK);
    }
}

static void USB_CtlEpDataHandle(void) 
{
    USBHW_CtrlEpPtrReset();
    g_stall = 0;
    USB_RequestHandle(USB_IRQ_DATA_REQ);
    if (g_stall) {
        USBHW_CtrlEpCtrlWrite(FLD_EP_DAT_STALL);
    }
    else {
        USBHW_CtrlEpCtrlWrite(FLD_EP_DAT_ACK);
    }   
}

static void USB_CtlEpStatusHandle() 
{
    if (g_stall) {
        USBHW_CtrlEpCtrlWrite(FLD_EP_STA_STALL);
    }
    else {
        USBHW_CtrlEpCtrlWrite(FLD_EP_STA_ACK);
    }	
}

/**
 * @brief This function handles the USB related irq.
 * @param   none
 * @return none
 */
void USB_IrqHandle(void) 
{
    unsigned int irq = USBHW_CtrlEpIrqGet();
    if (irq & FLD_CTRL_EP_IRQ_SETUP) {
        USBHW_CtrlEpIrqClr(FLD_CTRL_EP_IRQ_SETUP);
        USB_CtlEpSetupHandle();
    }
    if (irq & FLD_CTRL_EP_IRQ_DATA) {
        USBHW_CtrlEpIrqClr(FLD_CTRL_EP_IRQ_DATA);
        USB_CtlEpDataHandle();
    }
    if (irq & FLD_CTRL_EP_IRQ_STA) {
        USBHW_CtrlEpIrqClr(FLD_CTRL_EP_IRQ_STA);
        USB_CtlEpStatusHandle();
    }
    if (reg_irq_src & FLD_IRQ_USB_RST_EN){ //USB reset
        reg_irq_src3 = BIT(1); //Clear USB reset flag
    }
    irq = REG_USB_IRQ; // data irq
    unsigned char l;
    g_stall = 0;
    if(irq & BIT((USB_EDP_CDC_OUT & 0x07))){
        REG_USB_IRQ = BIT((USB_EDP_CDC_OUT & 0x07)); // clear ime
        USBCDC_DataRecv();
        USBHW_DataEpAck(USB_EDP_CDC_OUT);
    }

    if(irq & BIT((USB_EDP_CDC_IN & 0x07))){
        REG_USB_IRQ = BIT((USB_EDP_CDC_IN & 0x07));        // clear ime
        l = USBCDC_BulkDataSend();
        if (l > 0) {
            USBHW_DataEpAck(USB_EDP_CDC_OUT);
        }
    }
}

#define BM_CLR2(x, mask)       	( (x) &= ~(mask) )
static void USB_InterruptInit() 
{
    USBHW_ManualInterruptEnable(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC);
    BM_CLR2(REG_USB_MASK, BIT(USB_EDP_CDC_IN & 0x07) | BIT(USB_EDP_CDC_OUT & 0x07));
    USBHW_DataEpAck(USB_EDP_CDC_OUT);
}

/**
 * @brief This function initializes the USB interface
 * @param   none
 * @return none
 */
void USB_Init()
{
    USBCDC_Init();
    USB_InterruptInit();
}
