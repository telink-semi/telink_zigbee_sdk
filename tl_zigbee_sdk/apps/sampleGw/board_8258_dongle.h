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

#if (BOARD == BOARD_8258_DONGLE_1M)
	/* Running chip flash size select. If '1' - 1M, otherwise (or undefined) - 512K. */
	#define FLASH_SIZE_1M			1
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


#if ZBHCI_UART
#define UART_TX_PIN         		GPIO_PB1
#define PB1_FUNC                	AS_UART
#define PB1_INPUT_ENABLE        	0
#define PB1_OUTPUT_ENABLE       	1
#define PB1_DATA_STRENGTH       	0

#define UART_RX_PIN         		GPIO_PB7
#define PB7_FUNC                	AS_UART
#define PB7_INPUT_ENABLE        	1
#define PB7_OUTPUT_ENABLE       	0
#define PB7_DATA_STRENGTH       	0
#define PULL_WAKEUP_SRC_PB7     	PM_PIN_PULLUP_10K

#define UART_PIN_CFG				uart_gpio_set(UART_TX_PB1, UART_RX_PB7);// uart tx/rx pin set
#endif


//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC4//print
	#define PC4_OUTPUT_ENABLE		1
	#define PC4_INPUT_ENABLE		0
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


#if ZBHCI_UART
#define UART_TX_PIN         		GPIO_PD7
#define PD7_FUNC                	AS_UART
#define PD7_INPUT_ENABLE        	0
#define PD7_OUTPUT_ENABLE       	1
#define PD7_DATA_STRENGTH       	0

#define UART_RX_PIN         		GPIO_PA0
#define PA0_FUNC                	AS_UART
#define PA0_INPUT_ENABLE        	1
#define PA0_OUTPUT_ENABLE       	0
#define PA0_DATA_STRENGTH       	0
#define PULL_WAKEUP_SRC_PA0     	PM_PIN_PULLUP_10K

#define UART_PIN_CFG				uart_gpio_set(UART_TX_PD7, UART_RX_PA0);// uart tx/rx pin set
#endif

//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC6//print
	#define PC6_OUTPUT_ENABLE		1
	#define PC6_INPUT_ENABLE		0
#endif
/*******************************************************************************************************
*********************8258Dongle with 48 Pins: End*****************************************************
*******************************************************************************************************/
#else
	#error "Board defined error!"
#endif


#define PULL_WAKEUP_SRC_PA7           PM_PIN_PULLUP_1M  //SWS, should be pulled up, otherwise single wire would be triggered
#define PULL_WAKEUP_SRC_PA5           PM_PIN_PULLUP_1M  //DM
#define PULL_WAKEUP_SRC_PA6           PM_PIN_PULLUP_1M  //DP


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
