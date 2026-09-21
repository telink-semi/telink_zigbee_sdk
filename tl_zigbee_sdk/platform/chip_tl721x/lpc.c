/********************************************************************************************************
 * @file    lpc.c
 *
 * @brief   This is the source file for TL721X
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
#include "lpc.h"

/**
 * @brief       This function selects input reference voltage for low power comparator.
 * @param[in]   mode    - lower power comparator working mode includes normal mode and low power mode.
 * @param[in]   ref     - selected input reference voltage.
 * @return      none.
 */
void lpc_set_input_ref(lpc_mode_e mode, lpc_reference_e ref)
{
    if (mode == LPC_LOWPOWER) {
        //switch uvlo vref
        analog_write_reg8(0x0b, analog_read_reg8(0x0b) & 0xf7);
        analog_write_reg8(0x0d, analog_read_reg8(0x0d) | 0x80);
    } else if (mode == LPC_NORMAL) {
        //switch bg vref
        analog_write_reg8(0x0b, analog_read_reg8(0x0b) | 0x08);
        analog_write_reg8(0x0d, analog_read_reg8(0x0d) & 0x7f);
    }

    analog_write_reg8(0x0d, (analog_read_reg8(0x0d) & 0x8f) | (ref << 4));
}

/**
 * @brief       This function is used to initialize GPIO voltage detection.
 * @param[in]   mode    - lower power comparator working mode includes normal mode and low power mode.
 * @param[in]   pin     - selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @param[in]   ref     - selected input reference voltage.
 * @param[in]   divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return      none.
 */
void lpc_gpio_vol_detect_init(lpc_mode_e mode, lpc_input_channel_e pin, lpc_reference_e ref,lpc_scaling_e divider)
{
    lpc_set_input_chn(pin);
    lpc_set_input_ref(mode,ref);
    lpc_set_scaling_coeff(divider);
}
