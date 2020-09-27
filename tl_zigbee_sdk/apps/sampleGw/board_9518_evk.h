/********************************************************************************************************
 * @file     board_8258_dongle.h
 *
 * @brief    board configuration for 8258 dongle
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define FLASH_SIZE_1M				1


// BUTTON
#define BUTTON1               		GPIO_PC2
#define PC2_FUNC			  		AS_GPIO
#define PC2_OUTPUT_ENABLE	  		0
#define PC2_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC2	  		PM_PIN_PULLDOWN_100K

#define BUTTON2               		GPIO_PC0
#define PC0_FUNC			  		AS_GPIO
#define PC0_OUTPUT_ENABLE	  		0
#define PC0_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC0	  		PM_PIN_PULLDOWN_100K

#define BUTTON3               		GPIO_PC3
#define PC3_FUNC			  		AS_GPIO
#define PC3_OUTPUT_ENABLE	  		0
#define PC3_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC3	  		PM_PIN_PULLUP_10K

#define BUTTON4               		GPIO_PC1
#define PC1_FUNC			  		AS_GPIO
#define PC1_OUTPUT_ENABLE	  		0
#define PC1_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC1	  		PM_PIN_PULLUP_10K

// LED
#define LED_R     					GPIO_PB7
#define PB7_FUNC					AS_GPIO
#define PB7_OUTPUT_ENABLE			1
#define PB7_INPUT_ENABLE			0

#define LED_W     					GPIO_PB6
#define PB6_FUNC					AS_GPIO
#define PB6_OUTPUT_ENABLE			1
#define PB6_INPUT_ENABLE			0

#define LED_G     					GPIO_PB5
#define PB5_FUNC					AS_GPIO
#define PB5_OUTPUT_ENABLE			1
#define PB5_INPUT_ENABLE			0

#define LED_B     					GPIO_PB4
#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G



// UART1
#if ZBHCI_UART
#define UART1_TX_PIN         		GPIO_PE0
#define UART1_RX_PIN         		GPIO_PE2

#define UART_PIN_CFG()				uart_set_pin(UART1_TX_PIN, UART1_RX_PIN);// uart tx/rx pin set
#endif


// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC7//print

	#define DEBUG_TX_PIN_INIT()		do{	\
										gpio_function_en(DEBUG_INFO_TX_PIN);						\
										gpio_set_output_en(DEBUG_INFO_TX_PIN, 1);					\
										gpio_set_up_down_res(DEBUG_INFO_TX_PIN, PM_PIN_PULLUP_1M);	\
									}while(0)
#endif


#define PULL_WAKEUP_SRC_PA7         PM_PIN_PULLUP_1M  //SWS, should be pulled up, otherwise single wire would be triggered
#define PULL_WAKEUP_SRC_PA5         PM_PIN_PULLUP_1M  //DM
#define PULL_WAKEUP_SRC_PA6         PM_PIN_PULLUP_1M  //DP

#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#define HW_USB_CFG()				do{ \
										usb_set_pin(GPIO_PA5, GPIO_PA6);	\
										usb_dp_pullup_en(1);				\
										usb_edp_en();						\
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

#define KB_DRIVE_PINS  	{GPIO_PC2, GPIO_PC0}
#define KB_SCAN_PINS   	{GPIO_PC3, GPIO_PC1}

#define	KB_LINE_MODE		0
#define	KB_LINE_HIGH_VALID	0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
