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

#ifndef CONSOLE_BAUDRATE
#define CONSOLE_BAUDRATE                                115200
#endif

#if GSUART_PRINTF_MODE
    #if !defined(CONSOLE_GPIO_TX_PIN)
        #error "CONSOLE_GPIO_TX_PIN undefined"
    #else
        #define DEBUG_INFO_TX_PIN                       CONSOLE_GPIO_TX_PIN
    #endif
#elif UART_PRINTF_MODE
    #if defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || \
        defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || \
        defined(MCU_CORE_TL521X)
        #ifndef CONSOLE_UART_IDX
            #define CONSOLE_UART_IDX                    UART1
        #endif
    #endif

    #if !defined(CONSOLE_UART_TX_PIN) || !defined(CONSOLE_UART_RX_PIN)
        #error "CONSOLE_UART_TX_PIN and CONSOLE_UART_RX_PIN undefined"
    #endif
#elif USB_PRINTF_MODE

#endif

#if CONSOLE_ENABLE
#define TRACE(...)                                      printf(__VA_ARGS__)

#define DEBUG(compileFlag, ...)                         do {                                    \
                                                            if(compileFlag) TRACE(__VA_ARGS__); \
                                                        } while(0)

#define DEBUG_ARRAY(compileFlag, arrayAddr, len)        do {                                                            \
                                                            if (compileFlag) {                                          \
                                                                TRACE("[");                                             \
                                                                unsigned char i = 0;                                    \
                                                                do {                                                    \
                                                                    TRACE(" %02x", ((unsigned char *)arrayAddr)[i++]);  \
                                                                } while(i < len);                                       \
                                                                TRACE(" ]\n");                                          \
                                                            }                                                           \
                                                        } while(0)
#else
#define TRACE(...)
#define DEBUG(compileFlag, ...)
#define DEBUG_ARRAY(compileFlag, arrayAddr, len)
#endif

void drv_console_init(void);
