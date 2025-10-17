/********************************************************************************************************
 * @file    board_ml7218a_gaia.h
 *
 * @brief   This is the header file for board_ml7218a_gaia
 *
 * @author  Zigbee Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *                      All rights reserved.
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

/***************************************************************/
/* RGB or CCT or Brightness */
#define COLOR_RGB_SUPPORT       0
#define COLOR_CCT_SUPPORT       0
#define BRIGHTNESS_SUPPORT      0
/***************************************************************/

//BUTTON
//SW4 - TL_IO11
#define BUTTON1                 GPIO_PE3
#define PE3_FUNC                AS_GPIO
#define PE3_OUTPUT_ENABLE       0
#define PE3_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PE3     GPIO_PIN_PULLUP_10K

//SW6 - TL_IO09
#define BUTTON2                 GPIO_PE5
#define PE5_FUNC                AS_GPIO
#define PE5_OUTPUT_ENABLE       0
#define PE5_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PE5     GPIO_PIN_PULLUP_10K

//LED
/***************************************************************
* LED_W         GPIO_PE1        //D1  -- white
* LED_G         GPIO_PE2        //D2  -- green
****************************************************************/
#if defined(COLOR_RGB_SUPPORT) && (COLOR_RGB_SUPPORT == 1)

#elif defined(COLOR_CCT_SUPPORT) && (COLOR_CCT_SUPPORT == 1)

#elif defined(BRIGHTNESS_SUPPORT) && (BRIGHTNESS_SUPPORT == 1)

#else
//D1 - TL_IO13
#define LED_W                   GPIO_PE1
#define PE1_FUNC                AS_GPIO
#define PE1_OUTPUT_ENABLE       1
#define PE1_INPUT_ENABLE        0

//D2 - TL_IO12
#define LED_G                   GPIO_PE2
#define PE2_FUNC                AS_GPIO
#define PE2_OUTPUT_ENABLE       1
#define PE2_INPUT_ENABLE        0

#define COOL_LIGHT_GPIO         LED_W

#define LED_POWER               LED_G
#define LED_PERMIT              LED_G
#endif

//ADC
#if VOLTAGE_DETECT_ENABLE
#warning "not support."
#undef VOLTAGE_DETECT_ENABLE
#endif

//UART
#if ZBHCI_UART
#error please configurate uart PIN!!!!!!
#endif

//DEBUG
#if UART_PRINTF_MODE
//1 - use HW UART mode; 0 - use IO mode
#define CONSOLE_UART_ENABLE     0
#if CONSOLE_UART_ENABLE
#define CONSOLE_TX_PIN          //GPIO_PC6
#define CONSOLE_RX_PIN          //GPIO_PC7
#else
//TL_IO15
#define DEBUG_INFO_TX_PIN       GPIO_PB6//print
#endif
#endif

enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
};

#define KB_MAP_NORMAL           { \
                                    {VK_SW1,}, \
                                    {VK_SW2,}, \
                                }

#define KB_MAP_NUM              KB_MAP_NORMAL
#define KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {0}
#define KB_SCAN_PINS            {BUTTON1, BUTTON2}

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
