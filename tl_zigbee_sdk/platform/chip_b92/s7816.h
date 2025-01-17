/********************************************************************************************************
 * @file    s7816.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page S7816
 *
 * Introduction
 * ===============
 * The chip supports the establishment of communication with IC cards conforming to ISO-7816 protocol.
 * 
 * API Reference
 * ===============
 * Header File: s7816.h
 *
 * How to use this driver(S7816 protocol is realized by UART module of SoC)
 * ==============
  -# S7816 Initialization and Configuration
     -# nodma_trx:
        - Initialize the rst/vcc/clk/trx pin by s7816_set_pin() API;
        - Configure UART channels (UART0 and UART1), IC card clock, and adjust IC card clock frequency by F (default 372) and the special rate adjustment factor D (1 by default) by s7816_init() API;
        - After the configuration is completed, enable the 7816 module by s7816_en () API;
  -# UART Interrupts Configuration and Response
     -# UART interrupts initial configuration:
        - nodma_trx_tx
           - polling send, it does not need to configure interrupt;
        - nodma_trx_rx
           - uart_rx_irq_trig_level() / uart_set_irq_mask() : UART_RX_IRQ_MASK ;
        - core_interrupt_enable() and plic_interrupt_enable() API
     -# UART interrupt response(for details, see the enumeration uart_irq_status_e):
        - uart_get_irq_status() API;
        - nodma_trx_rx
            - UART_RXBUF_IRQ_STATUS;
  -# S7816 Use
     -# IC card activation and cold reset
        - Configure the vcc pin is pulled up, enable the clk pin output clock signal, configure the trx pin is set to the receive state, the reset pin is pulled up by s7816_coldreset() API;
        - Configure the s7816_rst_time time in a cold reset by s7816_set_time() API;
     -# warm reset
        - If the answer to reset received by the interrupt does not meet the specified requirements, the terminal will initiate a warm reset and obtain a reset signal from the IC card by s7816_warmreset() API;
     -# release
        - Configure the terminal initiates a contact release timing sequence by s7816_release_trig() API;
     -# s7816 send
        - Configure the terminal sends data by s7816_send_byte() API;
     -# s7816 receive
        - The trx pin is the rx function by default. When there is data in tx_fifo and the interface uart_rtx_pin_tx_trig is called, it is converted to tx function until tx_fifo is empty and converted to rx;
 */

#ifndef S7816_H_
#define S7816_H_

#include "gpio.h"
#include "uart.h"
#include "stimer.h"
#define s7816_en(uart_num)                         uart_rtx_en(uart_num)
#define s7816_set_rtx_pin(uart_num, s7816_trx_pin) uart_set_rtx_pin(uart_num, s7816_trx_pin)

/**
 *  @brief  Define 7816 clock.
 */
typedef enum
{
    S7816_4MHZ  = 4,
    S7816_6MHZ  = 6,
    S7816_12MHZ = 12,
} s7816_clock_e;

/**
 * @brief       This function is used to set the s7816 clock.
 * @param[in]   div - set the divider of clock of 7816 module.
 * @return      none.
 * @note        the clk-source of s7816 is 24M-pad,the clk of clk-pin can be divided as follow.
 *              div:        0x06-4Mhz     0x04-6Mhz   0x02-12Mhz
 *              baudrate:   0x06-10752    0x04-16194  0x02-32388
 */
extern void s7816_set_clk(unsigned char div);


/**
 * @brief       This function is used to initialize the s7816 module.
 * @param[in]   uart_num     - UART0 or UART1.
 * @param[in]   clock        - the clock of s7816.
 * @param[in]   f            - the clock frequency conversion factor of s7816.
 * @param[in]   d            - the bitrate regulator of s7816.
 * @return      none.
 */
extern void s7816_init(uart_num_e uart_num, s7816_clock_e clock, int f, int d);

/**
 * @brief       This function is used to active the IC card,set the trx pin and coldreset.
 * @param[in]   none.
 * @return      none.
 * @note        extra time is needed for initial-atr after the function.
 */
extern void s7816_coldreset(void);

/**
 * @brief       This function is used to set all the pin of s7816 module.
 * @param[in]   rst_pin     - the rst pin of s7816.
 * @param[in]   vcc_pin     - the vcc pin of s7816.
 * @param[in]   clk_pin     - the clk pin of s7816.
 * @param[in]   trx_pin     - the trx pin of s7816.
 * @return      none.
 */
extern void s7816_set_pin(gpio_pin_e rst_pin, gpio_pin_e vcc_pin, gpio_func_pin_e clk_pin, gpio_func_pin_e rtx_pin);

/**
 * @brief       This function is used to release the trigger
 * @param[in]   none.
 * @return      none.
 */
extern void s7816_release_trig(void);

/**
 * @brief       This function is used to set the RST pin of s7816.
 * @param[in]   pin_7816_rst - the RST pin of s7816.
 * @return      none.
 */
extern void s7816_set_rst_pin(gpio_pin_e pin_7816_rst);

/**
 * @brief       This function is used to set the VCC pin of s7816.
 * @param[in]   pin_7816_vcc - the VCC pin of s7816.
 * @return      none.
 */
extern void s7816_set_vcc_pin(gpio_pin_e pin_7816_vcc);

/**
 * @brief       This function is used to warmreset.
 * @param[in]   none.
 * @return      none.
 * @note        the warmreset is required after the IC-CARD active,extra time is needed for initial-atr after the function.
 */
extern void s7816_warmreset(void);

/**
 * @brief       This function is used to set the rst-wait time of the s7816 module.
 * @param[in]   rst_time_us - set the s7816_rst_time.
 * @return      none.
 */
extern void s7816_set_time(int rst_time_us);

/**
 * @brief       This function is used to warmreset.
 * @param[in]   uart_num - UART0 or UART1.
 * @param[in]   tx_data  - the data need to send.
 * return       none.
 */
extern void s7816_send_byte(uart_num_e uart_num, unsigned char tx_data);


#endif /* S7816_H_ */
