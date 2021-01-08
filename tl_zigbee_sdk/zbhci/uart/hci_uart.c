/********************************************************************************************************
 * @file	hci_uart.c
 *
 * @brief	This is the source file for hci_uart
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

#if (ZBHCI_EN && ZBHCI_UART)
#include "zb_task_queue.h"
#include "../zbhci.h"

#define UART_TX_BUF_SIZE    64
#define UART_RX_BUF_SIZE    64


//begin
typedef struct{
	u32 dataLen;
	u8 dataPayload[1];
}uart_rxData_t;

__attribute__((aligned(4))) u8 uartTxBuf[UART_TX_BUF_SIZE] = {0};
__attribute__((aligned(4))) u8 uartRxBuf[UART_RX_BUF_SIZE] = {0};

void uart_data_handler(void *arg){

	 /*
	 * the format of the uart rx data: length(4 Bytes) + payload
	 *
	 * */
	u8 ret[4] = {0,0,0,0};
	u8 st = SUCCESS;

	uart_rxData_t *rxData = (uart_rxData_t *)uartRxBuf;
	zbhci_msg_t *msg = (zbhci_msg_t *)(rxData->dataPayload);

	if(rxData->dataLen == 0){
		st = ZBHCI_MSG_STATUS_UART_EXCEPT;
	}

	if(rxData->dataLen > (UART_RX_BUF_SIZE - 4)){
		st = ZBHCI_MSG_STATUS_MSG_OVERFLOW;
	}

	if(st == SUCCESS){
		if(msg->startFlag == ZBHCI_MSG_START_FLAG){
			msg->msgType16H = rxData->dataPayload[1];
			msg->msgType16L = rxData->dataPayload[2];
			msg->msgLen16H = rxData->dataPayload[3];
			msg->msgLen16L = rxData->dataPayload[4];

			/* check the start flag */
			u16 pktLen = (msg->msgLen16H << 8) | msg->msgLen16L;
			if((pktLen + ZBHCI_MSG_HDR_LEN) == rxData->dataLen){
				/* check the end flag */
				if(rxData->dataPayload[rxData->dataLen - 1] != ZBHCI_MSG_END_FLAG){
				   st = ZBHCI_MSG_STATUS_ERROR_END_CHAR;
				}
			}else{
				st = ZBHCI_MSG_STATUS_BAD_MSG;
			}
		}else{
			st = ZBHCI_MSG_STATUS_ERROR_START_CHAR;
		}
	}

	if(st == SUCCESS){
		u16 pktLen = (msg->msgLen16H << 8) | msg->msgLen16L;
		u16 msgType = (msg->msgType16H<<8) + msg->msgType16L;

	    rxData->dataLen = 0;

	    zbhciCmdHandler(msgType, pktLen, msg->pData);
	}else{
		rxData->dataLen = 0;

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
}

void uartRcvHandler(void){

	TL_SCHEDULE_TASK(uart_data_handler, uartRxBuf);
}

void uart_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data)
{
    int n;
    u8 crc8 = crc8Calculate(u16Type, u16Length, pu8Data);

    u8 *p = uartTxBuf;
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

    drv_uart_tx_start(uartTxBuf, p - uartTxBuf);
}


void hci_uart_init(void){
	UART_PIN_CFG();
	drv_uart_init(115200, uartRxBuf, UART_RX_BUF_SIZE, uartRcvHandler);
}

#endif


