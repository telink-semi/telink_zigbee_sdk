/********************************************************************************************************
 * @file	drv_uart.h
 *
 * @brief	This is the header file for drv_uart
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
#pragma once


#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define UART_CLOCK_SOURCE			CLOCK_SYS_CLOCK_HZ
#elif defined(MCU_CORE_B91)
	/* PCLK provides clock source for UART module. */
	#define UART_CLOCK_SOURCE			(sys_clk.pclk * 1000 * 1000)
#endif

enum{
	UART_STA_IDLE,
	UART_STA_TX_DOING,
	UART_STA_TX_DONE,
	UART_STA_RX_DONE,
};

typedef void (*uart_irq_callback)(void);
typedef u8 (*uart_data_send)(u8 *data);

typedef struct{
	uart_irq_callback recvCb;
	uart_data_send send;
	volatile u8 status;
}drv_uart_t;


/****************************************************************************************
 *	@brief	uart tx/rx pin set
 *
 *	@param  txPin		TX pin
 *	@param  rxPin		RX pin
 *
 *	@return	none
 */
void drv_uart_pin_set(u32 txPin, u32 rxPin);

/****************************************************************************************
 *	@brief	init UART receive callback function
 *
 *	@param  baudRate		baud rate
 *	@param  rxBuf			the buffer for data receiving
 *	@param  rxBufLen		the length of the uart RX buffer, must be a multiple of 4
 *	@param	uart_recvCb		UART receive callback function.
 *
 *	@return	none
 */
void drv_uart_init(u32 baudRate, u8 *rxBuf, u16 rxBufLen, uart_irq_callback uartRecvCb);

/****************************************************************************************
 *	@brief	uart Rx ISR
 *
 */
void drv_uart_rx_irq_handler(void);

/****************************************************************************************
 *	@brief	uart Tx ISR
 *
 */
void drv_uart_tx_irq_handler(void);

/****************************************************************************************
 *	@brief	uart data send function
 *
 *	@param	data	start address of the data
 *	@param	len		length of data
 *
 *	@return	1: finished, 0: failed
 */
u8 drv_uart_tx_start(u8 *data, u32 len);

/****************************************************************************************
 *	@brief	check if exception occurs
 *
 */
void drv_uart_exceptionProcess(void);
