/********************************************************************************************************
 * @file    analog_afe1v_reg.h
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
#ifndef ANALOG_AFE1V_REG_H
#define ANALOG_AFE1V_REG_H
#include "soc.h"

#define areg_0x84 0x84
#define areg_0x85 0x85

enum
{
    FLD_LOCK_DET_SIG_ENABLE = BIT(7), //lock detect enable signal
};

#define areg_0x86 0x86

enum
{
    FLD_LOCK_DET_SIG_RESET = BIT(7), //lock detect reset signal
};

#define areg_0x87 0x87

#define areg_0x88 0x88

enum
{
    FLD_BBPLL_LOCK_DETECTOR = BIT(5),

    FLD_XO_READY_ANA = BIT(7),
};

#define areg_0x8b 0x8b

enum
{
    FLD_XO_DYN_CAP_ANA = BIT(1), //Cap control to optimize the startup time.

    FLD_XO_CNT_OFF_ANA = BIT(3), //Disable xo counter
};

#define areg_0x8c 0x8c

enum
{
    FLD_XO_EN_CLK_ANA = BIT(1), //xtal_24M clock to analog   0:turn off  1:turn on   default :0
};

/**
 * BIT[0:1] Adjust output current in CODEC_BIAS.
 * BIT[2] 0: Unmute PGA 1: Mute PGA.
 * BIT[3] 0: Enable PGA input 1: Disable PGA input.
 * BIT[4:7] 1111 for min PGA gain; 0000 for max PGA gain.
 */
#define areg_0x8d 0x8d

enum
{
    FLD_AUDIO_IBSEL      = BIT_RNG(0, 1),
    FLD_AUDIO_MUTE_PGA   = BIT(2),
    FLD_AUDIO_INMUTE_PGA = BIT(3),
    FLD_AUDIO_PGAVOL_IN  = BIT_RNG(4, 7),
};

/**
 * BIT[0] 0: enable 0.6V Vmid  1:keep 0.9V Vmid.
 * BIT[3] 0:PD signal for 1st OPA in PGA. 0 for power-on.
 * BIT[4] PD signal for 2nd OPA in PGA. 0 for power-on.
 * BIT[5] No actual use.
 * BIT[6] PD signal for sigma-delta ADC. 0 for power-on.
 * BIT[7] PD signal for CODEC_BIAS. 0 for power-on.
 */
#define areg_0x8e 0x8e

enum
{
    FLD_AUDIO_PD_ENABLE_0R6 = BIT(0),
    FLD_AUDIO_PD_INPPGA     = BIT(3),
    FLD_AUDIO_PD_PGABUF     = BIT(4),
    FLD_AUDIO_PD_PGABOOST   = BIT(5),
    FLD_AUDIO_PD_ASDM       = BIT(6),
    FLD_AUDIO_PD_BIAS       = BIT(7),
};

/**
 * BIT[1] Enable VMID voltage (A0,A1 version 1:turn off  0:turn on default:0;A2 version 0:turn off  1:turn on default:0.)
 */
#define areg_0x8f 0x8f

enum
{
    FLD_AUDIO_VMID_PD = BIT(1),
};

#define areg_0xc6 0xc6

enum
{
    FLD_CAL_32K_RC_DISABLE = 0xf6,
    FLD_CAL_32K_RC_ENABLE  = 0xf7,
};

#define areg_0xc7 0xc7

enum
{
    FLD_CAL_24M_RC_DISABLE = 0x0e,
    FLD_CAL_24M_RC_ENABLE  = 0x0f,
};

#define areg_0xc8 0xc8
#define areg_0xc9 0xc9
#define areg_0xca 0xca
#define areg_0xcb 0xcb

#define areg_0xcf 0xcf

enum
{
    FLD_CAL_32K_DONE = BIT(6),
    FLD_CAL_24M_DONE = BIT(7),
};

#endif
