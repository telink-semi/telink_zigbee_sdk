/********************************************************************************************************
 * @file    lpc.h
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
#pragma once

#include "lib/include/analog.h"

/**
 * define input IO.
 */
typedef enum{
    LPC_INPUT_PB1 = 1,
    LPC_INPUT_PB2 = 2,
    LPC_INPUT_PB3 = 3,
    LPC_INPUT_PB4 = 4,
    LPC_INPUT_PB5 = 5,
    LPC_INPUT_PB6 = 6,
    LPC_INPUT_PB7 = 7,
}lpc_input_channel_e;

/**
 * define work mode.
 */
typedef enum{
    LPC_NORMAL = 0,
    LPC_LOWPOWER,
}lpc_mode_e;

/**
 * define Reference voltage.
 * When BG or UVLO is enabled, the same value in the register represents a different vref value.
 * Default is UVLO Vref.
 */
typedef enum{
    LPC_REF_974MV  = 1,
    LPC_UVLO_REF_1088MV = 1,
    LPC_REF_923MV  = 2,
    LPC_UVLO_REF_1036MV = 2,
    LPC_REF_872MV  = 3,
    LPC_UVLO_REF_983MV  = 3,
    LPC_REF_820MV  = 4,
    LPC_UVLO_REF_931MV  = 4,
    LPC_REF_PB0    = 5,
    LPC_REF_PB3    = 6,
}lpc_reference_e;

/**
 * define scale.
 * The input voltage is multiplied by the voltage divider factor and then compared to the reference voltage.
 */
typedef enum{
    LPC_SCALING_PER25  = 0,// 1/4
    LPC_SCALING_PER50  = 1,// 2/4
    LPC_SCALING_PER75  = 2,// 3/4
    LPC_SCALING_PER100 = 3,// 4/4
}lpc_scaling_e;

/**
 * @brief       This function servers to powers down low power comparator.
 * @return      none.
 */
static inline void lpc_power_down(void)
{
    analog_write_reg8(0x06,(analog_read_reg8(0x06))|0x02);
}

/**
 * @brief       This function servers to power on low power comparator.
 * @return      none.
 */
static inline void lpc_power_on(void)
{
    analog_write_reg8(0x06,analog_read_reg8(0x06) & 0xfd);
}

/**
 * @brief       This function selects input channel for low power comparator.
 * @param[in]   pin     - selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @return      none.
 */
static inline void lpc_set_input_chn(lpc_input_channel_e pin)
{
    analog_write_reg8(0x0d,(analog_read_reg8(0x0d) & 0xf8) | pin);
}

/**
 * @brief       This function serves to set scaling_coefficient for low power comparator.
 * @param[in]   divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return      none.
 */
static inline void lpc_set_scaling_coeff(lpc_scaling_e divider)
{
    analog_write_reg8(0x0b,(analog_read_reg8(0x0b)&0xcf)|(divider<<4));
}

/**
 * @brief       This function serves to get the comparison results.if Vin>Vref 0x88[6]=0,else 0x88[6]=1.
 * @return      comparison results.
 */
static inline unsigned char lpc_get_result(void)
{
    return ((analog_read_reg8(0x88)&0x40)>>6);
}

/**
 * @brief       This function serves to set the low power comparator diff mode.
 * @return      none
 */
static inline void lpc_set_diff_mode(void)
{
    analog_write_reg8(0x0c, analog_read_reg8(0x0c) & 0xf7); //ana_0x0c[3]
}

/**
 * @brief       This function serves to set the low power comparator single mode.
 * @return      none
 */
static inline void lpc_set_single_mode(void)
{
    analog_write_reg8(0x0c, analog_read_reg8(0x0c) | 0x08); //ana_0x0c[3]
}

/**
 * @brief       This function selects input reference voltage for low power comparator.
 * @param[in]   mode    - lower power comparator working mode includes normal mode and low power mode.
 * @param[in]   ref     - selected input reference voltage.
 * @return      none.
 */
void lpc_set_input_ref(lpc_mode_e mode, lpc_reference_e ref);


