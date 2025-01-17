/********************************************************************************************************
 * @file    uart.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page UART
 *
 *  Introduction
 *  ===============
 *  supports two uart: uart0~ uart1.
 *  -# support nodma/dma
 *  -# support cts/rts
 *  -# support s7816
 *
 *  API Reference
 *  ===============
 *  Header File: uart.h
 *
 *  How to use this driver
 *  ==============
 - The uart two working modes nodma/dma,can be used as follows:
    -# UART Initialization and Configuration
         -# dma/nodma:
             - To prevent the uart module from storing history information, before use call uart_hw_fsm_reset API;
             - Initializes the tx/rx pin by uart_set_pin API;
             - Configure the baud rate/stop bit/ parity by uart_cal_div_and_bwpc/uart_init API;
         -# dma
             - Configure rx_timeout by uart_set_rx_timeout;
             - The default value of tx_done is 1, which needs to be cleared,by uart_clr_tx_done API;
             - dma initial configuration by uart_set_tx_dma_config/uart_set_rx_dma_config API;
    -# UART interrupts handling
       -# nodma:data is received via rx_irq interrupt,and err receive detection.
             - rx_irq interrupt:when the number of rxfifo reaches the threshold uart_rx_irq_trig_level, the rx_irq is interrupted,
               configured by uart_rx_irq_trig_level/uart_set_irq_mask(UART_RX_IRQ_MASK) API;
             - detect the UART receives data incorrectly(such as a parity error or a stop bit error),configured by uart_set_irq_mask(UART_ERR_IRQ_MASK) API;
                 - If err occurs during the receiving process, perform the following operations: 
                    -# uart_hw_fsm_reset();
                    -# uart_clr_rx_index();
                    -# clear the data received in ram buff;
             - uart module interrupt enable and total interrupt enable by plic_interrupt_enable/core_interrupt_enable API;
       -# dma:use tx_done and rx_done(or dma tc_mask) to check whether the sending and receiving are complete,and err receive detection when rx_done.
             - tx_done:use the tx_done of the uart module,use uart_set_irq_mask(UART_TXDONE_MASK);
             - rx_done:use rx_done of uart module rx_done,use uart_set_irq_mask(UART_RXDONE_MASK) (When the length of the send is not known):
                  - After rxdone is generated and before configuring the next dma, the rxfifo hardware pointer must be set to 0 (uart_hw_fsm_reset())
                    in order to prevent the next dma from working properly due to the rxfifo pointer not being in the default state(When the sending length is greater than the receiving length).
                  - When the send length is less than the receive length, although the rxdone interrupt is generated, the dma has not reached the configured length, and the dma is still working.
                    When configuring the next dma, dma_chn_dis() needs to be configured before the next dma(this action has been processed in uart_receive_dma).
             - tc_mask:use tc_mask of dma module,use dma_set_irq_mask() : TC_MASK (If the transmission length is fixed):
                  - A dma interrupt can only occur if the receive length is equal to the length configured by the dma.
                  - If the dma stops working and reconfigures it, perform the following operations:
                      -# uart_hw_fsm_reset();
                      -# uart_receive_dma();(dma_chn_dis has been implemented in uart_receive_dma())
             - If err is displayed during dma receiving, the recommended action is as follows:
                      -# uart_hw_fsm_reset();
                      -# uart_receive_dma();(dma_chn_dis has been implemented in uart_receive_dma())
             - uart module interrupt or dma module interrupt enable and total interrupt by plic_interrupt_enable/core_interrupt_enable API;
    -# UART enter suspend sleep before and after handling
        -# nodma_tx:
              - before no dma tx goes to suspend sleep, it is necessary to judge the uart busy bit to ensure the completion of UART data transmission.
              - after suspend sleep wake up, perform the following operations: 
                      -# uart_clr_tx_index();
                      -# clear the send data in ram buff;
        -# nodma_rx
              - If go to suspend sleep during no dma rx, the recommended action when waking up is as follows: 
                      -# uart_clr_rx_index();
                      -# clear the data received in ram buff;
        -# dma:
              - If go to suspend sleep during dma sending, the recommended action when waking up is as follows:
                      -# uart_send_dma();(dma_chn_dis has been implemented in uart_send_dma())
              - If go to suspend sleep during dma receiving, the recommended action when waking up is as follows:
                      -# uart_receive_dma();(dma_chn_dis has been implemented in uart_receive_dma())
    -# UART TX and RX
         -# nodma_tx
            - It can be sent in byte/half word/word polling, use uart_send_byte/uart_send_hword/uart_send_word API;
         -# nodma_rx
            - data is received via rx_irq interrupt,the data is read in the interrupt by uart_read_byte API;
         -# dma_tx
            - send data by uart_send_dma API;
         -# dma_rx
            - receive data by uart_receive_dma API;
    -# Note:
         - the points to note when receiving nodma:
            - Since there is no rxdone interrupt under nodma:
                - if the length of the received data is not known, rx_level should be set to 1;
                - if the length of the received data is known, rx_level should be set to less than 8 and an integer multiple of the received length;
            - rx_irq interrupt processing: Use uart_get_rxfifo_num() to determine the number of FIFOs and use uart_read_byte() to read all data in fifo;
            - The depth size of the uart fifo is 8. If the time before and after entering the rx_irq interrupt exceeds the time of receiving 8 bytes, the fifo pointer may be disturbed, resulting in abnormal received data.
              You can determine whether uart_get_rxfifo_num() is greater than 8 as an exception,If this exception occurs, it is recommended to use dma mode to receive.
         - the points to note when receiving dma:
            - the receive length needs to be calculated manually by uart_get_dma_rev_data_len API;
            - Advantages of this method: prevents rx_buff from crossing boundaries;
            - Possible problems:
                - the receive length cannot be greater than the DMA set length,otherwise excess data will be discarded.
                - In an application, there may be a high priority interrupt source that interrupts the uart rx_done interrupt processing sequence,
                  in interruption period,if the next data is received to the fifo, when the re-entry of the UART RX_DONE interrupt,resulting in the error in computing the received length of the current data.
                - The two packets of data are very close to each other, but the rx_done signal of the previous data has also been generated. Before the rx_done interrupt flag and rx_fifo software are cleared,
                  the next data has been transferred, which leads to the error of clearing.
            - When using DMA to receive data, if rx_done occurs and there's data in rx fifo, rx fifo needs to be cleared for the next dma transmission. 
              in B91, clearing rxbuff interrupt status won't work, therefore requiring uart_hw_fsm_reset() function to do this operation,
              But using uart_hw_fsm_reset() function might break dma tx transmission, potentially causing data loss.
 - The UART flow control CTS/RTS
      -# CTS(when the cts pin receives an active level, it stops sending data)
         - Configure the cts pin and polarity,by uart_cts_config API;
         - enable cts,by uart_set_cts_en;
      -# RTS(In automatic mode,when the condition is reached, the rts active level is activated, and in manual mode, the rts level can be manually controlled)
         - Configure the rts pin and polarity,by uart_rts_config API;
         - enable rts,by uart_set_rts_en;
         - the rts has two modes:manual and auto
            - manual: can change the RTS pin's level manually,the related interfaces are as follows: uart_rts_manual_mode/uart_set_rts_level API;
            - auto:When the set conditions are met, rts is activated,there are two conditions as follows:
                   - When the number of rxfifo reaches the set threshold, rts is activated,when rx_fifo is less than the set threshold then rts will go down;
                     the related interfaces are as follows:uart_rts_auto_mode/uart_rts_trig_level_auto_mode API;
                   - Generates an rx_done signal,rts is activated,When rx_done is cleared, the rts signal automatically goes down,
                     the related interfaces are as follows:uart_rts_auto_mode API;
 - Support S7816
    -# S7816(used to communicate with the s7816,for details, see driver s7816)
        - This is just to talk specifically about the rtx function:
           - Configure the rtx pin and enable,by uart_set_rtx_pin/uart_rtx_en API;
           - this pin can be used as either tx or rx,it is the rx function by default,When there is data in tx_fifo and the interface uart_rtx_pin_tx_trig is called,
             it is converted to tx function until tx_fifo is empty and converted to rx.
 */
#ifndef UART_H_
#define UART_H_

#include "reg_include/register.h"
#include "gpio.h"
#include "dma.h"
#include "timer.h"

extern unsigned char uart_rx_byte_index[2];
extern unsigned char uart_tx_byte_index[2];

#define uart_rtx_pin_tx_trig(uart_num) uart_clr_tx_done(uart_num)

//for compatibility
#define uart_reset uart_hw_fsm_reset

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 *  @brief  Define parity type
 */
typedef enum
{
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN,
    UART_PARITY_ODD,
} uart_parity_e;

/**
 *  @brief  Define UART chn
 */
typedef enum
{
    UART0 = 0,
    UART1,
} uart_num_e;

/**
 *  @brief  Define mul bits
 */
typedef enum
{
    UART_BW_MUL1 = 0,
    UART_BW_MUL2 = 1,
    UART_BW_MUL3 = 2,
    UART_BW_MUL4 = 3,
} uart_timeout_mul_e;

/**
 *  @brief  Define the length of stop bit
 */
typedef enum
{
    UART_STOP_BIT_ONE          = 0,
    UART_STOP_BIT_ONE_DOT_FIVE = BIT(4),
    UART_STOP_BIT_TWO          = BIT(5),
} uart_stop_bit_e;

/**
 *  @brief  Define UART RTS mode
 */
typedef enum
{
    UART_RTS_MODE_AUTO = 0,
    UART_RTS_MODE_MANUAL,
} uart_rts_mode_e;

/**
 *  @brief  Define UART CTS pin : UART0(PA1 PB6 PD0), UART1(PC4 PD4 PE1)
 */
typedef enum
{
    UART0_CTS_PA1 = GPIO_PA1,
    UART0_CTS_PB6 = GPIO_PB6,
    UART0_CTS_PD0 = GPIO_PD0,

    UART1_CTS_PC4 = GPIO_PC4,
    UART1_CTS_PD4 = GPIO_PD4,
    UART1_CTS_PE1 = GPIO_PE1,
} uart_cts_pin_e;

/**
 *  @brief  Define UART RTS pin : UART0(PA2 PB4 PD1), UART1(PC5 PD5 PE3)
 */
typedef enum
{
    UART0_RTS_PA2 = GPIO_PA2,
    UART0_RTS_PB4 = GPIO_PB4,
    UART0_RTS_PD1 = GPIO_PD1,

    UART1_RTS_PC5 = GPIO_PC5,
    UART1_RTS_PD5 = GPIO_PD5,
    UART1_RTS_PE3 = GPIO_PE3,
} uart_rts_pin_e;

/**
 *  @brief  Define UART TX pin : UART0(PA3 PB2 PD2), UART1(PC6 PD6 PE0)
 */
typedef enum
{
    UART0_TX_PA3 = GPIO_PA3,
    UART0_TX_PB2 = GPIO_PB2,
    UART0_TX_PD2 = GPIO_PD2,

    UART1_TX_PC6 = GPIO_PC6,
    UART1_TX_PD6 = GPIO_PD6,
    UART1_TX_PE0 = GPIO_PE0,

    UART_TX_NONE_PIN = GPIO_NONE_PIN,
} uart_tx_pin_e;

/**
 *  @brief  Define UART RX pin : UART0(PA4 PB3 PD3), UART1(PC7 PD7 PE2)
 */
typedef enum
{
    UART0_RX_PA4 = GPIO_PA4,
    UART0_RX_PB3 = GPIO_PB3,
    UART0_RX_PD3 = GPIO_PD3,

    UART1_RX_PC7 = GPIO_PC7,
    UART1_RX_PD7 = GPIO_PD7,
    UART1_RX_PE2 = GPIO_PE2,

    UART_RX_NONE_PIN = GPIO_NONE_PIN,
} uart_rx_pin_e;

/**
 *  @brief  Define UART IRQ MASK.The enumeration variable is just a index, and actually needs to be operated registers behind.
 */
typedef enum
{
    UART_RX_IRQ_MASK  = BIT(0), //reg_uart_ctrl0(uart_num)       BIT(6)
    UART_TX_IRQ_MASK  = BIT(1), //reg_uart_ctrl0(uart_num)       BIT(7)
    UART_RXDONE_MASK  = BIT(2), //reg_uart_rx_timeout1(uart_num) BIT(2)
    UART_TXDONE_MASK  = BIT(3), //reg_uart_rx_timeout1(uart_num) BIT(6)
    UART_ERR_IRQ_MASK = BIT(4), //reg_uart_rx_timeout1(uart_num) BIT(7)
} uart_irq_mask_e;

/**
 *  @brief  Define UART IRQ BIT STATUS FOR GET
 *  -# UART_RXBUF_IRQ_STATUS:When the number of rxfifo reaches the set threshold(uart_rx_irq_trig_level), an interrupt is generated, and the interrupt flag is automatically cleared;
 *  -# UART_TXBUF_IRQ_STATUS:When the number of txfifo is less than or equal to the set threshold(uart_tx_irq_trig_level), an interrupt is generated and the interrupt flag is automatically cleared;
 *  -# UART_RXDONE:no data is received in rx_timeout, rx_done is generated,the interrupt flag must be manually cleared,this interrupt is only used in dma mode;
 *  -# UART_TXDONE:when there is no data in the tx_fifo, tx_done is generated, and the interrupt flag bit needs to be manually cleared;
 *  -# UART_RX_ERR:when the UART receives data incorrectly(such as a parity error or a stop bit error), the interrupt is generated,the interrupt flag bit needs to be manually cleared;
 */
typedef enum
{
    UART_RX_ERR           = BIT(7),
    UART_TXDONE           = BIT(0),
    UART_TXBUF_IRQ_STATUS = BIT(1),
    UART_RXDONE           = BIT(2),
    UART_RXBUF_IRQ_STATUS = BIT(3),
} uart_irq_status_get_e;

/**
 *  @brief  Define UART IRQ BIT STATUS FOR CLR
 */
typedef enum
{
    UART_CLR_RX = BIT(6),
    UART_CLR_TX = BIT(7),
} uart_irq_status_clr_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/**
 * @brief     Get the rxfifo cnt,when data enters rxfifo, the rxfifo cnt increases,When reading data from rx_fifo, rxfifo cnt decays.
 * @param[in] uart_num - UART0/UART1.
 * @return    none
 */
static inline unsigned char uart_get_rxfifo_num(uart_num_e uart_num)
{
    return reg_uart_buf_cnt(uart_num) & FLD_UART_RX_BUF_CNT;
}

/**
 * @brief     Get the txfifo cnt,tx_fifo cnt decreases when data is sent from tx_fifo, and tx_fifo cnt increases when data is written to tx_fifo.
 * @param[in] uart_num - UART0/UART1.
 * @return    none
 */
static inline unsigned char uart_get_txfifo_num(uart_num_e uart_num)
{
    return (reg_uart_buf_cnt(uart_num) & FLD_UART_TX_BUF_CNT) >> 4;
}

/**
 * @brief     uart finite state machine reset(the configuration register is still there and does not need to be reconfigured),
 *            For compatibility define uart_reset uart_hw_fms_reset, uart_hw_fms_reset is used when the driver is invoked (no matter at the driver layer or demo layer),
 *            before using UART, it is needed to call uart_hw_fsm_reset() to avoid affecting the use of UART.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 * @note -
 *            this function will clear rx and tx status and fifo.
 */
static inline void uart_hw_fsm_reset(uart_num_e uart_num)
{
    /**
      In B91, tx_done is 1 by default, after uart reset(write 0, then write 1) write 0,UART_TXDONE will be restored to its default value,
      if UART_TX_IRQ_MASK is turned on in advance, it will enter interrupt,in the interrupt, there is the action of clearing UART_TXDONE, but after the clear, immediately becomes 1,
      out of the interrupt, and immediately in the interrupt, and so on loop, resulting in the feeling that the program did not go down.
     */
    unsigned char tx_mask_flag = 0;
    if (reg_uart_rx_timeout1(uart_num) & FLD_UART_MASK_TXDONE) {
        tx_mask_flag = 1;
        reg_uart_rx_timeout1(uart_num) &= ~FLD_UART_MASK_TXDONE;
    }
    reg_rst0 &= (~((uart_num) ? FLD_RST0_UART1 : FLD_RST0_UART0));
    reg_rst0 |= ((uart_num) ? FLD_RST0_UART1 : FLD_RST0_UART0);
    reg_uart_state(uart_num) |= FLD_UART_CLR_TXDONE;
    if (tx_mask_flag == 1) {
        reg_uart_rx_timeout1(uart_num) |= FLD_UART_MASK_TXDONE;
    }
}

/**
 * @brief     Enable the clock of UART module.
 * @param[in] uart_num - UART0/UART1.
 * @return    none
 */
static inline void uart_clk_en(uart_num_e uart_num)
{
    reg_clk_en0 |= ((uart_num) ? FLD_CLK0_UART1_EN : FLD_CLK0_UART0_EN);
}

/**
  * @brief      Initializes the UART module.
  * @param[in]  uart_num    - UART0 or UART1.
  * @param[in]  div         - uart clock divider.
  * @param[in]  bwpc        - bitwidth, should be set to larger than 2.
  * @param[in]  parity      - selected parity type for UART interface.
  * @param[in]  stop_bit    - selected length of stop bit for UART interface.
  * @return     none
  * @note
  * -# A few simple examples of sys_clk/baud rate/div/bwpc correspondence:
   @verbatim
               sys_clk      baudrate   g_uart_div         g_bwpc

               16Mhz        9600          118                13
                            19200         118                 6
                            115200          9                13

               24Mhz        9600          249                 9
                            19200         124                 9
                            115200         12                15

               32Mhz        9600          235                13
                            19200         235                 6
                            115200         17                13

               48Mhz        9600          499                 9
                            19200         249                 9
                            115200         25                15
   @endverbatim
    -# uart_init() set the baud rate by the div and bwpc of the uart_cal_div_and_bwpc, some applications have higher timing requirements,
       can first calculate the div and bwpc, and then just call uart_init.
 */
void uart_init(uart_num_e uart_num, unsigned short div, unsigned char bwpc, uart_parity_e parity, uart_stop_bit_e stop_bit);

/**
 * @brief       Calculate the best bwpc(bit width).
 * @param[in]   baudrate - baud rate of UART.
 * @param[in]   pclk     - pclk.
 * @param[out]  div      - uart clock divider.
 * @param[out]  bwpc     - bitwidth, should be set to larger than 2,range[3-15].
 * @return      none
 * @note        BaudRate*(div+1)*(bwpc+1) = pclk.
 * <p>
 *              simplify the expression: div*bwpc =  constant(z).
 * <p>
 *              bwpc range from 3 to 15.so loop and get the minimum one decimal point.
 * @note
     -# The maximum baud rate depends on the hardware environment (such as cable length, etc.) and pclk/cclk/hclk:
         - pclk is the main factor affecting the upper baud rate of UART
         - cclk and pclk affect interrupt processing times(increase the frequency of cclk will increase the maximum baud rate of NDMA, but it has no obvious effect on the maximum baud rate of DMA)
     -# The maximum baud rate must meet two testing conditions: 
         - proper parsing by the logic analyzer
         - successful communication on the development board
     -# Note on the actual use of the maximum baud rate:
         - if only communication on the development board is considered, the baud rate can be set higher
         - setting a significantly higher baud rate may result in a deviation between the set and actual baud rates, leading to incorrect parsing by the logic analyzer and possible communication failures with other devices
     -# Using the B91 development board, the test results:
         - CCLK_16M_HCLK_16M_PCLK_16M: in nodma,the maximum speed is 2 MHz; in dma,the maximum speed is 2 MHz;
         - CCLK_24M_HCLK_24M_PCLK_24M: in nodma,the maximum speed is 3 MHz; in dma,the maximum speed is 3 MHz;
         - CCLK_32M_HCLK_32M_PCLK_16M: in nodma,the maximum speed is 2 MHz; in dma,the maximum speed is 2 MHz;
         - CCLK_48M_HCLK_48M_PCLK_24M: in nodma,the maximum speed is 3 MHz; in dma,the maximum speed is 3 MHz;
         - CCLK_96M_HCLK_48M_PCLK_24M: in nodma,the maximum speed is 3 MHz; in dma,the maximum speed is 3 MHz;
 */
void uart_cal_div_and_bwpc(unsigned int baudrate, unsigned int pclk, unsigned short *div, unsigned char *bwpc);

/**
 * @brief    Set rx_timeout.
   @verbatim
       The effect:
         - When no data is received within the rx_timeout period, that is rx timeout, the UART_RXDONE interrupt is generated.
         - The UART_RXDONE interrupt is required to process the remaining data below the threshold(the DMA Operation threshold is fixed at 4,
            the NDMA threshold can be configured through uart_rx_irq_trig_level)
       How to set:
         rx_timeout =(1/baudrate)* bit_cnt* mul(s)
         note: ((bwpc+1) * bit_cnt)<=0xff, since this is a one byte register.
   @endverbatim
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] bwpc     - bitwidth,bwpc range from 3 to 15,it is calculated by the interface uart_cal_div_and_bwpc.
 * @param[in] bit_cnt  - bit number,conditions that need to be met:
 *                         - for example, if transferring one bytes (1start bit+8bits data+1 priority bit+2stop bits) total 12 bits,then set it to at least 12;
 *                         - ((bwpc+1) * bit_cnt)<=0xff;
 * @param[in] mul      - mul.
 * @return    none
 */
void uart_set_rx_timeout(uart_num_e uart_num, unsigned char bwpc, unsigned char bit_cnt, uart_timeout_mul_e mul);

/**
 * @brief     Configure the trigger level of the UART_RXBUF_IRQ_STATUS interrupt,when the number of rx_fifo is greater than or equal to the trigger level, UART_RXBUF_IRQ_STATUS interrupt rises.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] rx_level - the trigger level,the range is less than 8,since no UART_RXDONE interrupted under nodma,only be set to 1.
 *                       Because the receive interrupt is triggered too frequently, if the data is received too fast (baud rate is too high), it may be
                         the previous data has not been received, and the next data has arrived. If this happens, it can be properly reduced,Low baud rate to solve.
 * @return    none
 * @note      This interface is only used in no_dma mode.
 */
static inline void uart_rx_irq_trig_level(uart_num_e uart_num, unsigned char rx_level)
{
    reg_uart_ctrl3(uart_num) = (reg_uart_ctrl3(uart_num) & (~FLD_UART_RX_IRQ_TRIQ_LEV)) | (rx_level & 0x0f);
}

/**
 * @brief     Config the trigger level of the UART_TXBUF_IRQ_STATUS,when the number of tx_fifo data is less than or equal to the trigger level, UART_TXBUF_IRQ_STATUS interrupt rises.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] tx_level - the trigger level,the range is less than 8.
 * @return    none
 * @note      This interface is only used in no_dma mode.
 */
static inline void uart_tx_irq_trig_level(uart_num_e uart_num, unsigned char tx_level)
{
    reg_uart_ctrl3(uart_num) = (reg_uart_ctrl3(uart_num) & (~FLD_UART_TX_IRQ_TRIQ_LEV)) | (tx_level << 4);
}

/**
  * @brief     Send uart data by byte in no_dma mode.
  * @param[in] uart_num - UART0 or UART1.
  * @param[in] tx_data  - the data to be send.
  * @return    none
  */
void uart_send_byte(uart_num_e uart_num, unsigned char tx_data);

/**
 * @brief     Receive uart data by byte in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 */
unsigned char uart_read_byte(uart_num_e uart_num);
/**
 * @brief     Judge if the transmission of uart is done.
 * @param[in] uart_num - UART0 or UART1.
 * @return    0:tx is done     1:tx isn't done
 */
unsigned char uart_tx_is_busy(uart_num_e uart_num);
/**
 * @brief     Send uart data by halfword in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] data  - the data to be send.
 * @return    none
 */
void uart_send_hword(uart_num_e uart_num, unsigned short data);

/**
 * @brief     Send uart data by word in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] data - the data to be send.
 * @return    none
 */
void uart_send_word(uart_num_e uart_num, unsigned int data);


/**
 * @brief     Sets the RTS pin's level manually,this function is used in combination with uart_rts_manual_mode.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] polarity - set the output of RTS pin.
 * @return    none
 */
void uart_set_rts_level(uart_num_e uart_num, unsigned char polarity);

/**
 *  @brief      Set pin for UART cts function,the pin connection mode:CTS<->RTS.
 *  @param[in]  cts_pin -To set cts pin.
 *  @return     none
 */
void uart_set_cts_pin(uart_cts_pin_e cts_pin);

/**
 *  @brief      Set pin for UART rts function,the pin connection mode:RTS<->CTS.
 *  @param[in]  rts_pin - To set rts pin.
 *  @return     none
 */
void uart_set_rts_pin(uart_rts_pin_e rts_pin);

/**
* @brief      Select pin for UART module,the pin connection mode:TX<->RX RX<->TX.
* @param[in]  tx_pin   - the pin to send data.
* @param[in]  rx_pin   - the pin to receive data.
* @return     none
*/
void uart_set_pin(uart_tx_pin_e tx_pin, uart_rx_pin_e rx_pin);

/**
* @brief      Set rtx pin for UART module,this pin can be used as either tx or rx,it is the rx function by default,
*             When there is data in tx_fifo and the interface uart_rtx_pin_tx_trig is called, it is converted to tx function until tx_fifo is empty and converted to rx.
* @param[in]  rtx_pin  - the rtx pin need to set.
* @return     none
*/
void uart_set_rtx_pin(uart_rx_pin_e rtx_pin);


/**
 * @brief       Send an amount of data in DMA mode
 * @param[in]   uart_num - uart channel
 * @param[in]   addr     - Pointer to data buffer. It must be 4-bytes aligned address
 * @param[in]   len      - Amount of data to be sent in bytes, range from 1 to 0xFFFFFC
 * @return      1  DMA start send.
 *              0  the length is error.       
 */
unsigned char uart_send_dma(uart_num_e uart_num, unsigned char *addr, unsigned int len);

/**
* @brief     Send an amount of data in NODMA mode
* @param[in] uart_num - UART0 or UART1.
* @param[in] addr     - pointer to the buffer.
* @param[in] len      - NDMA transmission length.
* @return    1
*/
unsigned char uart_send(uart_num_e uart_num, unsigned char *addr, unsigned char len);

/**
 * @brief      Receive an amount of data in DMA mode
 * @param[in]  uart_num - UART0 or UART1.
 * @param[in]  addr     - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *                        and the actual buffer size defined by the user needs to be not smaller than the rev_size, otherwise there may be an out-of-bounds problem.
 * @param[in]  rev_size - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
 * @return     none
 */
void uart_receive_dma(uart_num_e uart_num, unsigned char *addr, unsigned int rev_size);

/**
 * @brief     Get the length of the data that dma received.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] chn      - dma channel.
 * @return    data length.
 */
unsigned int uart_get_dma_rev_data_len(uart_num_e uart_num, dma_chn_e chn);

/**
  * @brief     Configures the uart tx_dma channel control register.
  * @param[in] uart_num - UART0 or UART1.
  * @param[in] chn      - dma channel.
  * @return    none
  * @note      In the case that the DMA transfer is not completed(bit 0 of reg_dma_ctr0(chn): 1-the transmission has not been completed,0-the transmission is completed), re-calling the DMA-related functions may cause problems.
  *            If you must do this, you must perform the following sequence:
  *            1. dma_chn_dis(uart_dma_tx_chn[uart_num]) 2.uart_hw_fsm_reset() 3.uart_send_dma()
  */
void uart_set_tx_dma_config(uart_num_e uart_num, dma_chn_e chn);

/**
 * @brief     Configures uart rx_dma channel control register.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] chn      - dma channel.
 * @return    none
 * @note      In the case that the DMA transfer is not completed(bit 0 of reg_dma_ctr0(chn): 1-the transmission has not been completed,0-the transmission is completed), re-calling the DMA-related functions may cause problems.
 *            If you must do this, you must perform the following sequence:
 *            1. dma_chn_dis(uart_dma_rx_chn[uart_num]) 2.uart_hw_fsm_reset() 3.uart_receive_dma()
 */
void uart_set_rx_dma_config(uart_num_e uart_num, dma_chn_e chn);

/**
 * @brief     Configure the irq of uart.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] mask     - uart irq mask.
 * @return    none
 */
static inline void uart_set_irq_mask(uart_num_e uart_num, uart_irq_mask_e mask)
{
    if ((mask & UART_RX_IRQ_MASK) || (mask & UART_TX_IRQ_MASK)) {
        reg_uart_ctrl0(uart_num) |= (((mask & UART_RX_IRQ_MASK) ? 1 : 0) << 6) | (((mask & UART_TX_IRQ_MASK) ? 1 : 0) << 7);
    }

    if ((mask & UART_RXDONE_MASK) || (mask & UART_TXDONE_MASK) || (mask & UART_ERR_IRQ_MASK)) {
        reg_uart_rx_timeout1(uart_num) |= (((mask & UART_RXDONE_MASK) ? 1 : 0) << 2) | (((mask & UART_TXDONE_MASK) ? 1 : 0) << 6) | (((mask & UART_ERR_IRQ_MASK) ? 1 : 0) << 7);
    }
}

/**
 * @brief     Clear the irq of uart.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] mask     - uart irq mask.
 * @return    none
 */
static inline void uart_clr_irq_mask(uart_num_e uart_num, uart_irq_mask_e mask)
{
    if ((mask & UART_RX_IRQ_MASK) || (mask & UART_TX_IRQ_MASK)) {
        reg_uart_ctrl0(uart_num) &= ~((((mask & UART_RX_IRQ_MASK) ? 1 : 0) << 6) | (((mask & UART_TX_IRQ_MASK) ? 1 : 0) << 7));
    }
    if ((mask & UART_RXDONE_MASK) || (mask & UART_TXDONE_MASK) || (mask & UART_ERR_IRQ_MASK)) {
        reg_uart_rx_timeout1(uart_num) &= ~((((mask & UART_RXDONE_MASK) ? 1 : 0) << 2) | (((mask & UART_TXDONE_MASK) ? 1 : 0) << 6) | (((mask & UART_ERR_IRQ_MASK) ? 1 : 0) << 7));
    }
}

/**
 * @brief     Get the irq status of uart.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] status   - uart irq status.
 * @retval    non-zero   -  the interrupt occurred.
 * @retval    zero  -  the interrupt did not occur.
 */
static inline unsigned int uart_get_irq_status(uart_num_e uart_num, uart_irq_status_get_e status)
{
    if (status == UART_RX_ERR) {
        return (reg_uart_status1(uart_num) & (status));
    } else {
        return (reg_uart_status2(uart_num) & (status));
    }
}

/**
 * @brief     Clear the irq status of uart.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] status - uart irq status.
 * @return    none
 */
static inline void uart_clr_irq_status(uart_num_e uart_num, uart_irq_status_clr_e status)
{
    /**
        [2:0]: rbcnt   --read only
        [3]  : irq_o   --read only
        [6:4]: wbcnt   -- R: wbcnt,   W:[6] write 1 to clear rx
        [7]  : rx_err  -- R: rx_err,  W:[7] write 1 to clear tx
        The status flag bit is write 1 clear 0 and all other bits in this register except the status flag bit are read-only,
        so the correct way to write "=" instead of "|=" can be used here.
        (using "|=" will have the problem of clearing the flag bit by mistake) (added by xianren.yang, confirmed by xuqiang.zhang 20231017)
     */
    reg_uart_status1(uart_num) = (status);
}

/**
 * @brief     Enable uart rts.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 */
static inline void uart_set_rts_en(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) |= FLD_UART_RTS_EN; //enable RTS function
}

/**
 * @brief     Disable uart rts.
 *            The functions are as follows:
 *            -# If uart_set_rts_dis:the threshold -> rts->timeout->UART_RXDONE_IRQ_STATUS interrupt.
              -# If uart_set_rts_en: the threshold -> rts->timeout stop->the UART_RXDONE_IRQ_STATUS interrupt is not generated.
 *            How to use:
 *            -# When the rts function is not in use, this function need to be called, otherwise it will affect the generation of uart rxdone,
 *               this function has been called by uart_init() API.
 *            -# If rts flow control is used, it is recommended to call uart_set_rts_en():
 *               otherwise, when the sender cts pin receives the active level of rts pin, it stops sending data,
 *               If this function is called at this time, no data will be sent, resulting in an interruption in rxdone,
 *               When UART_RXDONE_IRQ_STATUS is cleared, the rx_fifo data will be cleared, causing the rts level to fail.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 */
static inline void uart_set_rts_dis(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_EN); //disable RTS function
}

/**
 * @brief     Enable uart cts.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 */
static inline void uart_set_cts_en(uart_num_e uart_num)
{
    reg_uart_ctrl1(uart_num) |= FLD_UART_TX_CTS_ENABLE; //enable CTS function
}

/**
 * @brief     Disable uart cts.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 */
static inline void uart_set_cts_dis(uart_num_e uart_num)
{
    reg_uart_ctrl1(uart_num) &= (~FLD_UART_TX_CTS_ENABLE); //disable CTS function
}

/**
  * @brief     Configure UART hardware flow CTS.
  * @param[in] uart_num   - UART0 or UART1.
  * @param[in] cts_pin    - cts pin select.
  * @param[in] cts_parity -  1:Active high,when the cts receives an active level, it stops sending data.
  *                          0:Active low
  * @return    none
  */
void uart_cts_config(uart_num_e uart_num, uart_cts_pin_e cts_pin, unsigned char cts_parity);

/**
 * @brief     Configure UART hardware flow RTS.
 * @param[in] uart_num     - UART0 or UART1.
 * @param[in] rts_pin      - RTS pin select.
 * @param[in] rts_parity   - 0: Active high  1: Active low
 *                           in auto mode,when the condition is reached, the rts active level is activated, and in manual mode, the rts level can be manually controlled.
 * @param[in] auto_mode_en - set the mode of RTS(auto or manual).
 * @return    none
 */
void uart_rts_config(uart_num_e uart_num, uart_rts_pin_e rts_pin, unsigned char rts_parity, unsigned char auto_mode_en);

/**
 * @brief     Set uart rts trig level,when the number of rx_fifo reaches the rts trig level, rts is raised.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] level    - the rts trigger level,the range is less than 8.
 * @return    none
 */
static inline void uart_rts_trig_level_auto_mode(uart_num_e uart_num, unsigned char level)
{
    reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_TRIQ_LEV);
    reg_uart_ctrl2(uart_num) |= (level & FLD_UART_RTS_TRIQ_LEV);
}

/**
 * @brief     Set uart rts auto mode,flow control generation condition:
 *            -# rx_fifo number is greater than or equal to the set threshold,when the number of rx_fifo is less than the set threshold, the level automatically becomes invalid;
 *            -# UART_RXDONE signal generation (if uart_rxdone_rts_en enable)),when the UART_RXDONE signal is cleared,the level automatically becomes invalid;
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 */
static inline void uart_rts_auto_mode(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_MANUAL_M);
}

/**
 * @brief     Set uart rts manual mode.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none
 */
static inline void uart_rts_manual_mode(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) |= (FLD_UART_RTS_MANUAL_M);
}

/**
 * @brief     Set the 'uart_rx_byte_index' to 0,'uart_rx_byte_index' is used to synchronize the rxfifo hardware pointer in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none.
 * @note      note the following:
 *            -# After calling the uart reset interface, uart_clr_tx_index and uart_clr_rx_index must be called to clear the read/write pointer,
 *               after the uart reset interface is invoked, the hardware read and write Pointers are cleared to zero.
 *               Therefore, the software read and write Pointers are cleared to ensure logical correctness.
 *            -# After suspend wakes up, you must call uart_clr_tx_index and uart_clr_rx_index to clear read and write pointers,
 *               because after suspend wakes up, the chip is equivalent to performing a uart_hw_fsm_reset,
 *               so the software read and write pointer also needs to be cleared to zero.
 */
static inline void uart_clr_rx_index(uart_num_e uart_num)
{
    uart_rx_byte_index[uart_num] = 0;
}

/**
 * @brief     Set the 'uart_tx_byte_index' to 0,'uart_tx_byte_index' is used to synchronize the txfifo hardware pointer in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
 * @return    none.
 * @note      note the following:
 *            -# After calling the uart reset interface, uart_clr_tx_index and uart_clr_rx_index must be called to clear the read/write pointer,
 *               after the uart reset interface is invoked, the hardware read and write Pointers are cleared to zero.
 *               Therefore, the software read and write Pointers are cleared to ensure logical correctness.
 *            -# After suspend wakes up, you must call uart_clr_tx_index and uart_clr_rx_index to clear read and write pointers,
 *               because after suspend wakes up, the chip is equivalent to performing a uart_hw_fsm_reset,
 *               so the software read and write pointer also needs to be cleared to zero.
 */
static inline void uart_clr_tx_index(uart_num_e uart_num)
{
    uart_tx_byte_index[uart_num] = 0;
}

/**
 * @brief     Clear uart tx_done.
 * @param[in] uart_num
 * @return    none.
 */
static inline void uart_clr_tx_done(uart_num_e uart_num)
{
    reg_uart_state(uart_num) |= FLD_UART_CLR_TXDONE;
}

/**
 * @brief       Enable the rtx .
 * @param[in]   chn - UART0 or UART1.
 * @return      none.
 */
static inline void uart_rtx_en(uart_num_e chn)
{
    reg_uart_rx_timeout1(chn) |= FLD_UART_P7816_EN;
}

#endif /* UART_H_ */
