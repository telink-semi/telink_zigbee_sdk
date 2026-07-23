/********************************************************************************************************
 * @file    board_tl521x_evk.h
 *
 * @brief   This is the header file for board_tl521x_evk
 *
 * @author  Zigbee Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

//BUTTON
//key1
#define BUTTON1                 GPIO_PC2
#define PC2_FUNC                AS_GPIO
#define PC2_OUTPUT_ENABLE       0
#define PC2_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PC2     GPIO_PIN_PULLDOWN_100K

//key2
#define BUTTON2                 GPIO_PC3
#define PC3_FUNC                AS_GPIO
#define PC3_OUTPUT_ENABLE       0
#define PC3_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PC3     GPIO_PIN_PULLDOWN_100K

//key3
#define BUTTON3                 GPIO_PB1
#define PB1_FUNC                AS_GPIO
#define PB1_OUTPUT_ENABLE       0
#define PB1_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PB1     GPIO_PIN_PULLUP_20K

//key4
#define BUTTON4                 GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       0
#define PB4_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PB4     GPIO_PIN_PULLUP_20K

//LED
#define LED_W                   GPIO_PA0
#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       1
#define PA0_INPUT_ENABLE        0

#define LED_R                   GPIO_PA3
#define PA3_FUNC                AS_GPIO
#define PA3_OUTPUT_ENABLE       1
#define PA3_INPUT_ENABLE        0

#define LED_G                   GPIO_PA2
#define PA2_FUNC                AS_GPIO
#define PA2_OUTPUT_ENABLE       1
#define PA2_INPUT_ENABLE        0

#define LED_B                   GPIO_PA4
#define PA4_FUNC                AS_GPIO
#define PA4_OUTPUT_ENABLE       1
#define PA4_INPUT_ENABLE        0

#define LED_POWER               LED_R
#define LED_PERMIT              LED_G

//ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  ADC_GPIO_PB4//TODO
#endif

//UART
#if MODULE_UART_ENABLE
#define UART_IDX                UART0
#define UART_TX_PIN             GPIO_PB3
#define UART_RX_PIN             GPIO_PB2
#define UART_PIN_CFG()          drv_uart_pin_set(UART_TX_PIN, UART_RX_PIN)
#endif

//DEBUG
#if GSUART_PRINTF_MODE
#define CONSOLE_GPIO_TX_PIN     GPIO_PC7//print
#elif UART_PRINTF_MODE
#define CONSOLE_UART_IDX        UART1
#define CONSOLE_UART_TX_PIN     GPIO_PB3//TODO
#define CONSOLE_UART_RX_PIN     GPIO_PB2//TODO
#endif

//USB
#if MODULE_USB_ENABLE
#define HW_USB_CFG()            drv_usb_init()
#endif

enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04
};

#define KB_MAP_NORMAL           { \
                                    {VK_SW1, VK_SW3}, \
                                    {VK_SW4, VK_SW2}, \
                                }

#define KB_MAP_NUM              KB_MAP_NORMAL
#define KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {BUTTON1, BUTTON2}
#define KB_SCAN_PINS            {BUTTON3, BUTTON4}

#define KB_LINE_MODE            0
#define KB_LINE_HIGH_VALID      0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
