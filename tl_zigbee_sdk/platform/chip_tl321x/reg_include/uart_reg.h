/********************************************************************************************************
 * @file    uart_reg.h
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
#ifndef UART_REG_H
#define UART_REG_H
#include "soc.h"

/******************************* uart0 registers: 0x140080 *******************************/
/******************************* uart1 registers: 0x1400c0 *******************************/
/******************************* uart2 registers: 0x1402C0 *******************************/
#define uart_base_addr(i)             (((i) < 2) ? (0x140080 + (i) * 0x40) : (0x1402C0)) /* uart[0-2] */

#define reg_uart_data_buf(i, j)       REG_ADDR8(uart_base_addr(i) + (j))                 /* uart[0-2] buf[0-3] */
#define reg_uart_data_hword_buf(i, j) REG_ADDR16(uart_base_addr(i) + (j) * 2)
#define reg_uart_data_word_buf(i)     REG_ADDR32(uart_base_addr(i))                      /* uart[0-2] */

#define reg_uart_clk_div(i)           REG_ADDR16(uart_base_addr(i) + 0x04)

enum
{
    FLD_UART_CLK_DIV    = BIT_RNG(0, 14),
    FLD_UART_CLK_DIV_EN = BIT(15)
};

#define reg_uart_ctrl0(i) REG_ADDR8(uart_base_addr(i) + 0x06)

enum
{
    FLD_UART_BPWC_O           = BIT_RNG(0, 3),
    FLD_UART_RX_CLR_EN        = BIT(4),
    FLD_UART_NDMA_RXDONE_EN   = BIT(5), /**< just for ndma mode,dma mode must close. */
    FLD_UART_RXTIMEOUT_RTS_EN = BIT(6),
    FLD_UART_S7816_EN         = BIT(7),
};

#define reg_uart_ctrl1(i) REG_ADDR8(uart_base_addr(i) + 0x07)

enum
{
    FLD_UART_TX_CTS_POLARITY = BIT(0),
    FLD_UART_TX_CTS_ENABLE   = BIT(1),
    FLD_UART_PARITY_ENABLE   = BIT(2),
    FLD_UART_PARITY_POLARITY = BIT(3), /**< 1:odd parity   0:even parity. */
    FLD_UART_STOP_SEL        = BIT_RNG(4, 5),
    FLD_UART_TTL_ENABLE      = BIT(6),
    FLD_UART_LOOPBACK_O      = BIT(7),
};

#define reg_uart_ctrl2(i) REG_ADDR8(uart_base_addr(i) + 0x08)

enum
{
    FLD_UART_RTS_TRIQ_LEV = BIT_RNG(0, 3),
    FLD_UART_RTS_POLARITY = BIT(4),
    FLD_UART_RTS_MANUAL_V = BIT(5),
    FLD_UART_RTS_MANUAL_M = BIT(6),
    FLD_UART_RTS_EN       = BIT(7),
};

#define reg_uart_ctrl3(i) REG_ADDR8(uart_base_addr(i) + 0x09)

enum
{
    FLD_UART_RX_IRQ_TRIQ_LEV = BIT_RNG(0, 3),
    FLD_UART_TX_IRQ_TRIQ_LEV = BIT_RNG(4, 7),
};

#define reg_uart_rx_timeout0(i) REG_ADDR8(uart_base_addr(i) + 0x0a)

enum
{
    FLD_UART_TIMEOUT_BW = BIT_RNG(0, 7),
};

#define reg_uart_rx_timeout1(i) REG_ADDR8(uart_base_addr(i) + 0x0b)
#define reg_uart_mask(i)        REG_ADDR8(uart_base_addr(i) + 0x0b)

enum
{
    FLD_UART_TIMEOUT_MUL  = BIT_RNG(0, 1),
    FLD_UART_MASK_RX_IRQ  = BIT(2),
    FLD_UART_MASK_TX_IRQ  = BIT(3),
    FLD_UART_MASK_RXDONE  = BIT(4),
    FLD_UART_MASK_TXDONE  = BIT(5),
    FLD_UART_MASK_ERR_IRQ = BIT(6)
};

#define reg_uart_buf_cnt(i) REG_ADDR8(uart_base_addr(i) + 0x0c)

enum
{
    FLD_UART_RX_BUF_CNT = BIT_RNG(0, 3),
    FLD_UART_TX_BUF_CNT = BIT_RNG(4, 7),
};

#define reg_uart_status(i) REG_ADDR8(uart_base_addr(i) + 0x0d)

enum
{
    FLD_UART_RBCNT  = BIT_RNG(0, 2),
    FLD_UART_IRQ_O  = BIT(3),
    FLD_UART_WBCNT  = BIT_RNG(4, 6),
    FLD_UART_RXDONE = BIT(7),
};

#define reg_uart_irq(i) REG_ADDR8(uart_base_addr(i) + 0x0e)

enum
{
    FLD_UART_RX_MEM_CNT = BIT_RNG(0, 1),
    FLD_UART_RX_BUF_IRQ = BIT(2),
    FLD_UART_TX_BUF_IRQ = BIT(3),
    FLD_UART_RXDONE_IRQ = BIT(4),
    FLD_UART_TXDONE_IRQ = BIT(5),
    FLD_UART_RX_ERR_IRQ = BIT(6),
    FLD_UART_TIMEOUT    = BIT(7),
};

/**
 * @brief state machine use for IC debug.
 */
#define reg_uart_state(i) REG_ADDR8(uart_base_addr(i) + 0x0f)

enum
{
    FLD_UART_TSTATE_i = BIT_RNG(0, 2), /**< only for dma default 1.*/
    FLD_UART_RX_FULL  = BIT(3),
    FLD_UART_RSTATE_i = BIT_RNG(4, 7),
};

#define reg_uart_ctrl4(i) REG_ADDR8(uart_base_addr(i) + 0x10)

enum
{
    FLD_UART_RXDONE_RTS_EN         = BIT(0),
    FLD_UART_TIMEOUT_EN            = BIT(1),
    FLD_UART_RX_TIMEOUT_RELOAD_SEL = BIT(2),
    FLD_UART_RTS_STOP_TIMEOUT_EN   = BIT(3),
    FLD_UART_PEM_EVENT_EN          = BIT(4),
    FLD_UART_EN                    = BIT(6),
};

#define reg_uart_rxtimeout_o_exp(i) REG_ADDR8(uart_base_addr(i) + 0x11)

#define reg_uart_pem_task_en(i)     REG_ADDR8(uart_base_addr(i) + 0x12)

enum
{
    FLD_UART_PEM_TASK_EN = BIT_RNG(0, 4),
};

/******************************* 7816 registers: 0x140830 ******************************/
#define reg_7816_clk_div REG_ADDR8(0x140830)

enum
{
    FLD_7816_CLK_DIV = BIT_RNG(4, 6),
    FLD_7816_CLK_EN  = BIT(7),
};

#endif
