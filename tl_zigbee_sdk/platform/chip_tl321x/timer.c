/********************************************************************************************************
 * @file    timer.c
 *
 * @brief   This is the source file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

/**
 * @brief     the specified timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
void timer_start(timer_type_e type)
{
    switch (type) {
    case TIMER0:
        reg_tmr_ctrl0 |= FLD_TMR0_EN;
        break;
    case TIMER1:
        reg_tmr_ctrl0 |= FLD_TMR1_EN;
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
void timer_stop(timer_type_e type)
{
    switch (type) {
    case TIMER0:
        reg_tmr_ctrl0 &= (~FLD_TMR0_EN);
        break;
    case TIMER1:
        reg_tmr_ctrl0 &= (~FLD_TMR1_EN);
        break;
    default:
        break;
    }
}

/**
 * @brief     set mode of timer.
 * @param[in] type - select the timer to start.
 * @param[in] mode - select mode for timer.
 * @return    none
 */
void timer_set_mode(timer_type_e type, timer_mode_e mode)
{
    switch (type) {
    case TIMER0:
        reg_tmr_sta1 = FLD_TMR0_MODE_IRQ; //clear irq status
        reg_tmr_ctrl0 &= (~FLD_TMR0_MODE);
        reg_tmr_ctrl0 |= mode;
        break;
    case TIMER1:
        reg_tmr_sta1 = FLD_TMR1_MODE_IRQ; //clear irq status
        reg_tmr_ctrl0 &= (~FLD_TMR1_MODE);
        reg_tmr_ctrl0 |= (mode << 4);
        break;
    default:
        break;
    }
}

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer.
 * @param[in] type - select the timer to start.
 * @param[in] pin - select pin for timer.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer_gpio_init(timer_type_e type, gpio_pin_e pin, gpio_pol_e pol)
{
    gpio_function_en(pin);
    gpio_output_dis(pin); //disable output
    gpio_input_en(pin);   //enable input
    switch (type) {
    case TIMER0:
        if (pol == POL_FALLING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
            gpio_set_irq(GPIO_IRQ1, pin, INTR_LOW_LEVEL);
            gpio_set_irq_mask(GPIO_IRQ_IRQ1);
        } else if (pol == POL_RISING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
            gpio_set_irq(GPIO_IRQ1, pin, INTR_HIGH_LEVEL);
            gpio_set_irq_mask(GPIO_IRQ_IRQ1);
        }
        break;

    case TIMER1:
        if (pol == POL_FALLING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
            gpio_set_irq(GPIO_IRQ2, pin, INTR_LOW_LEVEL);
            gpio_set_irq_mask(GPIO_IRQ_IRQ2);
        } else if (pol == POL_RISING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
            gpio_set_irq(GPIO_IRQ2, pin, INTR_HIGH_LEVEL);
            gpio_set_irq_mask(GPIO_IRQ_IRQ2);
        }
        break;

    default:
        break;
    }
}
