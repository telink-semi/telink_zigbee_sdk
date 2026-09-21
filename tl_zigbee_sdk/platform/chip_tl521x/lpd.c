/********************************************************************************************************
 * @file    lpd.c
 *
 * @brief   This is the source file for tl521x
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "lpd.h"

/**
 * @brief       This function serves to protect the flash during the chip power-down process.
 * @return      none.
 * @note       -# In order to improve the robustness of the chip during high-speed operation, the low power detection (LPD) is used to
 *              protect the flash during power-down of the chip .
 *             -# This feature is enabled by default, and the chip power supply voltage is limited to 2.1V to 4.5V.
 */
void lpd_power_down_protect_ana_config(void)
{
    reg_rst1 |=  FLD_RST1_ALGM;
    reg_clk_en1 |= FLD_CLK1_ALGM_EN;
#if WITH_HYSTERESIS
    lpd_set_vbat_threshold(LPD_FALLING_1P673V_RISING_1P777V);
#else
    lpd_set_vbat_threshold(LPD_FALLING_1P673V);
#endif
    lpd_ana_enable();
}

/**
 * @brief       This function serves to protect the flash during the chip power-down process for deep retention wake up.
 * @return      none.
 * @note       -# In order to improve the robustness of the chip during high-speed operation, the low power detection (LPD) is used to
 *              protect the flash during power-down of the chip .
 *             -# This feature is enabled by default, and the chip power supply voltage is limited to 2.1V to 4.5V.
 */
void lpd_power_down_protect_ana_config_for_deep_ret(void)
{
    reg_rst1 |=  FLD_RST1_ALGM;
    reg_clk_en1 |= FLD_CLK1_ALGM_EN;

    //Deep retention mode does not lose analog register 0xC, so there is NO need to call lpd_set_vbat_threshold() to save time.
    lpd_ana_enable();
}

/**
 * @brief       This function used for setting the LPD vbat threshold
 * @param[in]   thres_vol -The threshold voltage of the LPD
 * @return      none.
 */
void lpd_set_vbat_threshold(lpd_threshold_vol_e thres_vol)
{
#if WITH_HYSTERESIS
    /*lpd_hy_en :0x10<7>=1b'1 en_lpd_high_detect :0x13<3>=1b'0, select the threshold voltage table According to the LPD_Voltage_table document*/
    analog_write_reg8(0x10, analog_read_reg8(0x10) | BIT(7));
    analog_write_reg8(0x13, analog_read_reg8(0x13) & (~BIT(3)));
#else
    analog_write_reg8(0x10, analog_read_reg8(0x10) & (~BIT(7)));
    analog_write_reg8(0x13, analog_read_reg8(0x13) & (~BIT(3)));
#endif
    analog_write_reg8(0x9, (analog_read_reg8(0x9) & 0xf0) | thres_vol);
}
