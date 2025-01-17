/********************************************************************************************************
 * @file    pem_reg.h
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
#ifndef PEM_REG_H
#define PEM_REG_H

#include "soc.h"

#define PEM_BASE_ADDR  0x142000
#define reg_pem_ctr(i) REG_ADDR32((PEM_BASE_ADDR + (i) * 0x04))

enum
{
    FLD_PEM_EVENT_MODULE_SEL             = BIT_RNG(0, 4),
    FLD_PEM_TASK_MODULE_SEL              = BIT_RNG(8, 12),
    FLD_PEM_EVENT_SIG_SEL                = BIT_RNG(16, 18),
    FLD_PEM_TASK_SIG_SEL                 = BIT_RNG(19, 21),
    FLD_PEM_EVENT_CLK_SEL                = BIT_RNG(22, 23),
    FLD_PEM_EVENT_CHANGE_LVL_TO_EDGE_SEL = BIT_RNG(24, 26),
    FLD_PEM_CH_EN                        = BIT(27),
    FLD_PEM_EVENT_LVL                    = BIT(28),
    FLD_PEM_TASK_LVL                     = BIT(29),
    FLD_PEM_TASK_CLK_SEL                 = BIT_RNG(30, 31),
};
#endif /* PEM_REG_H_ */
