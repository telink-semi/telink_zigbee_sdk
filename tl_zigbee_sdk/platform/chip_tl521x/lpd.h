/********************************************************************************************************
 * @file    lpd.h
 *
 * @brief   This is the header file for tl521x
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
#pragma once

#include "lib/include/analog.h"
#include "lib/include/stimer.h"

#define WITH_HYSTERESIS 0

#if WITH_HYSTERESIS
/**
 * @brief       This enumeration is defined based on LPD_Voltage_table document,according to the LPD_Voltage_table tab 4. The corresponding analog register configuration is as follows: 
 *              en_lpd_hy       0x10<7>=1b'1
 *              en_high_detect  0x13<3>=1b'0
*/
typedef enum
{
    LPD_FALLING_1P596V_RISING_1P719V, // sel=0000
    LPD_FALLING_1P639V_RISING_1P749V, // sel=0001
    LPD_FALLING_1P673V_RISING_1P777V, // sel=0010
    LPD_FALLING_1P703V_RISING_1P804V, // sel=0011
    LPD_FALLING_1P731V_RISING_1P831V, // sel=0100
    LPD_FALLING_1P757V_RISING_1P857V, // sel=0101
    LPD_FALLING_1P783V_RISING_1P882V, // sel=0110
    LPD_FALLING_1P809V_RISING_1P908V, // sel=0111
    LPD_FALLING_1P835V_RISING_1P933V, // sel=1000
    LPD_FALLING_1P860V_RISING_1P956V, // sel=1001
    LPD_FALLING_1P911V_RISING_2P009V, // sel=1010
    LPD_FALLING_2P012V_RISING_2P110V, // sel=1011
    LPD_FALLING_2P115V_RISING_2P211V, // sel=1100
    LPD_FALLING_2P213V_RISING_2P312V, // sel=1101
    LPD_FALLING_2P314V_RISING_2P413V, // sel=1110
    LPD_FALLING_2P415V_RISING_2P514V, // sel=1111

} lpd_threshold_vol_e;
#else

/**
 * @brief       This enumeration is defined based on LPD_Voltage_table document,according to the LPD_Voltage_table tab 4. The corresponding analog register configuration is as follows: 
 *              en_lpd_hy       0x10<7>=1b'0
 *              en_high_detect  0x13<3>=1b'0
*/
typedef enum
{
    LPD_FALLING_1P596V, // sel=0000
    LPD_FALLING_1P639V, // sel=0001
    LPD_FALLING_1P673V, // sel=0010
    LPD_FALLING_1P703V, // sel=0011
    LPD_FALLING_1P731V, // sel=0100
    LPD_FALLING_1P757V, // sel=0101
    LPD_FALLING_1P783V, // sel=0110
    LPD_FALLING_1P809V, // sel=0111
    LPD_FALLING_1P835V, // sel=1000
    LPD_FALLING_1P860V, // sel=1001
    LPD_FALLING_1P911V, // sel=1010
    LPD_FALLING_2P012V, // sel=1011
    LPD_FALLING_2P113V, // sel=1100
    LPD_FALLING_2P213V, // sel=1101
    LPD_FALLING_2P314V, // sel=1110
    LPD_FALLING_2P415V, // sel=1111
} lpd_threshold_vol_e;
#endif


/**
 * @brief       This function is used to select the MSPI or MSPI and MCU that is reset hold(not release) when LPD is triggered.
 * @param[in]   mcu_reset_hold_en -Whether to enable MCU reset hold
 * @return      none.
 * @note        lpd_trigger_reset_config_and_enable() must be called at least 100 us after lpd_ana_enable().
 *              Otherwise, the LPD may be erroneously triggered during power-up or wake-up from sleep, causing the chip to crash.
 */
static _always_inline void lpd_trigger_reset_config_and_enable(unsigned char mcu_reset_hold_en)
{
    if (mcu_reset_hold_en) {
        reg_lpd_ctrl |= (FLD_LPD_FLASH_SEL | FLD_LPD_FLASH_RST_MCU);//mspi and mcu reset hold, not release
    } else {
        reg_lpd_ctrl |= FLD_LPD_FLASH_SEL;//mspi reset hold, not release
    }
    analog_write_reg8(0x0C, (analog_read_reg8(0x0C) & (~BIT(6))));//release the analog lpd reset signal
    reg_lpd_int_status |= FLD_LPD_FLASH_STATUS;//clear lpd irq status to prevent accidental triggering
#if !WITH_HYSTERESIS
/* minimum cycles based on pclk to trigger LPD flash protection. actual minimum cycles = (lpd_thres+1),lpd_thres=0~0xFE */
    reg_lpd_ctrl2 = 0xfe;
#endif
    reg_lpd_ctrl |= FLD_LPD_FLASH_EN;//lpd digital enable
}

/**
 * @brief       This function is used to enable the LPD analog configuration and select the 4th threshold voltage table.
 * @param[in]   none.
 * @return      none.
 * @note        lpd_trigger_reset_config_and_enable() must be called at least 100 us after lpd_ana_enable().
 *              Otherwise, the LPD may be erroneously triggered during power-up or wake-up from sleep, causing the chip to crash.
 */
static _always_inline void lpd_ana_enable(void)
{
    analog_write_reg8(0x0C, (analog_read_reg8(0x0C) & (~BIT(5))));//lpd analog configuration must be set before lpd digital configuration, otherwise lpd may be triggered abnormally.
    analog_write_reg8(0x0C, analog_read_reg8(0x0C) | BIT(6));//hold the analog lpd reset signal to prevent accidental triggering
}

static _always_inline void lpd_disable(void)
{
    reg_lpd_ctrl &= ~(FLD_LPD_FLASH_SEL | FLD_LPD_FLASH_EN | FLD_LPD_FLASH_RST_MCU);//lpd digital disable
    analog_write_reg8(0x0C, (analog_read_reg8(0x0C) | BIT(5)));
}

/**
 * @brief       This function used for setting the LPD vbat threshold
 * @param[in]   thres_vol -The threshold voltage of the LPD
 * @return      none.
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