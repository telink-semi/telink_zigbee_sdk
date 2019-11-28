/********************************************************************************************************
 * @file     board_826x_dongle_pa.h
 *
 * @brief    board configuration for 826x dongle with external PA
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

//KEY
#define	BUTTON1 					GPIO_PA0
#define PA0_FUNC					AS_GPIO
#define PA0_OUTPUT_ENABLE			0
#define PA0_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PA0			PM_PIN_PULLUP_10K

#define	BUTTON2 					GPIO_PA1
#define PA1_FUNC					AS_GPIO
#define PA1_OUTPUT_ENABLE			0
#define PA1_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PA1			PM_PIN_PULLUP_10K

//LED
#define LED_G						GPIO_PC4
#define PC4_FUNC					AS_GPIO
#define PC4_OUTPUT_ENABLE			1
#define PC4_INPUT_ENABLE			0

#define LED_R						GPIO_PB1
#define PB1_FUNC					AS_GPIO
#define PB1_OUTPUT_ENABLE			1
#define PB1_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

/****************NOT USED*************************/
#define LED2						GPIO_PE0
#define PE0_FUNC					AS_GPIO
#define PE0_OUTPUT_ENABLE			1
#define PE0_INPUT_ENABLE			0

#define LED4						GPIO_PE1
#define PE1_FUNC					AS_GPIO
#define PE1_OUTPUT_ENABLE			1
#define PE1_INPUT_ENABLE			0
/***********************************************/

//PA
#if PA_ENABLE
	#define PA_TX					GPIO_PD2
	#define PA_RX					GPIO_PC5
#endif

//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC2//print
	#define PC2_OUTPUT_ENABLE		1
	#define PC2_INPUT_ENABLE		0
#endif


#define PULL_WAKEUP_SRC_PB0           PM_PIN_PULLUP_1M  //SWS, should be pulled up, otherwise single wire would be triggered
#define PULL_WAKEUP_SRC_PE2           PM_PIN_PULLUP_1M  //DM
#define PULL_WAKEUP_SRC_PE3           PM_PIN_PULLUP_1M  //DP

#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
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
