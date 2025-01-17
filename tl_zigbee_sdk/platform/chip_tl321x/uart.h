/********************************************************************************************************
 * @file    uart.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *  supports uart: uart0~ uart2.
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
  -# UART Initialization and Configuration
     -# nodma/dma/dma_llp:
        - To prevent the UART module from storing history information, call uart_hw_fsm_reset() API before use ;
        - Initialize the tx/rx pin by uart_set_pin() API;
        - Configure the baud rate/stop bit/ parity by uart_cal_div_and_bwpc() / uart_init() API;
        - Configure rx_timeout by uart_set_rx_timeout();
     -# dma
        - dma initial configuration by uart_set_tx_dma_config() / uart_set_rx_dma_config() API;
     -# dma_llp(supports rx only,and supports the dma maximum receive length set to 0xfffffc):
        - dma initial configuration by uart_set_rx_dma_config() API;
     -# If using flow-controlled CTS/RTS:
            - CTS: uart_cts_config() / uart_set_cts_en() API;
            - RTS: uart_set_rts_en() API;
                - manual: uart_rts_config() / uart_set_rts_level() API;
                - auto: uart_rts_config() / uart_rts_trig_level_auto_mode() / uart_rxdone_rts_en() / uart_rts_stop_rxtimeout_en() API;
  -# UART Interrupts Configuration and Response
     -# UART interrupts initial configuration:
        - nodma_tx
           - polling send, it does not need to configure interrupt;
        - nodma_rx
           - uart_rx_irq_trig_level() / uart_set_irq_mask() : UART_RX_IRQ_MASK/UART_RXDONE_MASK/UART_ERR_IRQ_MASK ;
        - dma_tx
           - uart_set_irq_mask() : UART_TXDONE_MASK ;
        - dma_rx
           - dma_set_irq_mask() : TC_MASK ;
        - dma_llp
           - dma_set_irq_mask() : TC_MASK ;
        - plic_interrupt_enable() / core_interrupt_enable() API
     -# UART interrupt response(for details, see the enumeration uart_irq_status_e):
        - uart_get_irq_status() API;
        - uart_clr_irq_status() API;
        - dma_get_tc_irq_status() API;
        - nodma_rx
            - UART_RX_ERR/UART_RXBUF_IRQ_STATUS/UART_RXDONE_IRQ_STATUS ;
               - If err occurs during the receiving process, perform the following operations:
                  -# uart_clr_irq_status(): UART_RXBUF_IRQ_STATUS;
                  -# clear the data received in ram buff;
        - dma_tx
            - UART_TXDONE_IRQ_STATUS ;
        - dma_rx
            - If the transmit length is less than or equal to the receive length, a tc interrupt is generated and the received data length is the actual transmitted length.
            - If the transmit length is greater than the receive length, a tc interrupt is generated and the received data length is the actual transmitted length, \n
                but the data in the buffer is only valid for the configured receive length data, dma does not move more sent data into the buffer.
            - If the dma stops working and reconfigures it, perform the following operations:
                -# uart_receive_dma();(dma_chn_dis-> uart_clr_irq_status(): dma_chn_dis has been implemented in uart_receive_dma())
            - If an error occurs while dma receiving, the recommended action is as follows:
               -# uart_receive_dma();(dma_chn_dis-> uart_clr_irq_status(): dma_chn_dis has been implemented in uart_receive_dma())
        - dma_llp
              - dma_get_tc_irq_status(tc_chn) ;
  -# UART enter suspend sleep before and after handling
      - nodma_tx
          - before nodma tx goes to suspend sleep, it is necessary to judge the uart busy bit to ensure the completion of UART data transmission.
          - after suspend sleep wake up, perform the following operations:
            -# uart_clr_tx_index();
            -# clear the send data in ram buff;
      - nodma_rx
          - If go to suspend sleep during nodma rx, the recommended action when waking up is as follows:
            -# uart_clr_rx_index();
            -# clear the data received in ram buff;
      - dma_tx
          - If go to suspend sleep during dma sending, the recommended action when waking up is as follows:
            -# uart_send_dma();(dma_chn_dis has been implemented in uart_send_dma())
      - dma_rx
          - If go to suspend sleep during dma receiving, the recommended action when waking up is as follows:
            -# uart_receive_dma();(dma_chn_dis has been implemented in uart_receive_dma())
  -# UART TX and RX
     -# nodma_tx
        - It can be sent in byte/half word/word polling, use uart_send_byte() / uart_send_hword() / uart_send_word() API;
     -# nodma_rx
        - The data is received via UART_RX_IRQ_MASK and UART_RXDONE_MASK interrupt,the data is read in the interrupt by uart_read_byte() API;
        - the points to note when receiving nodma:
           - rx_irq interrupt processing: Use uart_get_rxfifo_num() to determine the number of FIFOs and use uart_read_byte() to read all data in fifo;
           - The depth size of the uart fifo is 8. If the time before and after entering the rx_irq interrupt exceeds the time of receiving 8 bytes, the fifo pointer may be disturbed, resulting in abnormal received data.
             You can determine whether uart_get_rxfifo_num() is greater than 8 as an exception,If this exception occurs, it is recommended to use dma mode to receive.
     -# dma_tx
        - send data by uart_send_dma() API;
     -# dma_rx
        - receive data by uart_receive_dma() API
        - There is no length limitation on the receive length, the maximum length is 0xfffffc, which needs to be a multiple of 4,
          and there is a write-back length information, it should be noted that:
          when the send length is greater than the receive length, the data length size of the buff received is the length of the dma configuration,
          and the excess data will be discarded, while the write-back length is the actual send length.
     -# dma_llp
       - Receive data by uart_set_dma_chain_llp() / uart_rx_dma_add_list_element() API;
       - Differences between dma_llp and dma_rx:
         - dma_rx: After receiving data, uart_receive_dma() needs to be reconfigured before receiving the next data;
         - dma_llp: After the current packet is received, uart_receive_dma() does not need to be reconfigured,the hardware will automatically jump to the next chain node.
       - Use restriction:
         - There are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length(the maximum length is 0xfffffc, which needs to be a multiple of 4).
- Timeout mechanism
     -# uart_set_error_timeout():define global variable g_uart_error_timeout_us,the default value is a large value,can use the preceding interfaces to adjust the value based on actual applications.
     -# uart_get_error_timeout_code(): when an error timeout abnormally, can use the above interface to read which error belongs to uart_api_error_code_e.
     -# uart_timeout_handler():when an error timeout exits abnormally,can do timeout processing at the application layer or the application layer redefines the interface.
 */
#ifndef UART_H_
#define UART_H_

#include "gpio.h"
#include "dma.h"
#include "reg_include/register.h"

extern unsigned char uart_rx_byte_index[3];
extern unsigned char uart_tx_byte_index[3];

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
    UART2,
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
 *  @brief  Define UART IRQ MASK.
 */
typedef enum
{
    UART_RX_IRQ_MASK  = BIT(2), /**< When the number of rxfifo reaches the set threshold(uart_rx_irq_trig_level), an interrupt is generated, and the interrupt flag is automatically cleared. */
    UART_TX_IRQ_MASK  = BIT(3), /**< When the number of txfifo is less than or equal to the set threshold(uart_tx_irq_trig_level), an interrupt is generated and the interrupt flag is automatically cleared. */
    UART_RXDONE_MASK  = BIT(4), /**< When no data is received in rx_timeout, rx_done is generated. If uart_auto_clr_rx_fifo_ptr is enabled, the interrupt flag is automatically cleared. If uart_auto_clr_rx_fifo_ptr is disabled, the interrupt flag must be manually cleared. */
    UART_TXDONE_MASK  = BIT(5), /**< When there is no data in the tx_fifo, tx_done is generated, and the interrupt flag bit needs to be manually cleared.*/
    UART_ERR_IRQ_MASK = BIT(6), /**< When the UART receives data incorrectly(such as a parity error or a stop bit error), the interrupt is generated,the interrupt flag bit needs to be manually cleared. */
} uart_irq_mask_e;

/**
 * @brief  Define UART IRQ bit status and explain what needs to be done in the interrupt.
 */
typedef enum
{
    UART_RXBUF_IRQ_STATUS = BIT(2),  /**<
                                        get interrupt status:uart_get_irq_status(), clr interrupt status: automatically cleared.
                                    <p>
                                        in nodma mode,the received data is read by uart_get_rxfifo_num() / uart_read_byte().
                                    <p>
                                        in dma mode,dma does not need this interrupt.
                                     */
    UART_TXBUF_IRQ_STATUS = BIT(3),  /**<
                                        in general, this interrupt is not required, and data is sent in nodma by polling,in dma mode,dma does not need this interrupt.
                                      */

    UART_RXDONE_IRQ_STATUS = BIT(4), /**<
                                        get interrupt status:uart_get_irq_status(),clr interrupt status:uart_clr_irq_status()
                                      <p>
                                        in nodma mode,the received data is read by uart_get_rxfifo_num() / uart_read_byte().

                                      <p>
                                         in dma mode, use dma tc interrupt.
                                       */

    UART_TXDONE_IRQ_STATUS = BIT(5), /**<
                                        get interrupt status:uart_get_irq_status(),clr interrupt status:uart_clr_irq_status().
                                       */
    UART_RX_ERR            = BIT(6), /**<
                                         in nodma:get interrupt status:uart_get_irq_status(),clr interrupt status:uart_clr_irq_status() : UART_RXBUF_IRQ_STATUS to clear data in rxfifo.
                                      <p>
                                         in dma:When obtaining UART_RXDONE_IRQ_STATUS, by the uart_get_irq_status() to obtain the status and check whether an exception occurs. Clear UART_RXDONE_IRQ_STATUS to clear UART_RX_ERR.
                                       */
} uart_irq_status_e;

typedef enum
{
    UART_NO_DMA_MODE = 0,
    UART_DMA_MODE    = 1,
} uart_rxdone_sel_e;

// uart api error code
typedef enum
{
    UART_API_ERROR_TIMEOUT_NONE       = 0x00,
    UART_API_ERROR_TIMEOUT_SEND_BYTE  = 0x01,
    UART_API_ERROR_TIMEOUT_SEND_HWORD = 0x02,
    UART_API_ERROR_TIMEOUT_SEND_WORD  = 0x03,
} uart_api_error_timeout_code_e;

typedef struct
{
    unsigned int                           g_uart_error_timeout_us;   // uart_x error timeout(us),a large value is set by default,can set it by uart_set_error_timeout();
    timeout_handler_fp                     uart_timeout_handler;      // uartx_timeout_handler;
    volatile uart_api_error_timeout_code_e g_uart_error_timeout_code; // record uart_x error timeout code, can obtain the value through the uart_get_error_timeout_code() interface;
} uart_timeout_error_t;

extern uart_timeout_error_t g_uart_timeout_error[3];

#define uart_rtx_pin_tx_trig(uart_num) uart_clr_irq_status(uart_num, UART_TXDONE_IRQ_STATUS)
// for compatibility
#define uart_reset uart_hw_fsm_reset

/**********************************************************************************************************************
 *                                          Internal interface                                             *
 *********************************************************************************************************************/
/*!
 * @name   Internal functions
 * @{
 */
/**
 * @brief     Choose rxdone(UART_RXDONE_IRQ_STATUS) function,nodma needs to be opened, dma needs to be closed. For the internal interface, the upper layer does not need to be called.
 * @param[in]  uart_num  - UART0/UART1/UART2.
 * @param[in] sel - 0:no_dma  1:dma
 * @return    none.
 */
static inline void uart_rxdone_sel(uart_num_e uart_num, uart_rxdone_sel_e sel)
{
    if (sel == UART_NO_DMA_MODE) {
        reg_uart_ctrl0(uart_num) |= FLD_UART_NDMA_RXDONE_EN;
    } else if (sel == UART_DMA_MODE) {
        reg_uart_ctrl0(uart_num) &= ~FLD_UART_NDMA_RXDONE_EN;
    }
}

/** @} */

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/*!
 * @name   External interface
 * @{
 */

/**
 * @brief     Get the rxfifo cnt,when data enters rxfifo, the rxfifo cnt increases; when reading data from rx_fifo, rxfifo cnt decays.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none
 */
static inline unsigned char uart_get_rxfifo_num(uart_num_e uart_num)
{
    return reg_uart_buf_cnt(uart_num) & FLD_UART_RX_BUF_CNT;
}

/**
 * @brief     Get the txfifo cnt,tx_fifo cnt decreases when data is sent from tx_fifo, and tx_fifo cnt increases when data is written to tx_fifo.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none
 */
static inline unsigned char uart_get_txfifo_num(uart_num_e uart_num)
{
    return (reg_uart_buf_cnt(uart_num) & FLD_UART_TX_BUF_CNT) >> 4;
}

/**
 * @brief      This function enable the clock of UART module.
 * @param[in]  uart_num  - UART0/UART1/UART2.
 * @return     none
 */
static inline void uart_clk_en(uart_num_e uart_num)
{
    switch (uart_num) {
    case UART0:
        reg_clk_en0 |= FLD_CLK0_UART0_EN;
        break;
    case UART1:
        reg_clk_en0 |= FLD_CLK0_UART1_EN;
        break;
    case UART2:
        reg_clk_en5 |= FLD_CLK5_UART2_EN;
        break;
    default:
        break;
    }
}

/**
 * @brief     Select whether to enable auto clr rx fifo pointer.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] en - 1:enable, when UART_RXDONE_IRQ_STATUS trigger,the hardware will automatically clear the rx_fifo pointer,the software only needs to clear UART_RXDONE_IRQ_STATUS interrupt flag bit.
 *                 0:disable,the rxfifo pointer is cleared only when the software clears the UART_RXDONE_IRQ_STATUS interrupt flag bit.
 * @return    none.
 * @note      This function cannot be turned on when no_dma.
 */
static inline void uart_auto_clr_rx_fifo_ptr(uart_num_e uart_num, unsigned char en)
{
    if (en == 1) {
        reg_uart_ctrl0(uart_num) |= FLD_UART_RX_CLR_EN;
    } else if (en == 0) {
        reg_uart_ctrl0(uart_num) &= ~FLD_UART_RX_CLR_EN;
    }
}

/**
 * @brief     Configure the trigger level of the UART_RXBUF_IRQ_STATUS interrupt. When the number of rx_fifo is greater than or equal to the trigger level, UART_RXBUF_IRQ_STATUS interrupt rises.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] rx_level - the trigger level,the range is less than 8.
 * @return    none
 * @note      This interface is only used in no_dma mode.
 */
static inline void uart_rx_irq_trig_level(uart_num_e uart_num, unsigned char rx_level)
{
    // uart_rx_irq_trig_level interface nodma must be invoked,dma does not call,so put the uart_rxdone_sel interface in this interface.
    uart_rxdone_sel(uart_num, UART_NO_DMA_MODE);
    uart_auto_clr_rx_fifo_ptr(uart_num, 0);
    reg_uart_ctrl3(uart_num) = (reg_uart_ctrl3(uart_num) & (~FLD_UART_RX_IRQ_TRIQ_LEV)) | (rx_level & 0x0f);
}

/**
 * @brief     Configure the trigger level of the UART_TXBUF_IRQ_STATUS interrupt,when the number of tx_fifo data is less than or equal to the trigger level, UART_TXBUF_IRQ_STATUS interrupt rises.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] tx_level - the trigger level,the range is less than 8.
 * @return    none
 * @note      This interface is used in no_dma mode only.
 */
static inline void uart_tx_irq_trig_level(uart_num_e uart_num, unsigned char tx_level)
{
    reg_uart_ctrl3(uart_num) = (reg_uart_ctrl3(uart_num) & (~FLD_UART_TX_IRQ_TRIQ_LEV)) | (tx_level << 4);
}

/**
 * @brief     Enable the irq of UART.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] mask     - enum uart irq mask.
 * @return    none
 */
static inline void uart_set_irq_mask(uart_num_e uart_num, uart_irq_mask_e mask)
{
    reg_uart_mask(uart_num) |= mask;
}

/**
 * @brief     Disable the irq of UART.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] mask     - enum uart irq mask.
 * @return    none
 */
static inline void uart_clr_irq_mask(uart_num_e uart_num, uart_irq_mask_e mask)
{
    reg_uart_mask(uart_num) &= ~mask;
}

/**
 * @brief     Set the 'uart_rx_byte_index' to 0,'uart_rx_byte_index' is used to synchronize the rxfifo hardware pointer in no_dma mode.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none.
 * @note      Note the following:
 *            -# After waking up from suspend, you must call uart_clr_tx_index and uart_clr_rx_index to clear read and write pointers,
 *               because after suspend wakes up, the chip is equivalent to performing a uart_hw_fsm_reset,
 *               so the software read and write pointer also needs to be cleared to zero.
 */
static inline void uart_clr_rx_index(uart_num_e uart_num)
{
    uart_rx_byte_index[uart_num] = 0;
}

/**
 * @brief     Set the 'uart_tx_byte_index' to 0,'uart_tx_byte_index' is used to synchronize the txfifo hardware pointer in no_dma mode.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none.
 * @note      Note the following:
 *            -# After waking up from suspend, you must call uart_clr_tx_index and uart_clr_rx_index to clear read and write pointers,
 *               because after waking up from suspend, the chip is equivalent to performing a uart_hw_fsm_reset,
 *               so the software read and write pointer also needs to be cleared to zero.
 */
static inline void uart_clr_tx_index(uart_num_e uart_num)
{
    uart_tx_byte_index[uart_num] = 0;
}

/**
 * @brief     uart finite state machine reset(the configuration register is still there and does not need to be reconfigured),
 *            For compatibility define uart_reset uart_hw_fms_reset, uart_hw_fms_reset is used when the driver is invoked (no matter at the driver layer or demo layer),
 *            before using UART, it is needed to call uart_hw_fsm_reset() to avoid affecting the use of UART.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @return    none
 * @note      this function will clear rx and tx status and fifo.
 */
static inline void uart_hw_fsm_reset(uart_num_e uart_num)
{
    switch (uart_num) {
    case UART0:
        BM_CLR(reg_rst0, FLD_RST0_UART0);
        BM_SET(reg_rst0, FLD_RST0_UART0);
        break;
    case UART1:
        BM_CLR(reg_rst0, FLD_RST0_UART1);
        BM_SET(reg_rst0, FLD_RST0_UART1);
        break;
    case UART2:
        BM_CLR(reg_rst5, FLD_RST5_UART2);
        BM_SET(reg_rst5, FLD_RST5_UART2);
        break;
    default:
        break;
    }
    uart_clr_tx_index(uart_num);
    uart_clr_rx_index(uart_num);
    g_uart_timeout_error[uart_num].g_uart_error_timeout_code = UART_API_ERROR_TIMEOUT_NONE;
}

/**
 * @brief     Get the irq status of UART.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] status   - enum uart irq status.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned int uart_get_irq_status(uart_num_e uart_num, uart_irq_status_e status)
{
    return reg_uart_irq(uart_num) & status;
}

/**
 * @brief     Clear the irq status of UART.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] status   - enum uart irq status.
 * @return    none
 */

static inline void uart_clr_irq_status(uart_num_e uart_num, uart_irq_status_e status)
{
    /*
     * the purpose of judging the status of rx_done:
     * when rx_done is cleared, the rx_buff is also cleared, because rx_fifo is cleared, and the software pointer is also cleared:
     * dma: rx_fifo is cleared because when the send length is larger than the receive length, when rx_done is generated, there is data in rx_fifo, and rx_done interrupt will always be generated, affecting the function.
     * no_dma: for unified processing with DMA, because rx_fifo is cleared, the software pointer also needs to be cleared, otherwise an exception occurs.
     * the purpose of judging the status of the rx_buff interrupt:
     *  Because the state of the err needs to be cleared by the clearing rx_buff when an err interrupt is generated, the software pointer needs to be cleared.
     */
    if (status == UART_RXDONE_IRQ_STATUS) {
        reg_uart_irq(uart_num) = UART_RXBUF_IRQ_STATUS;
        uart_clr_rx_index(uart_num); // clear software pointer
    }
    if (status == UART_RXBUF_IRQ_STATUS) {
        uart_clr_rx_index(uart_num); // clear software pointer
    }
    reg_uart_irq(uart_num) = status;
}

/**
 * @brief     Enable UART rts.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none
 */
static inline void uart_set_rts_en(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) |= FLD_UART_RTS_EN; // enable RTS function
}

/**
 * @brief     Disable UART rts.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none
 */
static inline void uart_set_rts_dis(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_EN); // disable RTS function
}

/**
 * @brief     Enable UART cts.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none
 */
static inline void uart_set_cts_en(uart_num_e uart_num)
{
    reg_uart_ctrl1(uart_num) |= FLD_UART_TX_CTS_ENABLE; // enable CTS function
}

/**
 * @brief     Disable UART cts.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @return    none
 */
static inline void uart_set_cts_dis(uart_num_e uart_num)
{
    reg_uart_ctrl1(uart_num) &= (~FLD_UART_TX_CTS_ENABLE); // disable CTS function
}

/**
 * @brief     Set UART rts trig level,when the number of rx_fifo reaches the rts trig level, rts pin level is active.
 * @param[in] uart_num  - UART0/UART1/UART2.
 * @param[in] level    - the rts trigger level,the range is less than 8.
 * @return    none
 */
static inline void uart_rts_trig_level_auto_mode(uart_num_e uart_num, unsigned char level)
{
    reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_TRIQ_LEV);
    reg_uart_ctrl2(uart_num) |= (level & FLD_UART_RTS_TRIQ_LEV);
}

/**
 * @brief       Get the remain count of rxfifo,the range is 0~3, to get the rx_fifo current pointer position when rxfifo received data less than 1 word.
 * @param[in]   uart_num  - UART0/UART1/UART2.
 * @return      the remain count of rxfifo.
 */
static inline unsigned char uart_get_rxfifo_rem_cnt(uart_num_e uart_num)
{
    unsigned char rx_cnt = 0;
    rx_cnt               = (reg_uart_irq(uart_num) & FLD_UART_RX_MEM_CNT);
    return rx_cnt;
}

/**
 * @brief      This function serves to set uart rts auto mode,flow control generation condition 1. rx_fifo number is greater than or equal to the set threshold 2. rx_done signal generation (if uart_rxdone_rts_en enable)).
 * @param[in]  uart_num  - UART0/UART1/UART2.
 * @return     none
 */
static inline void uart_rts_auto_mode(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_MANUAL_M);
}

/**
 * @brief      This function serves to set uart rts manual mode,in this mode, the rts level changes are controlled by calling the uart_set_rts_level interface.
 * @param[in]  uart_num  - UART0/UART1/UART2.
 * @return     none
 */
static inline void uart_rts_manual_mode(uart_num_e uart_num)
{
    reg_uart_ctrl2(uart_num) |= (FLD_UART_RTS_MANUAL_M);
}

/**
 * @brief       Enable the rtx function.
 * @param[in]   chn -UART0/UART1/UART2.
 * @return      none.
 */
static inline void uart_rtx_en(uart_num_e chn)
{
    reg_uart_ctrl0(chn) |= FLD_UART_S7816_EN;
}

/**
 * @brief     Enable rts stop rxtimeout.
 *            The functions are as follows:
 *            -# If uart_rts_stop_rxtimeout_dis:the threshold -> rts->timeout->UART_RXDONE_IRQ_STATUS interrupt.
 *            -# If uart_rts_stop_rxtimeout_en:the threshold -> rts->timeout stop -> the UART_RXDONE_IRQ_STATUS interrupt is not generated.
 *<p>
 *            How to use:
 *            -# When the rts function is not in use, this function does not need to be turned on, otherwise it will affect the generation of uart rxdone,
 *               this function has been turned off by uart_init() API(uart_rts_stop_rxtimeout_dis).
 *            -# If rts flow control is used, it is recommended to turn on this function:
 *               otherwise, when the sender cts pin receives the active level of rts pin, it stops sending data,
 *               If this function is turned off at this time, no data will be sent, resulting in an interruption in rxdone,
 *               When UART_RXDONE_IRQ_STATUS is cleared, the rx_fifo data will be cleared, causing the rts level to fail.
 * @param[in] uart_num
 * @return    none.
 * @note      this function is turned off by uart_init, this function is used in combination with rts function enable, the application determines whether to configure this function.
 */
static inline void uart_rts_stop_rxtimeout_en(uart_num_e uart_num)
{
    reg_uart_ctrl0(uart_num) |= FLD_UART_RXTIMEOUT_RTS_EN;
}

/**
 * @brief     Disable rts stop rxtimeout.
 *            The uart_rts_stop_rxtimeout_dis is called, the timeout will not be stopped when the rts signal is generated,the UART_RXDONE_IRQ_STATUS signal will not be affected.
 * @param[in] uart_num
 * @return    none.
 */
static inline void uart_rts_stop_rxtimeout_dis(uart_num_e uart_num)
{
    reg_uart_ctrl0(uart_num) &= ~FLD_UART_RXTIMEOUT_RTS_EN;
}

/**
 * @brief     Enable rxdone(UART_RXDONE_IRQ_STATUS) rts:
 *            -# If the rts function is used and this interface is called, generating the UART_RXDONE_IRQ_STATUS signal will trigger the rts pin level active,
 *               and if the interface uart_clr_irq_status is called to clear the UART_RXDONE_IRQ_STATUS signal, the rts pin level is invalid.
 * <p>
 *            How to use:
 *            -# When the rts function is not in use, this function does not need to be turned on,the function is turned off by default in uart_init() API.
 *            -# When the rts function is used:
 *                -# In nodma :It is recommended to open to avoid data overwriting due to late reception.
 *                -# In DMA:not recommended,the hardware will automatically clear rx_done, there is no mistake to clear the next data.
 * @param[in]  uart_num  - UART0/UART1/UART2.
 * @return    none.
 */
static inline void uart_rxdone_rts_en(uart_num_e uart_num)
{
    reg_uart_ctrl4(uart_num) |= FLD_UART_RXDONE_RTS_EN;
}

/**
 * @brief     Disable rxdone(UART_RXDONE_IRQ_STATUS) rts,rts will not be active after rx_done is generated.
 * @param[in]  uart_num  - UART0/UART1/UART2.
 * @return    none.
 */
static inline void uart_rxdone_rts_dis(uart_num_e uart_num)
{
    reg_uart_ctrl4(uart_num) &= ~FLD_UART_RXDONE_RTS_EN;
}

/**
 * @brief      Initialize the UART module.
 * @param[in]  uart_num    - UART0/UART1/UART2.
 * @param[in]  div         - UART clock divider.
 * @param[in]  bwpc        - bitwidth, should be set to larger than 2.
 * @param[in]  parity      - selected parity type for UART interface.
 * @param[in]  stop_bit    - selected length of stop bit for UART interface.
 * @return     none
 * @note
 * -# A few simple examples of sys_clk/baud rate/div/bwpc correspondence:
    @verbatim
                sys_clk      baudrate   g_uart_div         g_bwpc

                16MHz        9600          118                13
                             19200         118                 6
                             115200          9                13

                24MHz        9600          249                 9
                             19200         124                 9
                             115200         12                15

                32MHz        9600          235                13
                             19200         235                 6
                             115200         17                13

                48MHz        9600          499                 9
                             19200         249                 9
                             115200         25                15
    @endverbatim
    -# uart_init() set the baud rate by the div and bwpc of the uart_cal_div_and_bwpc, some applications have higher timing requirements, \n
       you can first calculate the div and bwpc, and then call uart_init.
 *
 */
void uart_init(uart_num_e uart_num, unsigned short div, unsigned char bwpc, uart_parity_e parity, uart_stop_bit_e stop_bit);

/**
 * @brief      Calculate the best bwpc(bit width),bwpc range from 3 to 15,loop and get the minimum one decimal point(BaudRate*(div+1)*(bwpc+1) = pclk).
 * @param[in]  baudrate - baud rate of UART.
 * @param[in]  pclk     - pclk.
 * @param[out] div      - UART clock divider.
 * @param[out] bwpc     - bitwidth, should be set to larger than 2,range[3-15].
 * @return     none
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
    -# Using the B92 development board,the test results:
         - CCLK_16M_HCLK_16M_PCLK_16M: in nodma,the maximum speed is 2 MHz; in dma,the maximum speed is 2 MHz;
         - CCLK_24M_HCLK_24M_PCLK_24M: in nodma,the maximum speed is 3 MHz; in dma,the maximum speed is 3 MHz;
         - CCLK_32M_HCLK_32M_PCLK_16M: in nodma,the maximum speed is 2 MHz; in dma,the maximum speed is 2 MHz;
         - CCLK_48M_HCLK_48M_PCLK_24M: in nodma,the maximum speed is 3 MHz; in dma,the maximum speed is 3 MHz;
         - CCLK_96M_HCLK_48M_PCLK_24M: in nodma,the maximum speed is 3 MHz; in dma,the maximum speed is 3 MHz;
 */
void uart_cal_div_and_bwpc(unsigned int baudrate, unsigned int sysclk, unsigned short *div, unsigned char *bwpc);

/**
 * @brief      Set rx_timeout.
   @verbatim
       The effect:
         -# the time out counter inside the uart is updated at the stop bit, and when receiving data stops, the timeout counter decreases to 0 and generates the UART_RXDONE_IRQ_STATUS interrupt.
         -# the UART_RXDONE_IRQ_STATUS interrupt is required to process the remaining data below the threshold:
               - the DMA Operation threshold is fixed at 4, for example, when 17 bytes are received and less than 4 bytes remain, dma is told to move the last data by timeout;
               - the NDMA threshold can be configured through uart_rx_irq_trig_level,for example,uart_rx_irq_trig_level is 4,when 17 bytes are received and less than 4 bytes remain, generate rx_done interrupt by timeout to inform mcu to move the last data;
       How to set:
         rx_timeout = (((1/baudrate) * bit_cnt)* mul)*(2^rxtimeout_exp)  value = ((bwpc+1) * bit_cnt, the maximum of this value can be set to 0xff.
   @endverbatim
 * @param[in]  uart_num        - UART0/UART1/UART2.
 * @param[in]  bwpc            - bitwidth.
 * @param[in]  bit_cnt         - bit number(for example, if transferring one bytes (1start bit+8bits data+1 priority bit+2stop bits) total 12 bits,then set it to at least 12).
 * @param[in]  mul             - mul.
 * @param[in]  rxtimeout_exp   - the power exponent of mul.
 * @return     none
 */
void uart_set_rx_timeout_with_exp(uart_num_e uart_num, unsigned char bwpc, unsigned char bit_cnt, uart_timeout_mul_e mul, unsigned char rxtimeout_exp);

/**
 * @brief      Send UART data by byte in no_dma mode.
 * @param[in]  uart_num - UART0/UART1/UART2.
 * @param[in]  tx_data  - the data to be send.
 * @return    DRV_API_SUCCESS: operation successful;
 *            DRV_API_TIMEOUT: timeout exit(solution refer to the note for uart_set_error_timeout);
 */
drv_api_status_e uart_send_byte(uart_num_e uart_num, unsigned char tx_data);

/**
 * @brief     Receive UART data by byte in no_dma mode.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @return    none
 */
unsigned char uart_read_byte(uart_num_e uart_num);

/**
 * @brief     Judge if the transmission of UART is done.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @return    0:tx is done     1:tx isn't done
 * @note      If upper-layer application calls the interface, if the timeout mechanism is used, the status cannot be detected because the uart send data is abnormal,
 *            see uart_set_error_timeout(time setting requirement).
 */
unsigned char uart_tx_is_busy(uart_num_e uart_num);

/**
 * @brief     Send UART data by halfword in no_dma mode.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] data  - the data to be send.
 * @return    DRV_API_SUCCESS: operation successful;
 *            DRV_API_TIMEOUT: timeout exit(solution refer to the note for uart_set_error_timeout);
 */
drv_api_status_e uart_send_hword(uart_num_e uart_num, unsigned short data);

/**
 * @brief     Send UART data by word in no_dma mode.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] data - the data to be send.
 * @return    DRV_API_SUCCESS: operation successful;
 *            DRV_API_TIMEOUT: timeout exit(solution refer to the note for uart_set_error_timeout);
 */
drv_api_status_e uart_send_word(uart_num_e uart_num, unsigned int data);

/**
 * @brief     Set the RTS pin's output level manually.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] polarity - 0: RTS pin outputs a low level  1: RTS pin outputs a high level.
 * @return    none
 */
void uart_set_rts_level(uart_num_e uart_num, unsigned char polarity);

/**
 *  @brief      Set pin for UART cts function, the pin connection mode: CTS<->RTS.
 *  @param[in]  uart_num - UART0/UART1/UART2.
 *  @param[in]  cts_pin -To set cts pin.
 *  @return     none
 */
void uart_set_cts_pin(uart_num_e uart_num, gpio_func_pin_e cts_pin);

/**
 *  @brief      Set pin for UART rts function, the pin connection mode: RTS<->CTS.
 *  @param[in]  uart_num - UART0/UART1/UART2.
 *  @param[in]  rts_pin - To set rts pin.
 *  @return     none
 */
void uart_set_rts_pin(uart_num_e uart_num, gpio_func_pin_e rts_pin);

/**
* @brief      Select pin for UART module, the pin connection mode: TX<->RX RX<->TX.
 * @param[in]  uart_num - UART0/UART1/UART2.
* @param[in]  tx_pin   - the pin to send data.
* @param[in]  rx_pin   - the pin to receive data.
 * @return     none
 */
void uart_set_pin(uart_num_e uart_num, gpio_func_pin_e tx_pin, gpio_func_pin_e rx_pin);

/**
* @brief      Set rtx pin for UART module, this pin can be used as either tx or rx. it is the rx function by default.
*             How to convert rx->tx:
*             -# There is data in tx_fifo,in nodma:fill txfifo by calling the interface uart_read_byte, which cannot be larger than 8 bytes before the second step;
*                in dma,uart_send_dma.
*             -# The interface uart_rtx_pin_tx_trig is called,if not calling this interface, the data cannot be sent out.
*             -# After converting to tx, if tx_fifo has no data, the hardware automatically converts to rx function.
 * @param[in]  uart_num - UART0/UART1/UART2.
 * @param[in]  rtx_pin  - the rtx pin need to set.
 * @return     none
 */
void uart_set_rtx_pin(uart_num_e uart_num, gpio_func_pin_e rtx_pin);

/**
 * @brief       Send an amount of data in DMA mode.
 * @param[in]   uart_num - UART0/UART1/UART2.
 * @param[in]   addr     - Pointer to data buffer. It must be 4-bytes aligned address
 * @param[in]   len      - Amount of data to be sent in bytes, range from 1 to 0xFFFFFC
 * @return      1  DMA start sending.
 *              0  the length is error.
 * @note
 *              -# After the DMA transfer is complete, the interface needs to be re-invoked to send the next batch of data.
 *              -# If the DMA transfer has not been completed when entering low-power suspend, after suspend, the interface needs to be re-invoked to send the next batch of data.
 */
unsigned char uart_send_dma(uart_num_e uart_num, unsigned char *addr, unsigned int len);

/**
* @brief     Send an amount of data in NODMA mode
 * @param[in] uart_num - UART0/UART1/UART2.
* @param[in] addr     - pointer to the buffer.
 * @param[in] len      - NDMA transmission length.
 * @return    1
 */
unsigned char uart_send(uart_num_e uart_num, unsigned char *addr, unsigned char len);

/**
 * @brief       This function serves to receive data function by DMA, this function tell the DMA to get data from the uart data fifo.
 *              regardless of the length of the DMA configuration, write_num is available,the dma automatically writes back the data receive length to the first four bytes of the rec_buff without manual calculation,
 *              so need addr = the receive buff addr +4.
 * @param[in]   uart_num - UART0/UART1/UART2.
 * @param[in]   addr     - pointer to the buffer  receive data.
 * @param[in]   rev_size - the receive length of DMA,The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      none
 * @note
 *              - addr must be aligned by word (4 bytes), otherwise the program will enter an exception.
 *              - A length greater than XX_len itself and a multiple of 4 is denoted as CEILING(XX_len,4). For example: XX_len=3 ,CEILING(XX_len,4)=4; XX_len=21 ,CEILING(Tx_length, 4)=24. \n
 *                The actual length sent by master  is denoted as Tx_len, The length (param[in]-rev_size) of the interface configuration is denoted as Rx_len. \n
 *                when CEILING(Tx_len,4) > CEILING(Rx_len,4), When the length of the DMA carry reaches Rx_len, the DMA will not stop working and the excess data will not be carried into the buff. \n
 *                For example:Tx_len=21,Rx_len=20,When the DMA stops working the buff is written with a length of 21 and only 20 bytes of data are stored.It is recommended to configure the appropriate Rx_len to avoid this situation.
 */
void uart_receive_dma(uart_num_e uart_num, unsigned char *addr, unsigned int rev_size);

/**
  * @brief     Configures the UART tx_dma channel control register.
  * @param[in] uart_num - UART0/UART1/UART2.
  * @param[in] chn      - dma channel.
  * @return    none
 */
void uart_set_tx_dma_config(uart_num_e uart_num, dma_chn_e chn);

/**
 * @brief      Configures UART rx_dma channel control register.
 * @param[in]  uart_num - UART0/UART1/UART2.
 * @param[in]  chn      - dma channel.
 * @return     none
 */
void uart_set_rx_dma_config(uart_num_e uart_num, dma_chn_e chn);

/**
  * @brief     Configure UART hardware flow CTS.
  * @param[in] uart_num - UART0/UART1/UART2.
  * @param[in] cts_pin    - cts pin select.
  * @param[in] cts_parity -  1:Active high,when the cts pin receives a high level, it stops sending data.
  *                          0:Active low,when the cts pin receives a low level, it stops sending data.
  * @return    none
 */
void uart_cts_config(uart_num_e uart_num, gpio_func_pin_e cts_pin, unsigned char cts_parity);

/**
 * @brief     Configure UART hardware flow RTS.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] rts_pin      - RTS pin select.
 * @param[in] rts_parity   - 0: Active high: rts level changed from low to high.
 *                           1: Active low: rts level changed from high to low.
 * @param[in] auto_mode_en - set the mode of RTS(1:auto mode  0:manual mode)
   @verbatim
    -# auto mode:
       -# the manual mode function is useless.
       -# rts_parity:It is used in combination with auto mode
       -# rx_fifo cnt is greater than or equal to the set threshold(uart_rts_trig_level_auto_mode), rts is valid,
          when the number of rx_fifo is less than the set threshold, the level automatically becomes invalid;
       -# rx_done signal generation (if uart_rxdone_rts_en enable)),rts is valid,
          when the UART_RXDONE_IRQ_STATUS signal is cleared,the level automatically becomes invalid;
       -# Several configurations related to rts,see the following interface for details:uart_rts_stop_rxtimeout_en/dis uart_rxdone_rts_en/dis.
    -# manual mode:
       -# the auto mode function is useless.
       -# Manually control the output level of the rts pin through the uart_set_rts_level interface.
   @endverbatim
 * @return    none
 */
void uart_rts_config(uart_num_e uart_num, gpio_func_pin_e rts_pin, unsigned char rts_parity, unsigned char auto_mode_en);

/*
  * @brief      Configure DMA head node.
 * @param[in]  uart_num - UART0/UART1/UART2.
  * @param[in]  chn         - DMA channel.
  * @param[in]  dst_addr    - Pointer to data buffer, which must be 4 bytes aligned.
  * @param[in]  data_len    - any length less than 0xfffffc(multiple of four).
  * @param[in]  head_of_list - the head address of DMA llp.
  * @return     none.
  * @note       there are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length:
  *              - The condition of linked list jump: one is when the length of the dma configuration is reached, the other is rx_done,
  *                no matter which condition has a write back length.
 */
void uart_set_dma_chain_llp(uart_num_e uart_num, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
  * @brief      Configure DMA cycle chain node.
 * @param[in]  uart_num - UART0/UART1/UART2.
  * @param[in]  chn         - DMA channel.
 * @param[in]  config_addr - to servers to configure the address of the current node.
  * @param[in]  llpointer   - to configure the address of the next node.
  * @param[in]  dst_addr    - Pointer to data buffer, which must be 4 bytes aligned.
  * @param[in]  data_len    - any length less than 0xfffffc(multiple of four).
  * @return     none.
  * @note       there are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length:
  *             - The condition of linked list jump: one is when the length of the dma configuration is reached, the other is rx_done,
  *               no matter which condition has a write back length.
 */
void uart_rx_dma_add_list_element(uart_num_e uart_num, dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *dst_addr, unsigned int data_len);

/**
  * @brief      Set DMA single chain transfer.
 * @param[in]  uart_num - UART0/UART1/UART2.
  * @param[in]  chn       - DMA channel
  * @param[in]  in_buff   - Pointer to data buffer, which must be 4 bytes aligned.
  * @param[in]  buff_size - any length less than 0xfffffc(multiple of four).
  * @return     none.
  * @note       there are no usage restrictions, there is no need to configure the dma length to a maximum length, and there is no limit to the send length:
  *             - The condition of linked list jump: one is when the length of the dma configuration is reached, the other is rx_done,
  *               no matter which condition has a write back length.
 */
void uart_rx_dma_chain_init(uart_num_e uart_num, dma_chn_e chn, unsigned char *in_buff, unsigned int buff_size);

/**
 * @brief     This function serves to record the api status.
 * @param[in] uart_error_timeout_code - uart_api_error_timeout_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario,can by g_uart_error_timeout_code to obtain details about the timeout reason,
 *            for the solution, refer to the uart_set_error_timeout note.
 */
__attribute__((weak)) void uart0_timeout_handler(unsigned int uart_error_timeout_code);
__attribute__((weak)) void uart1_timeout_handler(unsigned int uart_error_timeout_code);
__attribute__((weak)) void uart2_timeout_handler(unsigned int uart_error_timeout_code);
/**
 * @brief     This function serves to set the uart timeout(us).
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] timeout_us - the timeout(us).
 * @return    none.
 * @note      The default timeout (g_uart_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
 *            g_uart_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
 *            g_uart_error_timeout_us the minimum time must meet the following conditions:
 *            When not using cts flow control:
 *            1. eight uart data;
 *            2. maximum interrupt processing time;
 *            When using cts flow control:
 *            1. eight uart data;
 *            2. maximum interrupt processing time;
 *            3. The maximum normal cts flow control time;
 *            when timeout exits, solution:
 *            1.uart_hw_fsm_reset;
 *            2.solve why cts has been held up(When using cts flow control);
 */
void uart_set_error_timeout(uart_num_e uart_num, unsigned int timeout_us);

/**
 * @brief     This function serves to return the uart api error code.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @return    none.
 */
uart_api_error_timeout_code_e uart_get_error_timeout_code(uart_num_e uart_num);

#endif /* UART_H_ */
