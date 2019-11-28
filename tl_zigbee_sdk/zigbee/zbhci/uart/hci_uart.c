/********************************************************************************************************
 * @file     hci_uart.c
 *
 * @brief    hci API for UART
 *
 * @author
 * @date     June. 2, 2017
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
#include "../zbhci.h"
#include "../../common/includes/zb_task_queue.h"

#if (ZBHCI_EN && ZBHCI_UART)


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

		ret[0] = st;
		ret[1] = 0;
		ret[2] = msg->msgType16H;
		ret[3] = msg->msgType16L;

		zbhciTx(ZBHCI_CMD_ACK, 4, ret);
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

    uart_tx_start(uartTxBuf, p - uartTxBuf);
}


void hci_uart_init(void){
	UART_PIN_CFG;
	drv_uart_init(115200, uartRxBuf, UART_RX_BUF_SIZE, uartRcvHandler);
}

#endif


