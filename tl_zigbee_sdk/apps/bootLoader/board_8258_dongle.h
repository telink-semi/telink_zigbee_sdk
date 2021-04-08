/********************************************************************************************************
 * @file	board_8258_dongle.h
 *
 * @brief	This is the header file for board_8258_dongle
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define DONGLE_8258_32				0
#define	DONGLE_8258_48				1

#if defined DONGLE_8258_32 && (DONGLE_8258_32 == 1)
/*******************************************************************************************************
*********************8258Dongle with 32 Pins: Start*****************************************************
*******************************************************************************************************/
//KEY
#define	BUTTON1 					GPIO_PD7
#define PD7_FUNC					AS_GPIO
#define PD7_OUTPUT_ENABLE			0
#define PD7_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PD7			PM_PIN_PULLUP_10K

#define	BUTTON2 					GPIO_PA1
#define PA1_FUNC					AS_GPIO
#define PA1_OUTPUT_ENABLE			0
#define PA1_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PA1			PM_PIN_PULLUP_10K

//LED
#define LED_G						GPIO_PA0
#define PA0_FUNC					AS_GPIO
#define PA0_OUTPUT_ENABLE			1
#define PA0_INPUT_ENABLE			0

#define LED_R						GPIO_PD4
#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

// UART
#if ZBHCI_UART
	#define UART_TX_PIN         	UART_TX_PB1
	#define UART_RX_PIN         	UART_RX_PB7

	#define UART_PIN_CFG()			uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC4//print
#endif
/*******************************************************************************************************
*********************8258Dongle with 32 Pins: End*****************************************************
*******************************************************************************************************/
#elif defined DONGLE_8258_48 && (DONGLE_8258_48 == 1)
/*******************************************************************************************************
*********************8258Dongle with 48 Pins: Start*****************************************************
*******************************************************************************************************/
//KEY
#define	BUTTON1 					GPIO_PD6
#define PD6_FUNC					AS_GPIO
#define PD6_OUTPUT_ENABLE			0
#define PD6_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PD6			PM_PIN_PULLUP_10K

#define	BUTTON2 					GPIO_PD5
#define PD5_FUNC					AS_GPIO
#define PD5_OUTPUT_ENABLE			0
#define PD5_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PD5			PM_PIN_PULLUP_10K

//LED
#define LED_G						GPIO_PA2
#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define LED_R						GPIO_PA3
#define PA3_FUNC					AS_GPIO
#define PA3_OUTPUT_ENABLE			1
#define PA3_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

// UART
#if ZBHCI_UART
	#define UART_TX_PIN         	UART_TX_PD7
	#define UART_RX_PIN         	UART_RX_PA0

	#define UART_PIN_CFG()			uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC6//print
#endif
/*******************************************************************************************************
*********************8258Dongle with 48 Pins: End*****************************************************
*******************************************************************************************************/
#else
	#error "Board defined error!"
#endif

// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
	#define HW_USB_CFG()			do{ \
										usb_set_pin_en();	\
									}while(0)
#endif


enum{
	VK_SW1 = 0x01,
	VK_SW2 = 0x02
};

#define	KB_MAP_NORMAL	{\
		{VK_SW1,}, \
		{VK_SW2,}, }

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1,  BUTTON2}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
