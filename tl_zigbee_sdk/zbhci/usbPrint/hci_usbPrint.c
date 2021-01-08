/********************************************************************************************************
 * @file	hci_usbPrint.c
 *
 * @brief	This is the source file for hci_usbPrint
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
		zbhciProcessIncommingSerialCmd(msgType, pLen, msg->pData);
#else
		zbhciCmdHandler(msgType, pktLen, msg->pData);
#endif
		msg->startFlag = 0;
	}
}
#endif


