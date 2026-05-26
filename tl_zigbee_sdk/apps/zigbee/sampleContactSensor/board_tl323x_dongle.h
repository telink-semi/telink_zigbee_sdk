/********************************************************************************************************
 * @file    board_tl323x_dongle.h
 *
 * @brief   This is the header file for board_tl323x_dongle
 *
 * @author  Zigbee Group
 * @date    2026
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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


// BUTTON
#define BUTTON1                     GPIO_PA2    //SW1
#define PA2_FUNC                    AS_GPIO
#define PA2_OUTPUT_ENABLE           0
#define PA2_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PA2         GPIO_PIN_PULLUP_10K

#define BUTTON2                     GPIO_PA4    //SW2
#define PA4_FUNC                    AS_GPIO
#define PA4_OUTPUT_ENABLE           0
#define PA4_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PA4         GPIO_PIN_PULLUP_10K

// LED
#define LED_Y                       GPIO_PD0//D4
#define PD0_FUNC                    AS_GPIO
#define PD0_OUTPUT_ENABLE           1
#define PD0_INPUT_ENABLE            0

#define LED_R                       GPIO_PB0//D2
#define PB0_FUNC                    AS_GPIO
#define PB0_OUTPUT_ENABLE           1
#define PB0_INPUT_ENABLE            0

#define LED_G                       GPIO_PB1//D1
#define PB1_FUNC                    AS_GPIO
#define PB1_OUTPUT_ENABLE           1
#define PB1_INPUT_ENABLE            0

#define LED_W                       GPIO_PD2//D6
#define PD2_FUNC                    AS_GPIO
#define PD2_OUTPUT_ENABLE           1
#define PD2_INPUT_ENABLE            0

#define LED_B                       GPIO_PD1//D5
#define PD1_FUNC                    AS_GPIO
#define PD1_OUTPUT_ENABLE           1
#define PD1_INPUT_ENABLE            0

#define LED_O                       GPIO_PD3//D7
#define PD3_FUNC                    AS_GPIO
#define PD3_OUTPUT_ENABLE           1
#define PD3_INPUT_ENABLE            0

#define LED_POWER                   LED_R
#define LED_PERMIT                  LED_G

// ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  SD_ADC_GPIO_PB7P
#endif

// UART
#if MODULE_UART_ENABLE
#error please configurate uart PIN!!!!!!
#endif

//DEBUG
#if GSUART_PRINTF_MODE
#define CONSOLE_GPIO_TX_PIN     GPIO_PB6//print
#elif UART_PRINTF_MODE
#define CONSOLE_UART_IDX        UART1
#define CONSOLE_UART_TX_PIN     GPIO_PB6
#define CONSOLE_UART_RX_PIN     GPIO_PB7
#endif

enum{
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04
};

#define KB_MAP_NORMAL   {\
        {VK_SW2,}, \
        {VK_SW1,}, }

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS   {0}
#define KB_SCAN_PINS    {BUTTON1, BUTTON2}

#if PM_ENABLE
#define BUTTON_WAKEUP_CFG       { \
                                    {BUTTON1, PM_WAKEUP_LEVEL_LOW}, \
                                    {BUTTON2, PM_WAKEUP_LEVEL_LOW}, \
                                }
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
