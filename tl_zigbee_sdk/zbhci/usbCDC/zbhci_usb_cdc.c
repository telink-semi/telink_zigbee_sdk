/********************************************************************************************************
 * @file    zbhci_usb_cdc.c
 *
 * @brief   This is the source file for zbhci_usb_cdc
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
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

	u16 pktLen = (msg->msgLen16H << 8) | msg->msgLen16L;
	u16 msgType = (msg->msgType16H<<8) + msg->msgType16L;
	
	if(st == SUCCESS){
	    u8 crc8 = crc8Calculate(msgType, pktLen, msg->pData);
	    if((msgType == ZBHCI_CMD_OTA_START_REQUEST) || (msgType == ZBHCI_CMD_OTA_BLOCK_RESPONSE)){
	    	if(crc8 != msg->checkSum){
	    		st = ZBHCI_MSG_STATUS_CRC_ERROR;
	    	}
	    }
	}

	if(st == SUCCESS){
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


