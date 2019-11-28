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

#define PULL_WAKEUP_SRC_PA7           PM_PIN_PULLUP_1M  //SWS, should be pulled up, otherwise single wire would be triggered
#define PULL_WAKEUP_SRC_PA5           PM_PIN_PULLUP_1M  //DM
#define PULL_WAKEUP_SRC_PA6           PM_PIN_PULLUP_1M  //DP



#define GPIO_RF_TX			  	GPIO_PB1
#define GPIO_RF_RX            	GPIO_PB0

// BUTTON
#define BUTTON1               GPIO_PD1
#define PD1_FUNC			  AS_GPIO
#define PD1_OUTPUT_ENABLE	  0
#define PD1_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PD1	  PM_PIN_PULLUP_10K

#define BUTTON2               GPIO_PD2
#define PD2_FUNC			  AS_GPIO
#define PD2_OUTPUT_ENABLE	  0
#define PD2_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PD2	  PM_PIN_PULLUP_10K

#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

//LED
/***************************************************************
* PWM_R						GPIO_PD0	//D5 -- red
* PWM_G						GPIO_PD4	//D3 -- green
* PWM_B						GPIO_PD5	//D2 -- blue
* PWM_W						GPIO_PD3	//D4 -- yellow
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)

#error "GPIO_PD0 can't be configured as PWM!"

#else

//PWM configuration, LED_B as warm light, LED_W as cool light.
#define PWM_B						GPIO_PD5	//D2 -- blue		PWM0
#define PWM_W						GPIO_PD3	//D4 -- yellow		PWM1_N

#define PWM_W_CHANNEL				1	//PWM1_N
#define PWM_W_CHANNEL_SET()			do{\
										gpio_set_func(GPIO_PD3, AS_PWM1_N); \
										drv_pwm_n_invert(PWM_W_CHANNEL); \
									}while(0)

#define PWM_B_CHANNEL				0	//PWM0
#define PWM_B_CHANNEL_SET()			do{\
										gpio_set_func(GPIO_PD5, AS_PWM0); \
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_W_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_W_CHANNEL_SET()

//LED_R and LED_G as GPIO.
#define LED_R						GPIO_PD0
#define LED_G						GPIO_PD4

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define PD0_FUNC					AS_GPIO
#define PD0_OUTPUT_ENABLE			1
#define PD0_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

#endif


//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC7//print
	#define PC7_OUTPUT_ENABLE		1
	#define PC7_INPUT_ENABLE		0
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
