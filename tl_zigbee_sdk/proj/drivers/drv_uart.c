/********************************************************************************************************
 * @file     drv_uart.c
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
#include "drv_uart.h"
#include "platform_includes.h"
#include "../os/ev_buffer.h"
#include "../common/string.h"


drv_uart_t myUartDriver = {
		.status = UART_STA_IDLE,
		.recvCb = NULL,
#if	defined(MCU_CORE_826x)
		.send = uart_pktSend,
#elif defined(MCU_CORE_HAWK)
		.send = NULL,
#else
		.send = uart_dma_send,
#endif
};

static u8 *uartDrvTxBuf = NULL;

void drv_uart_init(u32 baudrate, u8 *rxBuf, u16 rxBufLen, uart_irq_callback uart_recvCb){
#if	defined (MCU_CORE_826x)
	uart_Init(baudrate, 1, 1, NOCONTROL);

	uart_RecBuffInit(rxBuf, rxBufLen);	/* configure receive buffer */
#elif defined(MCU_CORE_HAWK)

#else
	uart_recbuff_init( (unsigned short *)rxBuf, rxBufLen);
	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset

	uart_init(baudrate, PARITY_NONE, STOP_BIT_ONE);

	// dma mode
	uart_dma_enable(1, 1); 	//uart data in hardware buffer moved by dma, so we need enable them first
	irq_set_mask(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1); 	//uart Rx/Tx dma irq enable
	uart_irq_enable(0, 0);  	//uart Rx/Tx irq no need, disable them
#endif

	myUartDriver.recvCb = uart_recvCb;
}

void uart_rx_irq_handler(void){
	//TODO: Rx done
	if(myUartDriver.recvCb){
		myUartDriver.recvCb();
	}
}

void uart_tx_irq_handler(void){
	//TODO: Tx done
	if(uartDrvTxBuf){
		ev_buf_free(uartDrvTxBuf);
		uartDrvTxBuf = NULL;
	}
	myUartDriver.status = UART_STA_TX_DONE;
}

u8 uart_is_idel(void){
	return ((myUartDriver.status == UART_STA_IDLE) ? TRUE : FALSE);
}

u8 uart_tx_done(void){
	return ((myUartDriver.status == UART_STA_TX_DONE) ? TRUE : FALSE);
}

u8 uart_tx_start(u8 *data, u32 len){
	if(!uart_is_idel()){
		while(!uart_tx_done());
	}

	if(!uartDrvTxBuf){
		uartDrvTxBuf = (u8 *)ev_buf_allocate(len+4);
		if(uartDrvTxBuf){
			myUartDriver.status = UART_STA_TX_DOING;
			uartDrvTxBuf[0] = (unsigned char)(len);
			uartDrvTxBuf[1] = (unsigned char)(len>>8);
			uartDrvTxBuf[2] = (unsigned char)(len>>16);
			uartDrvTxBuf[3] = (unsigned char)(len>>24);
			memcpy(uartDrvTxBuf+4, data, len);
			if(myUartDriver.send){
				while(!myUartDriver.send(uartDrvTxBuf));
				return 1;
			}
		}
	}
	return 0;
}

void uart_exceptionProcess(void){
#if	defined (MCU_CORE_826x)
	uart_ErrorCLR();
#elif defined(MCU_CORE_8258)
	if(uart_is_parity_error()){
		uart_clear_parity_error();
	}
#endif
}

