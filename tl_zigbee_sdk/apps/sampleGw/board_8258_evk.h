/********************************************************************************************************
 * @file     board_8258_evk.h
 *
 * @brief    board configuration for 8258 evk
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

// BUTTON
#define BUTTON1               		GPIO_PD1
#define PD1_FUNC			  		AS_GPIO
#define PD1_OUTPUT_ENABLE	  		0
#define PD1_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PD1	  		PM_PIN_PULLUP_10K

#define BUTTON2               		GPIO_PD2
#define PD2_FUNC			  		AS_GPIO
#define PD2_OUTPUT_ENABLE	  		0
#define PD2_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PD2	  		PM_PIN_PULLUP_10K

// LED
#define LED_G     					GPIO_PD4
#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define LED_R     					GPIO_PD0
#define PD0_FUNC					AS_GPIO
#define PD0_OUTPUT_ENABLE			1
#define PD0_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G


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
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC7//print
	#define PC7_OUTPUT_ENABLE		1
	#define PC7_INPUT_ENABLE		0
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
