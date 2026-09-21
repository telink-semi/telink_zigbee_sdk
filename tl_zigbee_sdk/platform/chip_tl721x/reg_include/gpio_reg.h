/********************************************************************************************************
 * @file    gpio_reg.h
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
#ifndef GPIO_REG_H_
#define GPIO_REG_H_
#include "soc.h"
/*******************************      gpio registers: 0x140c00      ******************************/
#define GPIO_BASE_ADDR            0x140c00
#define reg_gpio_pa_setting1      REG_ADDR32(GPIO_BASE_ADDR)
#define reg_gpio_pa_in            REG_ADDR8(GPIO_BASE_ADDR)
#define reg_gpio_pa_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x01)
#define reg_gpio_pa_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x02)

#define reg_gpio_pa_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x04)
#define reg_gpio_pa_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x04)
#define reg_gpio_pa_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x05)
#define reg_gpio_pa_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x06)
#define reg_gpio_pa_irq_en        REG_ADDR8(GPIO_BASE_ADDR + 0x07)

#define reg_gpio_pa_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x08)
#define reg_gpio_pa_irq_risc0_en  REG_ADDR8(GPIO_BASE_ADDR + 0x08)
#define reg_gpio_pa_irq_risc1_en  REG_ADDR8(GPIO_BASE_ADDR + 0x09)
#define reg_gpio_pa_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x0a)
#define reg_gpio_pa_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x0b)

#define reg_gpio_pa_setting4      REG_ADDR32(GPIO_BASE_ADDR + 0x0c)
#define reg_gpio_pa_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x0c)
#define reg_gpio_pa_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x0d)
#define reg_gpio_pa_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x0e)

#define reg_gpio_pa_fs_l          REG_ADDR32(GPIO_BASE_ADDR + 0x70)
#define reg_gpio_pa0_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x70)
#define reg_gpio_pa1_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x71)
#define reg_gpio_pa2_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x72)
#define reg_gpio_pa3_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x73)

#define reg_gpio_pa_fs_h          REG_ADDR32(GPIO_BASE_ADDR + 0x74)
#define reg_gpio_pa4_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x74)

#define reg_gpio_pb_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x10)
#define reg_gpio_pb_in            REG_ADDR8(GPIO_BASE_ADDR + 0x10)
#define reg_gpio_pb_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x11)
#define reg_gpio_pb_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x12)

#define reg_gpio_pb_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x14)
#define reg_gpio_pb_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x14)
#define reg_gpio_pb_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x15)
#define reg_gpio_pb_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x16)
#define reg_gpio_pb_irq_en        REG_ADDR8(GPIO_BASE_ADDR + 0x17)

#define reg_gpio_pb_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x18)
#define reg_gpio_pb_irq_risc0_en  REG_ADDR8(GPIO_BASE_ADDR + 0x18)
#define reg_gpio_pb_irq_risc1_en  REG_ADDR8(GPIO_BASE_ADDR + 0x19)
#define reg_gpio_pb_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x1a)
#define reg_gpio_pb_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x1b)

#define reg_gpio_pb_setting4      REG_ADDR32(GPIO_BASE_ADDR + 0x1c)
#define reg_gpio_pb_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x1c)
#define reg_gpio_pb_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x1d)
#define reg_gpio_pb_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x1e)

#define reg_gpio_pb_fs_l          REG_ADDR32(GPIO_BASE_ADDR + 0x78)
#define reg_gpio_pb0_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x78)
#define reg_gpio_pb1_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x79)
#define reg_gpio_pb2_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x7a)
#define reg_gpio_pb3_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x7b)

#define reg_gpio_pb_fs_h          REG_ADDR32(GPIO_BASE_ADDR + 0x7c)
#define reg_gpio_pb4_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x7c)
#define reg_gpio_pb5_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x7d)
#define reg_gpio_pb6_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x7e)
#define reg_gpio_pb7_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x7f)

#define reg_gpio_pc_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x20)
#define reg_gpio_pc_in            REG_ADDR8(GPIO_BASE_ADDR + 0x20)
#define areg_gpio_pc_ie           0xbd
#define reg_gpio_pc_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x22)

#define reg_gpio_pc_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x24)
#define reg_gpio_pc_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x24)
#define areg_gpio_pc_ds           0xbf
#define reg_gpio_pc_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x26)
#define reg_gpio_pc_irq_en        REG_ADDR8(GPIO_BASE_ADDR + 0x27)

#define reg_gpio_pc_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x28)
#define reg_gpio_pc_irq_risc0_en  REG_ADDR8(GPIO_BASE_ADDR + 0x28)
#define reg_gpio_pc_irq_risc1_en  REG_ADDR8(GPIO_BASE_ADDR + 0x29)
#define areg_gpio_pc_pd           0xc0
#define areg_gpio_pc_pu           0xc1

#define reg_gpio_pc_setting4      REG_ADDR32(GPIO_BASE_ADDR + 0x2c)
#define reg_gpio_pc_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x2c)
#define reg_gpio_pc_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x2d)
#define reg_gpio_pc_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x2e)

#define reg_gpio_pc_fs_l          REG_ADDR32(GPIO_BASE_ADDR + 0x80)
#define reg_gpio_pc0_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x80)
#define reg_gpio_pc1_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x81)
#define reg_gpio_pc2_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x82)
#define reg_gpio_pc3_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x83)

#define reg_gpio_pc_fs_h          REG_ADDR32(GPIO_BASE_ADDR + 0x84)
#define reg_gpio_pc4_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x84)
#define reg_gpio_pc5_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x85)
#define reg_gpio_pc6_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x86)
#define reg_gpio_pc7_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x87)

#define reg_gpio_pd_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x30)
#define reg_gpio_pd_in            REG_ADDR8(GPIO_BASE_ADDR + 0x30)
#define areg_gpio_pd_ie           0xc2
#define reg_gpio_pd_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x32)

#define reg_gpio_pd_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x34)
#define reg_gpio_pd_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x34)
#define areg_gpio_pd_ds           0xc3
#define reg_gpio_pd_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x36)
#define reg_gpio_pd_irq_en        REG_ADDR8(GPIO_BASE_ADDR + 0x37)

#define reg_gpio_pd_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x38)
#define reg_gpio_pd_irq_risc0_en  REG_ADDR8(GPIO_BASE_ADDR + 0x38)
#define reg_gpio_pd_irq_risc1_en  REG_ADDR8(GPIO_BASE_ADDR + 0x39)
#define areg_gpio_pd_pd           0xc4
#define areg_gpio_pd_pu           0xc5

#define reg_gpio_pd_setting4      REG_ADDR32(GPIO_BASE_ADDR + 0x3c)
#define reg_gpio_pd_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x3c)
#define reg_gpio_pd_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x3d)
#define reg_gpio_pd_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x3e)

#define reg_gpio_pd_fs_l          REG_ADDR32(GPIO_BASE_ADDR + 0x88)
#define reg_gpio_pd0_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x88)
#define reg_gpio_pd1_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x89)
#define reg_gpio_pd2_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x8a)
#define reg_gpio_pd3_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x8b)

#define reg_gpio_pd_fs_h          REG_ADDR32(GPIO_BASE_ADDR + 0x8c)
#define reg_gpio_pd4_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x8c)
#define reg_gpio_pd5_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x8d)
#define reg_gpio_pd6_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x8e)
#define reg_gpio_pd7_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x8f)

#define reg_gpio_pe_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x40)
#define reg_gpio_pe_in            REG_ADDR8(GPIO_BASE_ADDR + 0x40)
#define reg_gpio_pe_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x41)
#define reg_gpio_pe_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x42)

#define reg_gpio_pe_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x44)
#define reg_gpio_pe_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x44)
#define reg_gpio_pe_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x45)
#define reg_gpio_pe_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x46)
#define reg_gpio_pe_irq_en        REG_ADDR8(GPIO_BASE_ADDR + 0x47)

#define reg_gpio_pe_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x48)
#define reg_gpio_pe_irq_risc0_en  REG_ADDR8(GPIO_BASE_ADDR + 0x48)
#define reg_gpio_pe_irq_risc1_en  REG_ADDR8(GPIO_BASE_ADDR + 0x49)
#define reg_gpio_pe_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x4a)
#define reg_gpio_pe_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x4b)

#define reg_gpio_pe_setting4      REG_ADDR32(GPIO_BASE_ADDR + 0x4c)
#define reg_gpio_pe_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x4c)
#define reg_gpio_pe_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x4d)
#define reg_gpio_pe_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x4e)

#define reg_gpio_pe0_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x90)
#define reg_gpio_pe6_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x96)
#define reg_gpio_pe7_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x97)

#define reg_gpio_pf_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x50)
#define reg_gpio_pf_in            REG_ADDR8(GPIO_BASE_ADDR + 0x50)
#define reg_gpio_pf_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x51)
#define reg_gpio_pf_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x52)

#define reg_gpio_pf_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x54)
#define reg_gpio_pf_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x54)
#define reg_gpio_pf_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x55)
#define reg_gpio_pf_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x56)
#define reg_gpio_pf_irq_en        REG_ADDR8(GPIO_BASE_ADDR + 0x57)

#define reg_gpio_pf_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x58)
#define reg_gpio_pf_irq_risc0_en  REG_ADDR8(GPIO_BASE_ADDR + 0x58)
#define reg_gpio_pf_irq_risc1_en  REG_ADDR8(GPIO_BASE_ADDR + 0x59)
#define reg_gpio_pf_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x5a)
#define reg_gpio_pf_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x5b)

#define reg_gpio_pf_setting4      REG_ADDR32(GPIO_BASE_ADDR + 0x5c)
#define reg_gpio_pf_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x5c)
#define reg_gpio_pf_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x5d)
#define reg_gpio_pf_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x5e)

#define reg_gpio_pf_fs_l          REG_ADDR32(GPIO_BASE_ADDR + 0x98)
#define reg_gpio_pf0_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x98)
#define reg_gpio_pf1_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x99)
#define reg_gpio_pf2_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x9a)
#define reg_gpio_pf3_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x9b)

#define reg_gpio_pf_fs_h          REG_ADDR32(GPIO_BASE_ADDR + 0x9c)
#define reg_gpio_pf4_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x9c)
#define reg_gpio_pf5_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x9d)
#define reg_gpio_pf6_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x9e)
#define reg_gpio_pf7_fs           REG_ADDR8(GPIO_BASE_ADDR + 0x9f)

#define reg_gpio_pg_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x60)
#define reg_gpio_pg_in            REG_ADDR8(GPIO_BASE_ADDR + 0x60)
#define reg_gpio_pg_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x61)
#define reg_gpio_pg_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x62)

#define reg_gpio_pg_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x64)
#define reg_gpio_pg_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x64)
#define reg_gpio_pg_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x65)
#define reg_gpio_pg_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x66)
#define reg_gpio_pg_irq_en        REG_ADDR8(GPIO_BASE_ADDR + 0x67)

#define reg_gpio_pg_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x68)
#define reg_gpio_pg_irq_risc0_en  REG_ADDR8(GPIO_BASE_ADDR + 0x68)
#define reg_gpio_pg_irq_risc1_en  REG_ADDR8(GPIO_BASE_ADDR + 0x69)
#define reg_gpio_pg_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x6a)
#define reg_gpio_pg_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x6b)

#define reg_gpio_pg_setting4      REG_ADDR32(GPIO_BASE_ADDR + 0x6c)
#define reg_gpio_pg_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x6c)
#define reg_gpio_pg_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x6d)
#define reg_gpio_pg_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x6e)


#define reg_gpio_in(i)            REG_ADDR8(GPIO_BASE_ADDR + ((i >> 8) << 4))
#define reg_gpio_ie(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x01 + ((i >> 8) << 4))
#define reg_gpio_oen(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x02 + ((i >> 8) << 4))


#define reg_gpio_pol(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x04 + ((i >> 8) << 4))
#define reg_gpio_ds(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x05 + ((i >> 8) << 4))
#define reg_gpio_func(i)          REG_ADDR8(GPIO_BASE_ADDR + 0x06 + ((i >> 8) << 4))
#define reg_gpio_irq_en(i)        REG_ADDR8(GPIO_BASE_ADDR + 0x07 + ((i >> 8) << 4))  // reg_irq_mask: FLD_IRQ_GPIO_EN
#define reg_gpio_irq_risc0_en(i)  REG_ADDR8(GPIO_BASE_ADDR + 0x08 + ((i >> 8) << 4))  // reg_irq_mask: FLD_IRQ_GPIO_RISC0_EN
#define reg_gpio_irq_risc1_en(i)  REG_ADDR8(GPIO_BASE_ADDR + 0x09 + ((i >> 8) << 4))  // reg_irq_mask: FLD_IRQ_GPIO_RISC1_EN
#define reg_gpio_pd(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x0a + ((i >> 8) << 4))
#define reg_gpio_pu(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x0b + ((i >> 8) << 4))
#define reg_gpio_out_set_clear(i) REG_ADDR16(GPIO_BASE_ADDR + 0x0c + ((i >> 8) << 4)) //
#define reg_gpio_out_set(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x0c + ((i >> 8) << 4))
#define reg_gpio_out_clear(i)     REG_ADDR8(GPIO_BASE_ADDR + 0x0d + ((i >> 8) << 4))
#define reg_gpio_out_toggle(i)    REG_ADDR8((GPIO_BASE_ADDR + 0x0e) + ((i >> 8) << 4))

#define reg_gpio_func_mux(i)      REG_ADDR8(GPIO_BASE_ADDR + 0x70 + ((i >> 8) << 3) + BIT_LOW_BIT(i))
#define reg_gpio_irq_ctrl         REG_ADDR8(GPIO_BASE_ADDR + 0xa2)

enum
{
    FLD_GPIO_CORE_WAKEUP_EN      = BIT(0),
    FLD_GPIO_CORE_INTERRUPT_EN   = BIT(1),
    FLD_GPIO_IRQ_MASK_GPIO       = BIT(2),
    FLD_GPIO_IRQ_MASK_GPIO2RISC0 = BIT(3),
    FLD_GPIO_IRQ_MASK_GPIO2RISC1 = BIT(4),

    FLD_GPIO_IRQ_LVL_GPIO       = BIT(5),
    FLD_GPIO_IRQ_LVL_GPIO2RISC0 = BIT(6),
    FLD_GPIO_IRQ_LVL_GPIO2RISC1 = BIT(7),
};

#define reg_gpio_pem_ctrl0 REG_ADDR8(GPIO_BASE_ADDR + 0xa3)
#define reg_gpio_irq_level REG_ADDR8(GPIO_BASE_ADDR + 0xa4)
#define reg_gpio_irq_sel   REG_ADDR8(GPIO_BASE_ADDR + 0xa5)

enum
{
    FLD_GPIO_PEM_EVENT_EN = BIT(3),
};

#define reg_gpio_irq_src_mask REG_ADDR8(GPIO_BASE_ADDR + 0xa6)
#define reg_gpio_pem_ctrl1    REG_ADDR8(GPIO_BASE_ADDR + 0xa7)

enum
{
    FLD_GPIO_PEM_EVENT_SEL = BIT_RNG(0, 3),
    FLD_GPIO_PEM_TASK_SEL  = BIT_RNG(4, 7),
};

#define reg_gpio_irq_clr REG_ADDR8(GPIO_BASE_ADDR + 0xa8)
#define reg_gpio_irq_src REG_ADDR8(GPIO_BASE_ADDR + 0xa9)

#endif
