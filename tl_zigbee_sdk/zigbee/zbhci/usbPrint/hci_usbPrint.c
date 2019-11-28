/********************************************************************************************************
 * @file     zbhci_usbPrint.c
 *
 * @brief    HCI  API for USB PRINT
 *
 * @author
 * @date     June. 3, 2017
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
#include "tl_common.h"
#include "../zbhci.h"


#if (ZBHCI_EN && ZBHCI_USB_PRINT)

u32 usb_print_inbuf[64/4];
u32 usb_print_outbuf[64/4];

typedef struct{
	u8 status;
	u8 payload[1];
}usbPrintCmd_t;

enum{
	USB_PRINT_CMD_IDLE = 0,
	USB_PRINT_CMD_INVALID = 0x03,
	USB_PRINT_CMD_AF_FROM_APP = 0x09,
	USB_PRINT_CMD_AF_FROM_STACK,
};

void usb_print_init(void){
	usb_dp_pullup_en(1);
	u16 usbDeviceId = *((u16 *)CFG_TELINK_USB_ID);
	write_reg8(0x800074, 0x53);

#ifdef CHIP_8269
	write_reg16 (0x80007e, usbDeviceId ^ 0x01);		// for 8269
#else
	write_reg16 (0x80007e, usbDeviceId);    		//for 8267
#endif
	write_reg8(0x800074, 0);

	write_reg16 (0x808004, (u16)usb_print_inbuf);/* write */
	write_reg16 (0x808006, (u16)usb_print_outbuf);/* read */
}

u8 usb_printTxMsg(u16 u16Type, u16 u16Length, u8 *pu8Data){
	 int n;
	u8 crc8 = crc8Calculate(u16Type, u16Length, pu8Data);

	u8 *p = usb_print_outbuf;
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
	return ZBHCI_TX_SUCCESS;
}

void usbPrintTask(void){
	zbhci_msg_t *msg = (zbhci_msg_t *)usb_print_inbuf;
	if(msg->startFlag == ZBHCI_MSG_START_FLAG){
		u16 pLen = (msg->msgLen16H<<8) + msg->msgLen16L;
		u16 msgType = (msg->msgType16H<<8) + msg->msgType16L;
#if ZB_TEST_ENABLE
		zbhciProcessIncommingSerialCmd(msgType, pLen, msg->pData);
#else
		zbhciCmdHandler(msgType, pktLen, msg->pData);
#endif
		msg->startFlag = 0;
	}
}
#endif


