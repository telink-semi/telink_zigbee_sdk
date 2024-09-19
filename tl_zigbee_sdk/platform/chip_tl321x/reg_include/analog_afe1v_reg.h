/********************************************************************************************************
 * @file    analog_afe1v_reg.h
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
#ifndef ANALOG_AFE1V_REG_H
#define ANALOG_AFE1V_REG_H
#include "soc.h"

#define areg_0x81                       0x81
enum {
    FLD_ENABLE_LOCK_DET_BBPLL           =  BIT(5),
    FLD_POWER_ON_BBPLL_SUPPLY_SWITCH    =  BIT(7),
};

#define areg_0x88                       0x88
enum {
    FLD_XO_READY_ANA                    =  BIT(1),
    FLD_BBPLL_LOCK_DETECTOR             =  BIT(2),
};

#define areg_0x8c                       0x8c

#define areg_0xc6                       0xc6
enum{
    FLD_CAL_32K_RC_DISABLE              =  0xf6,
    FLD_CAL_32K_RC_ENABLE               =  0xf7,
};

#define areg_0xc7                       0xc7
enum{
    FLD_CAL_24M_RC_DISABLE              =  0x0e,
    FLD_CAL_24M_RC_ENABLE               =  0x0f,
};

#define areg_0xc8                       0xc8
#define areg_0xc9                       0xc9
#define areg_0xca                       0xca
#define areg_0xcb                       0xcb

#define areg_0xcf                       0xcf
enum{
    FLD_CAL_32K_DONE                    =  BIT(6),
    FLD_CAL_24M_DONE                    =  BIT(7),
};

#endif
