/********************************************************************************************************
 * @file    watchdog.c
 *
 * @brief   This is the source file for TL321X
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
#include "watchdog.h"
#include "gpio.h"

_attribute_data_retention_sec_ unsigned int wd_32k_interval_ms = 5000;

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
 * @note      1.After deep sleep is awakened, the wd_32k_set_interval_ms interface needs to be called
 *            to reconfigure the watch dog reset time. Otherwise, the reset time will revert
 *            to the default value (5 seconds). This reset time might be unexpected.
 *            2.During the time when feeding the dogs, the watch dog will be turned off.
 *            During this period, the system is unprotected.
 *            Therefore, other watch dogs still need to be turned on.
 *            3.During the time when I was feeding the dogs, there was a period when the interruption was turned off.
 *            Duration of time when the interruption is disabled: PCLK_16M 20.2us, PCLK_24M 13.54us, PCLK_48M 6.72us.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_feed(void)
{
    unsigned int tmp_period_ms = 0;

    tmp_period_ms = clock_get_32k_tick() + 32 * wd_32k_interval_ms;

    unsigned char ang_0x79 = analog_read_reg8(0x79);

    unsigned int r  = core_interrupt_disable();

    analog_write_reg8_internal(0x79, ang_0x79 & (~FLD_32K_WATCHDOG_EN));

    analog_write_reg16_internal(0x7b, tmp_period_ms >> 16);
    analog_write_reg8_internal(0x7a, tmp_period_ms >> 8);

    analog_write_reg8_internal(0x79, ang_0x79 | FLD_32K_WATCHDOG_EN);

    core_restore_interrupt(r);
}

/**
 * @brief     get 32k watch dog count.
 * @return    32k watch dog count.
 */
_attribute_ram_code_sec_noinline_ unsigned int wd_32k_get_count_ms(void)
{
    unsigned int tmp_interval_ms = 0;
    unsigned int tmp_count_ms = 0;

    tmp_interval_ms = analog_read_reg8(0x7a)<<3|analog_read_reg8(0x7b)<<11|analog_read_reg8(0x7c)<<19;
    tmp_count_ms = clock_get_32k_tick()>>5;

    return (tmp_interval_ms-tmp_count_ms);
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

#if(COMPATIBLE_WITH_TL321X_AND_TL323X == 0)
/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is millisecond,the lower 8bit of the wd timer register is invalid,so ranges from 8~134,217,720ms.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms)
{

    wd_32k_interval_ms = period_ms;

    wd_32k_feed();
}
#else
/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is millisecond.
 * @return    none.
 * @note      | period(ms)                   | 256-8192 | 8192-262144 |  262144-8388608 | 8388608-268435456 |
 *            | One wd clock cycle width(ms) |   256    |    8192     |     262144      |       8388608     |
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms)
{
    if(period_ms < 256)
    {
        wd_32k_interval_ms = 256;
    }
    else if(period_ms < 7937)//8192-255
    {
        wd_32k_interval_ms = ((period_ms + 255) >> 8) << 8;//256ms
    }
    else if(period_ms < 253953)//262144-8191
    {
        wd_32k_interval_ms = ((period_ms + 8191) >> 13) << 13;//8.2s
    }
    else if(period_ms < 8126465)//8388608-262143
    {
        wd_32k_interval_ms = ((period_ms + 262143) >> 18) << 18;//4.4m
    }
    else if(period_ms < 125829120)//0xf0000000/32
    {
        wd_32k_interval_ms = ((period_ms + 8388607) >> 23) << 23;//2.3h
    }
    else
    {
        wd_32k_interval_ms = 125829120;
    }

    wd_32k_feed();
}
#endif
