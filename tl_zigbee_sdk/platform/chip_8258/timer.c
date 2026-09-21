/********************************************************************************************************
 * @file    timer.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "timer.h"
#include "compiler.h"

/**
 * @brief     This function performs to set sleep us.
 * @param[in] us - microseconds need to delay.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void sleep_us(unsigned long us)
{
    unsigned long t = clock_time();
    while(!clock_time_exceed(t, us)){
    }
}

/**
 * @brief     the specified timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
void timer_start(TIMER_TypeDef type)
{
    switch (type) {
        case TIMER0:
            reg_tmr_ctrl |= FLD_TMR0_EN;
            break;
        case TIMER1:
            reg_tmr_ctrl |= FLD_TMR1_EN;
            break;
        case TIMER2:
            reg_tmr_ctrl |= FLD_TMR2_EN;
            break;
        default:
            break;
    }
}

/**
 * @brief     the specified timer stop working.
 * @param[in] type - select the timer to stop.
 * @return    none
 */
void timer_stop(TIMER_TypeDef type)
{
    switch (type) {
        case TIMER0:
            reg_tmr_ctrl &= (~FLD_TMR0_EN);
            break;
        case TIMER1:
            reg_tmr_ctrl &= (~FLD_TMR1_EN);
            break;
        case TIMER2:
            reg_tmr_ctrl &= (~FLD_TMR2_EN);
            break;
        default:
            break;
    }
}

/**
 * @brief     set timer mode.
 * @param[in] type - the specified timer.
 * @param[in] mode - select mode for timer.
 * @return    none
 */
void timer_set_mode(TIMER_TypeDef type, TIMER_ModeTypeDef mode)
{
    switch (type) {
        case TIMER0:
            reg_tmr_sta = FLD_TMR_STA_TMR0;//clear irq status
            reg_tmr_ctrl &= (~FLD_TMR0_MODE);
            reg_tmr_ctrl |= (mode << 1);
            break;
        case TIMER1:
            reg_tmr_sta = FLD_TMR_STA_TMR1;//clear irq status
            reg_tmr_ctrl &= (~FLD_TMR1_MODE);
            reg_tmr_ctrl |= (mode << 4);
            break;
        case TIMER2:
            reg_tmr_sta = FLD_TMR_STA_TMR2;//clear irq status
            reg_tmr_ctrl &= (~FLD_TMR2_MODE);
            reg_tmr_ctrl |= (mode << 7);
            break;
        default:
            break;
    }
}

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer.
 * @param[in] type - the specified timer.
 * @param[in] pin - select pin for timer.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer_gpio_init(TIMER_TypeDef type, GPIO_PinTypeDef pin, GPIO_PolTypeDef pol)
{
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 0); //disable output
    gpio_set_input_en(pin, 1);//enable input

    switch (type) {
        case TIMER0:
            if (pol == POL_FALLING) {
                gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
            } else if (pol == POL_RISING) {
                gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
            }
            gpio_set_interrupt_risc0(pin, pol);
            break;
        case TIMER1:
            if (pol == POL_FALLING) {
                gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
            } else if (pol == POL_RISING) {
                gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
            }
            gpio_set_interrupt_risc1(pin, pol);
            break;
        case TIMER2:
            {
                unsigned char bit = pin & 0xff;
                if (pol == POL_FALLING) {
                    gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);
                    BM_SET(reg_gpio_pol(pin), bit);
                } else if (pol == POL_RISING) {
                    gpio_setup_up_down_resistor(pin, PM_PIN_PULLDOWN_100K);
                    BM_CLR(reg_gpio_pol(pin), bit);
                }
                BM_SET(reg_gpio_irq_risc2_en(pin), bit);
            }
            break;
        default:
            break;
    }
}
