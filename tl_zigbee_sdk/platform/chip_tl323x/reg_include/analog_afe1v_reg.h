/********************************************************************************************************
 * @file    analog_afe1v_reg.h
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
#ifndef ANALOG_AFE1V_REG_H
#define ANALOG_AFE1V_REG_H
#include "soc.h"

#define areg_0x101 0x101

enum
{
    FLD_ENABLE_LOCK_DET_BBPLL        = BIT(5),
    FLD_POWER_ON_BBPLL_SUPPLY_SWITCH = BIT(7),
};

#define areg_0x108 0x108

enum
{
    FLD_LC_COMP_OUT         = BIT(0),
    FLD_XO_READY_ANA        = BIT(1),
    FLD_BBPLL_LOCK_DETECTOR = BIT(2),
};


#define areg_0x146 0x146

enum
{
    FLD_CAL_32K_RC_DISABLE = 0xf6,
    FLD_CAL_32K_RC_ENABLE  = 0xf7,
};

#define areg_0x147 0x147

enum
{
    FLD_CAL_24M_RC_DISABLE = 0x0e,
    FLD_CAL_24M_RC_ENABLE  = 0x0f,
};

#define areg_0x148 0x148
#define areg_0x149 0x149
#define areg_0x14a 0x14a
#define areg_0x14b 0x14b

#define areg_0x14f 0x14f

enum
{
    FLD_CAL_32K_DONE = BIT(6),
    FLD_CAL_24M_DONE = BIT(7),
};

#endif
