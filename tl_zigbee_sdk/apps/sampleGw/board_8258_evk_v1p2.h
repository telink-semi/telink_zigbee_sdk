/********************************************************************************************************
 * @file     board_8258_evk_v1p2.h
 *
 * @brief    board configuration for 8258 evk v1.2
 *
 * @author
 * @date	 Dec. 1, 2018
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

// BUTTON
#define BUTTON1               		GPIO_PB2
#define PB2_FUNC			  		AS_GPIO
#define PB2_OUTPUT_ENABLE	  		0
#define PB2_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB2	  		PM_PIN_PULLDOWN_100K

#define BUTTON2               		GPIO_PB3
#define PB3_FUNC			  		AS_GPIO
#define PB3_OUTPUT_ENABLE	  		0
#define PB3_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB3	  		PM_PIN_PULLDOWN_100K

#define BUTTON3               		GPIO_PB4
#define PB4_FUNC			  		AS_GPIO
#define PB4_OUTPUT_ENABLE	  		0
#define PB4_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB4	  		PM_PIN_PULLUP_10K

#define BUTTON4               		GPIO_PB5
#define PB5_FUNC			  		AS_GPIO
#define PB5_OUTPUT_ENABLE	  		0
#define PB5_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB5	  		PM_PIN_PULLUP_10K

// LED
#define LED_R     					GPIO_PD5
#define PD5_FUNC					AS_GPIO
#define PD5_OUTPUT_ENABLE			1
#define PD5_INPUT_ENABLE			0

#define LED_G     					GPIO_PD3
#define PD3_FUNC					AS_GPIO
#define PD3_OUTPUT_ENABLE			1
#define PD3_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G


#define	PM_WAKEUP_LEVEL		  		PLATFORM_WAKEUP_LEVEL_HIGH


#if ZBHCI_UART
#define UART_TX_PIN         		GPIO_PB1
#define PB1_FUNC                	AS_UART
#define PB1_INPUT_ENABLE        	0
#define PB1_OUTPUT_ENABLE       	1
#define PB1_DATA_STRENGTH       	0

#define UART_RX_PIN         		GPIO_PB0
#define PB0_FUNC                	AS_UART
#define PB0_INPUT_ENABLE        	1
#define PB0_OUTPUT_ENABLE       	0
#define PB0_DATA_STRENGTH       	0
#define PULL_WAKEUP_SRC_PB0     	PM_PIN_PULLUP_10K

#define UART_PIN_CFG				uart_gpio_set(UART_TX_PB1, UART_RX_PB0);// uart tx/rx pin set
#endif


//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PD0//print
	#define PD0_OUTPUT_ENABLE		1
	#define PD0_INPUT_ENABLE		0
#endif


#define PULL_WAKEUP_SRC_PA7           PM_PIN_PULLUP_1M  //SWS, should be pulled up, otherwise single wire would be triggered
#define PULL_WAKEUP_SRC_PA5           PM_PIN_PULLUP_1M  //DM
#define PULL_WAKEUP_SRC_PA6           PM_PIN_PULLUP_1M  //DP


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

#define KB_DRIVE_PINS  {GPIO_PB2,  GPIO_PB3}
#define KB_SCAN_PINS   {GPIO_PB4,  GPIO_PB5}


#define	KB_LINE_MODE		0
#define	KB_LINE_HIGH_VALID	0


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
