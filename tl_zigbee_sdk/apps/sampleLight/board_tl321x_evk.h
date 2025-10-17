/********************************************************************************************************
 * @file    board_tl321x_evk.h
 *
 * @brief   This is the header file for board_tl321x_evk
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

/***************************************************************/
/* RGB or CCT or Brightness */
#define COLOR_RGB_SUPPORT       0
#define COLOR_CCT_SUPPORT       1
#define BRIGHTNESS_SUPPORT      0
/***************************************************************/

// BUTTON
//key1
#define BUTTON1                 GPIO_PB3
#define PB3_FUNC                AS_GPIO
#define PB3_OUTPUT_ENABLE       0
#define PB3_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PB3     GPIO_PIN_PULLDOWN_100K

//key2
#define BUTTON2                 GPIO_PB5
#define PB5_FUNC                AS_GPIO
#define PB5_OUTPUT_ENABLE       0
#define PB5_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PB5     GPIO_PIN_PULLDOWN_100K

//key3
#define BUTTON3                 GPIO_PB6
#define PB6_FUNC                AS_GPIO
#define PB6_OUTPUT_ENABLE       0
#define PB6_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PB6     GPIO_PIN_PULLUP_10K

//key4
#define BUTTON4                 GPIO_PB7
#define PB7_FUNC                AS_GPIO
#define PB7_OUTPUT_ENABLE       0
#define PB7_INPUT_ENABLE        1
#define	PULL_WAKEUP_SRC_PB7     GPIO_PIN_PULLUP_10K

// LED
/***************************************************************
* LED_R	        GPIO_PB1        //D1 -- red     PWM0
* LED_G	        GPIO_PB0        //D3 -- green   PWM1
* LED_B	        GPIO_PB2        //D4 -- blue    PWM2
* LED_W	        GPIO_PD0        //D2 -- white
****************************************************************/
// LED
#if defined(COLOR_RGB_SUPPORT) && (COLOR_RGB_SUPPORT == 1)
#define LED_R                   GPIO_PB1
#define LED_G                   GPIO_PB0
#define LED_B                   GPIO_PB2

#define PWM_R_CHANNEL           0//PWM0
#define PWM_R_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_R, PWM0); \
                                    pwm_set_pwm0_mode(PWM_NORMAL_MODE); \
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
#define LED_W                   GPIO_PD0

#define PD0_FUNC                AS_GPIO
#define PD0_OUTPUT_ENABLE       1
#define PD0_INPUT_ENABLE        0

#define LED_POWER               LED_W
#define LED_PERMIT              LED_W
#elif defined(COLOR_CCT_SUPPORT) && (COLOR_CCT_SUPPORT == 1)
//PWM configuration, LED_R as warm light, LED_B as cool light.
#define LED_R                   GPIO_PB1
#define LED_B                   GPIO_PB2

#define PWM_R_CHANNEL           0//PWM0
#define PWM_R_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_R, PWM0); \
                                    pwm_set_pwm0_mode(PWM_NORMAL_MODE); \
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
#define LED_W                   GPIO_PD0
#define LED_G                   GPIO_PB0

#define PD0_FUNC                AS_GPIO
#define PD0_OUTPUT_ENABLE       1
#define PD0_INPUT_ENABLE        0

#define PB0_FUNC                AS_GPIO
#define PB0_OUTPUT_ENABLE       1
#define PB0_INPUT_ENABLE        0

#define LED_POWER               LED_W
#define LED_PERMIT              LED_G
#elif defined(BRIGHTNESS_SUPPORT) && (BRIGHTNESS_SUPPORT == 1)
#define LED_B                   GPIO_PB2

#define PWM_B_CHANNEL           2//PWM2
#define PWM_B_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_B, PWM2); \
                                }while(0)

#define COOL_LIGHT_PWM_CHANNEL  PWM_B_CHANNEL
#define COOL_LIGHT_PWM_SET()    PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_W                   GPIO_PD0
#define LED_G                   GPIO_PB0
#define LED_R                   GPIO_PB1

#define PD0_FUNC                AS_GPIO
#define PD0_OUTPUT_ENABLE       1
#define PD0_INPUT_ENABLE        0

#define PB0_FUNC                AS_GPIO
#define PB0_OUTPUT_ENABLE       1
#define PB0_INPUT_ENABLE        0

#define PB1_FUNC                AS_GPIO
#define PB1_OUTPUT_ENABLE       1
#define PB1_INPUT_ENABLE        0

#define LED_POWER               LED_W
#define LED_PERMIT              LED_G
#else
//All LED as GPIO.
#define LED_W                   GPIO_PD0
#define LED_G                   GPIO_PB0
#define LED_R                   GPIO_PB1
#define LED_B                   GPIO_PB2

#define PD0_FUNC                AS_GPIO
#define PD0_OUTPUT_ENABLE       1
#define PD0_INPUT_ENABLE        0

#define PB0_FUNC                AS_GPIO
#define PB0_OUTPUT_ENABLE       1
#define PB0_INPUT_ENABLE        0

#define PB1_FUNC                AS_GPIO
#define PB1_OUTPUT_ENABLE       1
#define PB1_INPUT_ENABLE        0

#define PB2_FUNC                AS_GPIO
#define PB2_OUTPUT_ENABLE       1
#define PB2_INPUT_ENABLE        0

#define COOL_LIGHT_GPIO	        LED_B

#define LED_POWER               LED_W
#define LED_PERMIT              LED_G
#endif

// ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  ADC_GPIO_PB4
#endif

// UART
#if ZBHCI_UART
#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
//1 - use HW UART mode; 0 - use IO mode
#define CONSOLE_UART_ENABLE     0
#if CONSOLE_UART_ENABLE
#define CONSOLE_TX_PIN          GPIO_PE0
#define CONSOLE_RX_PIN          GPIO_PE1
#else
#define	DEBUG_INFO_TX_PIN       GPIO_PE0//print
#endif
#endif

// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#define HW_USB_CFG()            do{ \
                                    usb_set_pin_en(); \
                                    usb_edp_en();     \
                                }while(0)
#endif


enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04
};

#define	KB_MAP_NORMAL           { \
                                    {VK_SW1, VK_SW3}, \
                                    {VK_SW2, VK_SW4}, \
                                }

#define	KB_MAP_NUM              KB_MAP_NORMAL
#define	KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {GPIO_PB3, GPIO_PB5}
#define KB_SCAN_PINS            {GPIO_PB6, GPIO_PB7}

#define	KB_LINE_MODE            0
#define	KB_LINE_HIGH_VALID      0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
