/********************************************************************************************************
 * @file    timer_reg.h
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
#ifndef TIMER_REG_H
#define TIMER_REG_H
#include "soc.h"

/*******************************      timer registers: 0x140140     ******************************/
#define TIMER_BASE_ADDR 0x140140
#define reg_tmr_ctrl0   REG_ADDR8(TIMER_BASE_ADDR)
#define reg_tmr_ctrl16  REG_ADDR16(TIMER_BASE_ADDR)
#define reg_tmr_ctrl32  REG_ADDR32(TIMER_BASE_ADDR)

enum
{
    FLD_TMR0_MODE   = BIT_RNG(0, 1),
    FLD_TMR0_EN     = BIT(2),
    FLD_TMR0_NOWRAP = BIT(3),
    FLD_TMR1_MODE   = BIT_RNG(4, 5),
    FLD_TMR1_EN     = BIT(6),
    FLD_TMR1_NOWRAP = BIT(7),
};

#define reg_tmr_ctrl1 REG_ADDR8(TIMER_BASE_ADDR + 0x01)

enum
{

    FLD_TMR0_CAPT_MODE = BIT_RNG(0, 1),
    FLD_TMR0_COMP_MODE = BIT_RNG(2, 3),
    FLD_TMR1_CAPT_MODE = BIT_RNG(4, 5),
    FLD_TMR1_COMP_MODE = BIT_RNG(6, 7),
};

#define reg_tmr_ctrl2 REG_ADDR8(TIMER_BASE_ADDR + 0x02)

enum
{
    FLD_TMR0_COMP_POLA = BIT(0),
    FLD_TMR1_COMP_POLA = BIT(1),
    FLD_TMR0_COMP_INV  = BIT(2),
    FLD_TMR1_COMP_INV  = BIT(3),
    FLD_TMR0_CAPT_EN   = BIT(4),
    FLD_TMR0_COMP_EN   = BIT(5),
    FLD_TMR1_CAPT_EN   = BIT(6),
    FLD_TMR1_COMP_EN   = BIT(7),
};

#define reg_tmr_ctrl3 REG_ADDR8(TIMER_BASE_ADDR + 0x03)

enum
{
    FLD_TMR0_MODE_MASK = BIT(0),
    FLD_TMR0_CAPT_MASK = BIT(1),
    FLD_TMR0_COMP_MASK = BIT(2),
    FLD_TMR1_MODE_MASK = BIT(3),
    FLD_TMR1_CAPT_MASK = BIT(4),
    FLD_TMR1_COMP_MASK = BIT(5),
    FLD_TMR0_OV_MASK   = BIT(6),
    FLD_TMR1_OV_MASK   = BIT(7),
};

#define reg_tmr0_capt    REG_ADDR32(TIMER_BASE_ADDR + 0x04)
#define reg_tmr1_capt    REG_ADDR32(TIMER_BASE_ADDR + 0x08)
#define reg_tmr_capt(i)  REG_ADDR32(TIMER_BASE_ADDR + 0x04 + ((i) << 2))

#define reg_wt_target    REG_ADDR32(TIMER_BASE_ADDR + 0x0c)
#define reg_wt_target0   REG_ADDR8(TIMER_BASE_ADDR + 0x0c) // always is 0x00
#define reg_wt_target1   REG_ADDR8(TIMER_BASE_ADDR + 0x0d)
#define reg_wt_target2   REG_ADDR8(TIMER_BASE_ADDR + 0x0e)
#define reg_wt_target3   REG_ADDR8(TIMER_BASE_ADDR + 0x0f)

#define reg_tmr0_tick    REG_ADDR32(TIMER_BASE_ADDR + 0x10)
#define reg_tmr1_tick    REG_ADDR32(TIMER_BASE_ADDR + 0x14)
#define reg_tmr_tick(i)  REG_ADDR32(TIMER_BASE_ADDR + 0x10 + ((i) << 2))

#define reg_tmr0_ccapt   REG_ADDR32(TIMER_BASE_ADDR + 0x18)
#define reg_tmr1_ccapt   REG_ADDR32(TIMER_BASE_ADDR + 0x1c)
#define reg_tmr_ccapt(i) REG_ADDR32(TIMER_BASE_ADDR + 0x18 + ((i) << 2))

#define reg_tmr_sta0     REG_ADDR8(TIMER_BASE_ADDR + 0x20)

enum
{
    FLD_TMR_STA_WD     = BIT(0),
    FLD_TMR_WD_CNT_CLR = BIT(1),
    FLD_TMR_SW_IRQ     = BIT(7),
};

#define reg_tmr_sta1 REG_ADDR8(TIMER_BASE_ADDR + 0x21)

typedef enum
{
    FLD_TMR0_MODE_IRQ = BIT(0),
    FLD_TMR0_CAPT_IRQ = BIT(1),
    FLD_TMR0_COMP_IRQ = BIT(2),
    FLD_TMR1_MODE_IRQ = BIT(3),
    FLD_TMR1_CAPT_IRQ = BIT(4),
    FLD_TMR1_COMP_IRQ = BIT(5),
    FLD_TMR0_OV_IRQ   = BIT(6),
    FLD_TMR1_OV_IRQ   = BIT(7),
} timer_irq_e;

#define reg_tmr_wd_en REG_ADDR8(TIMER_BASE_ADDR + 0x22)

enum
{
    FLD_TMR_WD_EN      = BIT(0),
    FLD_PEM_EVENT_EN   = BIT(1),
    FLD_PEM_TASK1_0_EN = BIT(2),
    FLD_PEM_TASK1_1_EN = BIT(3),
    FLD_PEM_TASK1_2_EN = BIT(4),
    FLD_PEM_TASK1_3_EN = BIT(5),
};

#define reg_tmr_pem_task_en REG_ADDR8(TIMER_BASE_ADDR + 0x23)

enum
{
    FLD_PEM_TASK_0_EN = BIT(0),
    FLD_PEM_TASK_1_EN = BIT(1),
    FLD_PEM_TASK_2_EN = BIT(2),
    FLD_PEM_TASK_3_EN = BIT(3),
    FLD_PEM_TASK_4_EN = BIT(4),
    FLD_PEM_TASK_5_EN = BIT(5),
    FLD_PEM_TASK_6_EN = BIT(6),
    FLD_PEM_TASK_7_EN = BIT(7),
};

#endif
