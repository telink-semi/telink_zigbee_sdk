/********************************************************************************************************
 * @file    lpc.h
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
#pragma once

#include "lib/include/analog.h"
#include "lib/include/pm/pm.h"

/**
 * define input IO.
 */
typedef enum
{
    LPC_FLOAT = 0,
    LPC_INPUT_PB5 = 1,
    LPC_INPUT_PB6 = 2,
    LPC_INPUT_PB7 = 3,
    LPC_INPUT_PC0 = 4,
    LPC_INPUT_PC1 = 5,
    LPC_INPUT_PC2 = 6,
    LPC_INPUT_PC3 = 7,
} lpc_input_channel_e;

/**
 * define work mode.
 */
typedef enum
{
    LPC_NORMAL = 0,
    LPC_LOWPOWER,
} lpc_mode_e;

/**
 * define Reference voltage.
 * When BG or UVLO is enabled, the same value in the register represents a different vref value.
 * Default is UVLO Vref.
 */
typedef enum
{
    LPC_REF_974MV = 1,
    LPC_REF_923MV = 2,
    LPC_REF_872MV = 3,
    LPC_REF_820MV = 4,
    LPC_REF_PB4   = 5,
    LPC_REF_PB7   = 6,
} lpc_reference_e;

/**
 * define scale.
 * The input voltage is multiplied by the voltage divider factor and then compared to the reference voltage.
 */
typedef enum
{
    LPC_SCALING_PER25  = 0, // 1/4
    LPC_SCALING_PER50  = 1, // 2/4
    LPC_SCALING_PER75  = 2, // 3/4
    LPC_SCALING_PER100 = 3, // 4/4
} lpc_scaling_e;

/**
 * @brief lpc vbat detect threshold
 * |            |                    |
 * | :--------- | :----------------- |
 * |   <7:4>    |        <3:0>       |
 * |bg trim gear| threshold voltage  |
 */
typedef enum
{
    LPC_VBAT_FALLING_2P50V_RISING_2P60V  = 1 | (1<<4),
    LPC_VBAT_FALLING_2P34V_RISING_2P45V  = 2 | (1<<4),
    LPC_VBAT_FALLING_2P20V_RISING_2P30V  = 3 | (1<<4),
    LPC_VBAT_FALLING_2P04V_RISING_2P15V  = 4 | (1<<4),

    LPC_VBAT_FALLING_3P10V_RISING_3P20V  = 1 | (2<<4),
    LPC_VBAT_FALLING_2P94V_RISING_3P05V  = 2 | (2<<4),
    LPC_VBAT_FALLING_2P80V_RISING_2P90V  = 3 | (2<<4),
    LPC_VBAT_FALLING_2P64V_RISING_2P75V  = 4 | (2<<4),

} lpc_vbat_threshold_vol_e;

/**
 * @brief       This function servers to powers down low power comparator.
 * @return      none.
 */
static inline void lpc_power_down(void)
{
#if (PM_POWER_OPTIMIZATION)
    g_areg_aon_06 |= FLD_PD_LC_COMP_3V;
    analog_write_reg8(areg_aon_0x06, g_areg_aon_06);
#else
    analog_write_reg8(areg_aon_0x06, analog_read_reg8(areg_aon_0x06) | FLD_PD_LC_COMP_3V);
#endif
}

/**
 * @brief       This function servers to power on low power comparator.
 * @return      none.
 */
static inline void lpc_power_on(void)
{
#if (PM_POWER_OPTIMIZATION)
    g_areg_aon_06 &= ~(FLD_PD_LC_COMP_3V);
    analog_write_reg8(areg_aon_0x06, g_areg_aon_06);
#else
    analog_write_reg8(areg_aon_0x06, analog_read_reg8(areg_aon_0x06) & ~(FLD_PD_LC_COMP_3V));
#endif
}

/**
 * @brief       This function selects input channel for low power comparator.
 * @param[in]   pin     - selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @return      none.
 */
static inline void lpc_set_input_chn(lpc_input_channel_e pin)
{
    analog_write_reg8(0x0d, (analog_read_reg8(0x0d) & 0xf8) | pin);
}

/**
 * @brief       This function serves to set scaling_coefficient for low power comparator.
 * @param[in]   divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return      none.
 */
static inline void lpc_set_scaling_coeff(lpc_scaling_e divider)
{
    analog_write_reg8(0x0b, (analog_read_reg8(0x0b) & 0xcf) | (divider << 4));
}

/**
 * @brief       This function serves to get the comparison results.if Vin>Vref 0x88[6]=0,else 0x88[6]=1.
 * @return      comparison results.
 */
static inline unsigned char lpc_get_result(void)
{
    return (analog_read_reg8(0x108) & 0x01);
}

/**
 * @brief       This function serves to enable the vbat low power detection.
 * @return      none.
 */
static inline void lpc_vbat_detect_enable(void)
{
     analog_write_reg8(0x0f, (analog_read_reg8(0x0f) | 0x08));
     analog_write_reg8(0x0d, (analog_read_reg8(0x0d)  | 0x08));
}

/**
 * @brief       This function serves to disable the vbat low power detection.
 * @return      none.
 */
static inline void lpc_vbat_detect_disable(void)
{
    analog_write_reg8(0x0f, (analog_read_reg8(0x0f) & ~(0x08)));
    analog_write_reg8(0x0d, (analog_read_reg8(0x0d)  & ~(0x08)));
}

/**
 * @brief       This function selects input reference voltage for low power comparator.
 * @param[in]   mode    - lower power comparator working mode includes normal mode and low power mode.
 * @param[in]   ref     - selected input reference voltage.
 * @return      none.
 */
void lpc_set_input_ref(lpc_mode_e mode, lpc_reference_e ref);

/**
 * @brief       This function is used to initialize GPIO voltage detection.
 * @param[in]   mode    - lower power comparator working mode includes normal mode and low power mode.
 * @param[in]   pin     - selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @param[in]   ref     - selected input reference voltage.
 * @param[in]   divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return      none.
 */
void lpc_gpio_vol_detect_init(lpc_mode_e mode, lpc_input_channel_e pin, lpc_reference_e ref,lpc_scaling_e divider);
/**
 * @brief       This function is used to initialize vbat low power detection.
 * @param[in]   thres_vol   - the threshold voltage for vbat low power detection.
 * @return      none.
 * @note        -# When using the bat low power detection feature, the reference voltage can ONLY be set to BG, and this feature CANNOT be used in sleep mode.
 */
void lpc_vbat_vol_detect_init(lpc_vbat_threshold_vol_e thres_vol);
/**
 * @brief       This function is used to disable vbat low power detection.
 * @param[in]   none.
 * @return      none.
 * @note        -# disable the vbat low power detection feature.
 */
void lpc_vbat_vol_detect_deinit(void);