/********************************************************************************************************
 * @file    board_826x_evk.h
 *
 * @brief   This is the header file for board_826x_evk
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

//KEY
#define	BUTTON1                 GPIO_PD2
#define PD2_FUNC                AS_GPIO
#define PD2_OUTPUT_ENABLE       0
#define PD2_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PD2     PM_PIN_PULLUP_10K

#define	BUTTON2                 GPIO_PC7
#define PC7_FUNC                AS_GPIO
#define PC7_OUTPUT_ENABLE       0
#define PC7_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PC7     PM_PIN_PULLUP_10K

//LED
#define LED1                    GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       1
#define PB4_INPUT_ENABLE        0

#define LED2                    GPIO_PD6
#define PD6_FUNC                AS_GPIO
#define PD6_OUTPUT_ENABLE       1
#define PD6_INPUT_ENABLE        0

#define LED3                    GPIO_PD7
#define PD7_FUNC                AS_GPIO
#define PD7_OUTPUT_ENABLE       1
#define PD7_INPUT_ENABLE        0

#define LED4                    GPIO_PD5
#define PD5_FUNC                AS_GPIO
#define PD5_OUTPUT_ENABLE       1
#define PD5_INPUT_ENABLE        0

#define LED_POWER               LED3

#define	PM_WAKEUP_LEVEL         PM_WAKEUP_LEVEL_LOW

//ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  NOINPUT
#endif

//UART
#if ZBHCI_UART
#error please configurate uart PIN!!!!!!
#endif

//DEBUG
#if UART_PRINTF_MODE
#define	DEBUG_INFO_TX_PIN       GPIO_PC5//print
#endif


enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02
};

#define	KB_MAP_NORMAL           { \
                                    {VK_SW1,}, \
                                    {VK_SW2,}, \
                                }

#define	KB_MAP_NUM              KB_MAP_NORMAL
#define	KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {0}
#define KB_SCAN_PINS            {BUTTON1, BUTTON2}

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
