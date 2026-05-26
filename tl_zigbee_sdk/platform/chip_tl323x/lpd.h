/********************************************************************************************************
 * @file    lpd.h
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
#include "lib/include/stimer.h"

typedef enum
{
    LPD_FALLING_1P60V_RISING_1P70V,
    LPD_FALLING_1P70V_RISING_1P80V = 1,//default
    LPD_FALLING_1P79V_RISING_1P88V,
    LPD_FALLING_1P96V_RISING_2P06V,
    LPD_FALLING_2P10V_RISING_2P20V,
    LPD_FALLING_2P28V_RISING_2P38V,
    LPD_FALLING_2P37V_RISING_2P47V,
    LPD_FALLING_2P46V_RISING_2P56V,

    LPD_FALLING_1P98V_RISING_2P08V,
    LPD_FALLING_2P12V_RISING_2P22V,
    LPD_FALLING_2P27V_RISING_2P37V,
    LPD_FALLING_2P45V_RISING_2P55V,
    LPD_FALLING_2P59V_RISING_2P69V,
    LPD_FALLING_2P77V_RISING_2P87V,
    LPD_FALLING_2P86V_RISING_2P95V,
    LPD_FALLING_2P95V_RISING_3P04V,

} lpd_threshold_vol_e;

/**
 * @brief       This function is used to select the MSPI or MSPI and MCU that is reset hold(not release) when LPD is triggered.
 * @param[in]   mcu_reset_hold_en -Whether to enable MCU reset hold
 * @return      none.
 * @note        lpd_trigger_reset_config_and_enable() must be called at least 100 us after lpd_ana_enable().
 *              Otherwise, the LPD may be erroneously triggered during power-up or wake-up from sleep, causing the chip to crash.
 */
static _always_inline void lpd_trigger_reset_config_and_enable(unsigned char mcu_reset_hold_en)
{
    if(mcu_reset_hold_en)
    {
        reg_pvd_config = (reg_pvd_config & 0xee) | BIT(0) | BIT(4);//mspi and mcu reset hold, not release
    } else {
        reg_pvd_config = reg_pvd_config  | BIT(0) ;//mspi reset hold, not release
    }
    analog_write_reg8(0x14, (analog_read_reg8(0x14) & (~BIT(5))));//release the analog lpd reset signal
    reg_lpd_int_status |= FLD_LPD_FLASH_STATUS;//clear lpd irq status to prevent accidental triggering
    reg_pvd_config = reg_pvd_config | BIT(2) ;//lpd digital enable
}

static _always_inline void lpd_ana_enable(void)
{
    analog_write_reg8(0x14, (analog_read_reg8(0x14) & (~BIT(4))));//lpd analog configuration must be set before lpd digital configuration, otherwise lpd may be triggered abnormally.
    analog_write_reg8(0x14, analog_read_reg8(0x14) | BIT(5));//hold the analog lpd reset signal to prevent accidental triggering

}

static _always_inline void lpd_disable(void)
{
    reg_pvd_config &= ~(BIT(0) | BIT(2) | BIT(4));//lpd digital disable
    analog_write_reg8(0x14, (analog_read_reg8(0x14) | BIT(4)));
}

/**
 * @brief       This function used for setting the LPD vbat threshold
 * @return      none.
 * @note       -# A0 is not calibrated, so the register value is set to the default position of 1. 
 *              For future versions including A1, the g_lpd_vbat_efuse_code is read from the efuse, looked up in the lpd_vbat_1p7_trim_tab table, and the corresponding register value is set.
 */
void lpd_set_vbat_threshold(lpd_threshold_vol_e thres_vol);

/**
 * @brief       This function serves to protect the flash during the chip power-down process.
 * @return      none.
 * @note       -# In order to improve the robustness of the chip during high-speed operation, the low power detection (LPD) is used to
 *              protect the flash during power-down of the chip .
 *             -# This feature is enabled by default, and the chip power supply voltage is limited to 2.1V to 4.5V.
 */
void lpd_power_down_protect_ana_config(void);


/**
 * @brief       This function serves to protect the flash during the chip power-down process for deep retention wake up.
 * @return      none.
 * @note       -# In order to improve the robustness of the chip during high-speed operation, the low power detection (LPD) is used to
 *              protect the flash during power-down of the chip .
 *             -# This feature is enabled by default, and the chip power supply voltage is limited to 2.1V to 4.5V.
 */
void lpd_power_down_protect_ana_config_for_deep_ret(void);
