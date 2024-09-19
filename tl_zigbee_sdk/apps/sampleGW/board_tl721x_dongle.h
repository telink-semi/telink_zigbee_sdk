/********************************************************************************************************
 * @file    board_tl721x_dongle.h
 *
 * @brief   This is the header file for board_tl721x_dongle
 *
 * @author  Zigbee Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#define BUTTON1               		GPIO_PE7//SW1
#define PE7_FUNC			  		AS_GPIO
#define PE7_OUTPUT_ENABLE	  		0
#define PE7_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PE7	  		GPIO_PIN_PULLUP_1M//GPIO_PIN_PULLUP_10K

#define BUTTON2               		GPIO_PF5//SW2
#define PF5_FUNC			  		AS_GPIO
#define PF5_OUTPUT_ENABLE	  		0
#define PF5_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PF5	  		GPIO_PIN_PULLUP_1M//GPIO_PIN_PULLUP_10K

// LED
#define LED_Y     					GPIO_PA2//D3
#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define LED_R     					GPIO_PA1//D2
#define PA1_FUNC					AS_GPIO
#define PA1_OUTPUT_ENABLE			1
#define PA1_INPUT_ENABLE			0

#define LED_G     					GPIO_PF6//D1
#define PF6_FUNC					AS_GPIO
#define PF6_OUTPUT_ENABLE			1
#define PF6_INPUT_ENABLE			0

#define LED_B     					GPIO_PA3//D4
#define PA3_FUNC					AS_GPIO
#define PA3_OUTPUT_ENABLE			1
#define PA3_INPUT_ENABLE			0

#define LED_W     					GPIO_PA4//D5
#define PA4_FUNC					AS_GPIO
#define PA4_OUTPUT_ENABLE			1
#define PA4_INPUT_ENABLE			0

#define LED_O     					GPIO_PD4//D6
#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

#define VOLTAGE_DETECT_PIN			ADC_GPIO_PB7//todo

// UART
#if ZBHCI_UART
	#define UART_TX_PIN         	GPIO_PD1
	#define UART_RX_PIN         	GPIO_PD3

	#define UART_PIN_CFG()			drv_uart_pin_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC0//print
#endif

// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
	#define HW_USB_CFG()			do{ \
										usb_set_pin_en();	\
										usb_edp_en();		\
									}while(0)
#endif


enum{
	VK_SW1 = 0x01,
	VK_SW2 = 0x02,
};

#define	KB_MAP_NORMAL	{\
		{VK_SW1,}, \
		{VK_SW2,}, }

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  	{0}
#define KB_SCAN_PINS   	{BUTTON1, BUTTON2}



/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
