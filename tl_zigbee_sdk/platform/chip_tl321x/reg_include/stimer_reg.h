/********************************************************************************************************
 * @file    stimer_reg.h
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
#ifndef STIMER_REG_H_
#define STIMER_REG_H_

#include "soc.h"
/*******************************      sys clock registers: 0x140200       ******************************/
#define STIMER_BASE_ADDR     0x140200
#define reg_system_tick      REG_ADDR32(STIMER_BASE_ADDR)

#define reg_system_irq_level REG_ADDR32(STIMER_BASE_ADDR + 0x4)

#define reg_system_irq_mask  REG_ADDR8(STIMER_BASE_ADDR + 0x8)

typedef enum
{
    FLD_SYSTEM_IRQ_MASK          = BIT(0), /**<
                                            When the current tick value of the system timer reaches the preset tick value,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                          */
    FLD_SYSTEM_IRQ_32K_CAL_MASK  = BIT(1), /**<
                                            When the system timer completes the calibration of 32k timer,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                          */
    FLD_SYSTEM_IRQ_WAIT          = BIT(2),
    FLD_SYSTEM_TRIG_PAST_EN      = BIT(3),
    FLD_SYSTEM_CAPT_IRQ_MASK     = BIT(4), /**<
                                            When gpio's edge signal triggers the capture, systimer's current count is loaded into the capture register,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                          */
    FLD_SYSTEM_OVERFLOW_IRQ_MASK = BIT(5), /**<
                                            When the system timer captures the signal again, but the previous value is not read, resulting in a data overwrite,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                          */
} stimer_irq_mask_e;

#define reg_system_cal_irq REG_ADDR8(STIMER_BASE_ADDR + 0x9)

typedef enum
{
    FLD_SYSTEM_IRQ          = BIT(0), /**<
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                          */
    FLD_SYSTEM_32K_IRQ      = BIT(1), /**<
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                         <p>
                                            In Non-dma mode,the received data is read by stimer_get_tracking_32k_value().
                                          */
    FLD_SYSTEM_IRQ_CAPT     = BIT(2), /**<
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                         <p>
                                            In Non-dma mode,the received data is read by stimer_get_input_capt_value().
                                         <p>
                                            Note: The GPIO interrupt must be cleared at the same time -gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR).
                                          */
    FLD_SYSTEM_IRQ_OVERFLOW = BIT(3), /**<
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                         <p>
                                            In Non-dma mode,the received data is read by stimer_get_input_capt_value().
                                         <p>
                                            Note: The GPIO interrupt must be cleared at the same time -gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR),
                                            and capture interrupt -stimer_clr_irq_status(FLD_SYSTEM_IRQ_CAPT).
                                          */
} stimer_irq_status_e;

#define reg_system_ctrl REG_ADDR8(STIMER_BASE_ADDR + 0xa)

enum
{
    FLD_SYSTEM_32K_WR_EN    = BIT(0),
    FLD_SYSTEM_TIMER_EN     = BIT(1),
    FLD_SYSTEM_TIMER_AUTO   = BIT(2),
    FLD_SYSTEM_32K_TRACK_EN = BIT(3),
    FLD_SYSTEM_32K_CAL_MODE = BIT_RNG(4, 7), //(2^(16-cal_32k_mode)) cycles of 32k clock

};

#define reg_system_st REG_ADDR8(STIMER_BASE_ADDR + 0xb)

enum
{
    //The register write operation directly writes the corresponding bit.
    //Can not read out the whole register, and then modify the bit to be written, and then write in.
    FLD_SYSTEM_CMD_STOP         = BIT(1), //W:write 1, stop system timer when using auto mode
    FLD_SYSTEM_CMD_SYNC         = BIT(3), //W:write 1, start 32k count write; R:st_list3(wr_busy)
    FLD_SYSTEM_CLK_32K          = BIT(4), //R
    FLD_SYSTEM_CLR_RD_DONE      = BIT(5), //W:write 1, clear read 32k update flag; R:st_list5(rd_done)
    FLD_SYSTEM_RD_BUSY          = BIT(6), //R
    FLD_SYSTEM_CMD_SET_DLY_DONE = BIT(7), //R

};

#define reg_system_timer_set_32k  REG_ADDR32(STIMER_BASE_ADDR + 0xc)

#define reg_system_timer_read_32k REG_ADDR32(STIMER_BASE_ADDR + 0x10)

#define reg_system_cal_latch_32k  REG_ADDR32(STIMER_BASE_ADDR + 0x14)

#define reg_system_up_32k         REG_ADDR8(STIMER_BASE_ADDR + 0x18)

enum
{

    FLD_SYSTEM_UPDATE_UPON_32K  = BIT(0),
    FLD_SYSTEM_RUN_UPON_NXT_32K = BIT(1),
};

#define reg_system_timer_ctrl REG_ADDR8(STIMER_BASE_ADDR + 0x19)

typedef enum
{
    FLD_SYSTEM_CAPT_MODE     = BIT_RNG(0, 1),
    FLD_SYSTEM_CAPT_EN       = BIT(2),
    FLD_SYSTEM_PEM_EVENT_EN  = BIT(3),
    FLD_SYSTEM_PEM_TASK_0_EN = BIT(4),
    FLD_SYSTEM_PEM_TASK_1_EN = BIT(5),
    FLD_SYSTEM_PEM_TASK_2_EN = BIT(6),
} stimer_ctrl_e;

#define reg_system_timer_capt     REG_ADDR32(STIMER_BASE_ADDR + 0x1c)

#define reg_stimer_capt_data_addr STIMER_BASE_ADDR + 0x1c


#endif /* STIMER_REG_H_ */
