/********************************************************************************************************
 * @file    watchdog.c
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
#include "watchdog.h"

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
_attribute_ram_code_sec_noinline_ void wd_32k_set_target_value(wd_32k_clk_sel_e wd_32k_clk_sel, unsigned char wd_32k_target)
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
#if 0
    if(period_ms < 256)
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_256MS, 1);
    }
    else if(period_ms < 8064)//8192-128
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_256MS, (period_ms + 128) / 256);//256ms
    }
    else if(period_ms < 258048)//262144-4096
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_8192MS, (period_ms + 4096) / 8192);//8.2s
    }
    else if(period_ms < 8,257536)//8388608-131072
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_262144MS, (period_ms + 131072) / 262144);//4.4m
    }
    else if(period_ms < 264241152)//268435456-4194304
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_8388608MS, (period_ms + 4194304) / 8388608);//2.3h
    }
    else
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_8388608MS, 31);
    }
#else
    if(period_ms < 256)
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_256MS, 1);
    }
    else if(period_ms < 7937)//8192-255
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_256MS, (period_ms + 255) / 256);//256ms
    }
    else if(period_ms < 253953)//262144-8191
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_8192MS, (period_ms + 8191) / 8192);//8.2s
    }
    else if(period_ms < 8126465)//8388608-262143
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_262144MS, (period_ms + 262143) / 262144);//4.4m
    }
    else if(period_ms < 260046849)//268435456-8388607
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_8388608MS, (period_ms + 8388607) / 8388608);//2.3h
    }
    else
    {
        wd_32k_set_target_value(WK_32K_CLK_WIDTH_8388608MS, 31);
    }
#endif
}
