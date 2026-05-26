/********************************************************************************************************
 * @file    lpc.c
 *
 * @brief   This is the source file for tl323x
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
    lpc_vbat_detect_disable();
    lpc_set_input_chn(pin);
    lpc_set_input_ref(mode,ref);
    lpc_set_scaling_coeff(divider);
}

/**
 * @brief       This function is used to initialize vbat low power detection.
 * @param[in]   thres_vol   - the threshold voltage for vbat low power detection.
 * @return      none.
 * @note        -# When using the bat low power detection feature, the reference voltage can ONLY be set to BG, and this feature CANNOT be used in sleep mode.
 */
void lpc_vbat_vol_detect_init(lpc_vbat_threshold_vol_e thres_vol)
{
    //select bg vref
    analog_write_reg8(0x0b, analog_read_reg8(0x0b) | 0x08);
    analog_write_reg8(0x0d, analog_read_reg8(0x0d) & 0x7f);

    lpc_set_input_chn(LPC_FLOAT);//must be float
    lpc_set_scaling_coeff(LPC_SCALING_PER50);//lpc scaling coeff = 50%, CANNOT be change
    analog_write_reg8(0x16, (analog_read_reg8(0x16) & 0x3f) | (((thres_vol&0xf0)>>4)<<6));//ref_bg_trim
    analog_write_reg8(0x0d, (analog_read_reg8(0x0d) & 0x8f) | ((thres_vol&0x0f) << 4));
    lpc_vbat_detect_enable();
}
/**
 * @brief       This function is used to disable vbat low power detection.
 * @param[in]   none.
 * @return      none.
 * @note        -# disable the vbat low power detection feature.
 */
void lpc_vbat_vol_detect_deinit(void)
{
    lpc_vbat_detect_disable();
}