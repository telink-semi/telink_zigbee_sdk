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

#define areg_0x81 0x81

enum
{
    FLD_ENABLE_LOCK_DET_BBPLL        = BIT(5),
    FLD_POWER_ON_BBPLL_SUPPLY_SWITCH = BIT(7),
};

#define areg_0x88 0x88

enum
{
    FLD_XO_READY_ANA        = BIT(1),
    FLD_BBPLL_LOCK_DETECTOR = BIT(2),
};

/**
 * BIT[0] Force start for XO LDO PTAT loop.
 * BIT[1] xtal_24M clock to analog   0:turn off  1:turn on   default :0.
 * BIT[2] 0: DEM function of sigma-delta ADC is disabled 1: DEM function of sigma-delta ADC is enabled.
 * BIT[3:4] Dither value selection of sigma-delta ADC.
 * BIT[5] Enable dither function of sigma-delta ADC.
 * BIT[6] Enable dither signal into sigma-delta ADC loop.
 * BIT[7] Adjust output current in CODEC_BIAS.
 */
#define areg_0x8c 0x8c

enum
{
    FLD_XO_LDO_FORCE_ANA  = BIT(0),
    FLD_XO_EN_CLK_ANA_ANA = BIT(1),
    FLD_AUDIO_DEM_EN      = BIT(2),
    FLD_AUDIO_A_DITHER    = BIT_RNG(3, 4),
    FLD_AUDIO_EN_DITHER   = BIT(5),
    FLD_AUDIO_DITHER_IN   = BIT(6),
    FLD_AUDIO_LV_EN       = BIT(7),
};

/**
 * BIT[0:1] Control original bias current in CODEC_BIAS.
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
 * BIT[0:1] Control the filtering cap value in PGA.
 * BIT[2:3] Control the driving ability of VMID.
 * BIT[4] PD signal for PGA.
 * BIT[5] PD signal for CODEC Bias.
 * BIT[6] PD signal for ADC.
 * BIT[7] PD signal for VMID.
 */
#define areg_0x8e 0x8e

enum
{
    FLD_AUDIO_FILTCAP_CTL = BIT_RNG(0, 1),
    FLD_AUDIO_VMIDSEL     = BIT_RNG(2, 3),
    FLD_AUDIO_PD_PGA      = BIT(4),
    FLD_AUDIO_PD_BIAS     = BIT(5),
    FLD_AUDIO_PD_ADC      = BIT(6),
    FLD_AUDIO_PD_VMID     = BIT(7),
};

/**
 * Low-power mode enable signal.
   0: default mode 1: low-power mode
 * BIT[0:2] for adc Low-power mode.
 * BIT[3]   for pga Low-power mode
 */
#define areg_0x8f 0x8f

enum
{
    FLD_AUDIO_ADC_LOW_POWER_MODE = BIT_RNG(0, 2),
    FLD_AUDIO_PGA_LOW_POWER_MODE = BIT(3),
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
