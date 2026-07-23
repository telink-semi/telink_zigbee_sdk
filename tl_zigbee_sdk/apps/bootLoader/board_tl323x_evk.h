/********************************************************************************************************
 * @file    board_tl323x_evk.h
 *
 * @brief   This is the header file for board_tl323x_evk
 *
 * @author  Zigbee Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#define BUTTON1                 GPIO_PC4
#define PC4_FUNC                AS_GPIO
#define PC4_OUTPUT_ENABLE       0
#define PC4_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PC4     GPIO_PIN_PULLDOWN_100K

//key2
#define BUTTON2                 GPIO_PC5
#define PC5_FUNC                AS_GPIO
#define PC5_OUTPUT_ENABLE       0
#define PC5_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PC5     GPIO_PIN_PULLDOWN_100K

//key3
#define BUTTON3                 GPIO_PC6
#define PC6_FUNC                AS_GPIO
#define PC6_OUTPUT_ENABLE       0
#define PC6_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PC6     GPIO_PIN_PULLUP_10K

//key4
#define BUTTON4                 GPIO_PC7
#define PC7_FUNC                AS_GPIO
#define PC7_OUTPUT_ENABLE       0
#define PC7_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PC7     GPIO_PIN_PULLUP_10K

//LED
#define LED_R                   GPIO_PC2
#define PC2_FUNC                AS_GPIO
#define PC2_OUTPUT_ENABLE       1
#define PC2_INPUT_ENABLE        0

#define LED_W                   GPIO_PC0
#define PC0_FUNC                AS_GPIO
#define PC0_OUTPUT_ENABLE       1
#define PC0_INPUT_ENABLE        0

#define LED_G                   GPIO_PC1
#define PC1_FUNC                AS_GPIO
#define PC1_OUTPUT_ENABLE       1
#define PC1_INPUT_ENABLE        0

#define LED_B                   GPIO_PC3
#define PC3_FUNC                AS_GPIO
#define PC3_OUTPUT_ENABLE       1
#define PC3_INPUT_ENABLE        0

#define LED_POWER               LED_R
#define LED_PERMIT              LED_G

//ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  SD_ADC_GPIO_PB0P
#endif

//UART
#if MODULE_UART_ENABLE
#define UART_IDX                UART0
#define UART_TX_PIN             GPIO_PB4
#define UART_RX_PIN             GPIO_PB5
#define UART_PIN_CFG()          drv_uart_pin_set(UART_TX_PIN, UART_RX_PIN);
#endif

//DEBUG
#if GSUART_PRINTF_MODE
#define CONSOLE_GPIO_TX_PIN     GPIO_PA0//print
#elif UART_PRINTF_MODE
#define CONSOLE_UART_IDX        UART1
#define CONSOLE_UART_TX_PIN     GPIO_PA1
#define CONSOLE_UART_RX_PIN     GPIO_PD1
#endif

//USB
#if MODULE_USB_ENABLE
#warning "TL323X does not support."
#endif

enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04
};

#define	KB_MAP_NORMAL           { \
                                    {VK_SW1, VK_SW3}, \
                                    {VK_SW2, VK_SW4}, \
                                }

#define	KB_MAP_NUM              KB_MAP_NORMAL
#define	KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {BUTTON1, BUTTON2}
#define KB_SCAN_PINS            {BUTTON3, BUTTON4}

#define	KB_LINE_MODE            0
#define	KB_LINE_HIGH_VALID      0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
