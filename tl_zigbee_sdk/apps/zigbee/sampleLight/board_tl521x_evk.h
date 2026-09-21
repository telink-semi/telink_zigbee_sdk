/********************************************************************************************************
 * @file    board_tl521x_evk.h
 *
 * @brief   This is the header file for board_tl521x_evk
 *
 * @author  Zigbee Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

//BUTTON
//key1
#define BUTTON1                 GPIO_PC2
#define PC2_FUNC                AS_GPIO
#define PC2_OUTPUT_ENABLE       0
#define PC2_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PC2     GPIO_PIN_PULLDOWN_100K

//key2
#define BUTTON2                 GPIO_PC3
#define PC3_FUNC                AS_GPIO
#define PC3_OUTPUT_ENABLE       0
#define PC3_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PC3     GPIO_PIN_PULLDOWN_100K

//key3
#define BUTTON3                 GPIO_PB1
#define PB1_FUNC                AS_GPIO
#define PB1_OUTPUT_ENABLE       0
#define PB1_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PB1     GPIO_PIN_PULLUP_20K

//key4
#define BUTTON4                 GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       0
#define PB4_INPUT_ENABLE        1
#define PULL_WAKEUP_SRC_PB4     GPIO_PIN_PULLUP_20K

//LED
/***************************************************************
* LED_R	        GPIO_PA3        //D10 -- red     PWM0
* LED_G	        GPIO_PA2        //D9  -- green   PWM1
* LED_B	        GPIO_PA4        //D11 -- blue    PWM2
* LED_W	        GPIO_PA0        //D8  -- white
****************************************************************/
#if defined(COLOR_RGB_SUPPORT) && (COLOR_RGB_SUPPORT == 1)
#define LED_R                   GPIO_PA3
#define LED_G                   GPIO_PA2
#define LED_B                   GPIO_PA4

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
#define LED_W                   GPIO_PA0

#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       1
#define PA0_INPUT_ENABLE        0

#define LED_POWER               LED_W
#define LED_PERMIT              LED_W
#elif defined(COLOR_CCT_SUPPORT) && (COLOR_CCT_SUPPORT == 1)
//PWM configuration, LED_R as warm light, LED_B as cool light.
#define LED_R                   GPIO_PA3
#define LED_B                   GPIO_PA4

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
#define LED_W                   GPIO_PA0
#define LED_G                   GPIO_PA2

#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       1
#define PA0_INPUT_ENABLE        0

#define PA2_FUNC                AS_GPIO
#define PA2_OUTPUT_ENABLE       1
#define PA2_INPUT_ENABLE        0

#define LED_POWER               LED_W
#define LED_PERMIT              LED_G
#elif defined(BRIGHTNESS_SUPPORT) && (BRIGHTNESS_SUPPORT == 1)
#define LED_B                   GPIO_PA4

#define PWM_B_CHANNEL           2//PWM2
#define PWM_B_CHANNEL_SET()     do{ \
                                    pwm_set_pin(LED_B, PWM2); \
                                }while(0)

#define COOL_LIGHT_PWM_CHANNEL  PWM_B_CHANNEL
#define COOL_LIGHT_PWM_SET()    PWM_B_CHANNEL_SET()

//Others as GPIO.
#define LED_W                   GPIO_PA0
#define LED_G                   GPIO_PA2
#define LED_R                   GPIO_PA3

#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       1
#define PA0_INPUT_ENABLE        0

#define PA2_FUNC                AS_GPIO
#define PA2_OUTPUT_ENABLE       1
#define PA2_INPUT_ENABLE        0

#define PA3_FUNC                AS_GPIO
#define PA3_OUTPUT_ENABLE       1
#define PA3_INPUT_ENABLE        0

#define LED_POWER               LED_W
#define LED_PERMIT              LED_G
#else
//All LED as GPIO.
#define LED_W                   GPIO_PA0
#define LED_G                   GPIO_PA2
#define LED_R                   GPIO_PA3
#define LED_B                   GPIO_PA4

#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       1
#define PA0_INPUT_ENABLE        0

#define PA2_FUNC                AS_GPIO
#define PA2_OUTPUT_ENABLE       1
#define PA2_INPUT_ENABLE        0

#define PA3_FUNC                AS_GPIO
#define PA3_OUTPUT_ENABLE       1
#define PA3_INPUT_ENABLE        0

#define PA4_FUNC                AS_GPIO
#define PA4_OUTPUT_ENABLE       1
#define PA4_INPUT_ENABLE        0

#define COOL_LIGHT_GPIO	        LED_B

#define LED_POWER               LED_W
#define LED_PERMIT              LED_G
#endif

//ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  SD_ADC_GPIO_PB6P
#endif

//UART
#if MODULE_UART_ENABLE
#error "please configurate uart PIN!"
#endif

//DEBUG
#if GSUART_PRINTF_MODE
#define CONSOLE_GPIO_TX_PIN     GPIO_PC7//print
#elif UART_PRINTF_MODE
#define CONSOLE_UART_IDX        UART1
#define CONSOLE_UART_TX_PIN     GPIO_PB3//TODO
#define CONSOLE_UART_RX_PIN     GPIO_PB2//TODO
#endif

enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04
};

#define KB_MAP_NORMAL           { \
                                    {VK_SW1, VK_SW3}, \
                                    {VK_SW4, VK_SW2}, \
                                }

#define KB_MAP_NUM              KB_MAP_NORMAL
#define KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {BUTTON1, BUTTON2}
#define KB_SCAN_PINS            {BUTTON3, BUTTON4}

#define KB_LINE_MODE            0
#define KB_LINE_HIGH_VALID      0

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
