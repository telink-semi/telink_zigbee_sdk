/********************************************************************************************************
 * @file	drv_uart.c
 *
 * @brief	This is the source file for drv_uart
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
#include "../tl_common.h"

#if defined(MCU_CORE_B91)
#define UART_IDX					UART0
#define UART_DMA_CHANNEL_RX			DMA2
#define UART_DMA_CHANNEL_TX			DMA3

static u8 *pUartRxBuf = NULL;
static u32 uartRxBufLen = 0;

#define UART_RCV_DMA_LEN_FIX()		do{	\
										u32 rcvDataLen = 0;																\
										u8 cnt = (reg_uart_status1(UART_IDX) & FLD_UART_RBCNT) % 4;						\
										if(cnt){																		\
											rcvDataLen = 4 * (0xffffff - reg_dma_size(UART_DMA_CHANNEL_RX) - 1) + cnt;	\
										}else{																			\
											rcvDataLen = 4 * (0xffffff - reg_dma_size(UART_DMA_CHANNEL_RX));			\
										}																				\
										if(pUartRxBuf){																	\
											pUartRxBuf[0] = (u8)(rcvDataLen);											\
											pUartRxBuf[1] = (u8)(rcvDataLen >> 8);										\
											pUartRxBuf[2] = (u8)(rcvDataLen >> 16);										\
											pUartRxBuf[3] = (u8)(rcvDataLen >> 24);										\
										}																				\
									}while(0)

static u8 uart_dma_send(u8 *pBuf);
#endif	/* MCU_CORE_B91 */

drv_uart_t myUartDriver = {
		.status = UART_STA_IDLE,
		.recvCb = NULL,
#if	defined(MCU_CORE_826x)
		.send  = uart_pktSend,
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
		.send  = uart_dma_send,
#endif
};

static u8 *uartDrvTxBuf = NULL;

void drv_uart_init(u32 baudrate, u8 *rxBuf, u16 rxBufLen, uart_irq_callback uartRecvCb)
{
	myUartDriver.status = UART_STA_IDLE;
	myUartDriver.recvCb = uartRecvCb;

	if(uartDrvTxBuf){
		ev_buf_free(uartDrvTxBuf);
		uartDrvTxBuf = NULL;
	}

#if	defined(MCU_CORE_826x)
	uart_Reset();
	uart_Init(baudrate, 1, 1, NOCONTROL);
	/* configure receive buffer */
	uart_RecBuffInit(rxBuf, rxBufLen);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	uart_recbuff_init(rxBuf, rxBufLen);
	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset

	uart_init_baudrate(baudrate, UART_CLOCK_SOURCE, PARITY_NONE, STOP_BIT_ONE);

	// dma mode
	uart_dma_enable(1, 1); 	//uart data in hardware buffer moved by dma, so we need enable them first
	irq_set_mask(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1); 	//uart Rx/Tx dma irq enable
	uart_irq_enable(0, 0);  	//uart Rx/Tx irq no need, disable them
#elif defined(MCU_CORE_B91)
	u16 div = 0;
	u8 bwpc = 0;

	pUartRxBuf = rxBuf;
	uartRxBufLen = rxBufLen;

	uart_reset(UART_IDX);

	uart_cal_div_and_bwpc(baudrate, UART_CLOCK_SOURCE, &div, &bwpc);
	uart_set_dma_rx_timeout(UART_IDX, bwpc, 12, UART_BW_MUL1);
	uart_init(UART_IDX, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	dma_clr_irq_mask(UART_DMA_CHANNEL_TX, TC_MASK | ABT_MASK | ERR_MASK);
	dma_clr_irq_mask(UART_DMA_CHANNEL_RX, TC_MASK | ABT_MASK | ERR_MASK);
	uart_set_tx_dma_config(UART_IDX, UART_DMA_CHANNEL_TX);
	uart_set_rx_dma_config(UART_IDX, UART_DMA_CHANNEL_RX);

	uart_receive_dma(UART_IDX, pUartRxBuf + 4, uartRxBufLen);

	uart_clr_tx_done(UART_IDX);
	uart_clr_irq_mask(UART_IDX, UART_RX_IRQ_MASK | UART_TX_IRQ_MASK | UART_TXDONE_MASK | UART_RXDONE_MASK);
	uart_set_irq_mask(UART_IDX, UART_RXDONE_MASK | UART_TXDONE_MASK);
	plic_interrupt_enable((UART_IDX == UART0) ? IRQ19_UART0 : IRQ18_UART1);
#endif
}

void drv_uart_pin_set(u32 txPin, u32 rxPin)
{
#if	defined(MCU_CORE_826x)
	gpio_set_func(txPin, AS_UART);
	gpio_set_func(rxPin, AS_UART);
	gpio_set_output_en(txPin, 1);
	gpio_set_input_en(txPin, 0);
	gpio_set_output_en(rxPin, 0);
	gpio_set_input_en(rxPin, 1);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	uart_gpio_set(txPin, rxPin);
#elif defined(MCU_CORE_B91)
	uart_set_pin(txPin, rxPin);
#endif
}

void drv_uart_rx_irq_handler(void)
{
#if defined(MCU_CORE_B91)
	bool uartRxErr = 0;

	if((uart_get_irq_status(UART_IDX, UART_RX_ERR))){
		uartRxErr = 1;
	}else{
		/* Fix lost DMA length information, must before clear irq status. */
		UART_RCV_DMA_LEN_FIX();
	}

	uart_clr_irq_status(UART_IDX, UART_CLR_RX);

	/* Need to reconfigure RX DMA. */
	uart_receive_dma(UART_IDX, pUartRxBuf + 4, uartRxBufLen);

	if(uartRxErr){
		return;
	}
#endif

	if(myUartDriver.recvCb){
		myUartDriver.recvCb();
	}
}

void drv_uart_tx_irq_handler(void)
{
#if defined(MCU_CORE_B91)
	uart_clr_tx_done(UART_IDX);
#endif

	if(uartDrvTxBuf){
		ev_buf_free(uartDrvTxBuf);
		uartDrvTxBuf = NULL;
	}
	myUartDriver.status = UART_STA_TX_DONE;
}

#if defined(MCU_CORE_B91)
static u8 uart_dma_send(u8 *pBuf)
{
	u32 len = BUILD_U32(pBuf[0], pBuf[1], pBuf[2], pBuf[3]);

	return uart_send_dma(UART_IDX, pBuf + 4, len);
}
#endif

static bool uart_tx_done(void)
{
	return ((myUartDriver.status == UART_STA_TX_DONE) ? TRUE : FALSE);
}

static bool uart_is_idel(void)
{
	return ((myUartDriver.status == UART_STA_IDLE) ? TRUE : FALSE);
}

u8 drv_uart_tx_start(u8 *data, u32 len)
{
	if(!uart_is_idel()){
		while(!uart_tx_done());
	}

	if(!uartDrvTxBuf){
		uartDrvTxBuf = (u8 *)ev_buf_allocate(len + 4);
		if(uartDrvTxBuf){
			myUartDriver.status = UART_STA_TX_DOING;
			uartDrvTxBuf[0] = (u8)(len);
			uartDrvTxBuf[1] = (u8)(len >> 8);
			uartDrvTxBuf[2] = (u8)(len >> 16);
			uartDrvTxBuf[3] = (u8)(len >> 24);
			memcpy(uartDrvTxBuf + 4, data, len);
			if(myUartDriver.send){
				while(!myUartDriver.send(uartDrvTxBuf));
				return 1;
			}
		}
	}
	return 0;
}

void drv_uart_exceptionProcess(void)
{
#if	defined(MCU_CORE_826x)
	uart_ErrorCLR();
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)  
	if(uart_is_parity_error()){
		uart_clear_parity_error();
	}
#elif defined(MCU_CORE_B91)
	/* RX ERROR is handled in the RX interrupt processing function. */
#endif
}


