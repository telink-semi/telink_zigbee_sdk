/********************************************************************************************************
 * @file    timer.h
 *
 * @brief   This is the header file for TL721X
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
#include "dma.h"

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

/*
 *  @brief  Define timer capture mode
 */
typedef enum{
     TMR_CAPT_RISING_EDGE         = 0,
     TMR_CAPT_FALLING_EDGE        = 1,
     TMR_CAPT_RISING_FALLING_EDGE = 2,
} timer_capt_mode_e;


/*
 * @brief   Type of Timer mask.
 */
typedef enum{
    TMR0_MODE_MASK      =       BIT(0),
    TMR0_CAPT_MASK      =       BIT(1),
    TMR0_COMP_MASK      =       BIT(2),
    TMR1_MODE_MASK      =       BIT(3),
    TMR1_CAPT_MASK      =       BIT(4),
    TMR1_COMP_MASK      =       BIT(5),
    TMR0_OV_MASK        =       BIT(6),
    TMR1_OV_MASK        =       BIT(7),
}timer_mask_e;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/*
 * @brief     This function servers to enable timer input capture mode.
 * @return    none.
 */
static inline void timer_input_capture_en(timer_type_e type)
{
    switch(type)
        {
            case TIMER0:
                reg_tmr_ctrl2 |= FLD_TMR0_CAPT_EN;
                break;
            case TIMER1:
                reg_tmr_ctrl2 |= FLD_TMR1_CAPT_EN;
                break;
            default:
                break;
        }
}

/*
 * @brief     This function servers to disable timer input capture mode.
 * @return    none.
 */
static inline void timer_input_capture_dis(timer_type_e type)
{
    switch(type)
        {
            case TIMER0:
                reg_tmr_ctrl2 &= ~(FLD_TMR0_CAPT_EN);
                break;
            case TIMER1:
                reg_tmr_ctrl2 &= ~(FLD_TMR1_CAPT_EN);
                break;
            default:
                break;
        }
}

/**
 * @brief       This function refer to get timer irq status.
 * @param[in]   status - variable of enum to select the timer interrupt source.
 * @retval      non-zero  -  the interrupt occurred.
 * @retval      zero      -  the interrupt did not occur.
 */
static inline unsigned char timer_get_irq_status(timer_irq_e status)
{
    return reg_tmr_sta1 & status;
}

/**
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
 * @brief     This function refer to set timer0 tick .
 * @param[in] tick - the tick of timer0
 * @return    none
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
 * @brief     This function refer to set timer1 tick.
 * @param[in] tick - the tick of timer1
 * @return    none
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
/*
 * @brief     This function performs to get  timer capture value.
 * @return    timer capture value.
 */
static inline unsigned int timer_get_capture_value(timer_type_e type)
{
    return reg_tmr_ccapt(type);
}
/**
 * @brief     This function set timer irq mask.
 * @param[in] mask - variable of enumeration to select the timer interrupt mask.
 * @return    none.
 */
static inline void timer_set_irq_mask(timer_irq_e mask)
{
    reg_tmr_ctrl3 |= mask;
}

/**
 * @brief     This function clear timer irq mask.
 * @param[in] mask - variable of enumeration to select the timer interrupt mask.
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


/*
 * @brief     This function set timer input capture mode.
 * @param[in] capt_mode  - timer input capture mode.
 * @return    none.
 */
void timer_set_input_capture_mode(timer_type_e type, timer_capt_mode_e capt_mode, gpio_pin_e pin);

/*
 * @brief     This function is used to reset the tick value to 0 when it is equal to the preset capture tick value or when a capture is triggered.
 * @param[in] type - TIMER0 or TIMER1.
 * @param[in] reset - 1: reset the tick  0: do not reset the tick
 * @return    none.
 */
void timer_reset_tick(timer_type_e type, unsigned char reset);
/**
 * @brief     the specified timer stop working.
 * @param[in] type - select the timer to stop.
 * @return    none
 */
void timer_stop(timer_type_e type);

/**
 * @brief     This function serves to set timer rx_dam channel and config dma rx default.
 * @param[in] type - TIMER0 or TIMER1.
 * @param[in] chn  - dma channel.
 * @return    none
 */
void timer_set_rx_dma_config(timer_type_e type, dma_chn_e chn);

/**
 * @brief       This function serves to receive data function by DMA, this function tell the DMA to get data from the timer data fifo.
 *              1. if the receiving length information of DMA is set to 0xFFFFFC byte(max_value), and write_num is turned on,
 *                 then The length of the data received by dma will be written back to the first four bytes of addr.
 *              2. if the receiving length information of DMA is set to less than 0xFFFFFC byte, and write_num is turned on,
 *                 then the length of data received by DMA will not be written to the first four bytes of addr.
 * @param[in]   type     - TIMER0 or TIMER1.
 * @param[in]   addr     - pointer to the buffer receive data.
 * @param[in]   rev_size - the receive length of DMA,The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      none
 */
void timer_receive_dma(timer_type_e type, unsigned char *addr, unsigned int rev_size);


/**
 * @brief     This function servers to configure DMA head node,the chain function only applies to data_len = 0xFFFFFC.
 * @param[in] type - TIMER0/TIMER1.
 * @param[in] chn          - to select the DMA channel.
 * @param[in] dst_addr     - the dma address of destination
 * @param[in] data_len     - to configure DMA length.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void timer_set_dma_chain_llp(timer_type_e type, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief     This function servers to configure DMA cycle chain node.
 * @param[in] type - TIMER0/TIMER1.
 * @param[in] chn - to select the DMA channel.
 * @param[in] config_addr  - to servers to configure the address of the current node.
 * @param[in] llpoint - to configure the address of the next node configure.
 * @param[in] src_addr - to configure DMA source address.
 * @param[in] data_len - to configure DMA length.
 * @return    none
 */
void timer_set_rx_dma_add_list_element(timer_type_e type, dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpoint, unsigned short *dst_addr, unsigned int data_len);


#endif /* TIMER_H_ */
