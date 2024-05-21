/********************************************************************************************************
 * @file    board_b92_evk.h
 *
 * @brief   This is the header file for board_b92_evk
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


// BUTTON
//sw2
#define BUTTON1               		GPIO_PD2
#define PD2_FUNC			  		AS_GPIO
#define PD2_OUTPUT_ENABLE	  		0
#define PD2_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PD2	  		GPIO_PIN_PULLUP_1M

//sw3
#define BUTTON2               		GPIO_PD7
#define PD7_FUNC			  		AS_GPIO
#define PD7_OUTPUT_ENABLE	  		0
#define PD7_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PD7	  		GPIO_PIN_PULLUP_1M

//sw4
#define BUTTON3               		GPIO_PD6
#define PD6_FUNC			  		AS_GPIO
#define PD6_OUTPUT_ENABLE	  		0
#define PD6_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PD6	  		GPIO_PIN_PULLUP_1M

//sw5
#define BUTTON4               		GPIO_PF6
#define PF6_FUNC			  		AS_GPIO
#define PF6_OUTPUT_ENABLE	  		0
#define PF6_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PF6	  		GPIO_PIN_PULLUP_1M

// LED
#define LED_R     					GPIO_PE7
#define PE7_FUNC					AS_GPIO
#define PE7_OUTPUT_ENABLE			1
#define PE7_INPUT_ENABLE			0

#define LED_W     					GPIO_PE6
#define PE6_FUNC					AS_GPIO
#define PE6_OUTPUT_ENABLE			1
#define PE6_INPUT_ENABLE			0

#define LED_G     					GPIO_PD1
#define PD1_FUNC					AS_GPIO
#define PD1_OUTPUT_ENABLE			1
#define PD1_INPUT_ENABLE			0

#define LED_B     					GPIO_PD0
#define PD0_FUNC					AS_GPIO
#define PD0_OUTPUT_ENABLE			1
#define PD0_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

#define VOLTAGE_DETECT_PIN			ADC_GPIO_PB7

// UART
#if UART_ENABLE
	#define UART_TX_PIN         	GPIO_PB1
	#define UART_RX_PIN         	GPIO_PB2

	#define UART_PIN_CFG()			drv_uart_pin_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PE5//print
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
	VK_SW3 = 0x03,
	VK_SW4 = 0x04
};

#define	KB_MAP_NORMAL	{\
		{VK_SW1, VK_SW3}, \
		{VK_SW2, VK_SW4}, }

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  	{GPIO_PD2, GPIO_PD7}
#define KB_SCAN_PINS   	{GPIO_PD6, GPIO_PF6}

#define	KB_LINE_MODE		0
#define	KB_LINE_HIGH_VALID	0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
