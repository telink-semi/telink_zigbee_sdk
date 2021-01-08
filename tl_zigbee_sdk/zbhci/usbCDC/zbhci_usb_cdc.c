/********************************************************************************************************
 * @file	zbhci_usb_cdc.c
 *
 * @brief	This is the source file for zbhci_usb_cdc
 *
 * @author	Zigbee Group
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
#include "tl_common.h"

#if (ZBHCI_EN && ZBHCI_USB_CDC)
#include "drivers/usb/usb_common.h"
#include "zb_task_queue.h"
#include "../zbhci.h"


#define RX_NDATA_LEN					7
#define	GET_RXPACKETLEN(ptr)			(((ptr)[3] << 8) | ((ptr)[4]))

#define RX_BUF_LEN    					64 //in bytes
#define TX_BUF_LEN    					64 //in bytes

static ev_queue_t usbcdc_txPendingQ;


static void usb_cdc_send(usbcdc_txBuf_t *pTxBuf){
	if(usbcdc_isAvailable()){
		usbcdc_sendData(pTxBuf);
	}else{
		ev_queue_push(&usbcdc_txPendingQ, (u8*)pTxBuf);
	}
}

static void usbcdc_data_handler(void *arg){
	 /*
	 * the format of the uart rx data: length(4 Bytes) + payload
	 *
	 * */
	u8 ret[4] = {0,0,0,0};
	u8 st = SUCCESS;

	u8 *p = arg;
	u8 len = GET_RXPACKETLEN(p) + RX_NDATA_LEN;
	u8 endChar = *(p + len - 1);

	zbhci_msg_t *msg = (zbhci_msg_t *)arg;

	if(msg->startFlag == ZBHCI_MSG_START_FLAG){
		/* check the start flag */
		u16 pktLen = (msg->msgLen16H << 8) | msg->msgLen16L;
		if((pktLen + ZBHCI_MSG_HDR_LEN) < RX_BUF_LEN){
			/* check the end flag */
			if(endChar != ZBHCI_MSG_END_FLAG){
			   st = ZBHCI_MSG_STATUS_ERROR_END_CHAR;
			}
		}else{
			st = ZBHCI_MSG_STATUS_MSG_OVERFLOW;
		}
	}else{
		st = ZBHCI_MSG_STATUS_ERROR_START_CHAR;
	}

	if(st == SUCCESS){
		u16 pktLen = (msg->msgLen16H << 8) | msg->msgLen16L;
		u16 msgType = (msg->msgType16H<<8) + msg->msgType16L;

	    zbhciCmdHandler(msgType, pktLen, msg->pData);
	}
	else{
		if((st == ZBHCI_MSG_STATUS_UART_EXCEPT) || (st == ZBHCI_MSG_STATUS_MSG_OVERFLOW) || (st == ZBHCI_MSG_STATUS_ERROR_START_CHAR)){
			msg->msgType16H = 0xff;
			msg->msgType16L = 0xff;
		}

		ret[0] = msg->msgType16H;
		ret[1] = msg->msgType16L;
		ret[2] = st;
		ret[3] = 0;

		zbhciTx(ZBHCI_CMD_ACKNOWLEDGE, 4, ret);
	}

	ev_buf_free((u8 *)arg);
}

static void usbcdc_rxCb(u8 *pData){
	TL_SCHEDULE_TASK(usbcdc_data_handler, pData);

	/* Set the USB RX buffer again */
	u8 *pBuf = ev_buf_allocate(RX_BUF_LEN);
	if(!pBuf){
		while(1);
	}
	memset(pBuf, 0, RX_BUF_LEN);
	usbcdc_setRxBuf(pBuf);
}

static void usbcdc_txFinishCb(u8 *pData){
    /* Free the TX buffer at first */
    ev_buf_free(pData);

    /* If there is pending data, send it again */
    if(usbcdc_txPendingQ.curNum){
    	usbcdc_txBuf_t *pTxBuf = (usbcdc_txBuf_t *)ev_queue_pop(&usbcdc_txPendingQ);
    	usb_cdc_send(pTxBuf);
    }
}

void usb_cdc_init(void){
	/* Initialize USB-CDC parameters */
	usbcdc_init();

	/* Register callback */
	usbcdc_setCb(usbcdc_rxCb, usbcdc_txFinishCb);

	/* Set RX buffer to USB-CDC */
	u8 *pBuf = ev_buf_allocate(RX_BUF_LEN);
	if(!pBuf){
		while(1);
	}
	memset(pBuf, 0, RX_BUF_LEN);
	usbcdc_setRxBuf(pBuf);

    /* Initialize USB tx pending Queue */
    ev_queue_init(&usbcdc_txPendingQ, NULL);
}

zbhciTx_e usb_cdc_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data){
	if(u16Length > TX_BUF_LEN){
		return ZBHCI_TX_TOO_LONG;
	}

	usbcdc_txBuf_t *pTxBuf = (usbcdc_txBuf_t *)ev_buf_allocate(sizeof(u16) + u16Length + 7);
	if(!pTxBuf){
		return ZBHCI_TX_FAILED;
	}

	pTxBuf->len = u16Length + 7;
	u8 *p = pTxBuf->data;

	*p++ = 0x55;
	*p++ = (u16Type >> 8) & 0xff;
	*p++ = (u16Type >> 0) & 0xff;
	*p++ = (u16Length >> 8) & 0xff;
	*p++ = (u16Length >> 0) & 0xff;
	*p++ = crc8Calculate(u16Type, u16Length, pu8Data);
	for(u16 n = 0; n < u16Length; n++){
		*p++ = pu8Data[n];
	}
	*p++ = 0xAA;

	usb_cdc_send(pTxBuf);

	return ZBHCI_TX_SUCCESS;
}

#endif


