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

/***************************************************************/
/* RGB or CCT or Brightness */
#define COLOR_RGB_SUPPORT       0
#define COLOR_CCT_SUPPORT       1
#define BRIGHTNESS_SUPPORT      0
/****************************************************************/

// BUTTON
#define BUTTON1                 GPIO_PF7//SW1
#define PF7_FUNC                AS_GPIO
#define PF7_OUTPUT_ENABLE       0
#define PF7_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PF7     GPIO_PIN_PULLUP_1M//GPIO_PIN_PULLUP_10K

#define BUTTON2                 GPIO_PF2//SW2
#define PF2_FUNC                AS_GPIO
#define PF2_OUTPUT_ENABLE       0
#define PF2_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PF2     GPIO_PIN_PULLUP_1M//GPIO_PIN_PULLUP_10K

// LED
/***************************************************************
* LED_R	        GPIO_PF1        //D2  -- red    PWM0
* LED_G	        GPIO_PF0        //D1  -- green  PWM1
* LED_B	        GPIO_PF4        //D4  -- blue   PWM2
* LED_Y	        GPIO_PF3        //D3  -- yellow
* LED_W	        GPIO_PF5        //D5  -- blue
* LED_O	        GPIO_PF6        //D6  -- orange
****************************************************************/
#if defined(COLOR_RGB_SUPPORT) && (COLOR_RGB_SUPPORT == 1)
#define LED_R                   GPIO_PF1
#define LED_G                   GPIO_PF0
#define LED_B                   GPIO_PF4

#define PWM_R_CHANNEL           0//PWM0
#define PWM_R_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_R, PWM0); \
                                }while(0)

#define PWM_G_CHANNEL           1//PWM1
#define PWM_G_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_G, PWM1); \
                                }while(0)

#define PWM_B_CHANNEL           2//PWM2
#define PWM_B_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_B, PWM2); \
                                }while(0)

#define R_LIGHT_PWM_CHANNEL     PWM_R_CHANNEL
#define G_LIGHT_PWM_CHANNEL     PWM_G_CHANNEL
#define B_LIGHT_PWM_CHANNEL     PWM_B_CHANNEL
#define R_LIGHT_PWM_SET()       PWM_R_CHANNEL_SET()
#define G_LIGHT_PWM_SET()       PWM_G_CHANNEL_SET()
#define B_LIGHT_PWM_SET()       PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_Y                   GPIO_PF3
#define LED_W                   GPIO_PF5
#define LED_O                   GPIO_PF6

#define PF3_FUNC                AS_GPIO
#define PF3_OUTPUT_ENABLE       1
#define PF3_INPUT_ENABLE        0

#define PF5_FUNC                AS_GPIO
#define PF5_OUTPUT_ENABLE       1
#define PF5_INPUT_ENABLE        0

#define PF6_FUNC                AS_GPIO
#define PF6_OUTPUT_ENABLE       1
#define PF6_INPUT_ENABLE        0

#define LED_POWER               LED_Y
#define LED_PERMIT              LED_W
#elif defined(COLOR_CCT_SUPPORT) && (COLOR_CCT_SUPPORT == 1)
//PWM configuration, LED_R as warm light, LED_B as cool light.
#define LED_R                   GPIO_PF1
#define LED_B                   GPIO_PF4

#define PWM_R_CHANNEL           0//PWM0
#define PWM_R_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_R, PWM0); \
                                }while(0)

#define PWM_B_CHANNEL           2//PWM2
#define PWM_B_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_B, PWM2); \
                                }while(0)

#define WARM_LIGHT_PWM_CHANNEL  PWM_R_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL  PWM_B_CHANNEL
#define WARM_LIGHT_PWM_SET()    PWM_R_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()    PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_Y                   GPIO_PF3
#define LED_G                   GPIO_PF0
#define LED_W                   GPIO_PF5
#define LED_O                   GPIO_PF6

#define PF3_FUNC                AS_GPIO
#define PF3_OUTPUT_ENABLE       1
#define PF3_INPUT_ENABLE        0

#define PF0_FUNC                AS_GPIO
#define PF0_OUTPUT_ENABLE       1
#define PF0_INPUT_ENABLE        0

#define PF5_FUNC                AS_GPIO
#define PF5_OUTPUT_ENABLE       1
#define PF5_INPUT_ENABLE        0

#define PF6_FUNC                AS_GPIO
#define PF6_OUTPUT_ENABLE       1
#define PF6_INPUT_ENABLE        0

#define LED_POWER               LED_Y
#define LED_PERMIT              LED_G
#elif defined(BRIGHTNESS_SUPPORT) && (BRIGHTNESS_SUPPORT == 1)
#define LED_B                   GPIO_PF4

#define PWM_B_CHANNEL           2//PWM2
#define PWM_B_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_B, PWM2); \
                                }while(0)

#define COOL_LIGHT_PWM_CHANNEL  PWM_B_CHANNEL
#define COOL_LIGHT_PWM_SET()    PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_R                   GPIO_PF1
#define LED_Y                   GPIO_PF3
#define LED_G                   GPIO_PF0
#define LED_W                   GPIO_PF5
#define LED_O                   GPIO_PF6

#define PF1_FUNC                AS_GPIO
#define PF1_OUTPUT_ENABLE       1
#define PF1_INPUT_ENABLE        0

#define PF3_FUNC                AS_GPIO
#define PF3_OUTPUT_ENABLE       1
#define PF3_INPUT_ENABLE        0

#define PF0_FUNC                AS_GPIO
#define PF0_OUTPUT_ENABLE       1
#define PF0_INPUT_ENABLE        0

#define PF5_FUNC                AS_GPIO
#define PF5_OUTPUT_ENABLE       1
#define PF5_INPUT_ENABLE        0

#define PF6_FUNC                AS_GPIO
#define PF6_OUTPUT_ENABLE       1
#define PF6_INPUT_ENABLE        0

#define LED_POWER               LED_Y
#define LED_PERMIT              LED_G
#else
//All LED as GPIO.
#define LED_R                   GPIO_PF1
#define LED_Y                   GPIO_PF3
#define LED_B                   GPIO_PF4
#define LED_G                   GPIO_PF0
#define LED_W                   GPIO_PF5
#define LED_O                   GPIO_PF6

#define PF1_FUNC                AS_GPIO
#define PF1_OUTPUT_ENABLE       1
#define PF1_INPUT_ENABLE        0

#define PF3_FUNC                AS_GPIO
#define PF3_OUTPUT_ENABLE       1
#define PF3_INPUT_ENABLE        0

#define PF4_FUNC                AS_GPIO
#define PF4_OUTPUT_ENABLE       1
#define PF4_INPUT_ENABLE        0

#define PF0_FUNC                AS_GPIO
#define PF0_OUTPUT_ENABLE       1
#define PF0_INPUT_ENABLE        0

#define PF5_FUNC                AS_GPIO
#define PF5_OUTPUT_ENABLE       1
#define PF5_INPUT_ENABLE        0

#define PF6_FUNC                AS_GPIO
#define PF6_OUTPUT_ENABLE       1
#define PF6_INPUT_ENABLE        0

#define COOL_LIGHT_GPIO	        LED_B

#define LED_POWER               LED_Y
#define LED_PERMIT              LED_G
#endif

// ADC
#if VOLTAGE_DETECT_ENABLE
#warning "TL721X Dongle does not support."
#undef VOLTAGE_DETECT_ENABLE
#endif

// UART
#if ZBHCI_UART
#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
#define	DEBUG_INFO_TX_PIN       GPIO_PC4//print
#endif

enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
};

#define	KB_MAP_NORMAL           { \
                                    {VK_SW1,}, \
                                    {VK_SW2,}, \
                                }

#define	KB_MAP_NUM              KB_MAP_NORMAL
#define	KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {0}
#define KB_SCAN_PINS            {BUTTON1, BUTTON2}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
