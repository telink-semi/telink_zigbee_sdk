/********************************************************************************************************
 * @file    hci.h
 *
 * @brief   This is the header file for hci
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
#ifndef HCI_H
#define	HCI_H

#ifndef UART_BAUDRATE
#define UART_BAUDRATE                           115200
#endif

/** Macro to send a log message to the host machine
 *  First byte of the message is the level (0-7).
 *  Remainder of message is char buffer containing ascii message
 */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#define ZBHCI_MSG_STATUS_ERROR_START_CHAR       0xE0
#define ZBHCI_MSG_STATUS_MSG_OVERFLOW           0xE1
#define ZBHCI_MSG_STATUS_ERROR_END_CHAR         0xE2
#define ZBHCI_MSG_STATUS_BAD_MSG                0xE3
#define ZBHCI_MSG_STATUS_UART_EXCEPT            0xE4
#define ZBHCI_MSG_STATUS_CRC_ERROR              0xE5

#define	ZBHCI_MSG_START_FLAG                    0x55
#define	ZBHCI_MSG_END_FLAG                      0xAA
#define	ZBHCI_MSG_HDR_LEN                       0x07

#define	ZBHCI_CMD_ACKNOWLEDGE                   0x8000

typedef enum {
    ZBHCI_TX_SUCCESS,
    ZBHCI_TX_BUFFERFULL,
    ZBHCI_TX_BUSY,
    ZBHCI_TX_FAILED,
    ZBHCI_TX_TOO_LONG,
} zbhciTx_e;

typedef struct {
    u8 startFlag;
    u8 msgType16H;
    u8 msgType16L;
    u8 msgLen16H;
    u8 msgLen16L;
    u8 checkSum;
    u8 pData[1];
} zbhci_msg_t;

typedef struct {
    u16 cmdId;
    u16 payloadLen;
    u8  payload[1];
} zbhci_cmdHandler_t;

typedef void (*tl_hci_cmd_handler)(u16 msgType, u16 msgLen, u8 *p);
extern tl_hci_cmd_handler g_hci_cmd_handler;

u8 crc8Calculate(u16 type, u16 length, u8 *data);

extern zbhciTx_e zbhciTx(u16 u16Type, u16 u16Length, u8 *pu8Data);
extern void zbhciInit(tl_hci_cmd_handler cmd_cb);
extern void zbhciTask(void);

#endif
