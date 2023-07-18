/********************************************************************************************************
 * @file    hci_usbPrint.c
 *
 * @brief   This is the source file for hci_usbPrint
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
 *
 *******************************************************************************************************/

#include "tl_common.h"

#if (ZBHCI_EN && ZBHCI_USB_PRINT)
#include "../zbhci.h"

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
	u16 usbDeviceId = *((u16 *)CFG_TELINK_USB_ID);
	write_reg8(0x74, 0x53);

#ifdef CHIP_8269
	write_reg16(0x7e, usbDeviceId ^ 0x01);		// for 8269
#else
	write_reg16(0x7e, usbDeviceId);    		//for 8267
#endif
	write_reg8(0x74, 0);

	write_reg16(0x8004, (u16)usb_print_inbuf);/* write */
	write_reg16(0x8006, (u16)usb_print_outbuf);/* read */
}

u8 usb_print_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data){
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

void usb_print_task(void){
	zbhci_msg_t *msg = (zbhci_msg_t *)usb_print_inbuf;
	if(msg->startFlag == ZBHCI_MSG_START_FLAG){
		u16 pLen = (msg->msgLen16H<<8) + msg->msgLen16L;
		u16 msgType = (msg->msgType16H<<8) + msg->msgType16L;
#if ZB_TEST_ENABLE
		zbhciProcessIncomingSerialCmd(msgType, pLen, msg->pData);
#else
		zbhciCmdHandler(msgType, pLen, msg->pData);
#endif
		msg->startFlag = 0;
	}
}
#endif


