/********************************************************************************************************
 * @file    charger.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "reg_include/charger_reg.h"
#include "compiler.h"
#include "lib/include/analog.h"

/**
 * @brief     open charger 1p0v.
 * @return    none.
 */
static inline void charger_open(void)
{
    analog_write_reg8(0x22, analog_read_reg8(0x22) | 0x08); //<3> pm_chg_en_1p0v 1:open charge 0:close charge
}

/**
 * @brief     close charger 1p0v.
 * @return    none.
 */
static inline void charger_close(void)
{
    analog_write_reg8(0x22, analog_read_reg8(0x22) & 0xf7); //<3> pm_chg_en_1p0v 1:open charge 0:close charge
}

/**
 * @brief   This function serves the initialization of charger module charging function.
 * @return  none
 */
void charger_init(void);
