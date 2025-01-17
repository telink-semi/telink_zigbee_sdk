/********************************************************************************************************
 * @file    board_tl321x_dongle.h
 *
 * @brief   This is the header file for board_tl321x_dongle
 *
 * @author  Zigbee Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

// BUTTON
#define BUTTON1                 GPIO_PE7//SW1
#define PE7_FUNC                AS_GPIO
#define PE7_OUTPUT_ENABLE       0
#define PE7_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PE7     GPIO_PIN_PULLUP_10K

#define BUTTON2                 GPIO_PE6//SW2
#define PE6_FUNC                AS_GPIO
#define PE6_OUTPUT_ENABLE       0
#define PE6_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PE6     GPIO_PIN_PULLUP_10K

// LED
#define LED_Y                   GPIO_PB2//D7
#define PB2_FUNC                AS_GPIO
#define PB2_OUTPUT_ENABLE       1
#define PB2_INPUT_ENABLE        0

#define LED_R                   GPIO_PD0//D9
#define PD0_FUNC                AS_GPIO
#define PD0_OUTPUT_ENABLE       1
#define PD0_INPUT_ENABLE        0

#define LED_G                   GPIO_PB0//D6
#define PB0_FUNC                AS_GPIO
#define PB0_OUTPUT_ENABLE       1
#define PB0_INPUT_ENABLE        0

#define LED_W                   GPIO_PB3//D5
#define PB3_FUNC                AS_GPIO
#define PB3_OUTPUT_ENABLE       1
#define PB3_INPUT_ENABLE        0

#define LED_B                   GPIO_PB1//D8
#define PB1_FUNC                AS_GPIO
#define PB1_OUTPUT_ENABLE       1
#define PB1_INPUT_ENABLE        0

#define LED_R1                  GPIO_PD5//D10
#define PD5_FUNC                AS_GPIO
#define PD5_OUTPUT_ENABLE       1
#define PD5_INPUT_ENABLE        0

#define LED_POWER               LED_R
#define LED_PERMIT              LED_G

// ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  ADC_GPIO_PB6
#endif

// UART
#if ZBHCI_UART
#define UART_TX_PIN             GPIO_PA0
#define UART_RX_PIN             GPIO_PA1

#define UART_PIN_CFG()          drv_uart_pin_set(UART_TX_PIN, UART_RX_PIN);//uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
//1 - use HW UART mode; 0 - use IO mode
#define CONSOLE_UART_ENABLE     0
#if CONSOLE_UART_ENABLE
#define CONSOLE_TX_PIN          GPIO_PC6
#define CONSOLE_RX_PIN          GPIO_PC7
#else
#define	DEBUG_INFO_TX_PIN       GPIO_PC6//print
#endif
#endif

// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#define HW_USB_CFG()            do{ \
                                    usb_set_pin_en(); \
                                    usb_edp_en();     \
                                }while(0)
#endif


enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
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
