/********************************************************************************************************
 * @file    drv_console.c
 *
 * @brief   This is the source file for drv_console
 *
 * @author  Zigbee Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include "../tl_common.h"


#if UART_PRINTF_MODE
static void console_uart_set(u32 tx, u32 rx)
{
#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    uart_gpio_set(tx, rx);
#elif defined(MCU_CORE_B91)
    uart_set_pin(tx, rx);
#elif defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X)
    uart_set_pin(CONSOLE_UART_IDX, tx, rx);
#endif
}

static void console_uart_baudrate_set(u32 baudrate)
{
#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    uart_reset();
    uart_init_baudrate(baudrate, UART_CLOCK_SOURCE, PARITY_NONE, STOP_BIT_ONE);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X)
    u16 div = 0;
    u8 bwpc = 0;
    uart_reset(CONSOLE_UART_IDX);
    uart_cal_div_and_bwpc(baudrate, UART_CLOCK_SOURCE, &div, &bwpc);
    uart_init(CONSOLE_UART_IDX, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
#endif
}

static void console_uart_putc(const u8 byte)
{
#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    uart_send_byte(byte);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X)
    uart_send_byte(CONSOLE_UART_IDX, byte);
#endif
}
#endif

void drv_console_write(const u8 byte)
{
#if GSUART_PRINTF_MODE
    soft_uart_putc(byte);
#elif UART_PRINTF_MODE
    console_uart_putc(byte);
#elif USB_PRINTF_MODE
    hw_usb_putc(byte);
#endif
}

void drv_console_init(void)
{
#if CONSOLE_ENABLE
    tl_printf_register(drv_console_write);
#endif

#if GSUART_PRINTF_MODE
    DEBUG_TX_PIN_INIT();
#elif UART_PRINTF_MODE
    console_uart_set(CONSOLE_UART_TX_PIN, CONSOLE_UART_RX_PIN);
    console_uart_baudrate_set(CONSOLE_BAUDRATE);
#elif USB_PRINTF_MODE
    drv_usb_init();
#endif
}
