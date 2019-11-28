/********************************************************************************************************
 * @file     drv_uart.h
 *
 * @brief	 uart driver interface file
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "../common/types.h"
#include "../common/compiler.h"

typedef void (*uart_irq_callback)(void);

typedef unsigned char (* uart_data_send)(unsigned char *data);

typedef struct{
	uart_irq_callback recvCb;
	uart_data_send send;
	volatile u8 status;
}drv_uart_t;

enum {
	UART_STA_IDLE,
	UART_STA_TX_DOING,
	UART_STA_TX_DONE,
	UART_STA_RX_DONE,
};

/****************************************************************************************
*
*	@brief	init UART receive callback function
*
*
*	@param  baudRate		baud rate
*
*	@param  rxBuf			the buffer for data receiving
*
*	@param  rxBufLen		the lenght of the uart RX buffer
*
*	@param	*uart_recvCb:	UART receive callback function.
*
*	@return	none
*/
void drv_uart_init(u32 baudRate, u8 *rxBuf, u16 rxBufLen, uart_irq_callback uart_recvCb);


/*
 * uart Rx ISR
 *
 * */
void uart_rx_irq_handler(void);


/*
 * uart Tx ISR
 *
 * */
void uart_tx_irq_handler(void);


/****************************************************************************************
*
*	@brief	check if UART transmit is finished
*
*	@param
*
*	@return	1: finished, 0: not finished
*/
u8 uart_tx_done(void);


u8 uart_tx_start(u8 *data, u32 len);


/****************************************************************************************
*
*	@brief	check if exception occurs
*
*	@param
*
*/
void uart_exceptionProcess(void );
