/********************************************************************************************************
 * @file     usbvendor.c
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
#include "tl_common.h"

#if (USB_VENDOR_ENABLE)
#include "../usbstd/usbstd.h"
#include "../usbdesc.h"
#include "../usbhw.h"
#include "usbvendor.h"


hidvendor_ctrl_t hidvendor_vs;
hidvendor_ctrl_t *hidvendor_v = &hidvendor_vs;


/**
 * @brief This function initializes the USB vendor device
 * @param   none
 * @return none
 */
void hidvendor_init(void)
{
    hidvendor_v->lastIndex = 0;
}

u8 hidvendor_isAvailable(void)
{
	return (hidvendor_v->txBuf == NULL);
}

u8 hidvendor_isTxBusy(void)
{
    return usbhw_is_ep_busy(USB_EDP_HID_VENDOR_IN);
}

/**
 * @brief This function sets the USB vendor rx buffer
 * @param[in]   buf pointer to the rx buffer
 * @return none
 */
void hidvendor_setRxBuf(u8 *buf)
{
	hidvendor_v->rxBuf = buf;
}

/**
 * @brief This function sets the USB vendor tx and rx callback function
 * @param[in]   rxFunc rx callback function
 * @param[in]   txCb tx callback function
 * @return none
 */
void hidvendor_setCb(hidvendor_handlerFn_t rxFunc, hidvendor_handlerFn_t txCb)
{
	hidvendor_v->rxCb = rxFunc;
    hidvendor_v->txCb = txCb;
}

static s32 hidvendor_recvTimeoutCb(void *arg)
{
	u8 *p;

	hidvendor_v->lastIndex = 0;

	/* Clear the buffer */
	p = hidvendor_v->rxBuf;
	hidvendor_v->rxBuf = NULL;

	/* Callback */
	if(hidvendor_v->rxCb){
		hidvendor_v->rxCb(p);
	}

	hidvendor_v->timer = NULL;
	return -1;
}

static void hidvendor_recvData(void)
{
	u8 i;
	u8 *p;
	u8 len;
	u8 fEnd = 0;

	/* No buffer */
	if(!hidvendor_v->rxBuf){
		while(1);
	}

	if(hidvendor_v->timer){
		TL_ZB_TIMER_CANCEL(&hidvendor_v->timer);
	}

	len = reg_usb_ep_ptr(USB_EDP_HID_VENDOR_OUT & 0x07);
	fEnd = (len == HID_VENDOR_TXRX_EPSIZE) ? 0 : 1;
	usbhw_reset_ep_ptr(USB_EDP_HID_VENDOR_OUT);

	for(i = 0; i < len; i++){
		hidvendor_v->rxBuf[hidvendor_v->lastIndex++] = usbhw_read_ep_data(USB_EDP_HID_VENDOR_OUT);
	}

	if(fEnd){
		hidvendor_v->lastIndex = 0;

		/* Clear the buffer */
		p = hidvendor_v->rxBuf;
		hidvendor_v->rxBuf = NULL;

		/* Callback */
		if(hidvendor_v->rxCb){
			hidvendor_v->rxCb(p);
		}
	}else{
		hidvendor_v->timer = TL_ZB_TIMER_SCHEDULE(hidvendor_recvTimeoutCb, NULL, 500);
	}
}

/**
 * @brief This function sends bulk data to host as the USB vendor device
 * @param[in]   none
 * @return the length of the data send out
 */
static u8 hidvendor_sendData(void)
{
	u16 len;

	/* Wait until not busy */
	if(usbhw_is_ep_busy(USB_EDP_HID_VENDOR_IN)){
        /* Return to wait IRQ come again */
        return 0;
    }

    /* Get the length to send in this bulk transaction */
	len = (hidvendor_v->lenToSend > HID_VENDOR_TXRX_EPSIZE) ? HID_VENDOR_TXRX_EPSIZE : hidvendor_v->lenToSend;
	hidvendor_v->lenToSend -= len;

	if(len == 0){
		return 0;
	}

    reg_usb_ep_ptr(USB_EDP_HID_VENDOR_IN) = 0;

	/* Write data to USB fifo */
    foreach(i, len){
        reg_usb_ep_dat(USB_EDP_HID_VENDOR_IN) = hidvendor_v->txBuf->data[hidvendor_v->lastSendIndex++];
    }

	/* Write ACK */
    reg_usb_ep_ctrl(USB_EDP_HID_VENDOR_IN) = FLD_EP_DAT_ACK;        // ACK
    u16 t = 0;
    while(usbhw_is_ep_busy(USB_EDP_HID_VENDOR_IN)){
        if(t++ > 10000){
            reg_usb_ep_ctrl(USB_EDP_HID_VENDOR_IN) &= 0xfe; // clear bit(0)
        }
    };

    /* TX transaction finish */
	if(hidvendor_v->lenToSend == 0){
		hidvendor_v->lenToSend = 0;
		hidvendor_v->lastSendIndex = 0;

        if(hidvendor_v->txCb){
            //EV_SCHEDULE_TASK(hid_v->txCb, hid_v->txBuf);
            hidvendor_v->txCb((u8 *)hidvendor_v->txBuf);
        }

		hidvendor_v->txBuf = NULL;
	}

	return len;
}

/**
 * @brief This function sends data to host as the USB vendor device
 * @param[in]   buf pointer to the data need to send
 * @param[in]   len length in byte of the data need to send
 * @return none
 */
hidvendor_sts_t hidvendor_reportData(hidvendor_txBuf_t *txBuf)//u8 *buf, u8 len)
{
	if(hidvendor_v->txBuf){
		return USB_BUSY;
	}

	/* Init the bulk transfer */
    hidvendor_v->lenToSend = txBuf->len;
	hidvendor_v->txBuf = txBuf;
	hidvendor_v->lastSendIndex = 0;

    /* Send first bulk */
	hidvendor_sendData();

	usbhw_data_ep_ack(USB_EDP_HID_VENDOR_OUT);

	return USB_SUCCESS;
}

void hidvendor_dataHandler(void)
{
	u32 irq = reg_usb_irq;// data irq

	if(irq & BIT((USB_EDP_HID_VENDOR_OUT & 0x07))){
		reg_usb_irq = BIT((USB_EDP_HID_VENDOR_OUT & 0x07)); // clear ime

		hidvendor_recvData();

		usbhw_data_ep_ack(USB_EDP_HID_VENDOR_OUT);
	}

	if(irq & BIT((USB_EDP_HID_VENDOR_IN & 0x07))){
		reg_usb_irq = BIT((USB_EDP_HID_VENDOR_IN & 0x07)); // clear ime

		if(hidvendor_sendData()){
			usbhw_data_ep_ack(USB_EDP_HID_VENDOR_OUT);
		}
	}
}

#endif
