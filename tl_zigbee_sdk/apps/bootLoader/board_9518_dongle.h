/********************************************************************************************************
 * @file	board_9518_dongle.h
 *
 * @brief	This is the header file for board_9518_dongle
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


// BUTTON
#define BUTTON1               		GPIO_PB2//SW2
#define PB2_FUNC			  		AS_GPIO
#define PB2_OUTPUT_ENABLE	  		0
#define PB2_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB2	  		GPIO_PIN_PULLUP_10K

#define BUTTON2               		GPIO_PB3//SW7
#define PB3_FUNC			  		AS_GPIO
#define PB3_OUTPUT_ENABLE	  		0
#define PB3_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB3	  		GPIO_PIN_PULLUP_10K

// LED
#define LED_Y     					GPIO_PB5//D7
#define PB5_FUNC					AS_GPIO
#define PB5_OUTPUT_ENABLE			1
#define PB5_INPUT_ENABLE			0

#define LED_R     					GPIO_PB4//D9
#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define LED_G     					GPIO_PB0//D6
#define PB0_FUNC					AS_GPIO
#define PB0_OUTPUT_ENABLE			1
#define PB0_INPUT_ENABLE			0

#define LED_B     					GPIO_PB7//D8
#define PB7_FUNC					AS_GPIO
#define PB7_OUTPUT_ENABLE			1
#define PB7_INPUT_ENABLE			0

#define LED_B2     					GPIO_PB1//D10
#define PB1_FUNC					AS_GPIO
#define PB1_OUTPUT_ENABLE			1
#define PB1_INPUT_ENABLE			0

#define LED_B3     					GPIO_PE3//D11
#define PE3_FUNC					AS_GPIO
#define PE3_OUTPUT_ENABLE			1
#define PE3_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

// UART
#if ZBHCI_UART
	#define UART_TX_PIN         	UART0_TX_PA3
	#define UART_RX_PIN         	UART0_RX_PA4

	#define UART_PIN_CFG()			uart_set_pin(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC1//print
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

#define KB_DRIVE_PINS  	{NULL}
#define KB_SCAN_PINS   	{BUTTON1, BUTTON2}



/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
