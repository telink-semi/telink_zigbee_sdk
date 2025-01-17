/********************************************************************************************************
 * @file    timer.h
 *
 * @brief   This is the header file for TL321X
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
/** @page TIMER
 *
 *  Introduction
 *  ===============
 *  Supports two timers: Timer0~ Timer1. The two timers all support four modes:
 *      - Mode 0 (System Clock Mode),
 *      - Mode 1 (GPIO Trigger Mode),
 *      - Mode 2 (GPIO Pulse Width Mode),
 *      - Mode 3 (Tick Mode),
 *
 *  Timer 1 can also be configured as "watchdog" to monitor firmware running.
 *
 *  API Reference
 *  ===============
 *  Header File: timer.h
 */
#ifndef TIMER_H_
#define TIMER_H_

#include "lib/include/analog.h"
#include "gpio.h"
#include "reg_include/register.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 * @brief   Type of Timer
 */
typedef enum
{
    TIMER0 = 0,
    TIMER1 = 1,
} timer_type_e;

/**
 * @brief   Mode of Timer
 */
typedef enum
{
    TIMER_MODE_SYSCLK       = 0,
    TIMER_MODE_GPIO_TRIGGER = 1,
    TIMER_MODE_GPIO_WIDTH   = 2,
    TIMER_MODE_TICK         = 3,
} timer_mode_e;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief    This function refer to get timer irq status.
 * @param[in] status - variable of enum to select the timer interrupt source.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char timer_get_irq_status(timer_irq_e status)
{
    return reg_tmr_sta1 & status;
}

/*
 * @brief     This function refer to clear timer0 irq status.
 * @param[in] status - variable of enumeration to select the timer interrupt source.
 * @return    none
 */
static inline void timer_clr_irq_status(timer_irq_e status)
{
    reg_tmr_sta1 = status;
}

/**
 * @brief   This function refer to get timer0 tick.
 * @return  none
 */
static inline unsigned int timer0_get_gpio_width(void)
{
    return reg_tmr0_tick;
}

/**
 * @brief   This function refer to get timer1 tick.
 * @return  none
 */
static inline unsigned int timer1_get_gpio_width(void)
{
    return reg_tmr1_tick;
}

/**
 * @brief   This function refer to set timer0 tick .
 * @param[in] tick - the tick of timer0
 * @return  none
 */
static inline void timer0_set_tick(unsigned int tick)
{
    reg_tmr0_tick = tick;
}

/**
 * @brief   This function refer to get timer0 tick.
 * @return  none
 */
static inline unsigned int timer0_get_tick(void)
{
    return reg_tmr0_tick;
}

/**
 * @brief   This function refer to set timer1 tick.
 * @param[in] tick - the tick of timer1
 * @return  none
 */
static inline void timer1_set_tick(unsigned int tick)
{
    reg_tmr1_tick = tick;
}

/**
 * @brief   This function refer to get timer1 tick.
 * @return  none
 */
static inline unsigned int timer1_get_tick(void)
{
    return reg_tmr1_tick;
}

/**
 * @brief     This function set to initial tick for timer0/timer1.
 * @param[in] type - timer0/timer1.
 * @param[in] init_tick - initial tick value.
 * @return    none
 */
static inline void timer_set_init_tick(timer_type_e type, unsigned int init_tick)
{
    reg_tmr_tick(type) = init_tick;
}

/**
 * @brief     This function set to capture tick for timer0/timer1.
 * @param[in] type - timer0/timer1.
 * @param[in] cap_tick - set capture tick value.
 * @return    none
 */
static inline void timer_set_cap_tick(timer_type_e type, unsigned int cap_tick)
{
    reg_tmr_capt(type) = cap_tick;
}

/**
 * @brief     This function set timer irq mask.
 * @return    none.
 */
static inline void timer_set_irq_mask(timer_irq_e mask)
{
    reg_tmr_ctrl3 |= mask;
}

/**
 * @brief     This function clear timer irq mask.
 * @return    none.
 */
static inline void timer_clr_irq_mask(timer_irq_e mask)
{
    reg_tmr_ctrl3 &= ~mask;
}

/**
 * @brief     the specified timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
void timer_start(timer_type_e type);

/**
 * @brief     set mode of timer.
 * @param[in] type - select the timer to start.
 * @param[in] mode - select mode for timer.
 * @return    none
 */
void timer_set_mode(timer_type_e type, timer_mode_e mode);

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer.
 * @param[in] type - select the timer to start.
 * @param[in] pin - select pin for timer.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer_gpio_init(timer_type_e type, gpio_pin_e pin, gpio_pol_e pol);

/**
 * @brief     the specified timer stop working.
 * @param[in] type - select the timer to stop.
 * @return    none
 */
void timer_stop(timer_type_e type);

/**
 * @brief     This function set timer wrap mode.
 * @param[in] type - TIMER0 or TIMER1.
 * @return    none.
 */
static inline void timer_set_wrap(timer_type_e type)
{
    switch (type) {
    case TIMER0:
        BM_CLR(reg_tmr_ctrl0, FLD_TMR0_NOWRAP);
        break;
    case TIMER1:
        BM_CLR(reg_tmr_ctrl0, FLD_TMR1_NOWRAP);
        break;
    default:
        break;
    }
}


#endif /* TIMER_H_ */
