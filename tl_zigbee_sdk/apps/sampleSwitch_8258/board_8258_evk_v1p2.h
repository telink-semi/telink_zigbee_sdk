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


#define PB4_FUNC			  		AS_GPIO
#define PB4_OUTPUT_ENABLE	  		0
#define PB4_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB4	  		PM_PIN_PULLUP_10K

#define PB5_FUNC			  		AS_GPIO
#define PB5_OUTPUT_ENABLE	  		0
#define PB5_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB5	  		PM_PIN_PULLUP_10K

// LED
#define LED1     					GPIO_PD2
#define PD3_FUNC					AS_GPIO
#define PD3_OUTPUT_ENABLE			1
#define PD3_INPUT_ENABLE			0

#define LED3     					GPIO_PD5
#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define PM_DEBUG_PIN				GPIO_PD3
#define PD3_FUNC					AS_GPIO
#define PD3_OUTPUT_ENABLE			1
#define PD3_INPUT_ENABLE			0


#define	PM_WAKEUP_LEVEL		  		PLATFORM_WAKEUP_LEVEL_HIGH


#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
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
