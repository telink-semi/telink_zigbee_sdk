/********************************************************************************************************
 * @file    plic_reg.h
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
#ifndef INTERRUPT_REG_H
#define INTERRUPT_REG_H
#include "soc.h"


/*******************************     interrupt registers:  plic base 0xc4000000   ******************************/
#define reg_irq_feature              (*(volatile unsigned long*)(0 + (0xc4000000)))

#define reg_irq_pending(i)           (*(volatile unsigned long*)(0 + (0xc4001000+((i>31) ? 4 : 0))))

#define reg_irq_src0                 (*(volatile unsigned long*)(0 + (0xc4002000)))
#define reg_irq_src1                 (*(volatile unsigned long*)(0 + (0xc4002004)))

#define reg_irq_src(i)               (*(volatile unsigned long*)(0 + (0xc4002000+((i>31) ? 4 : 0) )))

#define reg_irq_threshold            (*(volatile unsigned long*)(0 + (0xc4200000)))
#define reg_irq_done                 (*(volatile unsigned long*)(0 + (0xc4200004)))

#define reg_irq_src_priority(i)      (*(volatile unsigned long*)(0 + 0xc4000000+((i)<<2)))

#endif
