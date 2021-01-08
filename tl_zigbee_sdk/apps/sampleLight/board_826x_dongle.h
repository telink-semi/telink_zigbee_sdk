/********************************************************************************************************
 * @file	board_826x_dongle.h
 *
 * @brief	This is the header file for board_826x_dongle
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#define	BUTTON2 					GPIO_PC5
#define PC5_FUNC					AS_GPIO
#define PC5_OUTPUT_ENABLE			0
#define PC5_INPUT_ENABLE			1
#define	PULL_WAKEUP_SRC_PC5			PM_PIN_PULLUP_10K

//LED
/***************************************************************
* LED_R		GPIO_PC2	//D3 -- red			PWM2
* LED_G		GPIO_PC3	//D1 -- green		PWM3
* LED_B		GPIO_PB6	//D2 -- blue		PWM5
* LED_W		GPIO_PB4	//D4 -- white		PWM4
****************************************************************/
#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)

#define PC2_FUNC					AS_PWM
#define PC2_OUTPUT_ENABLE			1
#define PC2_INPUT_ENABLE			0

#define PC3_FUNC					AS_PWM
#define PC3_OUTPUT_ENABLE			1
#define PC3_INPUT_ENABLE			0

#define PB6_FUNC					AS_PWM
#define PB6_OUTPUT_ENABLE			1
#define PB6_INPUT_ENABLE			0

#define PWM_R_CHANNEL_SET()			PWM2_CFG_GPIO_C2()
#define PWM_R_CHANNEL				2//PWM2

#define PWM_G_CHANNEL_SET()			PWM3_CFG_GPIO_C3()
#define PWM_G_CHANNEL				3//PWM3

#define PWM_B_CHANNEL_SET()			PWM5_CFG_GPIO_B6()
#define PWM_B_CHANNEL				5//PWM5

#define R_LIGHT_PWM_CHANNEL			PWM_R_CHANNEL
#define G_LIGHT_PWM_CHANNEL			PWM_G_CHANNEL
#define B_LIGHT_PWM_CHANNEL			PWM_B_CHANNEL
#define R_LIGHT_PWM_SET()			PWM_R_CHANNEL_SET()
#define G_LIGHT_PWM_SET()			PWM_G_CHANNEL_SET()
#define B_LIGHT_PWM_SET()			PWM_B_CHANNEL_SET()

#define LED_W						GPIO_PB4

#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define LED_POWER					NULL
#define LED_PERMIT					LED_W

#else

//PWM configuration, LED_B as warm light, LED_W as cool light.
#define PB6_FUNC					AS_PWM
#define PB6_OUTPUT_ENABLE			1
#define PB6_INPUT_ENABLE			0

#define PB4_FUNC					AS_PWM
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

#define PWM_B_CHANNEL_SET()			PWM5_CFG_GPIO_B6()
#define PWM_B_CHANNEL				5//PWM5

#define PWM_W_CHANNEL_SET()			PWM4_CFG_GPIO_B4()
#define PWM_W_CHANNEL				4//PWM4

#define WARM_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_W_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_W_CHANNEL_SET()

#define LED_R						GPIO_PC2
#define LED_G						GPIO_PC3

#define PC2_FUNC					AS_GPIO
#define PC2_OUTPUT_ENABLE			1
#define PC2_INPUT_ENABLE			0

#define PC3_FUNC					AS_GPIO
#define PC3_OUTPUT_ENABLE			1
#define PC3_INPUT_ENABLE			0

#define LED_POWER					LED_R
#define LED_PERMIT					LED_G

#endif

//UART
#if	ZBHCI_UART
	PC2 & PC3 are configured as LED pins
#endif

//DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PB5//print
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
