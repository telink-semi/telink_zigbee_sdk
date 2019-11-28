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

#define DONGLE_8258_32		0
#define	DONGLE_8258_48		1

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
/***************************************************************
* PWM_R						GPIO_PD4	//D1 -- red			PWM2_N
* PWM_G						GPIO_PA0	//D2 -- green		PWM0_N
* PWM_B						GPIO_PD3	//D3 -- blue		PWM1_N
* PWM_W						GPIO_PD2	//D4 -- yellow		PWM3
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)

#define PWM_R						GPIO_PD4	//D1 -- red			PWM2_N
#define PWM_G						GPIO_PA0	//D2 -- green		PWM0_N
#define PWM_B						GPIO_PD3	//D3 -- blue		PWM1_N

#define PWM_R_CHANNEL				2//PWM2_N
#define PWM_R_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_R, AS_PWM2_N); 	\
										drv_pwm_n_invert(PWM_R_CHANNEL); 	\
									}while(0)

#define PWM_G_CHANNEL				0//PWM0_N
#define PWM_G_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_G, AS_PWM0_N); 	\
										drv_pwm_n_invert(PWM_G_CHANNEL); 	\
									}while(0)

#define PWM_B_CHANNEL				1//PWM1_N
#define PWM_B_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_B, AS_PWM1_N); 	\
										drv_pwm_n_invert(PWM_B_CHANNEL); 	\
									}while(0)

#define R_LIGHT_PWM_CHANNEL			PWM_R_CHANNEL
#define G_LIGHT_PWM_CHANNEL			PWM_G_CHANNEL
#define B_LIGHT_PWM_CHANNEL			PWM_B_CHANNEL
#define R_LIGHT_PWM_SET()			PWM_R_CHANNEL_SET()
#define G_LIGHT_PWM_SET()			PWM_G_CHANNEL_SET()
#define B_LIGHT_PWM_SET()			PWM_B_CHANNEL_SET()

#define LED_W						GPIO_PD2

#define PD2_FUNC					AS_GPIO
#define PD2_OUTPUT_ENABLE			1
#define PD2_INPUT_ENABLE			0

#define LED_POWER					NULL
#define LED_PERMIT					LED_W

#else

//PWM configuration, LED_B as warm light, LED_W as cool light.
#define PWM_B						GPIO_PD3	//D3 -- blue		PWM1_N
#define PWM_W						GPIO_PD2	//D4 -- yellow		PWM3

#define PWM_B_CHANNEL				1//PWM1_N
#define PWM_B_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_B, AS_PWM1_N); 	\
										drv_pwm_n_invert(PWM_B_CHANNEL); 	\
									}while(0)

#define PWM_W_CHANNEL				3//PWM3
#define PWM_W_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_W, AS_PWM3); 		\
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_W_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_W_CHANNEL_SET()

//LED_R and LED_G as GPIO.
#define LED_R						GPIO_PD4
#define LED_G						GPIO_PA0

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define PA0_FUNC					AS_GPIO
#define PA0_OUTPUT_ENABLE			1
#define PA0_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

#endif

#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
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
/***************************************************************
* PWM_R						GPIO_PA3	//D2 -- red			PWM1
* PWM_G						GPIO_PA2	//D1 -- green		PWM0
* PWM_B						GPIO_PB0	//D4 -- blue		PWM3
* PWM_W						GPIO_PB1	//D5 -- white		PWM4
* PWM_Y						GPIO_PA4	//D3 -- yellow		PWM2
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)

#define PWM_R						GPIO_PA3	//D2 -- red			PWM1
#define PWM_G						GPIO_PA2	//D1 -- green		PWM0
#define PWM_B						GPIO_PB0	//D4 -- blue		PWM3

#define PWM_R_CHANNEL				1//PWM1
#define PWM_R_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_R, AS_PWM1); 		\
									}while(0)

#define PWM_G_CHANNEL				0//PWM0
#define PWM_G_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_G, AS_PWM0); 		\
									}while(0)

#define PWM_B_CHANNEL				3//PWM3
#define PWM_B_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_B, AS_PWM3); 		\
									}while(0)

#define R_LIGHT_PWM_CHANNEL			PWM_R_CHANNEL
#define G_LIGHT_PWM_CHANNEL			PWM_G_CHANNEL
#define B_LIGHT_PWM_CHANNEL			PWM_B_CHANNEL
#define R_LIGHT_PWM_SET()			PWM_R_CHANNEL_SET()
#define G_LIGHT_PWM_SET()			PWM_G_CHANNEL_SET()
#define B_LIGHT_PWM_SET()			PWM_B_CHANNEL_SET()

//LED_Y and LED_W as GPIO.
#define LED_Y						GPIO_PA4
#define LED_W						GPIO_PB1

#define PA4_FUNC					AS_GPIO
#define PA4_OUTPUT_ENABLE			1
#define PA4_INPUT_ENABLE			0

#define PB1_FUNC					AS_GPIO
#define PB1_OUTPUT_ENABLE			1
#define PB1_INPUT_ENABLE			0

#define LED_POWER					LED_W
#define LED_PERMIT					LED_Y

#else

//PWM configuration, LED_Y as warm light, LED_W as cool light.
#define PWM_Y						GPIO_PA4	//D3 -- yellow		PWM2
#define PWM_W						GPIO_PB1	//D5 -- white		PWM4

#define PWM_Y_CHANNEL				2//PWM2
#define PWM_Y_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_Y, AS_PWM2); 		\
									}while(0)

#define PWM_W_CHANNEL				4//PWM4
#define PWM_W_CHANNEL_SET()			do{										\
										gpio_set_func(PWM_W, AS_PWM4); 		\
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_Y_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_W_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_Y_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_W_CHANNEL_SET()

//LED_R and LED_G as GPIO.
#define LED_R						GPIO_PA3
#define LED_G						GPIO_PA2

#define PA3_FUNC					AS_GPIO
#define PA3_OUTPUT_ENABLE			1
#define PA3_INPUT_ENABLE			0

#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

#endif

#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
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
