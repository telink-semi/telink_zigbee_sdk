/********************************************************************************************************
 * @file     usb_cdc.c
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
#include "../../../tl_common.h"
#include "drivers.h"

extern ev_time_event_t *tl_zbTimerTaskPost(ev_timer_callback_t func, void *arg, u32 t_us);

static void USBCDC_Write32(unsigned int value);
static void USBCDC_Read32(unsigned int *value);
static int USBCDC_RecvTimeoutCb(void *arg);


typedef struct {
    unsigned char *rxBuf;
    unsigned char *txBuf;

    /* Following variables are used in the RX more than CDC_TXRX_EPSIZE */
    ev_time_event_t *timer;

    cdc_handlerFn_t rxCb;
    cdc_handlerFn_t txCb;

    unsigned short lenToSend;
    unsigned short lastSendIndex;
    unsigned char lastIndex;
} USBCDC_Ctrl_t;

USBCDC_Ctrl_t cdc_vs;
USBCDC_Ctrl_t *cdc_v;

USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
    {
        0, //ControlInterfaceNumber

        CDC_TX_EPNUM, // DataINEndpointNumber
        CDC_TXRX_EPSIZE, // DataINEndpointSize
        0, // DataINEndpointDoubleBank

        CDC_RX_EPNUM,  // DataOUTEndpointNumber
        CDC_TXRX_EPSIZE, // DataOUTEndpointSize
        0, // DataOUTEndpointDoubleBank

        CDC_NOTIFICATION_EPNUM, // NotificationEndpointNumber
        CDC_NOTIFICATION_EPSIZE, // NotificationEndpointSize
        0,  // NotificationEndpointDoubleBank
    },
};
USB_ClassInfo_CDC_Device_t *CDCInterfaceInfo = &VirtualSerial_CDC_Interface;

static void USBCDC_Write32(unsigned int value)
{
    USBHW_CtrlEpDataWrite(value & 0xff);
    USBHW_CtrlEpDataWrite((value >> 8) & 0xff);
    USBHW_CtrlEpDataWrite((value >> 16) & 0xff);
    USBHW_CtrlEpDataWrite((value >> 24) & 0xff);
}

static void USBCDC_Read32(unsigned int *value)
{
    unsigned int temp = 0;
    *value = USBHW_CtrlEpDataRead();

    temp =  USBHW_CtrlEpDataRead();
    *value = (temp << 8) | (*value);
    temp = 0;

    temp =  USBHW_CtrlEpDataRead();
    *value = (temp << 16) | (*value);
    temp = 0;

    temp =  USBHW_CtrlEpDataRead();
    *value = (temp << 24) | (*value);
}

/**
 * @brief This function handles the USB CDC class-specific requests from host
 * @param[in]   bRequest the request type
 * @param[in]   wValue word-sized field that varies according to request
 * @param[in]   wIndex word-sized field that varies according to request
 *              typically used to pass an index or offset
 * @param[in]   wLength number of bytes to transfer if there is a Data stage
 * @return none
 */
void USBCDC_ControlRequestProcesss(unsigned char bRequest, unsigned short wValue, unsigned short wIndex, unsigned short wLength)
{
    if (wIndex != CDCInterfaceInfo->Config.ControlInterfaceNumber)
        return;

    switch (bRequest) {
    case CDC_REQ_GetLineEncoding:
        USBCDC_Write32(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);
        USBHW_CtrlEpDataWrite(CDCInterfaceInfo->State.LineEncoding.CharFormat);
        USBHW_CtrlEpDataWrite(CDCInterfaceInfo->State.LineEncoding.ParityType);
        USBHW_CtrlEpDataWrite(CDCInterfaceInfo->State.LineEncoding.DataBits);
        break;

    case CDC_REQ_SetLineEncoding:
        USBCDC_Read32(&CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);
        CDCInterfaceInfo->State.LineEncoding.CharFormat  = USBHW_CtrlEpDataRead();
        CDCInterfaceInfo->State.LineEncoding.ParityType  = USBHW_CtrlEpDataRead();
        CDCInterfaceInfo->State.LineEncoding.DataBits    = USBHW_CtrlEpDataRead();
        break;

    case CDC_REQ_SetControlLineState:
        CDCInterfaceInfo->State.ControlLineStates.HostToDevice = wValue;
        break;

    case CDC_REQ_SendBreak:
        break;
    }
}

/**
 * @brief This function initializes the USB CDC device
 * @param   none
 * @return none
 */
void USBCDC_Init(void)
{
    /* Init UART parameters */
    CDCInterfaceInfo->State.LineEncoding.BaudRateBPS = 115200;
    CDCInterfaceInfo->State.LineEncoding.CharFormat = CDC_LINEENCODING_OneStopBit;
    CDCInterfaceInfo->State.LineEncoding.ParityType = CDC_PARITY_None;
    CDCInterfaceInfo->State.LineEncoding.DataBits = 8;

    cdc_v = &cdc_vs;
    cdc_v->lastIndex = 0;
}

/**
 * @brief This function sets the USB CDC rx buffer
 * @param[in]   buf pointer to the rx buffer
 * @return none
 */
void USBCDC_RxBufSet(unsigned char *buf)
{
    cdc_v->rxBuf = buf;
}

/**
 * @brief This function sets the USB CDC tx and rx callback function
 * @param[in]   rxFunc rx callback function
 * @param[in]   txCb tx callback function
 * @return none
 */
void USBCDC_CBSet(cdc_handlerFn_t rxFunc, cdc_handlerFn_t txCb)
{
    cdc_v->rxCb = rxFunc;
    cdc_v->txCb = txCb;
}

static int USBCDC_RecvTimeoutCb(void* arg)
{
    unsigned char* p;

    cdc_v->lastIndex = 0;

    /* Clear the buffer */
    p = cdc_v->rxBuf;
    cdc_v->rxBuf = NULL;

    /* Callback */
    if (cdc_v->rxCb) {
        cdc_v->rxCb(p);
    }
    cdc_v->timer = NULL;
    return -1;
}

void USBCDC_DataRecv(void)
{
    unsigned char i;
    unsigned char *p;
    unsigned char len;
    unsigned char fEnd = 0;

    /* No buffer */
    if (!cdc_v->rxBuf) {
        while(1);
    }

    if (cdc_v->timer) {
        ev_unon_timer((ev_time_event_t*)&cdc_v->timer);
    }

    len = REG_USB_EP_PTR(CDC_RX_EPNUM & 0x07);
    fEnd = (len == CDC_TXRX_EPSIZE) ? 0 : 1;
    USBHW_EpPtrReset(CDC_RX_EPNUM);

    for (i = 0; i < len; i++) {
        cdc_v->rxBuf[cdc_v->lastIndex++] = USBHW_EpDataRead(CDC_RX_EPNUM);
    }

    if (fEnd) {
        cdc_v->lastIndex = 0;

        /* Clear the buffer */
        p = cdc_v->rxBuf;
        cdc_v->rxBuf = NULL;

        /* Callback */
        if (cdc_v->rxCb) {
            cdc_v->rxCb(p);
        }
    } 
    else {
        cdc_v->timer = tl_zbTimerTaskPost(USBCDC_RecvTimeoutCb, NULL, 500);
    }
}

unsigned char usbWriteByte(u8 byte){
	if (USBHW_IsEpBusy(CDC_TX_EPNUM)) {
		/* Return to wait IRQ come again */
		return 0;
	}
	REG_USB_EP_PTR(CDC_TX_EPNUM) = 0;
	REG_USB_EP_DAT(CDC_TX_EPNUM) = byte;
	/* Write ACK */
	REG_USB_EP_CTRL(CDC_TX_EPNUM) = FLD_EP_DAT_ACK;        // ACK
	unsigned short t = 0;
	while(USBHW_IsEpBusy(CDC_TX_EPNUM)) {
		if (t++ > 10000) {
			REG_USB_EP_CTRL(CDC_TX_EPNUM) &= 0xfe; // clear bit(0)
		}
	}
	return 1;
}


/**
 * @brief This function sends bulk data to host as the USB CDC device
 * @param[in]   none
 * @return the length of the data send out
 */
unsigned char USBCDC_BulkDataSend(void)
{
    unsigned short len;

    /* Wait until not busy */
    if (USBHW_IsEpBusy(CDC_TX_EPNUM)) {
        /* Return to wait IRQ come again */
        return 0;
    }

    /* Get the length to send in this bulk transaction */
    len = (cdc_v->lenToSend > CDC_TXRX_EPSIZE) ? CDC_TXRX_EPSIZE : cdc_v->lenToSend;
    cdc_v->lenToSend -= len;

    if (len == 0) {
        return 0;
    }

    REG_USB_EP_PTR(CDC_TX_EPNUM) = 0;

    /* Write data to USB fifo */
    int i = 0;
    for (i = 0; i < len; i++) {
        REG_USB_EP_DAT(CDC_TX_EPNUM) = cdc_v->txBuf[cdc_v->lastSendIndex++];
    }

    /* Write ACK */
    REG_USB_EP_CTRL(CDC_TX_EPNUM) = FLD_EP_DAT_ACK;        // ACK
    unsigned short t = 0;
    while(USBHW_IsEpBusy(CDC_TX_EPNUM)) {
        if (t++ > 10000) {
            REG_USB_EP_CTRL(CDC_TX_EPNUM) &= 0xfe; // clear bit(0)
        }
    }

    /* TX transaction finish */
    if (cdc_v->lenToSend == 0) {
        cdc_v->lenToSend = 0;
        cdc_v->lastSendIndex = 0;

        if (cdc_v->txCb) {
            //EV_SCHEDULE_TASK((ev_task_callback_t)cdc_v->txCb, cdc_v->txBuf);
            cdc_v->txCb(cdc_v->txBuf);
            //ev_on_timer(cdc_v->txCb,cdc_v->txBuf, 1);
        }
        cdc_v->txBuf = NULL;
    }

    return len;
}

/**
 * @brief This function sends data to host as the USB CDC device
 * @param[in]   buf pointer to the data need to send
 * @param[in]   len length in byte of the data need to send
 * @return none
 */
USBCDC_Sts_t USBCDC_DataSend(unsigned char *buf, unsigned char len)
{
    if (cdc_v->txBuf) {
        return USB_BUSY;
    }

    /* Init the bulk transfer */
    cdc_v->lenToSend = len;
    cdc_v->txBuf = buf;
    cdc_v->lastSendIndex = 0;

    /* Send first bulk */
    USBCDC_BulkDataSend();

    USBHW_DataEpAck(USB_EDP_CDC_OUT);

    return (USBCDC_Sts_t)SUCCESS;
}

unsigned char USBCDC_IsAvailable(void)
{
    return (cdc_v->txBuf == NULL);
}
unsigned char USBCDC_TxBusy(void)
{
    return USBHW_IsEpBusy(CDC_TX_EPNUM);
}
