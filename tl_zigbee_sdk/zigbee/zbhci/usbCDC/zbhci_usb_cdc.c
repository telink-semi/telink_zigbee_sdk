/********************************************************************************************************
 * @file     zbhci_usb_cdc.c
 *
 * @brief    HCI  API for USB CDC
 *
 * @author
 * @date     June. 1, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "../../include/zb_common.h"
#include "../../../proj/drivers/usb/usbCDC/usb_ctrl.h"
#include "../../../proj/drivers/usb/usbCDC/usb_cdc.h"
#include "../zbhci.h"

#if (ZBHCI_EN && ZBHCI_USB_CDC)

typedef void (*zbhciRxCbFun)(u8 *buf, u8 len);
typedef void (*zbhciTxDoneCbFun)(u8 *buf);

//define rx buffer
#define RX_BUF_LEN    		64 //in bytes
#define TX_BUF_LEN    		64 //in bytes
#define RX_BUF_NUM    		4
#define RX_NDATA_LEN		7

static unsigned char rx_buf[RX_BUF_NUM][RX_BUF_LEN];
static unsigned char rx_ptr = 0;
__attribute__((aligned(4))) u8 usb_cdcTxbuf[TX_BUF_LEN] = {0};

u8 tx_rdPtr = 0;
zbhciRxCbFun hciCb = NULL;
zbhciTxDoneCbFun hciTxDoneCb = NULL;

void usbcdc_data_handler(void *arg);

static void USBCDC_RxCb(unsigned char *data){
    USBCDC_RxBufSet(rx_buf[(rx_ptr++&0x03)]);
    if(hciCb){
    	hciCb(data,RX_BUF_LEN);
    }
}

u8 push_data_flag = 0;
u8 checksum(u8 *data,u8 len){
	u8 ret = *data;
	for(u8 i=0;i<len - 1;i++){
		ret ^= data[i+1];
	}
	return ret;
}

bool usbRwBusy(void){
	return (!USBCDC_IsAvailable());
}

zbhciTx_e usbRwTx(u8 *buf, u8 len){
	if(USBCDC_IsAvailable()){
		return USBCDC_DataSend(buf, len);
	}
	return ZBHCI_TX_BUSY;
}

zbhciTx_e usb_cdc_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data){
	int n;
	u8 crc8 = crc8Calculate(u16Type, u16Length, pu8Data);

	u8 *p = usb_cdcTxbuf;
	*p++ = 0x55;
	*p++ = (u16Type >> 8) & 0xff;
	*p++ = (u16Type >> 0) & 0xff;
	*p++ = (u16Length >> 8) & 0xff;
	*p++ = (u16Length >> 0) & 0xff;
	*p++ = crc8;
	for(n = 0; n < u16Length; n++)
	{
		*p++ = pu8Data[n];
	}
	*p++ = 0xAA;

	return usbRwTx(usb_cdcTxbuf, p - usb_cdcTxbuf);
}

#define		GET_RXPACKETLEN(ptr)			(((ptr)[3]<<8)|((ptr)[4]))
struct zbhci_waitMoreData_t{
	u8					rxData[256];
	ev_time_event_t		*wmTimer;
	u8					wptrIndx;
}zbhci;

s32 zbhciPacketRxTimeoutCb(void *arg){
	zbhci.wmTimer = NULL;
	zbhci.wptrIndx = 0;
	return -1;
}

bool zbhciPacketRxCompleted(u8 **buf){
	u16 len = 0;
	if(!zbhci.wptrIndx){
		len = GET_RXPACKETLEN(*buf) + RX_NDATA_LEN;
	}else{
		len = GET_RXPACKETLEN(zbhci.rxData) + RX_NDATA_LEN;
	}
	if(len<RX_BUF_LEN){
		return TRUE;
	}

	memcpy(zbhci.rxData + zbhci.wptrIndx,*buf,RX_BUF_LEN);
	zbhci.wptrIndx += RX_BUF_LEN;

	if(zbhci.wptrIndx >= len){
		*buf = zbhci.rxData;
		zbhci.wptrIndx = 0;
		TL_ZB_TIMER_CANCEL(&zbhci.wmTimer);
		return TRUE;
	}
	if(!zbhci.wmTimer){
		zbhci.wmTimer = TL_ZB_TIMER_SCHEDULE(zbhciPacketRxTimeoutCb,NULL,30*1000);
	}
	return FALSE;
}

void zbhciRxCb(u8 *buf,u8 len){
	if(zbhciPacketRxCompleted(&buf)!=TRUE){
		return;
	}

	TL_SCHEDULE_TASK(usbcdc_data_handler, buf);
}

void usbcdc_data_handler(void *arg){
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

		ret[0] = st;
		ret[1] = 0;
		ret[2] = msg->msgType16H;
		ret[3] = msg->msgType16L;

		zbhciTx(ZBHCI_CMD_ACK, 4, ret);
	}
}

void USB_LogInit(void)
{
    write_reg8(0x80013c, 0x40);
    write_reg8(0x80013d, 0x09);
}

void zbhciTxDoneCb(u8 *buf){

}

void usb_cdc_init(void){
	USB_Init();
	USB_LogInit();
	usb_dp_pullup_en (1);
	USBCDC_RxBufSet(rx_buf[(rx_ptr++&0x03)]);

	USBCDC_CBSet(USBCDC_RxCb, zbhciTxDoneCb);

	hciCb = zbhciRxCb;
	hciTxDoneCb = zbhciTxDoneCb;
}

void usbRwTask(void){
	USB_IrqHandle();
}
#endif


