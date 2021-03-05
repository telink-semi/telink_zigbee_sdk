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
#define LED_R						GPIO_PD2
#define LED_G						GPIO_PD4

#define PD2_FUNC					AS_GPIO
#define PD2_OUTPUT_ENABLE			1
#define PD2_INPUT_ENABLE			0

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G


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



//BLE DEBUG
#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)
	//define debug GPIO here according to your hardware


	#define PB0_FUNC			  		AS_GPIO
	#define PB0_OUTPUT_ENABLE	  		1
	#define PB0_INPUT_ENABLE	  		0

	#define PC5_FUNC			  		AS_GPIO
	#define PC5_OUTPUT_ENABLE	  		1
	#define PC5_INPUT_ENABLE	  		0

	#define PC6_FUNC			  		AS_GPIO
	#define PC6_OUTPUT_ENABLE	  		1
	#define PC6_INPUT_ENABLE	  		0

	#define PC7_FUNC			  		AS_GPIO
	#define PC7_OUTPUT_ENABLE	  		1
	#define PC7_INPUT_ENABLE	  		0

	#define PA2_FUNC			  		AS_GPIO
	#define PA2_OUTPUT_ENABLE	  		1
	#define PA2_INPUT_ENABLE	  		0

	#define PA3_FUNC			  		AS_GPIO
	#define PA3_OUTPUT_ENABLE	  		1
	#define PA3_INPUT_ENABLE	  		0

	#define PA4_FUNC			  		AS_GPIO
	#define PA4_OUTPUT_ENABLE	  		1
	#define PA4_INPUT_ENABLE	  		0

	#define GPIO_CHN0							GPIO_PB0
	#define GPIO_CHN1							GPIO_PC5
	#define GPIO_CHN2							GPIO_PC6
	#define GPIO_CHN3							GPIO_PC7  // PC2/PC3 may used for external crystal input
	#define GPIO_CHN4							GPIO_PA2  // PC2/PC3 may used for external crystal input
	#define GPIO_CHN5							GPIO_PA3
	#define GPIO_CHN6							GPIO_PA4


#if 0
	#define PB4_OUTPUT_ENABLE					1
	#define PB5_OUTPUT_ENABLE					1
	#define PB6_OUTPUT_ENABLE					1
//	#define PC2_OUTPUT_ENABLE					1
//	#define PC3_OUTPUT_ENABLE					1
	#define PB0_OUTPUT_ENABLE					1
	#define PB1_OUTPUT_ENABLE					1
#endif



	#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE









/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
