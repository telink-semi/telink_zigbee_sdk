/********************************************************************************************************
 * @file     uart_826x.h
 *
 * @brief    This file contains the details of enum variables and functions used in the uart.c file
 *
 * @author   junjun.xu@telink-semi.com
 * @date     June. 5, 2016
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

#ifndef 	uart_H
#define 	uart_H
enum HARDWARECONTROL{
	CTSWODDPARITY = 0x0e,
	CTSWEVENPARITY = 0x06,
	CTSONLY = 0x02,
	ODDPARITY = 0x0C,
	EVENPARITY = 0x04,
	NOCONTROL = 0x00,
};
enum UARTIRQSOURCE{
	NONE_DMA_IRQ,
	UART_RX_DMA0=1,
	UART_TX_DMA1,
	RF_RX_DMA2,
	RF_TX_DMA3=4,
};



//UART_TX/UART_RX gpio pin config
#define	   UART_GPIO_CFG_PA6_PA7()  do{\
										*(volatile unsigned char  *)0x800586 &= 0x3f;\
										*(volatile unsigned char  *)0x8005b0 |= 0x80;\
								    }while(0) 
#define	   UART_GPIO_CFG_PB2_PB3()  do{\
										*(volatile unsigned char  *)0x80058e &= 0xf3;\
										*(volatile unsigned char  *)0x8005b1 |= 0x0c;\
								    }while(0)  
#define	   UART_GPIO_CFG_PC2_PC3()  do{\
										*(volatile unsigned char  *)0x800596 &= 0xf3;\
										*(volatile unsigned char  *)0x8005b2 |= 0x0c;\
								    }while(0)  

/**********************************************************
*
*	@brief	reset uart module
*	
*	@param	none
*
*	@return	none
*/
extern void uart_Reset(void );

extern volatile unsigned char uart_status;
/**********************************************************
*	
*	@brief	clear error state of uart rx, maybe used when application detected UART not work
*
*	@parm	none
*
*	@return	'1' RX error flag rised and cleard success; '0' RX error flag not rised 
*
*/
unsigned char uart_ErrorCLR(void);


/*******************************************************
*
*	@brief	uart initiate, set uart clock divider, bitwidth and the uart work mode
*
*	@param	baud_rate - the selected baudrate for UART interface
*			en_rx_irq - '1' enable rx irq; '0' disable.
*			en_tx_irq - enable tx irq; '0' disable.
*			hdwC - enum variable of hardware control functions
*
*	@return	'1' set success; '0' set error probably bwpc smaller than 3.
*/
extern unsigned char uart_Init(unsigned int baud_rate,unsigned char en_rx_irq,unsigned char en_tx_irq,enum HARDWARECONTROL hdwC);

/********************************************************************************
*	@brief	uart send data function, this  function tell the DMA to get data from the RAM and start 
*			the DMA send function
*
*	@param	sendBuff - send data buffer
*
*	@return	'1' send success; '0' DMA busy
*/
extern unsigned char uart_Send(unsigned char* addr);

/********************************************************************************
*	@brief	uart send data function,
*
*	@param	data - send data buffer
*
*	@return	'1' send success; '0' fail
*/
unsigned char  uart_pktSend(unsigned char  *data);

/********************************************************************
*	
*	@brief	uart receive function, call this function to get the UART data
*
*	@param	userDataBuff - data buffer to store the uart data
*
*	@return	'0' rx error; 'rxLen' received data length
*/
//extern unsigned short uart_Rec(unsigned char* addr);

/******************************************************************************
*
*	@brief		get the uart IRQ source and clear the IRQ status, need to be called in the irq process function
*
*	@return		uart_irq_src- enum variable of uart IRQ source, 'UARTRXIRQ' or 'UARTTXIRQ'
*
*/
extern enum UARTIRQSOURCE DMA_IRQSourceGet(void);

/****************************************************************************************
*
*	@brief	data receive buffer initiate function. DMA would move received uart data to the address space, uart packet length
*			needs to be no larger than (recBuffLen - 4).
*
*	@param	*recAddr:	receive buffer's address info.
*			recBuffLen:	receive buffer's length, the maximum uart packet length should be smaller than (recBuffLen - 4)
*
*	@return	none
*/

extern void uart_RecBuffInit(unsigned char *recAddr, unsigned short recBuffLen);

#endif

