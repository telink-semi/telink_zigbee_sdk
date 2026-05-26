/********************************************************************************************************
 * @file    uart.c
 *
 * @brief   This is the source file for B91
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
#include "uart.h"

/**********************************************************************************************************************
 *                                            local constants                                                       *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              local macro                                                        *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
dma_config_t uart_tx_dma_config[2] = {
    {
     .dst_req_sel    = DMA_REQ_UART0_TX, //tx req
        .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT, //increment
        .dstmode        = DMA_HANDSHAKE_MODE, //handshake
        .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,                                                                                        //must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH, //must be word
        .src_burst_size = 0, //must be 0
        .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, //must be 0
    },
    {
     .dst_req_sel    = DMA_REQ_UART1_TX,                                   //tx req
        .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT,       //increment
        .dstmode        = DMA_HANDSHAKE_MODE,                                              //handshake
        .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, //must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH,                                              //must be word
        .src_burst_size = 0,                             //must be 0
        .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, //must be 0
    }
};
dma_config_t uart_rx_dma_config[2] = {
    {
     .dst_req_sel    = 0, //tx req
        .src_req_sel    = DMA_REQ_UART0_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, //must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH, ////must be word
        .src_burst_size = 0,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, //must be 0
    },
    {
     .dst_req_sel    = 0,                    //tx req
        .src_req_sel    = DMA_REQ_UART1_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, //must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH,                                              ////must be word
        .src_burst_size = 0,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, //must be 0
    }
};
/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/
static unsigned char uart_dma_tx_chn[2];
static unsigned char uart_dma_rx_chn[2];
static unsigned int  uart_dma_rev_size[2];
/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/
/**
  * @brief     This function is used to look for the prime.if the prime is found,it will return 1, or return 0.
  * @param[in] n - the value to judge.
  * @return    0 or 1
  */
static unsigned char uart_is_prime(unsigned int n);

/**
  * @brief  This function serves to set pin for UART function.
  * @param  tx_pin - To set TX pin.
  * @param  rx_pin - To set RX pin.
  * @return none
  */
static void uart_set_fuc_pin(uart_tx_pin_e tx_pin, uart_rx_pin_e rx_pin);

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

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
void uart_init(uart_num_e uart_num, unsigned short div, unsigned char bwpc, uart_parity_e parity, uart_stop_bit_e stop_bit)
{
    reg_uart_ctrl0(uart_num)   = ((reg_uart_ctrl0(uart_num) & (~FLD_UART_BPWC_O)) | bwpc); //set bwpc
    reg_uart_clk_div(uart_num) = (div | FLD_UART_CLK_DIV_EN);                              //set div_clock

    //parity config
    if (parity) {
        reg_uart_ctrl1(uart_num) |= FLD_UART_PARITY_ENABLE;          //enable parity function
        if (UART_PARITY_EVEN == parity) {
            reg_uart_ctrl1(uart_num) &= (~FLD_UART_PARITY_POLARITY); //enable even parity
        } else if (UART_PARITY_ODD == parity) {
            reg_uart_ctrl1(uart_num) |= FLD_UART_PARITY_POLARITY;    //enable odd parity
        }
    } else {
        reg_uart_ctrl1(uart_num) &= (~FLD_UART_PARITY_ENABLE);       //disable parity function
    }

    //stop bit config
    reg_uart_ctrl1(uart_num) = ((reg_uart_ctrl1(uart_num) & (~FLD_UART_STOP_SEL)) | stop_bit);
    //When dma receives data, the send length is greater than the receive length, and if the number of FIFOs exceeds uart_rts_trig_level_auto_mode,
    //rxdone cannot be generated normally, it is necessary to turn off the rts function and open it again if needed
    uart_set_rts_dis(uart_num);
}

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
        D_dec[i - 3]    = D_intdec[i - 3] - 10 * (D_intdec[i - 3] / 10); //get the decimal section
        D_int[i - 3]    = D_intdec[i - 3] / 10;                          //get the integer section
    }

    //find the max and min one decimation point
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
void uart_set_rx_timeout(uart_num_e uart_num, unsigned char bwpc, unsigned char bit_cnt, uart_timeout_mul_e mul)
{
    reg_uart_rx_timeout0(uart_num) = (bwpc + 1) * bit_cnt;                                                 //one byte includes 12 bits at most

    reg_uart_rx_timeout1(uart_num) = (((reg_uart_rx_timeout1(uart_num)) & (~FLD_UART_TIMEOUT_MUL)) | mul); //if over 2*(tmp_bwpc+1),one transaction end.
}

unsigned char uart_tx_byte_index[2] = {0};

/**
   * @brief     Send uart data by byte in no_dma mode.
   * @param[in] uart_num - UART0 or UART1.
   * @param[in] tx_data  - the data to be send.
   * @return    none
   */
void uart_send_byte(uart_num_e uart_num, unsigned char tx_data)
{
    while (uart_get_txfifo_num(uart_num) > 7)
        ;

    reg_uart_data_buf(uart_num, uart_tx_byte_index[uart_num]) = tx_data;
    uart_tx_byte_index[uart_num]++;
    (uart_tx_byte_index[uart_num]) &= 0x03;
}

unsigned char uart_rx_byte_index[2] = {0};

/**
 * @brief     Receive uart data by byte in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
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
 * @brief     Judge if the transmission of uart is done.
 * @param[in] uart_num - UART0 or UART1.
 * @return    0:tx is done     1:tx isn't done
 */
unsigned char uart_tx_is_busy(uart_num_e uart_num)
{
    return ((reg_uart_status2(uart_num) & FLD_UART_TX_DONE) ? 0 : 1);
}

/**
 * @brief     Send uart data by halfword in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] data  - the data to be send.
 * @return    none
 */
void uart_send_hword(uart_num_e uart_num, unsigned short data)
{
    static unsigned char uart_tx_hword_index[2] = {0};

    while (uart_get_txfifo_num(uart_num) > 6)
        ;

    reg_uart_data_hword_buf(uart_num, uart_tx_hword_index[uart_num]) = data;
    uart_tx_hword_index[uart_num]++;
    uart_tx_hword_index[uart_num] &= 0x01;
}

/**
 * @brief     Send uart data by word in no_dma mode.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] data - the data to be send.
 * @return    none
 */
void uart_send_word(uart_num_e uart_num, unsigned int data)
{
    while (uart_get_txfifo_num(uart_num) > 4)
        ;
    reg_uart_data_word_buf(uart_num) = data;
}

/**
 * @brief     Sets the RTS pin's level manually,this function is used in combination with uart_rts_manual_mode.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] polarity - set the output of RTS pin.
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
 *  @brief      Set pin for UART cts function,the pin connection mode:CTS<->RTS.
 *  @param[in]  cts_pin -To set cts pin.
 *  @return     none
 */
void uart_set_cts_pin(uart_cts_pin_e cts_pin)
{
    unsigned char val  = 0;
    unsigned char mask = 0xff;
    if (cts_pin == UART0_CTS_PA1) {
        mask = (unsigned char)~(BIT(2) | BIT(3));
        val  = BIT(2);
    } else if (cts_pin == UART0_CTS_PB6) {
        mask = (unsigned char)~(BIT(4) | BIT(5));
        val  = BIT(5);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (cts_pin == UART0_CTS_PD0) {
        mask = (unsigned char)~(BIT(0) | BIT(1));
        val  = 0;
    } else if (cts_pin == UART1_CTS_PC4) {
        mask = (unsigned char)~(BIT(0) | BIT(1));
        val  = BIT(1);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (cts_pin == UART1_CTS_PD4) {
        mask = (unsigned char)~(BIT(0) | BIT(1));
        val  = 0;
    } else if (cts_pin == UART1_CTS_PE1) {
        mask = (unsigned char)~(BIT(2) | BIT(3));
        val  = BIT(2);
    }
    reg_gpio_func_mux(cts_pin) = (reg_gpio_func_mux(cts_pin) & mask) | val;
    gpio_function_dis(cts_pin);
}

/**
 *  @brief      Set pin for UART rts function,the pin connection mode:RTS<->CTS.
 *  @param[in]  rts_pin - To set rts pin.
 *  @return     none
 */
void uart_set_rts_pin(uart_rts_pin_e rts_pin)
{
    unsigned char val  = 0;
    unsigned char mask = 0xff;
    if (rts_pin == UART0_RTS_PA2) {
        mask = (unsigned char)~(BIT(4) | BIT(5));
        val  = BIT(4);
    } else if (rts_pin == UART0_RTS_PB4) {
        mask = (unsigned char)~(BIT(0) | BIT(1));
        val  = BIT(1);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (rts_pin == UART0_RTS_PD1) {
        mask = (unsigned char)~(BIT(2) | BIT(3));
        val  = 0;
    } else if (rts_pin == UART1_RTS_PC5) {
        mask = (unsigned char)~(BIT(2) | BIT(3));
        val  = BIT(3);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (rts_pin == UART1_RTS_PD5) {
        mask = (unsigned char)~(BIT(2) | BIT(3));
        val  = 0;
    } else if (rts_pin == UART1_RTS_PE3) {
        mask = (unsigned char)~(BIT(6) | BIT(7));
        val  = BIT(6);
    }
    reg_gpio_func_mux(rts_pin) = (reg_gpio_func_mux(rts_pin) & mask) | val;
    gpio_function_dis(rts_pin);
}

/**
* @brief      Select pin for UART module,the pin connection mode:TX<->RX RX<->TX.
* @param[in]  tx_pin   - the pin to send data.
* @param[in]  rx_pin   - the pin to receive data.
* @return     none
*/
void uart_set_pin(uart_tx_pin_e tx_pin, uart_rx_pin_e rx_pin)
{
    //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    if (tx_pin != UART_TX_NONE_PIN) {
        gpio_input_en(tx_pin);
        gpio_set_up_down_res(tx_pin, GPIO_PIN_PULLUP_10K);
    }
    if (rx_pin != UART_RX_NONE_PIN) {
        gpio_input_en(rx_pin);
        gpio_set_up_down_res(rx_pin, GPIO_PIN_PULLUP_10K);
    }
    uart_set_fuc_pin(tx_pin, rx_pin); //set tx and rx pin
}

/**
* @brief      Set rtx pin for UART module,this pin can be used as either tx or rx,it is the rx function by default,
*             When there is data in tx_fifo and the interface uart_rtx_pin_tx_trig is called, it is converted to tx function until tx_fifo is empty and converted to rx.
* @param[in]  rtx_pin  - the rtx pin need to set.
* @return     none
*/
void uart_set_rtx_pin(uart_rx_pin_e rtx_pin)
{
    unsigned char val  = 0;
    unsigned char mask = 0xff;
    if (rtx_pin == UART0_RX_PA4) {
        mask = (unsigned char)~(BIT(1) | BIT(0));
        val  = BIT(0);
    } else if (rtx_pin == UART0_RX_PB3) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = BIT(7);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (rtx_pin == UART0_RX_PD3) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = 0;
    } else if (rtx_pin == UART1_RX_PC7) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = BIT(7);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (rtx_pin == UART1_RX_PD7) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = 0;
    } else if (rtx_pin == UART1_RX_PE2) {
        mask = (unsigned char)~(BIT(5) | BIT(4));
        val  = BIT(4);
    }
    //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en(rtx_pin);
    gpio_set_up_down_res(rtx_pin, GPIO_PIN_PULLUP_10K);
    reg_gpio_func_mux(rtx_pin) = (reg_gpio_func_mux(rtx_pin) & mask) | val;
    gpio_function_dis(rtx_pin);
}

/**
* @brief     Send an amount of data in NODMA mode
* @param[in] uart_num - UART0 or UART1.
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
 * @brief       Send an amount of data in DMA mode
 * @param[in]   uart_num - uart channel
 * @param[in]   addr     - Pointer to data buffer. It must be 4-bytes aligned address
 * @param[in]   len      - Amount of data to be sent in bytes, range from 1 to 0xFFFFFC
 * @return      1  DMA start send.
 *              0  the length is error.
 */
unsigned char uart_send_dma(uart_num_e uart_num, unsigned char *addr, unsigned int len)
{
    dma_chn_dis(uart_dma_tx_chn[uart_num]); /*Use DMA to send data and go to suspend sleep before the data is sent completely . 
                                             after the sleep is awakened up, need to call dma_chn_dis() first and then configure DMA,
                                             and then DMA will continue to send data. If don't call dma_chn_dis(), configure DMA directly.
                                             After configuration, DMA will not work.*/
    if (len != 0) {
        //In order to prevent the time between the last piece of data and the next piece of data is less than the set timeout time,
        //causing the receiver to treat the next piece of data as the last piece of data.
        uart_clr_tx_done(uart_num);
        dma_set_address(uart_dma_tx_chn[uart_num], (unsigned int)(addr), reg_uart_data_buf_adr(uart_num));
        dma_set_size(uart_dma_tx_chn[uart_num], len, DMA_WORD_WIDTH);
        dma_chn_en(uart_dma_tx_chn[uart_num]);
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief      Receive an amount of data in DMA mode
 * @param[in]  uart_num - UART0 or UART1.
 * @param[in]  addr     - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *                        and the actual buffer size defined by the user needs to be not smaller than the rev_size, otherwise there may be an out-of-bounds problem.
 * @param[in]  rev_size - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
 * @return     none
 */
void uart_receive_dma(uart_num_e uart_num, unsigned char *addr, unsigned int rev_size)
{
    uart_dma_rev_size[uart_num] = rev_size;
    dma_chn_dis(uart_dma_rx_chn[uart_num]);
    /*In order to be able to receive data of unknown length(A0 doesn't support),the DMA SIZE is set to the longest value 0xffffffff.After entering suspend and wake up, and then continue to receive,
    DMA will no longer move data from uart fifo, because DMA thinks that the last transmission was not completed and must disable dma_chn first.modified by minghai,confirmed qiangkai 2020.11.26.*/
    dma_set_address(uart_dma_rx_chn[uart_num], reg_uart_data_buf_adr(uart_num), (unsigned int)(addr));
    dma_set_size(uart_dma_rx_chn[uart_num], rev_size, DMA_WORD_WIDTH);
    dma_chn_en(uart_dma_rx_chn[uart_num]);
}

/**
  * @brief     Get the length of the data that dma received.
  * @param[in] uart_num - UART0 or UART1.
  * @param[in] chn      - dma channel.
  * @return    data length.
  */
unsigned int uart_get_dma_rev_data_len(uart_num_e uart_num, dma_chn_e chn)
{
    unsigned int data_len      = 0;
    unsigned int buff_data_len = (reg_uart_status1(uart_num) & FLD_UART_RBCNT) % 4;
    if (buff_data_len == 0) {
        data_len = 4 * ((uart_dma_rev_size[uart_num] / 4) - reg_dma_size(chn));
    } else {
        data_len = 4 * ((uart_dma_rev_size[uart_num] / 4) - reg_dma_size(chn) - 1) + buff_data_len;
    }
    return data_len;
}

/**
  * @brief     Configures the uart tx_dma channel control register.
  * @param[in] uart_num - UART0 or UART1.
  * @param[in] chn      - dma channel.
  * @return    none
  * @note      In the case that the DMA transfer is not completed(bit 0 of reg_dma_ctr0(chn): 1-the transmission has not been completed,0-the transmission is completed), re-calling the DMA-related functions may cause problems.
  *            If you must do this, you must perform the following sequence:
  *            1. dma_chn_dis(uart_dma_tx_chn[uart_num]) 2.uart_hw_fsm_reset() 3.uart_send_dma()
  */
void uart_set_tx_dma_config(uart_num_e uart_num, dma_chn_e chn)
{
    uart_dma_tx_chn[uart_num] = chn;
    dma_config(chn, &uart_tx_dma_config[uart_num]);
}

/**
  * @brief     Configures uart rx_dma channel control register.
  * @param[in] uart_num - UART0 or UART1.
  * @param[in] chn      - dma channel.
  * @return    none
  * @note      In the case that the DMA transfer is not completed(bit 0 of reg_dma_ctr0(chn): 1-the transmission has not been completed,0-the transmission is completed), re-calling the DMA-related functions may cause problems.
  *            If you must do this, you must perform the following sequence:
  *            1. dma_chn_dis(uart_dma_rx_chn[uart_num]) 2.uart_hw_fsm_reset() 3.uart_receive_dma()
  */
void uart_set_rx_dma_config(uart_num_e uart_num, dma_chn_e chn)
{
    uart_dma_rx_chn[uart_num] = chn;
    dma_config(chn, &uart_rx_dma_config[uart_num]);
}

/**
   * @brief     Configure UART hardware flow CTS.
   * @param[in] uart_num   - UART0 or UART1.
   * @param[in] cts_pin    - cts pin select.
   * @param[in] cts_parity -  1:Active high,when the cts receives an active level, it stops sending data.
   *                          0:Active low
   * @return    none
   */
void uart_cts_config(uart_num_e uart_num, uart_cts_pin_e cts_pin, unsigned char cts_parity)
{
    //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en(cts_pin); //enable input
    uart_set_cts_pin(cts_pin);
    if (cts_parity) {
        reg_uart_ctrl1(uart_num) |= FLD_UART_TX_CTS_POLARITY;
    } else {
        reg_uart_ctrl1(uart_num) &= (~FLD_UART_TX_CTS_POLARITY);
    }
}

/**
  * @brief     Configure UART hardware flow RTS.
  * @param[in] uart_num     - UART0 or UART1.
  * @param[in] rts_pin      - RTS pin select.
  * @param[in] rts_parity   - 0: Active high  1: Active low
  *                           in auto mode,when the condition is reached, the rts active level is activated, and in manual mode, the rts level can be manually controlled.
  * @param[in] auto_mode_en - set the mode of RTS(auto or manual).
  * @return    none
  */
void uart_rts_config(uart_num_e uart_num, uart_rts_pin_e rts_pin, unsigned char rts_parity, unsigned char auto_mode_en)
{
    uart_set_rts_pin(rts_pin);

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

/**********************************************************************************************************************
  *                                         local function implementation                                             *
  *********************************************************************************************************************/
/**
   * @brief     This function is used to look for the prime.if the prime is found,it will return 1, or return 0.
   * @param[in] n - the value to judge.
   * @return    0 or 1
   */
static unsigned char uart_is_prime(unsigned int n)
{
    unsigned int i = 5;
    if (n <= 3) {
        return 1; //although n is prime, the bwpc must be larger than 2.
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

/**
  * @brief  This function serves to set pin for UART function.
  * @param  tx_pin - To set TX pin.
  * @param  rx_pin - To set RX pin.
  * @return none
  */
static void uart_set_fuc_pin(uart_tx_pin_e tx_pin, uart_rx_pin_e rx_pin)
{
    unsigned char val  = 0;
    unsigned char mask = 0xff;

    if (tx_pin == UART0_TX_PA3) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = BIT(6);
    } else if (tx_pin == UART0_TX_PB2) {
        mask = (unsigned char)~(BIT(5) | BIT(4));
        val  = BIT(5);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (tx_pin == UART0_TX_PD2) {
        mask = (unsigned char)~(BIT(5) | BIT(4));
        val  = 0;
    } else if (tx_pin == UART1_TX_PC6) {
        mask = (unsigned char)~(BIT(5) | BIT(4));
        val  = BIT(5);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (tx_pin == UART1_TX_PD6) {
        mask = (unsigned char)~(BIT(5) | BIT(4));
        val  = 0;
    } else if (tx_pin == UART1_TX_PE0) {
        mask = (unsigned char)~(BIT(1) | BIT(0));
        ;
        val = BIT(0);
    }
    if (tx_pin != UART_TX_NONE_PIN) {
        reg_gpio_func_mux(tx_pin) = (reg_gpio_func_mux(tx_pin) & mask) | val;
        gpio_function_dis(tx_pin);
    }


    if (rx_pin == UART0_RX_PA4) {
        mask = (unsigned char)~(BIT(1) | BIT(0));
        val  = BIT(0);

    } else if (rx_pin == UART0_RX_PB3) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = BIT(7);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (rx_pin == UART0_RX_PD3) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = 0;
    } else if (rx_pin == UART1_RX_PC7) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = BIT(7);
        reg_gpio_pad_mul_sel |= BIT(0);
    } else if (rx_pin == UART1_RX_PD7) {
        mask = (unsigned char)~(BIT(7) | BIT(6));
        val  = 0;
    } else if (rx_pin == UART1_RX_PE2) {
        mask = (unsigned char)~(BIT(5) | BIT(4));
        val  = BIT(4);
    }
    //note:  setting pad the function  must before  setting no_gpio function, cause it will lead to uart transmit extra one byte data at begin.(confirmed by minghai&sunpeng)
    if (rx_pin != UART_RX_NONE_PIN) {
        reg_gpio_func_mux(rx_pin) = (reg_gpio_func_mux(rx_pin) & mask) | val;
        gpio_function_dis(rx_pin);
    }
}
