/********************************************************************************************************
 * @file    uart.h
 *
 * @brief   This is the header file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#ifndef UART_H
#define UART_H

#include "register.h"
#include "bit.h"
#include "gpio.h"


/**
 *  @brief  Define mul bits
 */
enum {
    UART_BW_MUL1        = 0,
    UART_BW_MUL2        = 1,
    UART_BW_MUL3        = 2,
    UART_BW_MUL4        = 3,
};

/**
 *  @brief  Define parity type
 */
typedef enum {
    PARITY_NONE         = 0,
    PARITY_EVEN,
    PARITY_ODD,
} UART_ParityTypeDef;

/**
 *  @brief  Define the length of stop bit
 */
typedef enum {
    STOP_BIT_ONE          = 0,
    STOP_BIT_ONE_DOT_FIVE = BIT(4),
    STOP_BIT_TWO          = BIT(5),
} UART_StopBitTypeDef;

/**
 *  @brief  Define UART RTS mode
 */
typedef enum {
    UART_RTS_MODE_AUTO  = 0,
    UART_RTS_MODE_MANUAL,
} UART_RTSModeTypeDef;

/**
 *  @brief  Define UART TX pin:A2 B1 C2 D0 D3 D7
 */

typedef enum {
    UART_TX_PA2 = GPIO_PA2,
    UART_TX_PB1 = GPIO_PB1,
    UART_TX_PC2 = GPIO_PC2,
    UART_TX_PD0 = GPIO_PD0,
    UART_TX_PD3 = GPIO_PD3,
    UART_TX_PD7 = GPIO_PD7,
    UART_TX_NONE_PIN = 0xfff,
} UART_TxPinDef;

/**
 *  @brief  Define UART RX pin: A0 B0 B7 C3 C5 D6
 */
typedef enum {
    UART_RX_PA0 = GPIO_PA0,
    UART_RX_PB0 = GPIO_PB0,
    UART_RX_PB7 = GPIO_PB7,
    UART_RX_PC3 = GPIO_PC3,
    UART_RX_PC5 = GPIO_PC5,
    UART_RX_PD6 = GPIO_PD6,
    UART_RX_NONE_PIN = 0xfff,
} UART_RxPinDef;

/**
 *  @brief  Define UART RTX pin: C2 D0 D3 D7
 */
typedef enum {
    UART_RTX_PC2 = GPIO_PC2,
    UART_RTX_PD0 = GPIO_PD0,
    UART_RTX_PD3 = GPIO_PD3,
    UART_RTX_PD7 = GPIO_PD7,
} UART_RTxPinDef;

/**
 *  @brief  Define UART CTS pin : A3 B2 C4 D1
 */
typedef enum {
    UART_CTS_PA3 = GPIO_PA3,
    UART_CTS_PB2 = GPIO_PB2,
    UART_CTS_PC4 = GPIO_PC4,
    UART_CTS_PD1 = GPIO_PD1,
} UART_CtsPinDef;

/**
 *  @brief  Define UART RTS pin : A4  B3 B6 C0
 */
typedef enum {
    UART_RTS_PA4 = GPIO_PA4,
    UART_RTS_PB3 = GPIO_PB3,
    UART_RTS_PB6 = GPIO_PB6,
    UART_RTS_PC0 = GPIO_PC0,
} UART_RtsPinDef;

//use this index to cycle the four register of uart. this index should be reset to 0,when send data after system wakeup.
extern unsigned char uart_TxIndex;

/**
 * @brief     This function serves to indicate Tx state. 
 * 			  Before entering power-saving mode,you need to call this function in order to ensure that the data sent is completed
 * @param[in] none.
 * @return    the state of Tx 0:Tx done 1:not.
 */
static inline unsigned char uart_tx_is_busy(void)
{
    return ((reg_uart_status1 & FLD_UART_TX_DONE) ? 0 : 1);
}

/**
 * @brief     This function is used to set the 'uart_TxIndex' to 0.
 *			  After wakeup from power-saving mode, you must call this function before sending the data.
 * @param[in] none.
 * @return    none.
 */
static inline void uart_ndma_clear_tx_index(void)
{
    uart_TxIndex=0;
}

/**
 * @brief     This function resets the UART module.
 * @param[in] none
 * @return    none
 * @note      After calling the uart_reset interface, uart_ndma_clear_tx_index and uart_ndma_clear_rx_index must be called to clear the read/write pointer,
 *            after the uart_reset interface is invoked, the hardware read and write Pointers are cleared to zero,therefore the software read and write Pointers are cleared to ensure logical correctness.
 */
static inline void uart_reset(void)
{
    reg_rst0 |= FLD_RST0_UART;
    reg_rst0 &= (~FLD_RST0_UART);
}

/**
 * @brief     This function serves to clear tx done.
 * @param[in] none
 * @return    none
 */
static inline void uart_clr_tx_done(void)
{
    reg_uart_state = BIT(7);
}

/**
 * @brief      	This function is used to enable the rtx function.
 * @return     	none.
 */
static inline void uart_rtx_en(void)
{
    reg_uart_rx_timeout1 |= FLD_UART_P7816_EN;
}

/**
 * @brief     This function serves to set the mask of uart_txdone
 * @param[in] none
 * @return    none
 */
static inline void uart_mask_tx_done_irq_enable(void)
{
    reg_uart_rx_timeout1 |= FLD_UART_MASK_TXDONE_IRQ;
    uart_clr_tx_done();//In order to use the tx_done irq, change its status from 1 to 0,otherwise it will make an interrupt after initialization.
}

/**
 * @brief      	This function initializes the UART module.
 * @param[in]  	g_uart_div  -  uart clock divider
 * @param[in]  	g_bwpc      -  bitwidth, should be set to larger than
 *
 *  			sys_clk      baud rate   g_uart_div         g_bwpc
 *
 *	  	  	  	16Mhz        9600          118   			 13
 *           	 	 	 	 19200         118     			 6
 *          	 	 	 	 115200         9       		 13
 *
 *	  	  	  	24Mhz        9600          249       		 9
 *           	 	 	 	 19200		   124               9
 *           	 	 	 	 115200         12    			 15
 *
 *	  	  	  	32Mhz        9600          235       		 13
 *           	 	 	 	 19200		   235               6
 *           	 	 	 	 115200         18    			 13
 *
 *	  	  	  	48Mhz        9600          499       		 9
 *           	 	 	 	 19200		   249               9
 *           	 	 	 	 115200         25    			 15
 *
 * @param[in]  Parity      - selected parity type for UART interface
 * @param[in]  StopBit     - selected length of stop bit for UART interface
 * @return     none
 */
extern void uart_init(unsigned short g_uart_div, unsigned char g_bwpc, UART_ParityTypeDef Parity, UART_StopBitTypeDef StopBit);

/**
 * @brief      This function initializes the UART module.
 * @param[in]  Baudrate  	- uart baud rate
 * @param[in]  System_clock - clock of system
 * @param[in]  Parity      	- selected parity type for UART interface
 * @param[in]  StopBit     	- selected length of stop bit for UART interface
 * @return     none
 */
extern void uart_init_baudrate(unsigned int Baudrate,unsigned int System_clock , UART_ParityTypeDef Parity, UART_StopBitTypeDef StopBit);

/**
 * @brief     enable uart DMA mode
 * @param[in] none
 * @return    none
 */
extern void uart_dma_enable(unsigned char rx_dma_en, unsigned char tx_dma_en);

/**
 * @brief     config the irq of uart tx and rx
 * @param[in] rx_irq_en - 1:enable rx irq. 0:disable rx irq
 * @param[in] tx_irq_en - 1:enable tx irq. 0:disable tx irq
 *                        (In general, nodma does not use this interrupt,is sent in polling mode, uart_tx_is_busy() is used to determine whether the sending is complete)
 * @return    none
 */
extern void uart_irq_enable(unsigned char rx_irq_en, unsigned char tx_irq_en);

/**
 * @brief     uart send data function with not DMA method.
 *            variable uart_TxIndex,it must cycle the four registers 0x90 0x91 0x92 0x93 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] uartData - the data to be send.
 * @return    none
 */
extern void uart_ndma_send_byte(unsigned char uartData);

/**
 * @brief     uart read data function with not DMA method.
 *            variable uart_RxIndex,it must cycle the four registers 0x90 0x91 0x92 0x93 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] none.
 * @return    data received.
 */
extern volatile unsigned char uart_ndma_read_byte(void);

//use this index to cycle the four register of uart. this index should be reset to 0,when send data after system wakeup.
extern unsigned char uart_RxIndex;

/**
 * @brief     This function is used to set the 'uart_RxIndex' to 0.
 *			  After wakeup from power-saving mode or uart_reset, you must call this function before read data.
 * @param[in] none.
 * @return    none.
 */
static inline void uart_ndma_clear_rx_index(void)
{
    uart_RxIndex=0;
}

/**
 * @brief     configure the trigger level setting the rx_irq and tx_irq interrupt
 * @param[in] rx_level - rx_irq trigger level value.When the number of rxfifo is greater than or equal to the rx_level, an interrupt is generated, and the interrupt flag is automatically cleared.
 * @param[in] tx_level - tx_irq trigger level value.When the number of txfifo is less than or equal to the tx_level, an interrupt is generated and the interrupt flag is automatically cleared.
 * @return    none
 * @note      Since there is no rxdone interrupt under nodma:
 *           -# if the length of the received data is not known, rx_level should be set to 1;
 *           -# if the length of the received data is known, rx_level should be set to less than 8 and an integer multiple of the received length;
 */
extern void uart_ndma_irq_triglevel(unsigned char rx_level, unsigned char tx_level);

/**
 * @brief     get the status of uart irq.
 * @param[in] none
 * @return    0: not uart irq ;
 *            not 0: indicate tx or rx irq
 */
extern unsigned char uart_ndmairq_get(void);

/**
 * @brief     Send an amount of data in DMA mode.
 * @param[in] Addr   - Pointer to data buffer. It must be 4-bytes aligned address,
 *                     The first four bytes of addr store the send length,the send length can only send (4079-4) bytes one time at most.
 * @return    none
 * @note      If you want to use uart DMA mode to send data, it is recommended to use this function.
 *            This function just triggers the sending action, you can use interrupt or polling with the FLD_UART_TX_DONE flag to judge whether the sending is complete. 
 *            After the current packet has been sent, this FLD_UART_TX_DONE will be set to 1, and FLD_UART_TX_DONE interrupt can be generated. 
 *			  If you use interrupt mode, you need to call uart_clr_tx_done() in the interrupt processing function, uart_clr_tx_done() will set FLD_UART_TX_DONE to 0.
 */
extern void uart_send_dma(unsigned char *Addr);

/**
 * @brief     This function is saved for compatibility with other SDK and isn't be used in driver sdk.Because it has the following problems:
 *			  You can't use this function if you open FLD_UART_TX_DONE irq,This function can only be used in polling method.
 *	          There may be a risk of data loss under certain usage conditions.
 *			  It will first check whether the last packet has been sent, if it is checked that the last packet has been sent, 
 *			  it will trigger the sending, otherwise it will not send.
 *		
 * @param[in] Addr - pointer to the buffer containing data need to send
 * @return    1: DMA triggered successfully
 *            0: UART busy : last packet not send over,you can't start to send the current packet data
 *
 * @note      DMA can only send (4079-4) bytes one time at most.
 *			  
 */
extern unsigned char uart_dma_send(unsigned char *Addr);

/**
 * @brief     uart send data function, this  function tell the DMA to get data from the RAM and start
 *            the DMA transmission
 * @param[in] byte - single byte data need to send
 * @return    1: send success ;
 *            0: DMA busy
 */
extern unsigned char uart_send_byte(unsigned char byte);

/**
 * @brief     Receive an amount of data in DMA mode.
 * @param[in] RecvAddr - Pointer to data buffer, it must be 4-bytes aligned.
 * @param[in] RecvBufLen - Length of DMA in bytes, it must be multiple of 16,the maximum value can be up to 4079,
 *                         RecvBufLen contains the first four bytes to indicate the received length,so uart packet length needs to be no larger than (recBuffLen - 4).
 * @return    none
 * @note      -# If the dma receive length reaches the set length, the uart is still receiving data, no rxtimeout is generated,
 *               the dma will continue to receive, but no buff overflow occurs, and the loopback receive overwrites the received data.
 */
extern void uart_recbuff_init(unsigned char *RecvAddr, unsigned short RecvBufLen);

/**
 * @brief     This function determines whether parity error occurs once a packet arrives.
 * @param[in] none
 * @return    1: parity error ;
 *            0: no parity error
 */
extern unsigned char uart_is_parity_error(void);

/**
 * @brief     This function clears parity error status once when it occurs.
 * @param[in] none
 * @return    none
 *
 * Note:
 *(1)DMA mode
 * RX FIFO will also be cleared when parity error flag is cleared .
 *(2)NON-DMA mode
 * When parity error occurs, clear parity error flag after UART receives the data.
 * Cycle the four registers (0x90 0x91 0x92 0x93) from register "0x90" to get data when UART receives the data next time.
 */
extern void uart_clear_parity_error(void);

/**
 * @brief     UART hardware flow control configuration. Configure RTS pin.
 * @param[in] Enable - enable or disable RTS function.
 * @param[in] Mode - set the mode of RTS(auto or manual).
 * @param[in] Thresh - threshold of trig RTS pin's level toggle(only for auto mode),
 *                     it means the number of bytes that has arrived in Rx buf.
 * @param[in] Invert - whether invert the output of RTS pin(only for auto mode)
 * @param[in] pin   - RTS pin select,it can be GPIO_PA4/GPIO_PB3/GPIO_PB6/GPIO_PC0.
 * @return    none
 */
extern void uart_set_rts(unsigned char Enable, UART_RTSModeTypeDef Mode, unsigned char Thresh, unsigned char Invert, UART_RtsPinDef pin);

/**
 * @brief     This function sets the RTS pin's level manually
 * @param[in] Polarity - set the output of RTS pin(only for manual mode)
 * @return    none
 */
extern void uart_set_rts_level(unsigned char Polarity);

/**
 * @brief      UART hardware flow control configuration. Configure CTS pin.
 * @param[in]  Enable - enable or disable CTS function.
 * @param[in]  Select - when CTS's input equals to select, tx will be stopped
 * @param[in]  pin   - CTS pin select,it can be GPIO_PA3/GPIO_PB2/GPIO_PC4/GPIO_PD1.
 * @return     none
 */
extern void uart_set_cts(unsigned char Enable, unsigned char Select, UART_CtsPinDef pin);

/**
* @brief      This function serves to select pin for UART module.
* @param[in]  tx_pin   - the pin to send data.
* @param[in]  rx_pin   - the pin to receive data.
* @return     none
*/
extern void uart_gpio_set(UART_TxPinDef tx_pin, UART_RxPinDef rx_pin);

/**
 * @brief   This function enables error data irq of UART module
 * @param[in] none
 * @return    none
 */
extern void uart_mask_error_irq_enable(void);

/**
* @brief      This function serves to set rtx pin for UART module.
* @param[in]  rx_pin  - the rtx pin need to set.
* @return     none
*/
extern void uart_set_rtx_pin(UART_RTxPinDef rtx_pin);

#endif

