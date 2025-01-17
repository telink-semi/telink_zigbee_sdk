/********************************************************************************************************
 * @file    plic_sw_reg.h
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
#ifndef __PLIC_SW_REG_H_
#define __PLIC_SW_REG_H_
#include "soc.h"

/*******************************     plic_sw registers:     ******************************/
#define reg_irq_sw_pending (*(volatile unsigned long *)(0xc6401000))

enum
{
    FLD_PLIC_SW_SRC = BIT(1),
};

#define reg_irq_sw_src  (*(volatile unsigned long *)(0xc6402000))

#define reg_irq_sw_done (*(volatile unsigned long *)(0xc6600004))

enum
{
    FLD_PLIC_SW_ID = BIT(0),
};


#endif /* end of __PLIC_SW_REG_H_ */
