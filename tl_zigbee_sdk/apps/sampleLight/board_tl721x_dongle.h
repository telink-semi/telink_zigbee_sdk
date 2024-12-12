/********************************************************************************************************
 * @file    board_tl721x_dongle.h
 *
 * @brief   This is the header file for board_tl721x_dongle
 *
 * @author  Zigbee Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#define BUTTON1               		GPIO_PE7//SW1
#define PE7_FUNC			  		AS_GPIO
#define PE7_OUTPUT_ENABLE	  		0
#define PE7_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PE7	  		GPIO_PIN_PULLUP_1M//GPIO_PIN_PULLUP_10K

#define BUTTON2               		GPIO_PF5//SW2
#define PF5_FUNC			  		AS_GPIO
#define PF5_OUTPUT_ENABLE	  		0
#define PF5_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PF5	  		GPIO_PIN_PULLUP_1M//GPIO_PIN_PULLUP_10K

// LED
/***************************************************************
* LED_R				GPIO_PA1	//D2  -- red		PWM0
* LED_G				GPIO_PF6	//D1  -- green		PWM1
* LED_B				GPIO_PA3	//D4  -- blue		PWM2
* LED_Y				GPIO_PA2	//D3  -- yellow
* LED_W				GPIO_PA4	//D5  -- blue
* LED_O				GPIO_PD4	//D6  -- orange
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)
#define LED_R						GPIO_PA1
#define LED_G						GPIO_PF6
#define LED_B						GPIO_PA3

#define PWM_R_CHANNEL				0	//PWM0
#define PWM_R_CHANNEL_SET()			do{\
										pwm_set_pin(LED_R, PWM0); \
									}while(0)

#define PWM_G_CHANNEL				1	//PWM1
#define PWM_G_CHANNEL_SET()			do{\
										pwm_set_pin(LED_G, PWM1); \
									}while(0)

#define PWM_B_CHANNEL				2	//PWM2
#define PWM_B_CHANNEL_SET()			do{\
										pwm_set_pin(LED_B, PWM2); \
									}while(0)

#define R_LIGHT_PWM_CHANNEL			PWM_R_CHANNEL
#define G_LIGHT_PWM_CHANNEL			PWM_G_CHANNEL
#define B_LIGHT_PWM_CHANNEL			PWM_B_CHANNEL
#define R_LIGHT_PWM_SET()			PWM_R_CHANNEL_SET()
#define G_LIGHT_PWM_SET()			PWM_G_CHANNEL_SET()
#define B_LIGHT_PWM_SET()			PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_Y						GPIO_PA2
#define LED_W						GPIO_PA4
#define LED_O						GPIO_PD4

#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define PA4_FUNC					AS_GPIO
#define PA4_OUTPUT_ENABLE			1
#define PA4_INPUT_ENABLE			0

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define LED_POWER					LED_Y
#define LED_PERMIT					LED_W
#elif defined COLOR_CCT_SUPPORT && (COLOR_CCT_SUPPORT == 1)
//PWM configuration, LED_R as warm light, LED_B as cool light.
#define LED_R						GPIO_PA1
#define LED_B						GPIO_PA3

#define PWM_R_CHANNEL				0	//PWM0
#define PWM_R_CHANNEL_SET()			do{\
										pwm_set_pin(LED_R, PWM0); \
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
#define LED_Y						GPIO_PA2
#define LED_G						GPIO_PF6
#define LED_W						GPIO_PA4
#define LED_O						GPIO_PD4

#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define PF6_FUNC					AS_GPIO
#define PF6_OUTPUT_ENABLE			1
#define PF6_INPUT_ENABLE			0

#define PA4_FUNC					AS_GPIO
#define PA4_OUTPUT_ENABLE			1
#define PA4_INPUT_ENABLE			0

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define LED_POWER					LED_Y
#define LED_PERMIT					LED_G
#elif defined ZCL_LEVEL_CTRL_SUPPORT && (ZCL_LEVEL_CTRL_SUPPORT == 1)
#define LED_B						GPIO_PA3

#define PWM_B_CHANNEL				2	//PWM2
#define PWM_B_CHANNEL_SET()			do{\
										pwm_set_pin(LED_B, PWM2); \
									}while(0)

#define COOL_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define COOL_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_R						GPIO_PA1
#define LED_Y						GPIO_PA2
#define LED_G						GPIO_PF6
#define LED_W						GPIO_PA4
#define LED_O						GPIO_PD4

#define PA1_FUNC					AS_GPIO
#define PA1_OUTPUT_ENABLE			1
#define PA1_INPUT_ENABLE			0

#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define PF6_FUNC					AS_GPIO
#define PF6_OUTPUT_ENABLE			1
#define PF6_INPUT_ENABLE			0

#define PA4_FUNC					AS_GPIO
#define PA4_OUTPUT_ENABLE			1
#define PA4_INPUT_ENABLE			0

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define LED_POWER					LED_Y
#define LED_PERMIT					LED_G
#else
//All LED as GPIO.
#define LED_R						GPIO_PA1
#define LED_Y						GPIO_PA2
#define LED_B						GPIO_PA3
#define LED_G						GPIO_PF6
#define LED_W						GPIO_PA4
#define LED_O						GPIO_PD4

#define PA1_FUNC					AS_GPIO
#define PA1_OUTPUT_ENABLE			1
#define PA1_INPUT_ENABLE			0

#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define PA3_FUNC					AS_GPIO
#define PA3_OUTPUT_ENABLE			1
#define PA3_INPUT_ENABLE			0

#define PF6_FUNC					AS_GPIO
#define PF6_OUTPUT_ENABLE			1
#define PF6_INPUT_ENABLE			0

#define PA4_FUNC					AS_GPIO
#define PA4_OUTPUT_ENABLE			1
#define PA4_INPUT_ENABLE			0

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define COOL_LIGHT_GPIO				LED_B

#define LED_POWER					LED_Y
#define LED_PERMIT					LED_G
#endif

// ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN		ADC_GPIO_PB7//todo
#endif

// UART
#if ZBHCI_UART
#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
#define	DEBUG_INFO_TX_PIN	    	GPIO_PC0//print
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
