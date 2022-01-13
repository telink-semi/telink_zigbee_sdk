/********************************************************************************************************
 * @file    hci_uart.c
 *
 * @brief   This is the source file for hci_uart
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


