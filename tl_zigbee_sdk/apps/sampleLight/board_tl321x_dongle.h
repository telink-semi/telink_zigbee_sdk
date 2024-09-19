/********************************************************************************************************
 * @file    board_tl321x_dongle.h
 *
 * @brief   This is the header file for board_tl321x_dongle
 *
 * @author  Zigbee Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


// BUTTON
#define BUTTON1               		GPIO_PE7	//SW1
#define PE7_FUNC			  		AS_GPIO
#define PE7_OUTPUT_ENABLE	  		0
#define PE7_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PE7	  		GPIO_PIN_PULLUP_10K

#define BUTTON2               		GPIO_PE6	//SW2
#define PE6_FUNC			  		AS_GPIO
#define PE6_OUTPUT_ENABLE	  		0
#define PE6_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PE6  		GPIO_PIN_PULLUP_10K

// LED
/***************************************************************
* LED_R				GPIO_PD0	//D9  -- red		PWM0
* LED_G				GPIO_PB0	//D6  -- green
* LED_B				GPIO_PB1	//D8  -- blue		PWM2
* LED_Y				GPIO_PB2	//D7  -- yellow
*
* LED_W				GPIO_PB3	//D5 -- white
* LED_R2			GPIO_PD5	//D10 -- red
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)
	#error "To do!"
#else
//PWM configuration, LED_R as warm light, LED_B1 as cool light.
#define LED_R						GPIO_PD0
#define LED_B						GPIO_PB1

#define PWM_R_CHANNEL				0	//PWM0
#define PWM_R_CHANNEL_SET()			do{\
										pwm_set_pin(LED_R, PWM0); \
										pwm_set_pwm0_mode(PWM_NORMAL_MODE);	\
									}while(0)

#define PWM_B_CHANNEL				2	//PWM2
#define PWM_B_CHANNEL_SET()			do{\
										pwm_set_pin(LED_B, PWM2); \
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_R_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_R_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_Y						GPIO_PB2
#define LED_G						GPIO_PB0
#define LED_W						GPIO_PB3
#define LED_R2						GPIO_PD5

#define PB2_FUNC					AS_GPIO
#define PB2_OUTPUT_ENABLE			1
#define PB2_INPUT_ENABLE			0

#define PB0_FUNC					AS_GPIO
#define PB0_OUTPUT_ENABLE			1
#define PB0_INPUT_ENABLE			0

#define PB3_FUNC					AS_GPIO
#define PB3_OUTPUT_ENABLE			1
#define PB3_INPUT_ENABLE			0

#define PD5_FUNC					AS_GPIO
#define PD5_OUTPUT_ENABLE			1
#define PD5_INPUT_ENABLE			0

#define LED_POWER					LED_Y
#define LED_PERMIT					LED_G
#endif

#define VOLTAGE_DETECT_PIN			ADC_GPIO_PB6

// UART
#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC6//print
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

#define KB_DRIVE_PINS  	{0}
#define KB_SCAN_PINS   	{BUTTON1, BUTTON2}



/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
