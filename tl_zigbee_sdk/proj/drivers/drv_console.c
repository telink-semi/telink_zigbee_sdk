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

void drv_console_pin_set(u32 tx, u32 rx)
{
#if defined(MCU_CORE_B91)
    uart_set_pin(tx, rx);
#elif defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    uart_set_pin(CONSOLE_UART_IDX, tx, rx);
#endif
}

void drv_console_init(u32 baudrate)
{
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    u16 div = 0;
    u8 bwpc = 0;
    uart_reset(CONSOLE_UART_IDX);
    uart_cal_div_and_bwpc(baudrate, UART_CLOCK_SOURCE, &div, &bwpc);
    uart_init(CONSOLE_UART_IDX, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
#endif
}

void drv_console_write(u8 byte)
{
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    uart_send_byte(CONSOLE_UART_IDX, byte);
#endif
}
