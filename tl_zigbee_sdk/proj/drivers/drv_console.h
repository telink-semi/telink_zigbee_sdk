/********************************************************************************************************
 * @file    drv_console.h
 *
 * @brief   This is the header file for drv_console
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
#pragma once

#ifndef CONSOLE_UART_IDX
#define CONSOLE_UART_IDX        UART1
#endif

#ifndef CONSOLE_UART_BAUDRATE
#define CONSOLE_UART_BAUDRATE   115200
#endif

void drv_console_pin_set(u32 tx, u32 rx);
void drv_console_init(u32 baudrate);
void drv_console_write(u8 byte);
