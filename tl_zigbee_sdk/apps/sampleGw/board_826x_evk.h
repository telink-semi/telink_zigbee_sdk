/********************************************************************************************************
 * @file     board_826x_evk.h
 *
 * @brief    board configuration for evk
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
#define	BUTTON1 					GPIO_PD2
#define PD2_FUNC					AS_GPIO
#define PD2_OUTPUT_ENABLE			0
#define PD2_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PD2			PM_PIN_PULLUP_10K

#define	BUTTON2 					GPIO_PC7
#define PC7_FUNC					AS_GPIO
#define PC7_OUTPUT_ENABLE			0
#define PC7_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PC7			PM_PIN_PULLUP_10K

//LED
#define LED_B						GPIO_PB4
#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define LED2						GPIO_PD6
#define PD6_FUNC					AS_GPIO
#define PD6_OUTPUT_ENABLE			1
#define PD6_INPUT_ENABLE			0

#define LED_W						GPIO_PD7
#define PD7_FUNC					AS_GPIO
#define PD7_OUTPUT_ENABLE			1
#define PD7_INPUT_ENABLE			0

#define LED4						GPIO_PD5
#define PD5_FUNC					AS_GPIO
#define PD5_OUTPUT_ENABLE			1
#define PD5_INPUT_ENABLE			0

#define LED_POWER					LED_W
#define LED_PERMIT					LED_B

//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC5//print

	#define DEBUG_TX_PIN_INIT()		do{	\
										gpio_set_func(DEBUG_INFO_TX_PIN, AS_GPIO);	\
										gpio_set_output_en(DEBUG_INFO_TX_PIN, 1);	\
										gpio_setup_up_down_resistor(DEBUG_INFO_TX_PIN, PM_PIN_PULLUP_1M); \
									}while(0)
#endif

#if	ZBHCI_UART
	#define UART_TX_PIN         		GPIO_PC2
	#define PC2_FUNC                	AS_UART
	#define PC2_INPUT_ENABLE        	0
	#define PC2_OUTPUT_ENABLE       	1
	#define PC2_DATA_STRENGTH       	0

	#define UART_RX_PIN         		GPIO_PC3
	#define PC3_FUNC                	AS_UART
	#define PC3_INPUT_ENABLE        	1
	#define PC3_OUTPUT_ENABLE       	0
	#define PC3_DATA_STRENGTH       	0
	#define PULL_WAKEUP_SRC_PC3     	GPIO_PULL_UP_10K

	#define UART_PIN_CFG()				UART_GPIO_CFG_PC2_PC3()
#endif


#define PULL_WAKEUP_SRC_PB0           	PM_PIN_PULLUP_1M  //SWS, should be pulled up, otherwise single wire would be triggered
#define PULL_WAKEUP_SRC_PE2           	PM_PIN_PULLUP_1M  //DM
#define PULL_WAKEUP_SRC_PE3           	PM_PIN_PULLUP_1M  //DP

#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#define HW_USB_CFG()					do{ \
											gpio_set_func(GPIO_PE2, AS_USB);	\
											gpio_set_func(GPIO_PE3, AS_USB);	\
											usb_dp_pullup_en(1);				\
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
