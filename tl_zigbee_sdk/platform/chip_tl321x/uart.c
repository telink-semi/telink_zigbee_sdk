/********************************************************************************************************
 * @file    uart.c
 *
 * @brief   This is the source file for TL321X
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
#include "uart.h"

/**********************************************************************************************************************
 *                                              local constants                                                       *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                                 local macro                                                        *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
dma_config_t uart_tx_dma_config[3] = {
    {
     .dst_req_sel    = DMA_REQ_UART0_TX, // tx req
        .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT,                           // increment
        .dstmode        = DMA_HANDSHAKE_MODE, // handshake
        .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,                                                                                        // must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH,                                           // must be word
        .src_burst_size = 0, // must be 0
        .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, // must be 0
    },
    {
     .dst_req_sel    = DMA_REQ_UART1_TX,                                   // tx req
        .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT,                                 // increment
        .dstmode        = DMA_HANDSHAKE_MODE,                                              // handshake
        .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, // must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH,                                                                                        // must be word
        .src_burst_size = 0,                             // must be 0
        .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, // must be 0
    },
    {
     .dst_req_sel    = DMA_REQ_UART2_TX,     // tx req
        .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT, // increment
        .dstmode        = DMA_HANDSHAKE_MODE,                                                    // handshake
        .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,                                           // must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH, // must be word
        .src_burst_size = 0,                                                                          // must be 0
        .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, // must be 0
    },
};
dma_config_t uart_rx_dma_config[3] = {
    {
     .dst_req_sel    = 0, // tx req
        .src_req_sel    = DMA_REQ_UART0_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, // must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH, ////must be word
        .src_burst_size = 0,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 1,
     .auto_en        = 0, // must be 0
    },
    {
     .dst_req_sel    = 0,                    // tx req
        .src_req_sel    = DMA_REQ_UART1_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, // must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH,                                              ////must be word
        .src_burst_size = 0,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 1,
     .auto_en        = 0, // must be 0
    },
    {
     .dst_req_sel    = 0,       // tx req
        .src_req_sel    = DMA_REQ_UART2_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,    // must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH,                                              ////must be word
        .src_burst_size = 0,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 1,
     .auto_en        = 0, // must be 0
    },
};

/**********************************************************************************************************************
 *                                              local variable                                                        *
 *********************************************************************************************************************/
static unsigned char uart_dma_tx_chn[3];
static unsigned char uart_dma_rx_chn[3];
static unsigned int  uart_dma_rev_size[3];
dma_chain_config_t   g_uart_rx_dma_list_cfg;
uart_timeout_error_t g_uart_timeout_error[3] = {
    {
     .g_uart_error_timeout_us   = 0xffffffff,
     .uart_timeout_handler      = uart0_timeout_handler,
     .g_uart_error_timeout_code = UART_API_ERROR_TIMEOUT_NONE,
     },
    {
     .g_uart_error_timeout_us   = 0xffffffff,
     .uart_timeout_handler      = uart1_timeout_handler,
     .g_uart_error_timeout_code = UART_API_ERROR_TIMEOUT_NONE,
     },
    {
     .g_uart_error_timeout_us   = 0xffffffff,
     .uart_timeout_handler      = uart2_timeout_handler,
     .g_uart_error_timeout_code = UART_API_ERROR_TIMEOUT_NONE,
     }
};
static unsigned char g_uart_auto_clr_rx_fifo_ptr_en = 1;
/**********************************************************************************************************************
 *                                          local function prototype                                                  *
 *********************************************************************************************************************/
/**
 * @brief      This function is used to look for the prime.if the prime is found,it will return 1, or return 0.
 * @param[in]  n - the value to judge.
 * @return     0 or 1
 */
static unsigned char uart_is_prime(unsigned int n);

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

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
void uart_init(uart_num_e uart_num, unsigned short div, unsigned char bwpc, uart_parity_e parity, uart_stop_bit_e stop_bit)
{
    reg_uart_ctrl0(uart_num)   = ((reg_uart_ctrl0(uart_num) & (~FLD_UART_BPWC_O)) | bwpc); // set bwpc
    reg_uart_clk_div(uart_num) = (div | FLD_UART_CLK_DIV_EN);                              // set div_clock

    // parity config
    if (parity) {
        reg_uart_ctrl1(uart_num) |= FLD_UART_PARITY_ENABLE;          // enable parity function
        if (UART_PARITY_EVEN == parity) {
            reg_uart_ctrl1(uart_num) &= (~FLD_UART_PARITY_POLARITY); // enable even parity
        } else if (UART_PARITY_ODD == parity) {
            reg_uart_ctrl1(uart_num) |= FLD_UART_PARITY_POLARITY;    // enable odd parity
        }
    } else {
        reg_uart_ctrl1(uart_num) &= (~FLD_UART_PARITY_ENABLE);       // disable parity function
    }

    // stop bit config
    reg_uart_ctrl1(uart_num) = ((reg_uart_ctrl1(uart_num) & (~FLD_UART_STOP_SEL)) | stop_bit);
    uart_rts_stop_rxtimeout_dis(uart_num);
    uart_rxdone_rts_dis(uart_num);
}

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
void uart_cal_div_and_bwpc(unsigned int baudrate, unsigned int pclk, unsigned short *div, unsigned char *bwpc)
{
    unsigned char i = 0, j = 0;
    unsigned int  primeInt = 0;
    unsigned char primeDec = 0;
    unsigned int  D_intdec[13], D_int[13];
    unsigned char D_dec[13];

    primeInt = pclk / baudrate;
    primeDec = 10 * pclk / baudrate - 10 * primeInt;

    if (uart_is_prime(primeInt)) { // primeInt is prime
        primeInt += 1;             //+1 must be not prime. and primeInt must be larger than 2.
    } else {
        if (primeDec > 5) {        // >5
            primeInt += 1;
            if (uart_is_prime(primeInt)) {
                primeInt -= 1;
            }
        }
    }

    for (i = 3; i <= 15; i++) {
        D_intdec[i - 3] = (10 * primeInt) / (i + 1);                     ////get the LSB
        D_dec[i - 3]    = D_intdec[i - 3] - 10 * (D_intdec[i - 3] / 10); /// get the decimal section
        D_int[i - 3]    = D_intdec[i - 3] / 10;                          /// get the integer section
    }

    // find the max and min one decimation point
    unsigned char position_min = 0, position_max = 0;
    unsigned int  min = 0xffffffff, max = 0x00;
    for (j = 0; j < 13; j++) {
        if ((D_dec[j] <= min) && (D_int[j] != 0x01)) {
            min          = D_dec[j];
            position_min = j;
        }
        if (D_dec[j] >= max) {
            max          = D_dec[j];
            position_max = j;
        }
    }

    if ((D_dec[position_min] < 5) && (D_dec[position_max] >= 5)) {
        if (D_dec[position_min] < (10 - D_dec[position_max])) {
            *bwpc = position_min + 3;
            *div  = D_int[position_min] - 1;
        } else {
            *bwpc = position_max + 3;
            *div  = D_int[position_max];
        }
    } else if ((D_dec[position_min] < 5) && (D_dec[position_max] < 5)) {
        *bwpc = position_min + 3;
        *div  = D_int[position_min] - 1;
    } else {
        *bwpc = position_max + 3;
        *div  = D_int[position_max];
    }
}

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
void uart_set_rx_timeout_with_exp(uart_num_e uart_num, unsigned char bwpc, unsigned char bit_cnt, uart_timeout_mul_e mul, unsigned char rxtimeout_exp)
{
    reg_uart_rx_timeout0(uart_num)     = (bwpc + 1) * bit_cnt; // one byte includes 12 bits at most
    reg_uart_rx_timeout1(uart_num)     = (((reg_uart_rx_timeout1(uart_num)) & (~FLD_UART_TIMEOUT_MUL)) | mul);
    reg_uart_rxtimeout_o_exp(uart_num) = rxtimeout_exp;
}

/**
  * @brief     This function serves to record the api status.
  * @param[in] uart_error_timeout_code - uart_api_error_timeout_code_e.
  * @return    none.
  * @note      This function can be rewritten according to the application scenario,can by error_timeout_code to obtain details about the timeout reason,
  *            for the solution, refer to the uart_set_error_timeout note.
  */
__attribute__((weak)) void uart0_timeout_handler(unsigned int uart_error_timeout_code)
{
    g_uart_timeout_error[UART0].g_uart_error_timeout_code = uart_error_timeout_code;
}

__attribute__((weak)) void uart1_timeout_handler(unsigned int uart_error_timeout_code)
{
    g_uart_timeout_error[UART1].g_uart_error_timeout_code = uart_error_timeout_code;
}

__attribute__((weak)) void uart2_timeout_handler(unsigned int uart_error_timeout_code)
{
    g_uart_timeout_error[UART2].g_uart_error_timeout_code = uart_error_timeout_code;
}

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
void uart_set_error_timeout(uart_num_e uart_num, unsigned int timeout_us)
{
    g_uart_timeout_error[uart_num].g_uart_error_timeout_us = timeout_us;
}

/**
  * @brief     This function serves to return the uart api error code.
  * @param[in] uart_num - UART0/UART1/UART2.
  * @return    none.
  */
uart_api_error_timeout_code_e uart_get_error_timeout_code(uart_num_e uart_num)
{
    return g_uart_timeout_error[uart_num].g_uart_error_timeout_code;
}

/**
* @brief     Check whether tx_fifo is full(byte).
* @param[in] uart_num - UART0/UART1/UART2.
* @return    0: not full  1:full
*/
static bool uart_byte_txfifo_is_full(uart_num_e uart_num)
{
    return (uart_get_txfifo_num(uart_num) > 7);
}

/**
* @brief     Check whether tx_fifo is full(byte).
* @param[in] uart_num - UART0/UART1/UART2.
* @return    0: not full  1:full
*/
static bool uart_hword_txfifo_is_full(uart_num_e uart_num)
{
    return (uart_get_txfifo_num(uart_num) > 6);
}

/**
* @brief     Check whether tx_fifo is full(byte).
* @param[in] uart_num - UART0/UART1/UART2.
* @return    0: not full  1:full
*/
static bool uart_word_txfifo_is_full(uart_num_e uart_num)
{
    return (uart_get_txfifo_num(uart_num) > 4);
}

#define UART_WAIT(condition, uart_num, g_uart_error_timeout_us, uart_timeout_handler, uart_api_error_code) wait_condition_fails_or_timeout_with_param(condition, (unsigned int)uart_num, g_uart_error_timeout_us, uart_timeout_handler, (unsigned int)uart_api_error_code)


unsigned char uart_tx_byte_index[3] = {0};

/**
 * @brief      Send UART data by byte in no_dma mode.
 * @param[in]  uart_num - UART0/UART1/UART2.
 * @param[in]  tx_data  - the data to be send.
 * @return    DRV_API_SUCCESS: operation successful;
 *            DRV_API_TIMEOUT: timeout exit(solution refer to the note for uart_set_error_timeout);
 */
drv_api_status_e uart_send_byte(uart_num_e uart_num, unsigned char tx_data)
{
    if (UART_WAIT(uart_byte_txfifo_is_full, uart_num, g_uart_timeout_error[uart_num].g_uart_error_timeout_us, g_uart_timeout_error[uart_num].uart_timeout_handler, UART_API_ERROR_TIMEOUT_SEND_BYTE)) {
        return DRV_API_TIMEOUT;
    }
    reg_uart_data_buf(uart_num, uart_tx_byte_index[uart_num]) = tx_data;
    uart_tx_byte_index[uart_num]++;
    (uart_tx_byte_index[uart_num]) &= 0x03;
    return DRV_API_SUCCESS;
}

unsigned char uart_rx_byte_index[3] = {0};

/**
 * @brief     Receive UART data by byte in no_dma mode.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @return    none
 */
unsigned char uart_read_byte(uart_num_e uart_num)
{
    unsigned char rx_data = reg_uart_data_buf(uart_num, uart_rx_byte_index[uart_num]);
    uart_rx_byte_index[uart_num]++;
    uart_rx_byte_index[uart_num] &= 0x03;
    return rx_data;
}

/**
 * @brief     Judge if the transmission of UART is done.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @return    0:tx is done     1:tx isn't done
 * @note      If upper-layer application calls the interface, if the timeout mechanism is used, the status cannot be detected because the uart send data is abnormal,
 *            see uart_set_error_timeout(time setting requirement).
 */
unsigned char uart_tx_is_busy(uart_num_e uart_num)
{
    return ((reg_uart_irq(uart_num) & FLD_UART_TXDONE_IRQ) ? 0 : 1);
}

/**
 * @brief     Send UART data by halfword in no_dma mode.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] data  - the data to be send.
 * @return    DRV_API_SUCCESS: operation successful;
 *            DRV_API_TIMEOUT: timeout exit(solution refer to the note for uart_set_error_timeout);
 */
drv_api_status_e uart_send_hword(uart_num_e uart_num, unsigned short data)
{
    static unsigned char uart_tx_hword_index[3] = {0};
    if (UART_WAIT(uart_hword_txfifo_is_full, uart_num, g_uart_timeout_error[uart_num].g_uart_error_timeout_us, g_uart_timeout_error[uart_num].uart_timeout_handler, UART_API_ERROR_TIMEOUT_SEND_HWORD)) {
        return DRV_API_TIMEOUT;
    }

    reg_uart_data_hword_buf(uart_num, uart_tx_hword_index[uart_num]) = data;
    uart_tx_hword_index[uart_num]++;
    uart_tx_hword_index[uart_num] &= 0x01;
    return DRV_API_SUCCESS;
}

/**
 * @brief     Send UART data by word in no_dma mode.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] data - the data to be send.
 * @return    DRV_API_SUCCESS: operation successful;
 *            DRV_API_TIMEOUT: timeout exit(solution refer to the note for uart_set_error_timeout);
 */
drv_api_status_e uart_send_word(uart_num_e uart_num, unsigned int data)
{
    if (UART_WAIT(uart_word_txfifo_is_full, uart_num, g_uart_timeout_error[uart_num].g_uart_error_timeout_us, g_uart_timeout_error[uart_num].uart_timeout_handler, UART_API_ERROR_TIMEOUT_SEND_WORD)) {
        return DRV_API_TIMEOUT;
    }
    reg_uart_data_word_buf(uart_num) = data;
    return DRV_API_SUCCESS;
}

/**
 * @brief     Set the RTS pin's output level manually.
 * @param[in] uart_num - UART0/UART1/UART2.
 * @param[in] polarity - 0: RTS pin outputs a low level  1: RTS pin outputs a high level.
 * @return    none
 */
void uart_set_rts_level(uart_num_e uart_num, unsigned char polarity)
{
    if (polarity) {
        reg_uart_ctrl2(uart_num) |= FLD_UART_RTS_MANUAL_V;
    } else {
        reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_MANUAL_V);
    }
}

/**
 *  @brief      Set pin for UART cts function, the pin connection mode: CTS<->RTS.
 *  @param[in]  uart_num - UART0/UART1/UART2.
 *  @param[in]  cts_pin -To set cts pin.
 *  @return     none
 */
void uart_set_cts_pin(uart_num_e uart_num, gpio_func_pin_e cts_pin)
{
    switch (uart_num) {
    case UART0:
        gpio_set_mux_function(cts_pin, UART0_CTS_I);
        break;
    case UART1:
        gpio_set_mux_function(cts_pin, UART1_CTS_I);
        break;
    case UART2:
        gpio_set_mux_function(cts_pin, UART2_CTS_I);
        break;
    default:
        break;
    }

    gpio_function_dis((gpio_pin_e)cts_pin);
}

/**
 *  @brief      Set pin for UART rts function, the pin connection mode: RTS<->CTS.
 *  @param[in]  uart_num - UART0/UART1/UART2.
 *  @param[in]  rts_pin - To set rts pin.
 *  @return     none
 */
void uart_set_rts_pin(uart_num_e uart_num, gpio_func_pin_e rts_pin)
{
    switch (uart_num) {
    case UART0:
        gpio_set_mux_function(rts_pin, UART0_RTS);
        break;
    case UART1:
        gpio_set_mux_function(rts_pin, UART1_RTS);
        break;
    case UART2:
        gpio_set_mux_function(rts_pin, UART2_RTS);
        break;
    default:
        break;
    }
    gpio_function_dis((gpio_pin_e)rts_pin);
}

/**
* @brief      Select pin for UART module, the pin connection mode: TX<->RX RX<->TX.
 * @param[in]  uart_num - UART0/UART1/UART2.
* @param[in]  tx_pin   - the pin to send data.
* @param[in]  rx_pin   - the pin to receive data.
 * @return     none
 */
void uart_set_pin(uart_num_e uart_num, gpio_func_pin_e tx_pin, gpio_func_pin_e rx_pin)
{
    // When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    // otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    if (tx_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)tx_pin);
        gpio_set_up_down_res((gpio_pin_e)tx_pin, GPIO_PIN_PULLUP_10K);
        switch (uart_num) {
        case UART0:
            gpio_set_mux_function(tx_pin, UART0_TX);
            break;
        case UART1:
            gpio_set_mux_function(tx_pin, UART1_TX);
            break;
        case UART2:
            gpio_set_mux_function(tx_pin, UART2_TX);
            break;
        default:
            break;
        }
        gpio_function_dis((gpio_pin_e)tx_pin);
    }
    if (rx_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)rx_pin);
        gpio_set_up_down_res((gpio_pin_e)rx_pin, GPIO_PIN_PULLUP_10K);
        switch (uart_num) {
        case UART0:
            gpio_set_mux_function(rx_pin, UART0_RTX_IO);
            break;
        case UART1:
            gpio_set_mux_function(rx_pin, UART1_RTX_IO);
            break;
        case UART2:
            gpio_set_mux_function(rx_pin, UART2_RTX_IO);
            break;
        default:
            break;
        }
        gpio_function_dis((gpio_pin_e)rx_pin);
    }
}

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
void uart_set_rtx_pin(uart_num_e uart_num, gpio_func_pin_e rtx_pin)
{
    // When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    // otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en((gpio_pin_e)rtx_pin);
    gpio_set_up_down_res((gpio_pin_e)rtx_pin, GPIO_PIN_PULLUP_10K);
    switch (uart_num) {
    case UART0:
        gpio_set_mux_function(rtx_pin, UART0_RTX_IO);
        break;
    case UART1:
        gpio_set_mux_function(rtx_pin, UART1_RTX_IO);
        break;
    case UART2:
        gpio_set_mux_function(rtx_pin, UART2_RTX_IO);
        break;
    default:
        break;
    }
    gpio_function_dis((gpio_pin_e)rtx_pin);
}

/**
* @brief     Send an amount of data in NODMA mode

* @param[in] addr     - pointer to the buffer.
 * @param[in] len      - NDMA transmission length.
 * @return    1
 */
unsigned char uart_send(uart_num_e uart_num, unsigned char *addr, unsigned char len)
{
    for (unsigned char i = 0; i < len; i++) {
        uart_send_byte(uart_num, addr[i]);
    }
    return 1;
}

/**
 * @brief       Send an amount of data in DMA mode.
 * @param[in]   uart_num - uart channel
 * @param[in]   addr     - Pointer to data buffer. It must be 4-bytes aligned address
 * @param[in]   len      - Amount of data to be sent in bytes, range from 1 to 0xFFFFFC
 * @return      1  DMA start sending.
 *              0  the length is error.
 * @note
 *              -# After the DMA transfer is complete, the interface needs to be re-invoked to send the next batch of data.
 *              -# If the DMA transfer has not been completed when entering low-power suspend, after suspend, the interface needs to be re-invoked to send the next batch of data.
 */
unsigned char uart_send_dma(uart_num_e uart_num, unsigned char *addr, unsigned int len)
{
    dma_chn_dis(uart_dma_tx_chn[uart_num]);
    if (len != 0) {
        // In order to prevent the time between the last piece of data and the next piece of data is less than the set timeout time,
        // causing the receiver to treat the next piece of data as the last piece of data.
        uart_clr_irq_status(uart_num, UART_TXDONE_IRQ_STATUS);
        dma_set_address(uart_dma_tx_chn[uart_num], (unsigned int)(addr), uart_base_addr(uart_num));
        dma_set_size(uart_dma_tx_chn[uart_num], len, DMA_WORD_WIDTH);
        dma_chn_en(uart_dma_tx_chn[uart_num]);
        return 1;
    } else {
        return 0;
    }
}

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
void uart_receive_dma(uart_num_e uart_num, unsigned char *addr, unsigned int rev_size)
{
    dma_chn_dis(uart_dma_rx_chn[uart_num]);
    uart_dma_rev_size[uart_num] = rev_size;
    // uart_receive_dma interface dma must be invoked,nodma does not call,so put the uart_rxdone_sel interface in this interface.
    uart_rxdone_sel(uart_num, UART_DMA_MODE);
    uart_auto_clr_rx_fifo_ptr(uart_num, g_uart_auto_clr_rx_fifo_ptr_en);
    dma_set_address(uart_dma_rx_chn[uart_num], uart_base_addr(uart_num), (unsigned int)(addr));
    dma_set_size(uart_dma_rx_chn[uart_num], rev_size, DMA_WORD_WIDTH);
    dma_chn_en(uart_dma_rx_chn[uart_num]);
}

/**
  * @brief     Configures the UART tx_dma channel control register.
  * @param[in] uart_num - UART0/UART1/UART2.
  * @param[in] chn      - dma channel.
  * @return    none
 */
void uart_set_tx_dma_config(uart_num_e uart_num, dma_chn_e chn)
{
    uart_dma_tx_chn[uart_num] = chn;
    dma_config(chn, &uart_tx_dma_config[uart_num]);
}

/**
 * @brief      Configures UART rx_dma channel control register.
 * @param[in]  uart_num - UART0/UART1/UART2.
 * @param[in]  chn      - dma channel.
 * @return     none
 */
void uart_set_rx_dma_config(uart_num_e uart_num, dma_chn_e chn)
{
    // no_dma mode: rxdone(timeout) function switch; 1:enable,0:disable;dma mode must disable.
    reg_uart_ctrl0(uart_num) &= ~FLD_UART_NDMA_RXDONE_EN;
    uart_dma_rx_chn[uart_num] = chn;
    dma_config(chn, &uart_rx_dma_config[uart_num]);
}

/**
  * @brief     Configure UART hardware flow CTS.
  * @param[in] uart_num - UART0/UART1/UART2.
  * @param[in] cts_pin    - cts pin select.
  * @param[in] cts_parity -  1:Active high,when the cts pin receives a high level, it stops sending data.
  *                          0:Active low,when the cts pin receives a low level, it stops sending data.
  * @return    none
 */
void uart_cts_config(uart_num_e uart_num, gpio_func_pin_e cts_pin, unsigned char cts_parity)
{
    // When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    // otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en((gpio_pin_e)cts_pin); // enable input
    uart_set_cts_pin(uart_num, cts_pin);

    if (cts_parity) {
        reg_uart_ctrl1(uart_num) |= FLD_UART_TX_CTS_POLARITY;
    } else {
        reg_uart_ctrl1(uart_num) &= (~FLD_UART_TX_CTS_POLARITY);
    }
}

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
void uart_rts_config(uart_num_e uart_num, gpio_func_pin_e rts_pin, unsigned char rts_parity, unsigned char auto_mode_en)
{
    uart_set_rts_pin(uart_num, rts_pin);

    if (auto_mode_en) {
        reg_uart_ctrl2(uart_num) |= FLD_UART_RTS_MANUAL_M;
    } else {
        reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_MANUAL_M);
    }
    if (rts_parity) {
        reg_uart_ctrl2(uart_num) |= FLD_UART_RTS_POLARITY;
    } else {
        reg_uart_ctrl2(uart_num) &= (~FLD_UART_RTS_POLARITY);
    }
}

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
void uart_set_dma_chain_llp(uart_num_e uart_num, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    uart_dma_rev_size[uart_num] = data_len;
    uart_auto_clr_rx_fifo_ptr(uart_num, 1);
    // uart_set_dma_chain_llp interface dma_chain must be invoked,nodma does not call,so put the uart_rxdone_sel interface in this interface.
    uart_rxdone_sel(uart_num, UART_DMA_MODE);
    dma_config(chn, &uart_rx_dma_config[uart_num]);
    dma_set_address(chn, uart_base_addr(uart_num), (unsigned int)(dst_addr));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)(head_of_list);
}

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
void uart_rx_dma_add_list_element(uart_num_e uart_num, dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *dst_addr, unsigned int data_len)
{
    uart_auto_clr_rx_fifo_ptr(uart_num, 1);
    config_addr->dma_chain_ctl      = reg_dma_ctrl(chn) | BIT(0);
    config_addr->dma_chain_src_addr = uart_base_addr(uart_num);
    config_addr->dma_chain_dst_addr = (unsigned int)(dst_addr);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)(llpointer);
}

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
void uart_rx_dma_chain_init(uart_num_e uart_num, dma_chn_e chn, unsigned char *in_buff, unsigned int buff_size)
{
    uart_dma_rev_size[uart_num] = buff_size;
    uart_auto_clr_rx_fifo_ptr(uart_num, 1);
    uart_set_dma_chain_llp(uart_num, chn, (unsigned char *)in_buff, buff_size, &g_uart_rx_dma_list_cfg);
    uart_rx_dma_add_list_element(uart_num, chn, &g_uart_rx_dma_list_cfg, &g_uart_rx_dma_list_cfg, (unsigned char *)in_buff, buff_size);
    dma_chn_en(chn);
}

/**********************************************************************************************************************
 *                                          local function implementation                                             *
 *********************************************************************************************************************/
/**
 * @brief      This function is used to look for the prime.if the prime is found,it will return 1, or return 0.
 * @param[in]  n - the value to judge.
 * @return     0 or 1
 */
static unsigned char uart_is_prime(unsigned int n)
{
    unsigned int i = 5;
    if (n <= 3) {
        return 1; // although n is prime, the bwpc must be larger than 2.
    } else if ((n % 2 == 0) || (n % 3 == 0)) {
        return 0;
    } else {
        for (i = 5; i * i < n; i += 6) {
            if ((n % i == 0) || (n % (i + 2)) == 0) {
                return 0;
            }
        }
        return 1;
    }
}
