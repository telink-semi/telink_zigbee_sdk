/********************************************************************************************************
 * @file    watchdog.c
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
#include "watchdog.h"

static const wd_32k_gear_t g_wd_gears[] = {
    {WK_32K_CLK_WIDTH_256MS,      256},
    {WK_32K_CLK_WIDTH_8192MS,    8192},
    {WK_32K_CLK_WIDTH_262144MS,  262144},
    {WK_32K_CLK_WIDTH_8388608MS, 8388608},
};

/**
 * @brief     start 32k watchdog.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_start(void)
{
    analog_write_reg8(0x79, analog_read_reg8(0x79) | FLD_32K_WATCHDOG_EN);
}

/**
 * @brief     stop 32k watchdog.
 * @return    none.
 * @note      -# the interface sys_init() must be called before this interface can be invoked.
 *            -# after calling this interface, wd_32k_get_status() will be cleared.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_stop(void)
{
    analog_write_reg8(0x79, analog_read_reg8(0x79) & (~FLD_32K_WATCHDOG_EN));
}

/**
 * @brief     feed 32k watch dog.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_feed(void)
{
    analog_write_reg8(0x69, 0x80);
}

/**
 * @brief     get 32k watch dog count.
 * @return    32k watch dog count.
 */
_attribute_ram_code_sec_noinline_ unsigned int wd_32k_get_count_ms(void)
{
    unsigned char temp = ((analog_read_reg8(0x79) & 0x60)>>5);
    return (analog_read_reg8(0x6b)<<((temp+1)*5));
}

/**
 * @brief     get 32k watchdog overflow status.
 * @return    watchdog overflow status.
 * @note      -# After the 32k watchdog reboot returns, the status is set to 1,before the mcu enters the next state, wd_32k_clear_status() must be used to clear the status,
 *               otherwise, the next status judgment of the mcu will be affected;
 *            -# When the status is set to 1, if it is not cleared by calling wd_32k_clear_status():
 *                - software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the interface status remains;
 *                - power cyele/reset pin/vbus detect come back, the status of the interface is lost;
 */
_attribute_ram_code_sec_noinline_ unsigned char wd_32k_get_status(void)
{
    return (analog_read_reg8(areg_aon_0x69) & FLD_32K_WD_OVERFLOW_STATUS);
}

/**
 * @brief     Clear the reset state caused by the 32k watchdog overflow.
 * @return    none.
 * @note      -# the interface sys_init() must be called before this interface can be invoked.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_clear_status(void)
{
    analog_write_reg8(areg_aon_0x69, FLD_32K_WD_OVERFLOW_STATUS);
}

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] wd_32k_clk_sel - The gear positions of the 32K watch dog.
 * @param[in] wd_wd_32k_target - The number of ticks under this level.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ static void wd_32k_set_target_value(wd_32k_clk_sel_e wd_32k_clk_sel, unsigned char wd_32k_target)
{
    unsigned int r  = core_interrupt_disable();
    analog_write_reg8(0x79, analog_read_reg8(0x79) & 0x7f);
    analog_write_reg8(0x79, (wd_32k_clk_sel|wd_32k_target));
    analog_write_reg8(0x79, (0x80|wd_32k_clk_sel|wd_32k_target));
    core_restore_interrupt(r);
}

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is millisecond.
 * @return    none.
 * @note      | period(ms)                   | 256-8192 | 8192-262144 |  262144-8388608 | 8388608-268435456 |
 *            | One wd clock cycle width(ms) |   256    |    8192     |     262144      |       8388608     |
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms)
{
    if (period_ms < 256) {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_256MS, 1);
        return;
    }
    for (unsigned char i = 0; i < sizeof(g_wd_gears) / sizeof(g_wd_gears[0]); i++) {
        unsigned int width = g_wd_gears[i].width_ms;
        /* target = ceil((period_ms + width/4) / width), width/4 as margin */
        unsigned int target = (period_ms + width / 4 + width - 1) / width;
        if (target <= 31) {
            wd_32k_set_target_value(g_wd_gears[i].clk_sel, (unsigned char)target);
            return;
        }
    }

    /* period_ms exceeds all g_wd_gears, use max gear with max target */
    wd_32k_set_target_value(WK_32K_CLK_WIDTH_8388608MS, 31);
}
