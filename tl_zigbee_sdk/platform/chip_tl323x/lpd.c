/********************************************************************************************************
 * @file    lpd.c
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
#include "lpd.h"

extern drv_api_status_e efuse_get_lpd_vbat_code(unsigned char *vol_code);
static unsigned char g_lpd_vbat_efuse_code = 0; /*LPD TRIM CODE is used to determine the value of the lpd_vbat_threshold register (0x14<3:0>), which is used as the index value for the lpd_vbat_1p7_trim_tab array.*/


/**
 * @brief lpd_vbat_1p7_trim_tab: LPD Trim Code Mapping for A1
 * LPD Trim Code Mapping Table:
 *
 * | Efuse Code | conditions 0x14<3:0> | falling edge (V) | rising edge (V) |
 * |------------|----------------------|------------------|-----------------|
 * | 5          | 11                   | 1.853            | 1.922           |
 * | 4          | 3                    | 1.829            | 1.897           |
 * | 3          | 10                   | 1.792            | 1.859           |
 * | 2          | 2                    | 1.767            | 1.834           |
 * | 1          | 9                    | 1.730            | 1.796           |
 * | 0          | 1 (a0 setting)       | 1.705            | 1.772           |
 * | 7          | 8                    | 1.672            | 1.734           |
 * | 6          | 0                    | 1.645            | 1.710           |
 */
static unsigned char lpd_vbat_1p7_trim_tab[] = {1, 9, 2, 10, 3, 11, 0, 8};

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

    lpd_set_vbat_threshold(LPD_FALLING_1P70V_RISING_1P80V);
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

    //Deep retention mode does not lose analog register 0x14, so there is NO need to call lpd_set_vbat_threshold() to save time.
    lpd_ana_enable();
}

/**
 * @brief       This function used for setting the LPD vbat threshold
 * @return      none.
 * @note       -# A0 is not calibrated, so the register value is set to the default position of 1. 
 *              For future versions including A1, the g_lpd_vbat_efuse_code is read from the efuse, looked up in the lpd_vbat_1p7_trim_tab table, and the corresponding register value is set.
 */
void lpd_set_vbat_threshold(lpd_threshold_vol_e thres_vol)
{
    if (CHIP_VERSION_A0 == g_chip_version) {
        analog_write_reg8(0x14, (analog_read_reg8(0x14) & 0xf0) | thres_vol);
    } else { // other version including A1
        unsigned char reg_val = 0;
        efuse_get_lpd_vbat_code(&g_lpd_vbat_efuse_code);
        if (g_lpd_vbat_efuse_code < sizeof(lpd_vbat_1p7_trim_tab) / sizeof(lpd_vbat_1p7_trim_tab[0])) {
            reg_val = lpd_vbat_1p7_trim_tab[g_lpd_vbat_efuse_code] & 0x0f;
        } else {
            reg_val = 1;
        }
        analog_write_reg8(0x14, (analog_read_reg8(0x14) & 0xf0) | reg_val);
    }
}
