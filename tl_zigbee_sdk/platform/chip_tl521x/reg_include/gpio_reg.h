/********************************************************************************************************
 * @file    gpio_reg.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef GPIO_REG_H_
#define GPIO_REG_H_
#include "soc.h"
/*******************************      gpio registers: 0x140c00      ******************************/
#define GPIO_BASE_ADDR            0x140c00

//GPIO_PAx
#define reg_gpio_pa_setting1      REG_ADDR32(GPIO_BASE_ADDR)
#define reg_gpio_pa_in            REG_ADDR8(GPIO_BASE_ADDR)
#define reg_gpio_pa_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x01)
#define reg_gpio_pa_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x02)
#define reg_gpio_pa_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x03)

#define reg_gpio_pa_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x04)
#define reg_gpio_pa_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x04)
#define reg_gpio_pa_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x05)
#define reg_gpio_pa_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x06)
#define reg_gpio_pa_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x07)

#define reg_gpio_pa_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x08)
#define reg_gpio_pa_pin0           REG_ADDR8(GPIO_BASE_ADDR + 0x09)
#define reg_gpio_pa_pin1           REG_ADDR8(GPIO_BASE_ADDR + 0x0a)

//GPIO_PBx
#define reg_gpio_pb_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x10)
#define reg_gpio_pb_in            REG_ADDR8(GPIO_BASE_ADDR + 0x10)
#define reg_gpio_pb_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x11)//GPIO_PB0-3
#define areg_gpio_pb_ie           0x120//GPIO_PB4-7
#define reg_gpio_pb_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x12)
#define reg_gpio_pb_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x13)

#define reg_gpio_pb_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x14)
#define reg_gpio_pb_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x14)
#define reg_gpio_pb_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x15)
#define reg_gpio_pb_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x16)
#define reg_gpio_pb_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x17)

#define reg_gpio_pb_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x18)
#define reg_gpio_pb_pin0           REG_ADDR8(GPIO_BASE_ADDR + 0x19)//GPIO_PB0-3
#define reg_gpio_pb_pin1           REG_ADDR8(GPIO_BASE_ADDR + 0x1a)//GPIO_PB0-3
#define areg_gpio_pb_pin0          0x122//GPIO_PB4-7
#define areg_gpio_pb_pin1          0x123//GPIO_PB4-7

//GPIO_PCx
#define reg_gpio_pc_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x20)
#define reg_gpio_pc_in            REG_ADDR8(GPIO_BASE_ADDR + 0x20)
#define areg_gpio_pc_ie           0x124
#define reg_gpio_pc_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x22)
#define reg_gpio_pc_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x23)

#define reg_gpio_pc_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x24)
#define reg_gpio_pc_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x24)
#define reg_gpio_pc_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x25)
#define reg_gpio_pc_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x26)
#define reg_gpio_pc_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x27)

#define areg_gpio_pc_pin0          0x126
#define areg_gpio_pc_pin1          0x127

//GPIO_PDx
#define reg_gpio_pd_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x30)
#define reg_gpio_pd_in            REG_ADDR8(GPIO_BASE_ADDR + 0x30)
#define reg_gpio_pd_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x31)
#define reg_gpio_pd_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x32)
#define reg_gpio_pd_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x33)

#define reg_gpio_pd_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x34)
#define reg_gpio_pd_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x34)
#define reg_gpio_pd_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x35)
#define reg_gpio_pd_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x36)
#define reg_gpio_pd_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x37)

#define reg_gpio_pd_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x38)
//#define reg_gpio_pd_sr            REG_ADDR8(GPIO_BASE_ADDR + 0x38)
#define reg_gpio_pd_pin0           REG_ADDR8(GPIO_BASE_ADDR + 0x39)
#define reg_gpio_pd_pin1           REG_ADDR8(GPIO_BASE_ADDR + 0x3a)

//GPIO_PEx
#define reg_gpio_pe_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x40)
#define reg_gpio_pe_in            REG_ADDR8(GPIO_BASE_ADDR + 0x40)
#define reg_gpio_pe_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x41)
#define reg_gpio_pe_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x42)
#define reg_gpio_pe_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x43)

#define reg_gpio_pe_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x44)
#define reg_gpio_pe_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x44)
#define reg_gpio_pe_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x45)
#define reg_gpio_pe_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x46)
#define reg_gpio_pe_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x47)

#define reg_gpio_pe_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x48)
#define reg_gpio_pe_pin0           REG_ADDR8(GPIO_BASE_ADDR + 0x49)
#define reg_gpio_pe_pin1           REG_ADDR8(GPIO_BASE_ADDR + 0x4a)

//GPIO_PFx
#define reg_gpio_pf_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x50)
#define reg_gpio_pf_in            REG_ADDR8(GPIO_BASE_ADDR + 0x50)
#define reg_gpio_pf_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x51)
#define reg_gpio_pf_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x52)
#define reg_gpio_pf_rsvd3         REG_ADDR8(GPIO_BASE_ADDR + 0x53)

#define reg_gpio_pf_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x54)
#define reg_gpio_pf_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x54)
#define reg_gpio_pf_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x55)
#define reg_gpio_pf_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x56)
#define reg_gpio_pf_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x57)

#define reg_gpio_pf_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x58)
#define reg_gpio_pf_pin0           REG_ADDR8(GPIO_BASE_ADDR + 0x59)
#define reg_gpio_pf_pin1           REG_ADDR8(GPIO_BASE_ADDR + 0x5a)


#define reg_gpio_in(i)            REG_ADDR8(GPIO_BASE_ADDR + ((i >> 8) << 4))
#define reg_gpio_ie(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x01 + ((i >> 8) << 4))
#define reg_gpio_oen(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x02 + ((i >> 8) << 4))
#define reg_gpio_pol(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x03 + ((i >> 8) << 4))
#define reg_gpio_out_set_clear(i) REG_ADDR16(GPIO_BASE_ADDR + 0x04 + ((i >> 8) << 4))
#define reg_gpio_out_set(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x04 + ((i >> 8) << 4))
#define reg_gpio_out_clear(i)     REG_ADDR8(GPIO_BASE_ADDR + 0x05 + ((i >> 8) << 4))
#define reg_gpio_func(i)          REG_ADDR8(GPIO_BASE_ADDR + 0x06 + ((i >> 8) << 4))
#define reg_gpio_out_toggle(i)    REG_ADDR8((GPIO_BASE_ADDR + 0x07) + ((i >> 8) << 4))
#define reg_gpio_pin0(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x09 + ((i >> 8) << 4))
#define reg_gpio_pin1(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x0a + ((i >> 8) << 4))

#define reg_gpio_irq_en(i, j)     REG_ADDR8((GPIO_BASE_ADDR + 0x108 + (j)) + ((i >> 8) << 4))

#define reg_gpio_func_mux(i)      REG_ADDR8(GPIO_BASE_ADDR + 0xb0 + ((i >> 8) << 3) + BIT_LOW_BIT(i))
#define reg_gpio_irq_ctrl         REG_ADDR8(GPIO_BASE_ADDR + 0x102)
enum
{
    FLD_GPIO_CORE_WAKEUP_EN    = BIT(0),
    FLD_GPIO_CORE_INTERRUPT_EN = BIT(1),
    FLD_GPIO_PEM_EVENT_EN      = BIT(4),
    FLD_TX_CLK_EN              = BIT(5),
};

#define reg_gpio_pem_ctrl0    REG_ADDR8(GPIO_BASE_ADDR + 0x103)
#define reg_gpio_irq_level    REG_ADDR8(GPIO_BASE_ADDR + 0x104)
#define reg_gpio_irq_clr      REG_ADDR8(GPIO_BASE_ADDR + 0x105)
#define reg_gpio_irq_src_mask REG_ADDR8(GPIO_BASE_ADDR + 0x106)
#define reg_gpio_pem_ctrl1    REG_ADDR8(GPIO_BASE_ADDR + 0x107)

enum
{
    FLD_GPIO_PEM_EVENT_SEL = BIT_RNG(0, 2),
    FLD_GPIO_PEM_TASK_SEL  = BIT_RNG(4, 6),
};


#endif
