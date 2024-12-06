/********************************************************************************************************
 * @file    hci_uart.c
 *
 * @brief   This is the source file for hci_uart
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

#if (ZBHCI_EN && ZBHCI_UART)
#include "zb_task_queue.h"
#include "../zbhci.h"

#define UART_TX_BUF_SIZE    128
#define UART_RX_BUF_SIZE    128


//begin
typedef struct{
	u32 dataLen;
	u8 dataPayload[1];
}uart_rxData_t;

__attribute__((aligned(4))) u8 uartTxBuf[UART_TX_BUF_SIZE] = {0};
__attribute__((aligned(4))) u8 uartRxBuf[UART_RX_BUF_SIZE] = {0};

void uart_data_handler(void *arg){
	 //the format of the uart rx buffer: length(4 Bytes) + payload
	u8 ret[4] = {0,0,0,0};
	u8 st = SUCCESS;

	uart_rxData_t *rxData = (uart_rxData_t *)uartRxBuf;
	u8 *pData = rxData->dataPayload;
	zbhci_msg_t *msg = NULL;
	u32 reLen = rxData->dataLen;
	u16 msgLen = 0;
	u16 msgType = 0;

	if(rxData->dataLen){
		if(rxData->dataLen > (UART_RX_BUF_SIZE - 4)){
			st = ZBHCI_MSG_STATUS_MSG_OVERFLOW;
		}else{
			while(reLen){
				msg = (zbhci_msg_t *)pData;

				/* check the start flag */
				if(msg->startFlag != ZBHCI_MSG_START_FLAG){
					st = ZBHCI_MSG_STATUS_ERROR_START_CHAR;
				}else{
					msgLen = (msg->msgLen16H << 8) + msg->msgLen16L;

					if(reLen < msgLen + ZBHCI_MSG_HDR_LEN){
						st = ZBHCI_MSG_STATUS_BAD_MSG;
					}else{
						u8 endChar = *(msg->pData + msgLen);

						/* check the end flag */
						if(endChar != ZBHCI_MSG_END_FLAG){
							st = ZBHCI_MSG_STATUS_ERROR_END_CHAR;
						}else{
							msgType = (msg->msgType16H << 8) + msg->msgType16L;

							u8 crc8 = crc8Calculate(msgType, msgLen, msg->pData);

							if((msgType == ZBHCI_CMD_OTA_START_REQUEST) || (msgType == ZBHCI_CMD_OTA_BLOCK_RESPONSE)){
								if(crc8 != msg->checkSum){
									st = ZBHCI_MSG_STATUS_CRC_ERROR;
								}
							}
						}
					}
				}

				if(st != SUCCESS){
					break;
				}else{
					zbhciCmdHandler(msgType, msgLen, msg->pData);

					if(reLen >= msgLen + ZBHCI_MSG_HDR_LEN){
						reLen -= msgLen + ZBHCI_MSG_HDR_LEN;
						pData += msgLen + ZBHCI_MSG_HDR_LEN;
					}
				}
			}
		}
	}else{
		st = ZBHCI_MSG_STATUS_UART_EXCEPT;
	}

	if(st != SUCCESS){
		if((st == ZBHCI_MSG_STATUS_UART_EXCEPT) ||
		   (st == ZBHCI_MSG_STATUS_MSG_OVERFLOW) ||
		   (st == ZBHCI_MSG_STATUS_ERROR_START_CHAR)){
			ret[0] = 0xff;
			ret[1] = 0xff;
		}else{
			ret[0] = msg->msgType16H;
			ret[1] = msg->msgType16L;
		}
		ret[2] = st;
		ret[3] = 0;

		zbhciTx(ZBHCI_CMD_ACKNOWLEDGE, 4, ret);
	}
}

void uartRcvHandler(void){

	TL_SCHEDULE_TASK(uart_data_handler, uartRxBuf);
}

zbhciTx_e uart_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data)
{
	if(u16Length + 7 > UART_TX_BUF_SIZE){
		return ZBHCI_TX_TOO_LONG;
	}

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

    return ZBHCI_TX_SUCCESS;
}


void hci_uart_init(void){
	UART_PIN_CFG();
	drv_uart_init(UART_BAUDRATE, uartRxBuf, UART_RX_BUF_SIZE, uartRcvHandler);
}

#endif


